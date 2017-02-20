local ModData = {}

ModData.path = Mod.getPath()
ModData.currentMapName = Mod.getCurrentMapName()

do
	local path = ModData.path
	local mapName = ModData.currentMapName

	function ModData.mapFile(file)
		return path .. '/maps/' .. mapName .. '/' .. file
	end
end

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
		local componentExists, componentTemplate = pcall(dofile, ModData.path .. '/entities/' .. entityName .. '/' .. componentConfigName .. '.lua')
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
	local map = dofile(ModData.mapFile 'map.lua')
	local zones = dofile(ModData.mapFile 'zones.lua')
	local zoneNames = {}
	for zoneName, zone in pairs(zones) do
		zoneNames[#zoneNames + 1] = zoneName
	end
	zones.names = zoneNames
	ModData.maps[mapName] = {
		name = mapName,
		map = map,
		zones = zones
	}
	ModData.currentMap = ModData.maps[ModData.currentMapName]
end

-- props
local props = dofile(ModData.path .. '/props/props.lua')
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
	end,
	getPath = function(propDir, propFile)
		return ModData.path .. '/props/' .. propDir .. '/' .. propFile .. '.png'
	end
}

for i = 1, #props do
	local propName = props[i]
	local prop = dofile(ModData.path .. '/props/' .. propName .. '/prop.lua')
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
		return ModData.path .. '/tiles/' .. tileDir .. '/' .. tileFile .. '.png'
	end
}

for i = 1, #tiles do
	local tileDir = tiles[i]
	local tile = dofile(ModData.path .. '/tiles/' .. tileDir .. '/tile.lua')
	ModData.tiles[tileDir] = tile
end

return ModData