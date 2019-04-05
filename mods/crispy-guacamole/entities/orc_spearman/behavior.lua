local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'
local Money = require 'mods/crispy-guacamole/scripts/money'

local function getClosestHut(spearman)
	return EntitiesByType:getClosests('orc_hut', spearman:getPosition():toVector2())[1]
end

local function getClosestMinerals(spearman)
	local hut = getClosestHut(spearman)
	if hut then
		local spearmanPosition = spearman:getPosition()
		local closestMineral
		local closestEntityDistance2 = math.huge
		local closestMinerals = EntitiesByType:getClosests('prop_minerals', hut:getPosition():toVector2())
		for i = 1, #closestMinerals do
			local mineral = closestMinerals[i]
			local distance2 = (spearmanPosition - mineral:getPosition()):length2()
			if distance2 < closestEntityDistance2 then
				closestMineral = mineral
				closestEntityDistance2 = distance2
			end
		end
		return closestMineral
	else
		return EntitiesByType:getClosests('prop_minerals', spearman:getPosition():toVector2())[1]
	end
end

local states = BehaviorHelper.customAttacker(
	BehaviorHelper.doNothing,
	BehaviorHelper.findClosestTarget,
	BehaviorHelper.isValidHostileAttackTarget
)

local init = states.init
function states:init(spearman)
	local moveAnimation = math.random(1, 2) == 1 and 'move2' or 'move'
	spearman:setCycleAnimation(moveAnimation)
	spearman:getExtraData().mineralsAmount = 0
	init(self, spearman)
end

function states:gatherMinerals(spearman)
	local minerals = spearman:getInteractionEntity()

	local extraData = spearman:getExtraData()
	local mineralsChanged = false
	if extraData.minerals ~= minerals then
		extraData.minerals = minerals
		mineralsChanged = true
	end

	local mineralsData = minerals:getExtraData()
	local collected = mineralsData:withdraw(1)
	extraData.mineralsAmount = collected
	if mineralsData.amount == 0 then
		minerals:despawn()
	end

	local hut = not mineralsChanged and spearman:getExtraData().hut or nil

	if not hut or not hut:isValid() then
		hut = getClosestHut(spearman)
		extraData.hut = hut
	end

	if hut then
		spearman:interactWith(hut)
	end
end

function states:backToWork(spearman)
	local hut = spearman:getInteractionEntity()

	local extraData = spearman:getExtraData()

	extraData.hut = hut

	if extraData.mineralsAmount > 0 then
		Money:add(extraData.mineralsAmount)
		extraData.mineralsAmount = 0
	end

	local minerals = extraData.minerals

	if not minerals or not minerals:isValid() then
		minerals = getClosestMinerals(spearman)
	end

	if minerals then
		spearman:interactWith(minerals)
	end
end

function states:missingInteractionEntity(spearman)
	spearman:clearPath()
	spearman:getExtraData().hut = nil
	local interactionStateName = spearman:getInteractionStateName()
	if interactionStateName == 'gatherMinerals' then
		local minerals = getClosestMinerals(spearman)
		if minerals then
			spearman:interactWith(minerals)
		else
			local hut = getClosestHut(spearman)
			if hut then
				spearman:interactWith(hut)
			end
		end
	elseif interactionStateName == 'backToWork' then
		local hut = getClosestHut(spearman)
		if hut then
			spearman:interactWith(hut)
		end
	else
		error('missing interaction entity for state: ' .. interactionStateName)
	end
end

return states