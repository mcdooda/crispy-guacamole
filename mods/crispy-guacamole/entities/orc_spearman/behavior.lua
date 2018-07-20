local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

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

local function findClosestEntityOfType(entity, range, type)
	local position = entity:getPosition():toVector2()
	local nearbyEntities = Map.getEntitiesInRange(position, 10)
	local closestEntity = nil
	local closestEntityDistance2 = math.huge
	for i = 1, #nearbyEntities do
		local nearbyEntity = nearbyEntities[i]
		if nearbyEntity:getTemplateName() == type then
			local nearbyEntityPosition = nearbyEntity:getPosition():toVector2()
			local distance2 = (nearbyEntityPosition - position):length2()
			if distance2 < closestEntityDistance2 then
				closestEntityDistance2 = distance2
				closestEntity = nearbyEntity
			end
		end
	end
	return closestEntity
end

function states:gatherMinerals(spearman)
	local minerals = spearman:getInteractionEntity()
	spearman:getExtraData().minerals = minerals

	local mineralsData = minerals:getExtraData()
	mineralsData.amount = mineralsData.amount - 1
	if mineralsData.amount == 0 then
		minerals:despawn()
	end

	local hut = spearman:getExtraData().hut

	if not hut or not hut:isValid() then
		hut = findClosestEntityOfType(spearman, 10, 'orc_hut')
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
		minerals = findClosestEntityOfType(spearman, 10, 'prop_minerals')
	end

	if minerals then
		spearman:interactWith(minerals)
	end
end

function states:missingInteractionEntity(spearman)
	local interactionStateName = spearman:getInteractionStateName()
	if interactionStateName == 'gatherMinerals' then
		local minerals = findClosestEntityOfType(spearman, 10, 'prop_minerals')
		if minerals then
			spearman:interactWith(minerals)
		end
	elseif interactionStateName == 'backToWork' then
		local hut = findClosestEntityOfType(spearman, 10, 'orc_hut')
		if hut then
			spearman:interactWith(hut)
		end
	end
end

return states