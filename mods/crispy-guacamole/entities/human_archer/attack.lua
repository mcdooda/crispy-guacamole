local atan2 = math.atan2

return {
	attackRange = 5,
	attackCooldown = 0.5,
	autoAttack = true,
	moveDuringAttack = false,
	attackFunc = function(archer)
		local target = archer:getAttackTarget()
		archer:lookAtEntity(target)
		archer:playAnimationAsync 'shoot'
		
		Timer.start(0.3, nil, function()
			if not archer:isValid() or not target:isValid() then
				return
			end
			
			local x, y, z = archer:getAttachPoint 'crossbow'
			local tx, ty, tz = target:getPosition()
			
			local heading = atan2(ty - y, tx - x)
			
			Entity.spawn('arrow', x, y, z, heading, math.pi / 4)
		end)
	end
}