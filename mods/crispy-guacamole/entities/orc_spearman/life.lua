return {
	maxHealth = 100,
	--[[spawn = function(spearman)
		spearman:playAnimation 'spawn'
	end,]]
	despawn = function(spearman)
		spearman:playAnimation 'death'
	end
}
