local math = math
local cos = math.cos
local sin = math.sin
local atan2 = math.atan
local sqrt = math.sqrt

return {
	attackRange = 5,
	attackCooldown = 0.5,
	autoAttack = true,
	moveDuringAttack = false,
	attackFunc = function(tower)
		local target = tower:getAttackTarget()

		if not target or not target:isValid() then
			return
		end
		
		local x, y, z = tower:getPosition()
		local tx, ty, tz = target:getPosition()
		
		local heading = atan2(ty - y, tx - x)
		
		Entity.spawn(
			'arrow',
			x + cos(heading) * sqrt(2.0) / 2,
			y + sin(heading) * sqrt(2.0) / 2,
			z + 1.5,
			heading,
			0
		)
	end
}