return {
	weight = 0,
	speed = 12,
	collidedCallback = function(projectile, collidedEntity)
		if not collidedEntity then
			local position = projectile:getPosition()
			local heading = projectile:getHeading()
			Entity.spawn('fx_explosion', position, heading)

			projectile:despawn()
			return false
		end
		return true
	end
}
