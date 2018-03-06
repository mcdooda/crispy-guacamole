--[[
return {
	maxHealth = 100,
	spawn = function(zombie)
		zombie:playAnimation 'spawn'
	end,
	despawn = function(zombie)
		zombie:playAnimation 'despawn'
	end
}
]]

return flat.graph.script.run(Mod.getPath() .. '/entities/zombie/life')