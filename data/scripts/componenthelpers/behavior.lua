local CollisionHelper = require 'data/scripts/componenthelpers/collision'

local getTime = Time.getTime
local yield = coroutine.yield
local huge = math.huge
local random = math.random
local sqrt = math.sqrt

local function init(initialState)
	return function(states, entity)
		entity:enterState(initialState)
	end
end

local function sleep(duration)
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end


--[[
	Wander
]]

local function wanderAround(entity, x, y)
	while true do
		local rx = x + (random() * 2 - 1) * 2
		local ry = y + (random() * 2 - 1) * 2
		entity:moveTo(rx, ry)
	end
end

local function wander(states, entity)
	local x, y = entity:getPosition()
	wanderAround(entity, x, y)
end

--[[
	Find target
]]

local function findClosestTarget(entity, isValidTarget)
	local minDistance2 = huge
	local closestTarget
	local x, y = entity:getPosition()
	for _, visibleEntity in entity:eachVisibleEntity() do
		local vx, vy = visibleEntity:getPosition()
		local distanceToVisibleEntity2 = (vx - x) * (vx - x) + (vy - y) * (vy - y)
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
				local x, y = entity:getPosition()
				local currentAttackTarget = entity:getAttackTarget()
				
				local tx, ty = currentAttackTarget:getPosition()
				local distanceToCurrentTarget2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
				
				local ex, ey = target:getPosition()
				local distanceToNewTarget2 = (ex - x) * (ex - x) + (ey - y) * (ey - y)
				
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
		entity:clearPath()
		local visionRange2 = 6 * 6
		while true do
			local currentAttackTarget = entity:getAttackTarget()
			if not currentAttackTarget then
				entity:enterState(findTargetState)
			else
				-- current attack target is too far -> clear it
				local x, y = entity:getPosition()
				local tx, ty = currentAttackTarget:getPosition()
				local distance2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
				
				if distance2 > visionRange2 then
					entity:setAttackTarget(nil)
					entity:enterState(findTargetState)
				else
					-- move closer to the current attack target but avoid collision
					local followStepDistance = 0.2
					local minFollowStepDistance = 0.01
					local distanceMinusRadius = CollisionHelper.distanceMinusRadius(entity, currentAttackTarget)

					if distanceMinusRadius < followStepDistance then
						followStepDistance = distanceMinusRadius
					end

					if followStepDistance > minFollowStepDistance then
						-- normalize direction and multiply by the distance to travel
						local distance = sqrt(distance2)
						local newX, newY = x + (tx - x) / distance * followStepDistance, y + (ty - y) / distance * followStepDistance
						entity:moveTo(newX, newY)
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

    sleep                            = sleep,

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