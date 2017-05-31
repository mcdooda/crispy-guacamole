return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(skeleton)
		local target = skeleton:getAttackTarget()
		skeleton:lookAtEntity(target)
		skeleton:playAnimationAsync 'attack'
		Timer.start(0.06, nil, function()
			if target:isValid() then
				target:dealDamage(10)
			end
		end)
	end
}