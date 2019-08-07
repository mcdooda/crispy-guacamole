local probabilities = {
	100, 5, 5, -- center
	100, -- top left
	100, -- top right
	100, 100,
	100, 100,
	100,
}

local centerTiles = {1, 2, 3}
local topLeftTiles = {4}
local topRightTiles = {5}
local topLeftRightTiles = {6}
local cornerLeft = {7}
local cornerRight = {8}
local cornerBottom = {9}
local cornerTop = {10}

local function selectTile(tl, tr, bl, br, t, r, b, l)
	if (not tr or tr == 'water') and (not tl or tl == 'water') and (t and t ~= 'water') then
		return cornerBottom
	elseif (not tl or tl == 'water') and (not tr or tr == 'water') then
		return centerTiles
	elseif (tl and tl ~= 'water') and (tr and tr ~= 'water') then
		return topLeftRightTiles
	elseif (tl and tl == 'water') and (not tr or tr ~= 'water') and (not t or t == 'water') then
		return cornerLeft
	elseif (tr and tr == 'water') and (not tl or tl ~= 'water') and (not t or t == 'water') then
		return cornerRight
	elseif tl and tl ~= 'water' then
		return topLeftTiles
	elseif tr and tr ~= 'water' then
		return topRightTiles
	else
		return topLeftRightTiles
	end
end

return {
	probabilities = probabilities,
	navigability = Map.Navigability.WATER,
	frameDuration = 0.15,
	numFrames = 6,
	selectTile = selectTile
}