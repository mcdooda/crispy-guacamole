return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(zombie)
		local target = zombie:getAttackTarget()
		zombie:lookAtEntity(target)
		zombie:playAnimationAsync 'attack'
	end
}