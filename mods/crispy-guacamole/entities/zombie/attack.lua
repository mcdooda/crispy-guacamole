local atan2 = math.atan2

return {
	attackRange = 0.2,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attackFunc = function(zombie, target)
		local x, y = zombie:getPosition()
		local tx, ty = target:getPosition()
		local heading = atan2(ty - y, tx - x)
		zombie:setHeading(heading) -- TODO: entity:lookAtEntity
		zombie:playAnimationAsync 'attack'
	end
}