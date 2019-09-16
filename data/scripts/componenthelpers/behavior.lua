local CollisionHelper = require 'data/scripts/componenthelpers/collision'
local AttackHelper = require 'data/scripts/componenthelpers/attack'

local yield = coroutine.yield
local huge = math.huge
local random = math.random
local sqrt = math.sqrt
local abs = math.abs
local min = math.min
local max = math.max

local function init(initialState)
	return function(states, entity)
		return initialState
	end
end

local function doNothing()
	while true do
		coroutine.yield()
	end
end

--[[
	Wander
]]

local function wanderAround(entity, initialPosition)
	local randomPosition
	while true do
		randomPosition = initialPosition + flat.Vector2((random() * 2 - 1) * 2, (random() * 2 - 1) * 2)
		entity:moveTo(randomPosition)
	end
end

local function wander(states, entity)
	local position = entity:getPosition()
	wanderAround(entity, position:toVector2())
end

--[[
	Find target
]]

local function findClosestTarget(entity, isValidTarget)
	local minDistance2 = huge
	local closestTarget
	local position2d = entity:getPosition():toVector2()
	for _, visibleEntity in entity:eachVisibleEntity() do
		local visibleEntityPosition2d = visibleEntity:getPosition():toVector2()
		local distanceToVisibleEntity2 = (visibleEntityPosition2d - position2d):length2()
		if distanceToVisibleEntity2 < minDistance2 and isValidTarget(entity, visibleEntity) then
			minDistance2 = distanceToVisibleEntity2
			closestTarget = visibleEntity
		end
	end
	return closestTarget
end

local function isValidHostileAttackTarget(entity, target)
	local isHostile = entity:isHostile(target) or target:isHostile(entity)
	local isLiving = target:isLiving()
	return isHostile and isLiving
end

--[[
	Detection
]]

local function onEntityEnteredVisionRangeAttack(isValidTarget, foundTargetState)
	return function(states, entity, target)
		if isValidTarget(entity, target) then
			local currentAttackTarget = entity:getAttackTarget()
			if not currentAttackTarget then
				-- no current target
				entity:setAttackTarget(target)
				return foundTargetState
			else
				-- the entity is closer than the current target
				local position = entity:getPosition()
				local currentAttackTarget = entity:getAttackTarget()
				
				local attackTargetPosition = currentAttackTarget:getPosition()
				local distanceToCurrentTarget2 = (attackTargetPosition - position):length2()
				
				local targetPosition = target:getPosition()
				local distanceToNewTarget2 = (targetPosition - position):length2()
				
				if distanceToNewTarget2 < distanceToCurrentTarget2 then
					entity:setAttackTarget(target)
					return foundTargetState
				end
			end
		end
	end
end

--[[
	Common behaviors
]]

local function findAttackTargetOrFallback(findTarget, isValidTarget, targetFoundState, fallbackState)
	return function(states, entity)
		local newAttackTarget = findTarget(entity, isValidTarget)
		if newAttackTarget then
			entity:setAttackTarget(newAttackTarget)
			return targetFoundState
		else
			return fallbackState
		end
	end
end

local function followAttackTarget(findTargetState)
	return function(states, entity)
		while true do
			local currentAttackTarget = entity:getAttackTarget()
			if not currentAttackTarget then
				return findTargetState
			else
				-- current attack target is too far -> clear it
				if not entity:canSee(currentAttackTarget) then
					entity:setAttackTarget(nil)
					return findTargetState
				else
					local position = entity:getPosition()
					local position2d = position:toVector2()
					local targetPosition = currentAttackTarget:getPosition()
					local targetPosition2d = targetPosition:toVector2()
					local move = targetPosition2d - position2d
					local distance = move:length()

					local attackRange = AttackHelper.getTemplate(entity).attackRange
					local _, entityRadius = CollisionHelper.getRadius(entity)
					local _, targetRadius = CollisionHelper.getRadius(currentAttackTarget)

					local followStepDistance = 0.7

					local desiredAttackDistance = attackRange + entityRadius + targetRadius
					if distance - entityRadius - targetRadius < attackRange * 0.05 then
						-- move away from the target
						followStepDistance = -attackRange * 0.05
					else
						-- move closer to the current attack target but avoid collision
						local desiredMoveDistance = distance - desiredAttackDistance + 0.01
						followStepDistance = min(desiredMoveDistance, followStepDistance)
						followStepDistance = max(0, followStepDistance)
					end
					
					-- move closer to the target or move back to avoid the "return to position" effect while in combat
					if followStepDistance ~= 0 then
						-- normalize direction and multiply by the distance to travel
						local destination = position2d + move:getNormalized() * followStepDistance
						entity:clearPath()
						entity:moveTo(destination)
					else
						-- nothing to do for now
						yield()
					end
				end
			end
		end
	end
end

--[[
	Basic states for rapid prototyping
]]

local function customAttacker(wander, findClosestTarget, isValidHostileAttackTarget)
	local states = {}

	states.init = init 'wander'
	states.wander = wander

	states.findAttackTargetOrWander = findAttackTargetOrFallback(
		findClosestTarget,
		isValidHostileAttackTarget,
		'followAttackTarget',
		'wander'
	)

	states.followAttackTarget = followAttackTarget 'findAttackTargetOrWander'

	states.onEntityEnteredVisionRange = onEntityEnteredVisionRangeAttack(
		isValidHostileAttackTarget,
		'followAttackTarget'
	)

	return states
end

local function basicAttacker()
	return customAttacker(doNothing, findClosestTarget, isValidHostileAttackTarget)
end

return {
	init                             = init,

	doNothing                        = doNothing,

	wanderAround                     = wanderAround,
	wander                           = wander,

	findClosestTarget                = findClosestTarget,
	isValidHostileAttackTarget       = isValidHostileAttackTarget,

	onEntityEnteredVisionRangeAttack = onEntityEnteredVisionRangeAttack,

	findAttackTargetOrFallback       = findAttackTargetOrFallback,
	followAttackTarget               = followAttackTarget,

	customAttacker                   = customAttacker,
	basicAttacker                    = basicAttacker
}