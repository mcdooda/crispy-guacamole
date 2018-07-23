local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local function getClosestHut(spearman)
	return EntitiesByType:getClosest('orc_hut', spearman:getPosition():toVector2())
end

local function getClosestMinerals(spearman)
	local hut = getClosestHut(spearman)
	if hut then
		return EntitiesByType:getClosest('prop_minerals', hut:getPosition():toVector2())
	else
		return EntitiesByType:getClosest('prop_minerals', spearman:getPosition():toVector2())
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
	spearman:setMoveAnimation(moveAnimation)
	init(self, spearman)
end

function states:gatherMinerals(spearman)
	local minerals = spearman:getInteractionEntity()
	spearman:getExtraData().minerals = minerals

	local mineralsData = minerals:getExtraData()
	mineralsData:withdraw(1)
	if mineralsData.amount == 0 then
		minerals:despawn()
	end

	local hut = spearman:getExtraData().hut

	if not hut or not hut:isValid() then
		hut = getClosestHut(spearman)
	end

	if hut then
		spearman:interactWith(hut)
	end
end

function states:backToWork(spearman)
	local hut = spearman:getInteractionEntity()
	spearman:getExtraData().hut = hut

	local minerals = spearman:getExtraData().minerals

	if not minerals or not minerals:isValid() then
		minerals = getClosestMinerals(spearman)
	end

	if minerals then
		spearman:interactWith(minerals)
	end
end

function states:missingInteractionEntity(spearman)
	local interactionStateName = spearman:getInteractionStateName()
	spearman:clearPath()
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