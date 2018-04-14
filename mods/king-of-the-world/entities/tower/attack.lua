local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local spawnArrow = ProjectileHelper.createSpawner 'bouncing_arrow'

local cos = math.cos
local sin = math.sin
local sqrt = math.sqrt
local atan2 = math.atan

return {
	attackRange = 15,
	attackCooldown = 1,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(tower)
		local target = tower:getAttackTarget()

		if not target or not target:isValid() then
			return
		end

		local towerPosition = tower:getPosition()
		local targetPosition = target:getPosition()

		local heading = atan2(targetPosition:y() - towerPosition:y(), targetPosition:x() - towerPosition:x())
		local spawnPosition = flat.Vector3(
			towerPosition:x() + cos(heading) * sqrt(2.5) / 2,
			towerPosition:y() + sin(heading) * sqrt(2.5) / 2,
			towerPosition:z() + 1.2
		)
		Entity.spawn('fx_explosion', spawnPosition)

		spawnArrow(spawnPosition, target)
	end
}