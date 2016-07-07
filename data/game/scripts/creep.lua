local Line = require 'data/scripts/line'
local buildings = require 'data/game/scripts/buildings'
local tileData = require 'data/game/scripts/tiledata'
local TileEffect = require 'data/game/scripts/tileeffect'

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local sqrt = math.sqrt
local sin = math.sin
local min = math.min
local random = math.random
local coroutine = coroutine
---------------------

local Creep = {
	wavePeriod         = 1.5,
	waveDuration       = 1.4,
	waveHeight         = 1.3,
	contaminationSpeed = 2
}

local CreepConfig = {
	creeps = {},
	towers = {},
	updatesPerSecond = 30
}

local creeps = CreepConfig.creeps
local updatesPerSecond = CreepConfig.updatesPerSecond
local updatePeriod = 1 / updatesPerSecond

-- Creep class

function Creep:new(x, y, radius, limitWidth, params)
	local brush = Brush.new('disc')
	brush:setSize(radius)
	brush:setLimitWidth(limitWidth)
	local o = setmetatable({
		x = x,
		y = y,
		brush = brush,
		age = 0,
		spreading = false
	}, { __index = self })
	for key, value in pairs(params) do
		o[key] = value
	end
	creeps[o] = true
	return o
end

function Creep:destroy()
	creeps[self] = nil
	self:stopLoop()
end

function Creep:stopLoop(age)
	self.age = age
	if self.spreading then
		Timer.setLoops(self.spreadTimer, false)
		Timer.setLoops(self.waveTimer, false)
		self.spreading = false
		checkVictory()
	end
end

function Creep:start()
	self.started = true
	if not self.spreading then
		self:startTimers()
	end
end

function Creep:continue()
	if self.started and not self.spreading then
		self:startTimers()
	end
end

function Creep:startTimers()
	self.spreading = false
	local creepAge = self.age
	if self:spread(creepAge) then
		self.spreading = true
		self.spreadTimer = Timer.new(
			updatePeriod,
			true,
			function(timer, age)
				local totalAge = creepAge + age
				if not self:update(totalAge) or not self:spread(totalAge) then
					self:stopLoop(totalAge)
				end
			end
		)
		self:wave()
		self.waveTimer = Timer.new(
			self.wavePeriod,
			true,
			function()
				self:wave()
			end
		)
	end
end

function Creep:update(age)
	return true
end

function Creep:getTiles()
	local brushTiles = self.brush:getTiles(self.x, self.y)
	for tile, effect in pairs(brushTiles) do
		local maxEffect = self:linkedToCenter(tile, effect)
		if maxEffect == 0 then
			brushTiles[tile] = nil
		else
			brushTiles[tile] = maxEffect
		end
	end
	return brushTiles
end

function Creep:linkedToCenter(tile, effect)
	local tx, ty = Tile.getPosition(tile)
	local line = Line.getTiles(self.x, self.y, tx, ty)
	local max = effect
	for i = 2, #line - 1 do
		local lineTile = line[i]
		if lineTile == Tile.NIL or tileData[lineTile].level == 0 then
			max = 0
			break
		elseif tileData[lineTile].level < max then
			max = tileData[lineTile].level
		end
	end
	return max
end

function Creep:spread()
	local brushTiles = self:getTiles()
	local numContaminations = 0
	for tile, effect in pairs(brushTiles) do
		local contamination = tileData[tile].level
		if contamination < 1 and self:contaminateTile(tile, effect) then
			numContaminations = numContaminations + 1
		end
	end
	return numContaminations > 0
end

function Creep:wave()
	if not self.spreading then
		return
	end
	
	local radius = self.brush:getSize()
	local sx = self.x
	local sy = self.y
	local waveTiles = {}
	local brushTiles = self:getTiles()
	for tile, effect in pairs(brushTiles) do
		local x, y = Tile.getPosition(tile)
		waveTiles[#waveTiles + 1] = {
			tile = tile,
			effect = effect,
			distance = sqrt((sx - x) ^ 2 + (sy - y) ^ 2),
			dz = 0
		}
	end
	
	Timer.new(
		updatePeriod,
		true,
		coroutine.wrap(function(timer, age)
			local waveDuration = self.waveDuration
			local waveHeight = self.waveHeight
			local oldAge = age
			local oldD
			local d = 0
			while age < waveDuration do
				oldD, d = d, age / waveDuration
				if d > 0.5 then
					d = 1 - d
				end
				for k, t in pairs(waveTiles) do
					local distance = t.distance
					local dz = ((-sin(-age * 10 + distance) * d) - (-sin(-oldAge * 10 + distance) * oldD)) * waveHeight * t.effect * (1 - distance / radius) * tileData[t.tile].max
					t.dz = t.dz + dz
					local z = Tile.getZ(t.tile)
					Tile.setZ(t.tile, z + dz)
				end
				oldAge = age
				timer, age = coroutine.yield()
			end
			for _, t in pairs(waveTiles) do
				local z = Tile.getZ(t.tile)
				Tile.setZ(t.tile, z - t.dz)
			end
			Timer.setLoops(timer, false)
		end)
	)
end

function Creep:contaminateTile(tile, level)
	local tileContamination = tileData[tile]
	level = min(level, tileContamination.level + self.contaminationSpeed * updatePeriod, tileContamination.max)
	if level > tileContamination.level then
		TileEffect.updateColor(tile)
		tileContamination.level = level
		if level > 0.5 then
			local building = Tile.getBuilding(tile)
			if building and buildings[building] then
				buildings[building]:destroy()
			else
				for _, soldier in pairs(Tile.getEntities(tile)) do
					if Entity.isUnit(soldier) and Entity.isOnTheFloor(soldier) then
						Timer.new(
							0,
							false,
							function()
								local x, y, z = Entity.getPosition(soldier)
								Entity.destroy(soldier)
								Timer.new(
									0,
									false,
									function()
										local ghost = Entity.newAbstract('ghost', x, y)
										Entity.setZ(ghost, z)
										local ghostColor = { 255, 255, 255, 128 }
										Timer.new(
											0.02,
											true,
											function(timer, age)
												if age < 1 then
													ghostColor[4] = (1 - age) * 128
													Entity.setColor(ghost, ghostColor)
													Entity.setZ(ghost, z + age * 3)
												else
													Entity.destroy(ghost)
													Timer.setLoops(timer, false)
												end
											end
										)
									end
								)
							end
						)
					end
				end
			end
		end
		return true
	else
		return false
	end
end

function Creep.allStoped()
	for creep in pairs(creeps) do
		if creep.spreading then
			return false
		end
	end
	return true
end

function Creep.getRatio()
	local totalSurface = 0
	local creepedSurface = 0
	for tile, tileContamination in pairs(tileData) do
		if tileContamination and type(tileContamination.level) == 'number' then
			totalSurface = totalSurface + 1
			creepedSurface = creepedSurface + tileContamination.level
		end
	end
	return creepedSurface / totalSurface
end

return { Creep, CreepConfig }
