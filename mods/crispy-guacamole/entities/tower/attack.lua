local ProjectileHelper = require 'data/scripts/componenthelpers/projectile'
local spawnArrow = ProjectileHelper.createSpawner 'arrow'

local math = math
local cos = math.cos
local sin = math.sin
local sqrt = math.sqrt
local atan2 = math.atan

return {
	attackRange = 5,
	attackCooldown = 0.5,
	autoAttack = true,
	moveDuringAttack = false,
	attack = function(tower)
		local target = tower:getAttackTarget()

		if not target or not target:isValid() then
			return
		end

		local x, y, z = tower:getPosition()
		local tx, ty, tz = target:getPosition()
		
		local heading = atan2(ty - y, tx - x)
		
		local x, y, z = tower:getPosition()
		x = x + cos(heading) * sqrt(2.0) / 2
		y = y + sin(heading) * sqrt(2.0) / 2
		z = z + 1.5
		
		spawnArrow(x, y, z, target)
	end
}