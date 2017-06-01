local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local sqrt = math.sqrt

local states = {}

states.init = BehaviorHelper.init 'wander'
states.wander = BehaviorHelper.wander

function states:findAttackTargetOrWander(zombie)
	local currentAttackTarget = BehaviorHelper.findClosestTarget(zombie, BehaviorHelper.isValidHostileAttackTarget)
	if currentAttackTarget then
		zombie:setAttackTarget(currentAttackTarget)
		zombie:enterState 'followAttackTarget'
	else
		zombie:enterState 'wander'
	end
end

function states:followAttackTarget(zombie)
	while true do
		local currentAttackTarget = zombie:getAttackTarget()
		if not currentAttackTarget then
			zombie:enterState 'findAttackTargetOrWander'
		else
			-- current attack target is too far -> clear it
			local x, y = zombie:getPosition()
			local tx, ty = currentAttackTarget:getPosition()
			local distance2 = (tx - x) * (tx - x) + (ty - y) * (ty - y)
			
			local maxDistance = 6
			if distance2 > maxDistance * maxDistance then
				zombie:setAttackTarget(nil)
			else
				-- move closer to the current attack target
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
	end
end

states.onEntityEnteredVisionRange = BehaviorHelper.onEntityEnteredVisionRangeAttack(
	BehaviorHelper.isValidHostileAttackTarget,
	'followAttackTarget'
)

return states
