local ModData = {}

local path = Mod.getPath()
ModData.path = path

-- entities
local entities = dofile(ModData.path .. '/entities/entities.lua')
ModData.entities = {
	names = entities
}
for i = 1, #entities do
	local entityName = entities[i]
	local entityBehavior = dofile(path .. '/entities/' .. entityName .. '/behavior.lua')
	local entityMovement = dofile(path .. '/entities/' .. entityName .. '/movement.lua')
	local entitySprite   = dofile(path .. '/entities/' .. entityName .. '/sprite.lua')
	ModData.entities[entityName] = {
		name     = entityName,
		behavior = entityBehavior,
		movement = entityMovement,
		sprite   = entitySprite
	}
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
	names = props
}
for i = 1, #props do
	local propName = props[i]
	-- nothing to do, for now
end

-- tiles
local tiles = dofile(ModData.path .. '/tiles/tiles.lua')
ModData.tiles = {
	names = tiles
}
for i = 1, #tiles do
	local tileName = tiles[i]
	local tile = dofile(path .. '/tiles/' .. tileName .. '/tile.lua')
	ModData.tiles[tileName] = tile
end

return ModData