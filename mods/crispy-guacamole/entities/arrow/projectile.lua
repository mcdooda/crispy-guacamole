return {
	weight = 0.5,
	speed = 10,
	collidedCallback = function(projectile, collidedEntity)
		print('Collided at ', projectile:getPosition())
	end
}