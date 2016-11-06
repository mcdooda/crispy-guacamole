return {
	weight = 10,
	speed = 5,
	collidedCallback = function(projectile, collidedEntity)
		projectile:despawn()
		return collidedEntity ~= nil
	end
}