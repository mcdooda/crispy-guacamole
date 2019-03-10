local probabilities = {
	100, 5, 5, -- center
	100, 5, 5, -- top left
	100, 5, 5, -- top right
	100        -- top left + right
}

local centerTiles = {1, 2, 3}
local topLeftTiles = {4, 5, 6}
local topRightTiles = {7, 8, 9}
local topLeftRightTiles = {10}

local function selectTile(tl, tr, br, bl)
	if (not tl or tl == 'water') and (not tr or tr == 'water') then
		return centerTiles
	elseif (not tl or tl ~= 'water') and (not tr or tr ~= 'water') then
		return topLeftRightTiles
	elseif not tl or tl ~= 'water' then
		return topLeftTiles
	elseif not tr or tr ~= 'water' then
		return topRightTiles
	else
		return topLeftRightTiles
	end
end

return {
	probabilities = probabilities,
	frameDuration = 0.2,
	numFrames = 3,
	navigability = Map.Navigability.WATER,
	selectTile = selectTile
}