local ceil = math.ceil
local floor = math.floor

local function round(n)
	local fn = floor(n)
	local ipart = n - fn
	if ipart < 0.5 then
		return fn
	else
		return ceil(n)
	end
end

return round
