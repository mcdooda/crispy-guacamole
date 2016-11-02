return {
	weight = 5,
	speed = 5,
	collidedCallback = function(projectile, collidedEntity)
		--print('Collided at ', projectile:getPosition())
		return collidedEntity ~= nil
	end
}