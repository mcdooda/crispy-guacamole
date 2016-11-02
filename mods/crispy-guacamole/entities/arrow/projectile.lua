return {
	weight = 1,
	speed = 1,
	collidedCallback = function(projectile, collidedEntity)
		print('Collided at ', projectile:getPosition())
		return false
	end
}