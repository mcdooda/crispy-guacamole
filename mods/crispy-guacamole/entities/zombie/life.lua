return {
	maxHealth = 100,
	spawn = function(zombie)
		zombie:playAnimation 'spawn'
		zombie:playAnimation 'move'
	end,
	despawn = function(zombie)
		print 'despawn'
	end
}
