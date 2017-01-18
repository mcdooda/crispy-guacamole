local math = math
local random = math.random
local sqrt = math.sqrt

local function sleep(duration)
	local getTime = Time.getTime
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

local states = {}

function states:init(zombie)
	zombie:enterState 'wander'
end

function states:despawn(zombie)
	zombie:despawn()
end

function states:idle(zombie)
	zombie:jump()
end

function states:wander(zombie)
	local x, y = zombie:getPosition()
	while true do
		
		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			zombie:moveTo(rx, ry)
		end
		
	end
end

function states:followAttackTarget(zombie)
	while true do
		local currentAttackTarget = zombie:getAttackTarget()
		if not currentAttackTarget then
			zombie:enterState 'wander'
		end
		local x, y = zombie:getPosition()
		local tx, ty = currentAttackTarget:getPosition()
		local distance2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
		
		local maxDistance = 6
		if distance2 > maxDistance * maxDistance then
			zombie:setAttackTarget(nil)
			zombie:enterState 'wander'
		end
		
		local newX, newY
		local followStepDistance = 0.5
		if distance2 < followStepDistance * followStepDistance then
			newX, newY = tx, ty
		else
			local distance = sqrt(distance2)
			newX, newY = x + (tx - x) / distance * followStepDistance, y + (ty - y) / distance * followStepDistance
		end
		zombie:moveTo(newX, newY)
	end
end

function states:onEntityEnteredVisionRange(zombie, entity)
	local isHostile = zombie:isHostile(entity) or entity:isHostile(zombie) -- zombies are mean
	if isHostile then
		local currentAttackTarget = zombie:getAttackTarget()
		if not currentAttackTarget then
			-- no current target
			zombie:setAttackTarget(entity)
			return 'followAttackTarget'
		else
			-- the entity is closer than the current target
			local x, y = zombie:getPosition()
			local currentAttackTarget = zombie:getAttackTarget()
			
			local tx, ty = currentAttackTarget:getPosition()
			local distanceToCurrentTarget2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
			
			local ex, ey = currentAttackTarget:getPosition()
			local distanceToEntity2 = (ex - x) * (ex - x) + (ey - y) * (ey - y)
			
			if distanceToEntity2 < distanceToCurrentTarget2 then
				zombie:setAttackTarget(entity)
				return 'followAttackTarget'
			end
		end
	end
end

return states
