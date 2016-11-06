return {
	weight = 10,
	speed = 5,
	collidedCallback = function(projectile, collidedEntity)
		if not collidedEntity then
			Timer.start(2, nil, function()
				projectile:despawn()
			end)
			return false
		end
		return true
	end
}