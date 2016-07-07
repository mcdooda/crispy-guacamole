local Creep, CreepConfig = unpack(require 'data/game/scripts/creep')
local tileData = require 'data/game/scripts/tiledata'
local buildings = require 'data/game/scripts/buildings'
local Tower = require 'data/game/scripts/tower'

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local ceil = math.ceil
local coroutine = coroutine
---------------------

local creeps = CreepConfig.creeps
local towers = CreepConfig.towers

local DefenseTower = setmetatable({
	name            = 'DefenseTower',
	buildingModel   = 'defense1',
	energy          = 0,
	maxEnergy       = 100,
	radius          = 5,
	limitWidth      = 2,
	energyPerSecond = 40,
	cost            = 100
}, Tower)
DefenseTower.__index = DefenseTower

-- DefenseTower class

function DefenseTower:new(x, y)
	local o = setmetatable({
		x = x,
		y = y,
		parents = {},
		parentLinks = {},
		tiles = {},
		defending = false
	}, self)
	o:setBuilding()
	o:setOverlay()
	towers[o] = true
	return o
end

function DefenseTower:start()
	self:removeResource()
	self:defend()
end

function DefenseTower:spendEnergy(elapsedTime)
	self.energy = self.energy - self.energyPerSecond * elapsedTime
	if self.energy <= 0 then
		self.energy = 0
	end
	if self.energy * 20 <= self.maxEnergy then -- energy < 5%
		self:stopDefending()
	else
		self:forceDefend()
	end
	self:updateEnergyBar()
end

function DefenseTower:destroy()
	self:stopDefending()
	self:explode()
	towers[self] = nil
	self:removeAllParents()
	checkDefeat()
end

function DefenseTower:forceDefend()
	local brush = Brush.new('disc')
	brush:setSize(self.radius)
	brush:setLimitWidth(self.limitWidth)

	local defendedTiles = brush:getTiles(self.x, self.y)
	for tile, effect in pairs(defendedTiles) do
		self:defendTile(tile, 1 - effect)
	end
end

function DefenseTower:defend()
	if not self.defending then
		self.defending = true
		self:forceDefend()
	end
end

function DefenseTower:stopDefending()
	if self.defending then
		self.defending = false
		
		local adjacentTowers = {}
		for tile, _ in pairs(self.tiles) do
			local tileContamination = tileData[tile]
			tileContamination.towers[self] = nil
			tileContamination.oldMax = tileContamination.max
			tileContamination.max = 1
			for tower, distance in pairs(tileContamination.towers) do
				adjacentTowers[tower] = true
			end
		end
	
		for tower in pairs(adjacentTowers) do
			tower:forceDefend()
		end
	
		local attackableTiles = {}
		for tile, _ in pairs(self.tiles) do
			local tileContamination = tileData[tile]
			if tileContamination.max > tileContamination.oldMax then
				attackableTiles[tile] = { tileContamination.oldMax, tileContamination.max }
			end
			tileContamination.max = tileContamination.oldMax
			tileContamination.oldMax = nil
		end
	
		Timer.new(
			coroutine.wrap(function(timer, age)
				while next(attackableTiles) do
					for tile, max in pairs(attackableTiles) do
						local newMax = max[1] + age / 2
						if newMax > max[2] then
							tileData[tile].max = max[2]
							attackableTiles[tile] = nil
						else
							tileData[tile].max = newMax
						end
					end
					timer, age = coroutine.yield()
				end
				Timer.setLoops(timer, false)
			end)
		)
		
		-- restarting creep
		Timer.new(
			0,
			false,
			function()
				for creep in pairs(creeps) do
					creep:continue()
				end
			end
		)
	end
end

function DefenseTower:defendTile(tile, max)
	self.tiles[tile] = max
	local tileX, tileY = Tile.getPosition(tile)
	local distance = math.sqrt((tileX - self.x) ^ 2 + (tileY - self.y) ^ 2)
	local tileContamination = tileData[tile]
	if max < tileContamination.max then
		tileContamination.max = max
	end
	tileContamination.towers[self] = distance
end

function DefenseTower:getRequiredEnergyIncome()
	return self.energyPerSecond
end

function DefenseTower:setEnergyIncome()
	local income = 0
	for parent in pairs(self.parents) do
		income = income + parent:getEnergyDebit() / parent.numChildren
	end
	self.energyIncome = income
end

function DefenseTower:upgradeEnergyCost()
	self.energyPerSecond = ceil(self.energyPerSecond * 0.7)
	Tower.mainTower:updateEnergyIncome()
end

function DefenseTower:upgradeEnergyCostCost()
	return 100
end

function DefenseTower:getEnergyPerSecond()
	return self.energyPerSecond
end

function DefenseTower:getUpgradedEnergyCost()
	return ceil(self.energyPerSecond * 0.7)
end

function DefenseTower:waveAttack()
	self:wave(2, 2, 10)
end

function DefenseTower:waveAttackCost()
	return 200
end

function DefenseTower:waveAttackEnergyCost()
	return 60
end

function DefenseTower:getLinkShift()
	return 0, 35
end

DefenseTower.upgrades = {
	{
		name = 'Conso-',
		upgradeMethod = DefenseTower.upgradeEnergyCost,
		costMethod = DefenseTower.upgradeEnergyCostCost,
		description = 'Baisse de la consommation',
		newValueMethod = DefenseTower.getUpgradedEnergyCost
	},
	{
		name = 'Attaque',
		upgradeMethod = DefenseTower.waveAttack,
		costMethod = DefenseTower.waveAttackCost,
		energyCostMethod = DefenseTower.waveAttackEnergyCost,
		description = 'Attaque en vague autour du bâtiment'
	}
}

DefenseTower.fields = {
	{
		name = 'Energie',
		valueMethod = DefenseTower.getEnergyText
	},
	{
		name = 'Consommation d\'énergie',
		valueMethod = DefenseTower.getEnergyPerSecond,
		displayBounds = true
	},
	{
		name = 'Revenu',
		valueMethod = DefenseTower.getEnergyIncomeText,
		displayBounds = true
	},
	{
		name = 'Tours',
		valueMethod = DefenseTower.getLinks
	}
}

return { DefenseTower, CreepConfig }
