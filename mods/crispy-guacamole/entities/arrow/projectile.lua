return {
	weight = 10,
	speed = 5,
	collidedCallback = function(projectile, collidedEntity)
		--print('Collided at ', projectile:getPosition())
		--projectile:despawn()
		return collidedEntity ~= nil
	end
}