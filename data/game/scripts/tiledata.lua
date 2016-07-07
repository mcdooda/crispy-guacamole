local tiles = { highlightedTiles = {} }

local mapTiles = Map.getTiles()
for _, tile in pairs(mapTiles) do
	local tileData
	if Tile.getTexturePath(tile):match('/resource/') then
		local x, y = Tile.getPosition(tile)
		local r = Entity.newAbstract('resource', x, y)
		Entity.setZ(r, -1.1)
		tileData = {
			level = 0,
			oldLevel = 0,
			max = 1,
			towers = {},
			highlighting = 0,
			resource = r
		}
	elseif Tile.getTexturePath(tile):match('/lava/') then
		tileData = {
			level = 0,
			oldLevel = 0,
			max = 1,
			towers = {},
			highlighting = 0,
			lava = true
		}
	else
		tileData = {
			level = 0,
			oldLevel = 0,
			max = 1,
			towers = {},
			highlighting = 0
		}
	end
	tiles[tile] = tileData
end

return tiles
