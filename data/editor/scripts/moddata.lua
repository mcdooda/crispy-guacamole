local ModData = {}

local dump = require 'data/scripts/dump'

local path = Mod.getPath()
ModData.path = path

-- entities
local entities = dofile(ModData.path .. '/entities/entities.lua')
ModData.entities = {
	names = entities
}
for i = 1, #entities do
	local entityName = entities[i]
	local entityTemplate = {
		name = entityName
	}
	local componentConfigNames = ComponentRegistry.getConfigNames()
	for j = 1, #componentConfigNames do
		local componentConfigName = componentConfigNames[j]
		local componentExists, componentTemplate = pcall(dofile, path .. '/entities/' .. entityName .. '/' .. componentConfigName .. '.lua')
		if componentExists then
			entityTemplate[componentConfigName] = componentTemplate
		end
	end
	ModData.entities[entityName] = entityTemplate
end

-- maps
local maps = dofile(ModData.path .. '/maps/maps.lua')
ModData.maps = {
	names = maps
}
for i = 1, #maps do
	local mapName = maps[i]
	local map = dofile(path .. '/maps/' .. mapName .. '/map.lua')
	ModData.maps[mapName] = map
end

-- props
local props = dofile(ModData.path .. '/props/props.lua')
ModData.props = {
	names = props,
	getHighest = function(propDir)
		local max = { nil, 0 }
		dump(ModData.props[propDir])
		for k,v in pairs(ModData.props[propDir]) do
			if max[2] < v then
				max = {k,v}
			end
		end
		return max[1]
	end,
	getPath = function(propDir, propFile)
		print(path .. '/props/' .. propDir .. '/' .. propFile .. '.png')
		return path .. '/props/' .. propDir .. '/' .. propFile .. '.png'
	end
}

for i = 1, #props do
	local propName = props[i]
	local prop = dofile(path .. '/props/' .. propName .. '/prop.lua')
	dump(prop)
	ModData.props[propName] = prop

end

-- tiles
local tiles = dofile(ModData.path .. '/tiles/tiles.lua')
ModData.tiles = {
	names = tiles,
	getHighest = function(tileDir)
		local max = { nil, 0 }
		for k,v in pairs(ModData.tiles[tileDir]) do
			if max[2] < v then
				max = {k,v}
			end
		end
		return max[1]
	end,
	getPath = function(tileDir, tileFile)
		return path .. '/tiles/' .. tileDir .. '/' .. tileFile .. '.png'
	end
}

for i = 1, #tiles do
	local tileDir = tiles[i]
	local tile = dofile(path .. '/tiles/' .. tileDir .. '/tile.lua')
	ModData.tiles[tileDir] = tile
end


return ModData