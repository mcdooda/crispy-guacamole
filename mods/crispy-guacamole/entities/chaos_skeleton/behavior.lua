local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local random = math.random
local sqrt = math.sqrt

local states = {}

function states:init(skeleton)
	skeleton:enterState 'wander'
end

function states:idle(skeleton)
	skeleton:jump()
end

function states:wander(skeleton)
	local x, y = skeleton:getPosition()
	while true do
		
		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			skeleton:moveTo(rx, ry)
		end
		
	end
end

function states:followAttackTarget(skeleton)
	while true do
		local currentAttackTarget = skeleton:getAttackTarget()
		if not currentAttackTarget then
			skeleton:enterState 'wander'
		end
		local x, y = skeleton:getPosition()
		local tx, ty = currentAttackTarget:getPosition()
		local distance2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
		
		local maxDistance = 6
		if distance2 > maxDistance * maxDistance then
			skeleton:setAttackTarget(nil)
			skeleton:enterState 'wander'
		end
		
		local newX, newY
		local followStepDistance = 0.5
		if distance2 < followStepDistance * followStepDistance then
			newX, newY = tx, ty
		else
			local distance = sqrt(distance2)
			newX, newY = x + (tx - x) / distance * followStepDistance, y + (ty - y) / distance * followStepDistance
		end
		skeleton:moveTo(newX, newY)
	end
end

function states:onEntityEnteredVisionRange(skeleton, entity)
	local isHostile = skeleton:isHostile(entity) or entity:isHostile(skeleton) -- skeletons are mean
	if isHostile and entity:isLiving() then
		local currentAttackTarget = skeleton:getAttackTarget()
		if not currentAttackTarget then
			-- no current target
			skeleton:setAttackTarget(entity)
			return 'followAttackTarget'
		else
			-- the entity is closer than the current target
			local x, y = skeleton:getPosition()
			local currentAttackTarget = skeleton:getAttackTarget()
			
			local tx, ty = currentAttackTarget:getPosition()
			local distanceToCurrentTarget2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
			
			local ex, ey = currentAttackTarget:getPosition()
			local distanceToEntity2 = (ex - x) * (ex - x) + (ey - y) * (ey - y)
			
			if distanceToEntity2 < distanceToCurrentTarget2 then
				skeleton:setAttackTarget(entity)
				return 'followAttackTarget'
			end
		end
	end
end

return states
