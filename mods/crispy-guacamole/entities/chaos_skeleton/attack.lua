local AttackHelper = require 'data/scripts/componenthelpers/attack'

return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(skeleton)
		local target = skeleton:getAttackTarget()
		skeleton:lookAtEntity(target)
		skeleton:playAnimationAsync 'attack'
		AttackHelper.dealDamageAfterDelay(target, 10, 0.06)
	end
}