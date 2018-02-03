local Path = require 'data/scripts/path'

local ModData = {}

-- entities
local entities = Mod.getEntities()
ModData.entities = {
	names = entities
}
for i = 1, #entities do
	local entityName = entities[i]
	local entityTemplate = {
		name = entityName
	}
	for componentConfigName, componentFlag in pairs(Component) do
		local componentExists, componentTemplate = pcall(dofile, Path.getComponentPath(entityName, componentConfigName) .. '.lua')
		if componentExists then
			entityTemplate[componentConfigName] = componentTemplate
		end
	end
	ModData.entities[entityName] = entityTemplate
end

-- maps
local maps = Mod.getMaps()
ModData.maps = {
	names = maps
}
for i = 1, #maps do
	local mapName = maps[i]
	local map = Path.requireMapConfig()
	ModData.maps[mapName] = {
		name = mapName,
		map = map
	}
	ModData.currentMap = ModData.maps[ModData.currentMapName]
end

-- props
local props = Mod.getProps()
ModData.props = {
	names = props,
	getHighest = function(propDir)
		local max = { nil, 0 }
		for k,v in pairs(ModData.props[propDir]) do
			if max[2] < v then
				max = {k,v}
			end
		end
		return max[1]
	end
}

for i = 1, #props do
	local propName = props[i]
	local prop = Path.requirePropConfig(propName)
	ModData.props[propName] = prop
end

-- tiles
local tiles = Mod.getTiles()
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
	end
}

for i = 1, #tiles do
	local tileDir = tiles[i]
	local tile = Path.requireTileConfig(tileDir)
	ModData.tiles[tileDir] = tile
end

return ModData