return {
	maxHealth = 100,
	--[[spawn = function(skeleton)
		skeleton:playAnimation 'spawn'
	end,]]
	despawn = function(skeleton)
		skeleton:playAnimation 'death'
	end
}
