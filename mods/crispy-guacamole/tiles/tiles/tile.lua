local probabilities = {
	1,2,3,1,1,1
}
local function selectTile(tl, tr, bl, br, t, r, b, l)
	if tl  ~= 'tiles' or tr  ~= 'tiles' or bl  ~= 'tiles' or br  ~= 'tiles' or t  ~= 'tiles' or r  ~= 'tiles' or b  ~= 'tiles' or l  ~= 'tiles' then
		return {4, 5, 6}
	end
	return {1, 2, 3}
end


return {
	probabilities = probabilities,
	frameDuration = 0.15,
	numFrames = 1,
	selectTile = selectTile
}