local probabilities = {
	120, 10, 5, 3, 15, 15,-- center
	100, -- top left
	100, -- top right
	100, 100,
	100, 100,
	100,
	15, -- big bubble
	10, -- small bubbles
}

local centerTiles = {1, 2, 3, 4, 5, 6, 14, 15}
local topLeftTiles = {7}
local topRightTiles = {8}
local topLeftRightTiles = {9}
local cornerLeft = {10}
local cornerRight = {11}
local cornerBottom = {12}
local cornerTop = {13}

local function selectTile(tl, tr, bl, br, t, r, b, l)
	if (not tr or tr == 'shallow_water') and (not tl or tl == 'shallow_water') and (t and t ~= 'shallow_water') then
		return cornerBottom
	elseif (not tl or tl == 'shallow_water') and (not tr or tr == 'shallow_water') then
		return centerTiles
	elseif (tl and tl ~= 'shallow_water') and (tr and tr ~= 'shallow_water') then
		return topLeftRightTiles
	elseif (tl and tl == 'shallow_water') and (not tr or tr ~= 'shallow_water') and (not t or t == 'shallow_water') then
		return cornerLeft
	elseif (tr and tr == 'shallow_water') and (not tl or tl ~= 'shallow_water') and (not t or t == 'shallow_water') then
		return cornerRight
	elseif tl and tl ~= 'shallow_water' then
		return topLeftTiles
	elseif tr and tr ~= 'shallow_water' then
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