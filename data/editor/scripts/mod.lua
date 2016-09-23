local Mod = {}

local path = 'mods/crispy-guacamole'
Mod.path = path

-- entities
local entities = dofile(Mod.path .. '/entities/entities.lua')
Mod.entities = {
	names = entities
}
for i = 1, #entities do
	local entityName = entities[i]
	local entityBehavior = dofile(path .. '/entities/' .. entityName .. '/behavior.lua')
	local entityPhysics  = dofile(path .. '/entities/' .. entityName .. '/physics.lua')
	local entitySprite   = dofile(path .. '/entities/' .. entityName .. '/sprite.lua')
	Mod.entities[entityName] = {
		name     = entityName,
		behavior = entityBehavior,
		physics  = entityPhysics,
		sprite   = entitySprite
	}
end

-- maps
local maps = dofile(Mod.path .. '/maps/maps.lua')
Mod.maps = {
	names = maps
}
for i = 1, #maps do
	local mapName = maps[i]
	local map = dofile(path .. '/maps/' .. mapName .. '/map.lua')
	Mod.maps[mapName] = map
end

-- props
local props = dofile(Mod.path .. '/props/props.lua')
Mod.props = {
	names = props
}
for i = 1, #props do
	local propName = props[i]
	-- nothing to do, for now
end

-- tiles
local tiles = dofile(Mod.path .. '/tiles/tiles.lua')
Mod.tiles = {
	names = tiles
}
for i = 1, #tiles do
	local tileName = tiles[i]
	local tile = dofile(path .. '/tiles/' .. tileName .. '/tile.lua')
	Mod.tiles[tileName] = tile
end

return Mod