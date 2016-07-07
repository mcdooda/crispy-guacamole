local Creep, CreepConfig = unpack(require 'data/game/scripts/creep')

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local coroutine = coroutine
---------------------

local updatesPerSecond = CreepConfig.updatesPerSecond
local updatePeriod = 1 / updatesPerSecond

local MovingCreep = setmetatable({}, { __index = Creep })

function MovingCreep:new(...)
	return setmetatable(Creep:new(...), { __index = self })
end

function MovingCreep:update(age)
	self.brush:growSize(self.radiusSpeed * updatePeriod)
	self.x = self.x + self.xSpeed * updatePeriod
	self.y = self.y + self.ySpeed * updatePeriod
	return true
end

return { MovingCreep, CreepConfig }
