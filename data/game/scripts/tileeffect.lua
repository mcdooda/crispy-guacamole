local tileData = require 'data/game/scripts/tiledata'
local Brush = Brush
local Tile = Tile
local math = math
local random = math.random

local TileEffect = {
	highlightedTowers = {}
}

function TileEffect.clearHighlight()
	for tile in pairs(tileData.highlightedTiles) do
		tileData[tile].highlighting = 0
		TileEffect.updateColor(tile)
	end
	tileData.highlightedTiles = {}
	TileEffect.highlightedTowers = {}
end

function TileEffect.highlight(tower)
	TileEffect.highlightedTowers[tower] = true
	local brush = tower:getBrush()
	local brushTiles = brush:getTiles(tower.x, tower.y)
	for tile, effect in pairs(brushTiles) do
		tileData[tile].highlighting = math.max(effect, tileData[tile].highlighting)
		tileData.highlightedTiles[tile] = true
		TileEffect.updateColor(tile)
	end
end

function TileEffect.updateHighlight()
	local highlightedTowers = TileEffect.highlightedTowers
	TileEffect.clearHighlight()
	for tower in pairs(highlightedTowers) do
		TileEffect.highlight(tower)
	end
end

function TileEffect.highlightTile(tile, highlighting)
	tileData[tile].highlighting = math.max(highlighting, tileData[tile].highlighting)
	tileData.highlightedTiles[tile] = true
	TileEffect.updateColor(tile)
end

function TileEffect.setTileHighlighting(tile, highlighting)
	tileData[tile].highlighting = highlighting
	tileData.highlightedTiles[tile] = true
	TileEffect.updateColor(tile)
end

local highlightColor = { 0, 0, 0 }
function TileEffect.setHighlightColor(color)
	highlightColor = color
end

local color = { 0, 0, 0 }
local creepColorComplement = { 140, 148, 120 }
function TileEffect.updateColor(tile)
	local tileContamination = tileData[tile]
	local level = tileContamination.level
	local oldLevel = tileContamination.oldLevel
	if level ~= oldLevel then
		if oldLevel == 0 then
			Tile.setTopLayerTexture(tile, 'data/game/mucus/mucus'..random(1, 4)..'.png')
		elseif level == 0 then
			Tile.setTopLayerTexture(tile, nil)
		end
		if level > 0 then
			Tile.setTopLayerColor(tile, { 255, 255, 255, 255 * level })
			Tile.setTopLayerYShift(tile, -8 * level + 5)
		end
		tileContamination.oldLevel = level
	end
	local highlighting = tileContamination.highlighting
	local max = tileContamination.max
	for i = 1, 3 do
		color[i] = ((255 - level * creepColorComplement[i]) + highlighting * highlightColor[i]) / (1 + highlighting)
	end
	Tile.setColor(tile, color)
end

return TileEffect
