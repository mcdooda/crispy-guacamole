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
			if not archer:isValid() then
				return
			end
			
			local x, y, z = archer:getAttachPoint 'crossbow'
			local heading = archer:getHeading()
			Entity.spawn('arrow', x, y, z, heading)
		end)
	end
}