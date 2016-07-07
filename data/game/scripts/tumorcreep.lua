local round = require 'data/scripts/round'

local MovingCreep, CreepConfig = unpack(require 'data/game/scripts/movingcreep')

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local coroutine = coroutine
---------------------

local tiles = CreepConfig.tiles
local creeps = CreepConfig.creeps
local updatesPerSecond = CreepConfig.updatesPerSecond
local updatePeriod = 1 / updatesPerSecond

local TumorCreep = setmetatable({}, { __index = MovingCreep })

function TumorCreep:new(x, y, radiusSpeed, maxSize)
	local o = MovingCreep:new(
		x,
		y,
		1, -- radius
		1, -- limit width
		{ radiusSpeed = radiusSpeed, xSpeed = 0, ySpeed = 0, maxSize = maxSize }
	)
	setmetatable(o, { __index = self })
	return o
end

function TumorCreep:update(age)
	self.x = self.x + self.xSpeed * updatePeriod
	self.y = self.y + self.ySpeed * updatePeriod
	local sx, sy = self.x, self.y
	local size = self.brush:getSize()
	size = size + self.radiusSpeed * updatePeriod
	if size > self.maxSize then
		size = self.maxSize
		local sizeMinusOne = math.max(0.6, size - 1)
		self.brush:setSize(size)
		local newCreeps = {}
		for i = 1, 5 do
			local n = 1
			local tile = Tile.NIL
			while n <= 20 do
				if tile ~= Tile.NIL and not newCreeps[tile] and self:linkedToCenter(tile, 1) > 0.2 then
					break
				else
					local phi = math.random() * 2 * math.pi
					local x, y = round(sx + math.cos(phi) * sizeMinusOne), round(sy + math.sin(phi) * sizeMinusOne)
					tile = Map.getTile(x, y)
					n = n + 1
				end
			end
			if tile ~= Tile.NIL then
				newCreeps[tile] = true
			end
		end
		if next(newCreeps) then
			for tile in pairs(newCreeps) do
				local x, y = Tile.getPosition(tile)
				local maxSize = math.max(3, self.maxSize + math.random(-2, 2))
				TumorCreep:new(x, y, self.radiusSpeed, maxSize):start()
			end
		end
		return false
	else
		self.brush:setSize(size)
		return true
	end
end

return { TumorCreep, CreepConfig }
