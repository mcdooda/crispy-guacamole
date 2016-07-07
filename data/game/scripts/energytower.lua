local Creep, CreepConfig = unpack(require 'data/game/scripts/creep')
local buildings = require 'data/game/scripts/buildings'
local Tower = require 'data/game/scripts/tower'
local List = require 'data/scripts/list'

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local min = math.min
local max = math.max
local floor = math.floor
local coroutine = coroutine
---------------------

local EnergyTower = setmetatable({
	name            = 'EnergyTower',
	buildingModel   = 'energy1',
	energy          = 0,
	maxEnergy       = 100,
	energyPerSecond = 100,
	radius          = 9,
	cost            = 300
}, Tower)
EnergyTower.__index = EnergyTower

local EnergyConfig = {}

function EnergyTower:new(x, y)
	local o = setmetatable({
		x = x,
		y = y,
		parents = {},
		children = {},
		childrenLinks = {},
		parentLinks = {}
	}, self)
	o:setBuilding()
	o:setOverlay()
	return o
end

function EnergyTower:destroy()
	self:explode()
	self:removeAllParents()
	self:removeAllChildren()
	checkDefeat()
end

function EnergyTower:connect()
	local l = List:new()
	l:pushLeft(self)
	while not l:isEmpty() do
		local tower = l:popRight()
		local brush = tower:getBrush()
		local entities = brush:getEntities(tower.x, tower.y, 0)
		for i = 1, #entities do
			local entity = entities[i]
			if entity ~= tower.building and buildings[entity] then
				local tower2 = buildings[entity]
				if not tower:isParent(tower2) and not tower:isChild(tower2) and not tower:hasSameParent(tower2) then
					if tower2.name ~= 'MainTower' and tower2.name ~= 'HarvesterTower' and (tower2:getType() ~= EnergyTower or tower2.numParents == 0) then
						tower:addChild(tower2)
						if tower2:getType() == EnergyTower then
							l:pushLeft(tower2)
						end
					end
				end
			end
		end
	end
end

function EnergyTower:start()
	self:removeResource()
	self:updateEnergyBar()
end

function EnergyTower:energizeRecursiveSuffix(elapsedTime)
	for child in pairs(self.children) do
		if child.energize then
			child:energizeRecursiveSuffix(elapsedTime)
		end
	end
	self:energizeChildren(elapsedTime)
	self:updateEnergyBar()
end

function EnergyTower:energizeChildren(elapsedTime)
	--[[
	local amount = math.min(self.energy, self.energyPerSecond * elapsedTime) / self.numChildren
	for child in pairs(self.children) do
		self:energize(child, amount)
	end
	]]
	local children = {}
	local numChildren = 0
	for child in pairs(self.children) do
		if not (child:getType() == EnergyTower and child.numChildren == 0) then
			children[child] = true
			numChildren = numChildren + 1
		end
	end
	local total = math.min(self.energy, self.energyPerSecond * elapsedTime)
	repeat
		local amount = total / numChildren
		local totalBonus = 0
		for child in pairs(children) do
			local bonus = self:energize(child, amount)
			child:updateEnergyBar()
			if bonus > 0 then
				totalBonus = totalBonus + bonus
				children[child] = nil
				numChildren = numChildren - 1
			end
		end
		total = totalBonus
	until total == 0 or numChildren == 0
end

function EnergyTower:energize(tower, amount)
	local initAmount = amount
	amount = math.min(amount, self.energy, tower.maxEnergy - tower.energy)
	self.energy = self.energy - amount
	tower.energy = tower.energy + amount
	return initAmount - amount
end

function EnergyTower:getRequiredEnergyIncome()
	local requiredIncome = 0
	for child in pairs(self.children) do
		requiredIncome = requiredIncome + child:getRequiredEnergyIncome() / child.numParents
	end
	return requiredIncome
end

function EnergyTower:getNumNonFullChildren()
	local numChildren = 0
	for child in pairs(self.children) do
		if child.energy < child.maxEnergy then
			numChildren = numChildren + 1
		end
	end
	return numChildren
end

function EnergyTower:setEnergyIncome()
	if self.name ~= 'MainTower' then
		local income = 0
		for parent in pairs(self.parents) do
			local numChildren = parent:getNumNonFullChildren()
			if numChildren == 0 then
				numChildren = parent.numChildren
			end
			income = income + parent:getEnergyDebit() / numChildren
		end
		self.energyIncome = income
	end
	for child in pairs(self.children) do
		child:setEnergyIncome()
	end
end

function EnergyTower:getEnergyDebit()
	return min(self.energyPerSecond, self.energyIncome)
end

--[[
function EnergyTower:setEnergyIncome()
	local numChildren = 0
	for child in pairs(self.children) do
		if child.energy < child.maxEnergy then
			numChildren = numChildren + 1
		end
	end
	if numChildren > 0 then
		local income = self.energyIncome / numChildren
		for child in pairs(self.children) do
			if child.energy < self.maxEnergy then
				child.energyIncome = child.energyIncome + income
			end
			child:setEnergyIncome()
		end
	else
		for parent in pairs(self.parents) do
			self.energyIncome = self.energyIncome + parent.energyIncome / parent.numChildren
		end
	end
end
]]

function EnergyTower:getRadius()
	return self.radius
end

function EnergyTower:upgradeDebit()
	self.energyPerSecond = floor(self.energyPerSecond * 1.2)
	Tower.mainTower:updateEnergyIncome()
end

function EnergyTower:upgradeDebitCost()
	return 70
end

function EnergyTower:getUpgradedDebit()
	return floor(self.energyPerSecond * 1.2)
end

function EnergyTower:getDebit()
	return self.energyPerSecond
end

function EnergyTower:getLinkShift()
	return 0, 40
end

EnergyTower.upgrades = {
	{
		name = 'Débit+',
		upgradeMethod = EnergyTower.upgradeDebit,
		costMethod = EnergyTower.upgradeDebitCost,
		description = 'Augmentation du débit',
		newValueMethod = EnergyTower.getUpgradedDebit
	}
}

EnergyTower.fields = {
	{
		name = 'Energie',
		valueMethod = EnergyTower.getEnergyText
	},
	{
		name = 'Débit maximal',
		valueMethod = EnergyTower.getDebit,
		displayBounds = true
	},
	{
		name = 'Revenu',
		valueMethod = EnergyTower.getEnergyIncomeText,
		displayBounds = true
	},
	{
		name = 'Tours',
		valueMethod = EnergyTower.getLinks
	}
}

return { EnergyTower, EnergyConfig }
