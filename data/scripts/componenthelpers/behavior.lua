local Time = Time
local getTime = Time.getTime
local yield = coroutine.yield
local huge = math.huge
local random = math.random

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

return {
	init                             = init,

    sleep                            = sleep,

	wanderAround                     = wanderAround,
	wander                           = wander,

	findClosestTarget                = findClosestTarget,
	isValidHostileAttackTarget       = isValidHostileAttackTarget,

	onEntityEnteredVisionRangeAttack = onEntityEnteredVisionRangeAttack
}