return {
	attackRange = 0.5,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(spearman)
		local target = spearman:getAttackTarget()
		local spearmanState, firstAttack = spearman:getExtraData 'spearmanState'
		spearman:lookAtEntity(target)
		local position2d = spearman:getPosition():toVector2()
		if firstAttack or (spearmanState.lastAttackPosition - position2d):length2() > 0.05 then
			spearman:playAnimation 'lowerSpear'
			spearmanState.lastAttackPosition = position2d
		end
		spearman:playAnimation('attack', 1, false)
		local timer = game.Timer()
		timer:onEnd(function()
			if target:isValid() then
				target:dealDamage(2)
			end
		end)
		timer:start(0.06)
	end
}