--[[
local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local spawnArrow = ProjectileHelper.createSpawnerFromEntity 'arrow'

return {
	attackRange = 5,
	attackCooldown = 0.5,
	autoAttack = true,
	moveDuringAttack = true,
	attackDuringMove = true,
	attack = function(archer)
		local target = archer:getAttackTarget()
		archer:lookAtEntity(target)
		archer:playAnimation('shoot', 1, false)
		
		Timer.start(0.3, nil, function()
			local target = archer:getAttackTarget()

			if not target or not target:isValid() then
				return
			end

			spawnArrow(archer, 'crossbow', target)
		end)
	end
}
--]]

return flat.graph.script.run(Mod.getPath() .. '/entities/human/human_archer/attack')
