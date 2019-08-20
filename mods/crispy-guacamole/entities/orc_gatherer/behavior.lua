local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'
local UnitSelection = require 'mods/crispy-guacamole/scripts/unitselection'
local Money = require 'mods/crispy-guacamole/scripts/money'

local function getClosestHut(gatherer)
	return EntitiesByType:getClosests('orc_hut', gatherer:getPosition():toVector2())[1]
end

local function getClosestMinerals(gatherer)
	local hut = getClosestHut(gatherer)
	if hut then
		local gathererPosition = gatherer:getPosition()
		local closestMineral
		local closestEntityDistance2 = math.huge
		local closestMinerals = EntitiesByType:getClosests('prop_minerals', hut:getPosition():toVector2())
		for i = 1, #closestMinerals do
			local mineral = closestMinerals[i]
			local distance2 = (gathererPosition - mineral:getPosition()):length2()
			if distance2 < closestEntityDistance2 then
				closestMineral = mineral
				closestEntityDistance2 = distance2
			end
		end
		return closestMineral
	else
		return EntitiesByType:getClosests('prop_minerals', gatherer:getPosition():toVector2())[1]
	end
end

local states = BehaviorHelper.customAttacker(
	BehaviorHelper.doNothing,
	BehaviorHelper.findClosestTarget,
	BehaviorHelper.isValidHostileAttackTarget
)

local init = states.init
function states:init(gatherer)
	UnitSelection.init(gatherer)
	gatherer:setCycleAnimation 'move'
	gatherer:getExtraData().mineralsAmount = 0
	init(self, gatherer)
end

function states:gatherMinerals(gatherer)
	local minerals = gatherer:getInteractionEntity()

	local extraData = gatherer:getExtraData()
	local mineralsChanged = false
	if extraData.minerals ~= minerals then
		extraData.minerals = minerals
		mineralsChanged = true
	end

	local mineralsData = minerals:getExtraData()
	local collected = mineralsData:withdraw(1)
	gatherer:setCycleAnimation 'full'
	extraData.mineralsAmount = collected
	if mineralsData.amount == 0 then
		minerals:despawn()
	end

	local hut = not mineralsChanged and gatherer:getExtraData().hut or nil

	if not hut or not hut:isValid() then
		hut = getClosestHut(gatherer)
		extraData.hut = hut
	end

	if hut then
		gatherer:interactWith(hut)
	end
end

function states:backToWork(gatherer)
	local hut = gatherer:getInteractionEntity()

	local extraData = gatherer:getExtraData()

	extraData.hut = hut

	if extraData.mineralsAmount > 0 then
		Money:add(extraData.mineralsAmount)
		gatherer:setCycleAnimation 'move'
		extraData.mineralsAmount = 0
	end

	local minerals = extraData.minerals

	if not minerals or not minerals:isValid() then
		minerals = getClosestMinerals(gatherer)
	end

	if minerals then
		gatherer:interactWith(minerals)
	end
end

function states:missingInteractionEntity(gatherer)
	gatherer:clearPath()
	gatherer:getExtraData().hut = nil
	local interactionStateName = gatherer:getInteractionStateName()
	if interactionStateName == 'gatherMinerals' then
		local minerals = getClosestMinerals(gatherer)
		if minerals then
			gatherer:interactWith(minerals)
		else
			local hut = getClosestHut(gatherer)
			if hut then
				gatherer:interactWith(hut)
			end
		end
	elseif interactionStateName == 'backToWork' then
		local hut = getClosestHut(gatherer)
		if hut then
			gatherer:interactWith(hut)
		end
	else
		error('missing interaction entity for state: ' .. interactionStateName)
	end
end

function states:build(gatherer)
	local building = gatherer:getInteractionEntity()
	local extraData = building:getExtraData()
	gatherer:playAnimation 'full'
	gatherer:sleep(0.5)
	if not building:isValid() or not extraData.buildingInProgress then
		if not gatherer:interactWith(building) then
			building:cancelCurrentActions()
		end
		return
	end
	local health = math.min(building:getHealth() + 1, building:getMaxHealth())
	building:setHealth(health)
	gatherer:interactWith(building)
end

return states