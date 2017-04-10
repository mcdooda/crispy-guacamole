local math = math
local cos = math.cos
local sin = math.sin
local sqrt = math.sqrt
local atan2 = math.atan
local atan = atan2

local arrowProjectile = require 'mods/crispy-guacamole/entities/arrow/projectile'

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
			local target = archer:getAttackTarget()

			if not target or not target:isValid() then
				return
			end
			
			local x, y, z = archer:getAttachPoint 'crossbow'

			local tx, ty, tz = target:getPosition()

			local heading = atan2(ty - y, tx - x)

			local px = x
			local py = y
			local pz = z

			local altitude = z - tz

			local speedXY = arrowProjectile.speed
			local weight = arrowProjectile.weight

			local dXY = sqrt((tx - x) * (tx - x) + (ty - y) * (ty - y))
			local dZ = tz - z

			local speedXY2 = speedXY * speedXY
			local speedXY4 = speedXY2 * speedXY2
			
			local elevation = atan(
				(speedXY2 + sqrt(speedXY4 - weight * (weight * dXY * dXY + 2 * dZ * speedXY2)))
				/ (weight * dXY)
			)

			if elevation == elevation then -- not NaN
				local arrow = Entity.spawn('arrow', px, py, pz, heading, elevation)
				local arrowData = arrow:getExtraData()
				arrowData.initElevation = elevation
				arrowData.initPosition = { x = px, y = py, z = pz }
			end
		end)
	end
}