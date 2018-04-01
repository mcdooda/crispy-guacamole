local CollisionHelper = require 'data/scripts/componenthelpers/collision'

local yield = coroutine.yield
local huge = math.huge
local random = math.random
local sqrt = math.sqrt

local function init(initialState)
	return function(states, entity)
		entity:enterState(initialState)
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
			entity:enterState(targetFoundState)
		else
			entity:enterState(fallbackState)
		end
	end
end

local function followAttackTarget(findTargetState)
	return function(states, entity)
		local visionRange2 = 6 * 6
		while true do
			local currentAttackTarget = entity:getAttackTarget()
			if not currentAttackTarget then
				entity:enterState(findTargetState)
			else
				-- current attack target is too far -> clear it
				local position = entity:getPosition()
				local position2d = position:toVector2()
				local targetPosition = currentAttackTarget:getPosition()
				local targetPosition2d = targetPosition:toVector2()
				local move = targetPosition2d - position2d
				local distance2 = move:length2()
				
				if distance2 > visionRange2 then
					entity:setAttackTarget(nil)
					entity:enterState(findTargetState)
				else
					-- move closer to the current attack target but avoid collision
					local followStepDistance = 3
					local minFollowStepDistance = 0.01
					local distanceMinusRadius = CollisionHelper.distanceMinusRadius(entity, currentAttackTarget)

					if distanceMinusRadius < followStepDistance then
						followStepDistance = distanceMinusRadius
					end

					if followStepDistance > minFollowStepDistance then
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
	return customAttacker(wander, findClosestTarget, isValidHostileAttackTarget)
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