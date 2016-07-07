---------------------
local math = math
local floor = math.floor
local abs = math.abs
local coroutine = coroutine
local wrap = coroutine.wrap
local yield = coroutine.yield
local getTile = Map.getTile
---------------------

local function round(n)
	local ipart = floor(n)
	local fpart = n - ipart
	if fpart < 0.5 then
		return ipart
	else
		return ipart + 1
	end
end

local function sign(a)
	if a > 0 then
		return 1
	elseif a < 0 then
		return -1
	else
		return 0
	end
end

-- Line --

local Line = {}

function Line.eachTile(fx, fy, tx, ty)
	return wrap(function()
		fx, fy, tx, ty = round(fx), round(fy), round(tx), round(ty)
		local dx = tx - fx
		local dy = ty - fy
		if dx == 0 and dy == 0 then -- source and destination are the same
			yield(getTile(fx, fy))
			return
		end
		local sx = sign(dx)
		local sy = sign(dy)
		local x = fx
		local y = fy
		if abs(dx) > abs(dy) then
			while x ~= tx do
				yield(getTile(x, y))
				x = x + sx
				y = round(fy + (ty - fy) * (x - fx) / dx)
			end
		else
			while y ~= ty do
				yield(getTile(x, y))
				y = y + sy
				x = round(fx + (tx - fx) * (y - fy) / dy)
			end
		end
		yield(getTile(tx, ty))
	end)
end

function Line.getTiles(fx, fy, tx, ty)
	local tiles = {}
	fx, fy, tx, ty = round(fx), round(fy), round(tx), round(ty)
	local dx = tx - fx
	local dy = ty - fy
	if dx == 0 and dy == 0 then -- source and destination are the same
		tiles[1] = getTile(fx, fy)
		return tiles
	end
	local sx = sign(dx)
	local sy = sign(dy)
	local x = fx
	local y = fy
	if abs(dx) > abs(dy) then
		while x ~= tx do
			tiles[#tiles + 1] = getTile(x, y)
			x = x + sx
			y = round(fy + (ty - fy) * (x - fx) / dx)
		end
	else
		while y ~= ty do
			tiles[#tiles + 1] = getTile(x, y)
			y = y + sy
			x = round(fx + (tx - fx) * (y - fy) / dy)
		end
	end
	tiles[#tiles + 1] = getTile(tx, ty)
	return tiles
end

return Line
