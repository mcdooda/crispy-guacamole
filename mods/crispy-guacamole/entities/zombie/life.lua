return {
	maxHealth = 100,
	spawn = function(zombie)
		zombie:playAnimation 'spawn'
	end,
	despawn = function(zombie)
		print 'despawn'
		zombie:playAnimation 'despawn'
	end
}
