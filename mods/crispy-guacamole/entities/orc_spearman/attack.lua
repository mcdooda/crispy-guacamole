local function distance2(pos1, pos2)
	local x = pos1[1] - pos2[1]
	local y = pos2[2] - pos2[2]
	return (x * x) + (y * y) 
end
	
return {
	attackRange = 0.05,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(spearman)
		local target = spearman:getAttackTarget()
		local spearmanState, firstAttack = spearman:getExtraData 'spearmanState'
		spearman:lookAtEntity(target)
		local position = { spearman:getPosition() }
		if firstAttack or distance2(spearmanState.lastAttackPosition, position) > 0.05 then
			spearman:playAnimation 'lowerSpear'
			spearmanState.lastAttackPosition = position
		end
		spearman:playAnimationAsync 'attack'
		Timer.start(0.06, nil, function()
			if target:isValid() then
				target:dealDamage(10)
			end
		end)
	end
}