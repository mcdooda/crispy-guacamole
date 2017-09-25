--[[
local AttackHelper = require 'data/scripts/componenthelpers/attack'

return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(zombie)
		local target = zombie:getAttackTarget()
		zombie:lookAtEntity(target)
		zombie:playAnimation('attack', 1, false)
		AttackHelper.dealDamageAfterDelay(target, 10, 0.08)
	end
}
--]]

--[[]]
local component = flat.graph.script.run 'mods/crispy-guacamole/entities/zombie/attack'

--[[
function component.attack(zombie)
	local target = zombie:getAttackTarget()
	zombie:lookAtEntity(target)
	zombie:playAnimation('attack', 1, false)
	AttackHelper.dealDamageAfterDelay(target, 10, 0.08)
end
]]

return component
--]]