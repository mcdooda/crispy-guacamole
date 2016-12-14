local atan2 = math.atan2

return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attackFunc = function(zombie)
		local target = zombie:getAttackTarget()
		zombie:lookAtEntity(target)
		zombie:playAnimationAsync 'attack'
	end
}