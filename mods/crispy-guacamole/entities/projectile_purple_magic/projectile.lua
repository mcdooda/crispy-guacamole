return {
	weight = 0,
	speed = 12,
	collidedCallback = function(projectile, collidedEntity)
		if not collidedEntity then
			local x, y, z = projectile:getPosition()
			local heading = projectile:getHeading()
			Entity.spawn('fx_explosion', x, y, z, heading)

			projectile:despawn()
			return false
		end
		return true
	end
}
