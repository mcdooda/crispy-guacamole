return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(zombie)
		local target = zombie:getAttackTarget()
		zombie:lookAtEntity(target)
		zombie:playAnimationAsync 'attack'
		Timer.start(0.08, nil, function()
			if target:isValid() then
				target:dealDamage(10)
			end
		end)
	end
}