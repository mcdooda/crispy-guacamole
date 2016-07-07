local Creep, CreepConfig = unpack(require 'data/game/scripts/creep')
local buildings = require 'data/game/scripts/buildings'
local Tower = require 'data/game/scripts/tower'
local EnergyTower, EnergyConfig = unpack(require 'data/game/scripts/energytower')
local Windows = require 'data/game/scripts/windows'

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local math = math
local coroutine = coroutine
---------------------

local energyUpdatePerSecond = EnergyTower.energyUpdatePerSecond
local energyUpdatePeriod = 1 / energyUpdatePerSecond
local towers = CreepConfig.towers

local MainTower = setmetatable({
	name            = 'MainTower',
	buildingModel   = 'main',
	energy          = 1000,
	maxEnergy       = 1000,
	energyPerSecond = 200,
	radius          = 9,
	cost            = -1
}, EnergyTower)
MainTower.__index = MainTower

function MainTower:start()
	self:removeResource()
	Tower.mainTower = self
	self:connect()
	self.energizeTimer = Timer.new(
		energyUpdatePeriod,
		true,
		coroutine.wrap(function(timer, age)
			local oldAge
			local _
			local elapsedTime
			while true do
				oldAge = age
				_, age = coroutine.yield()
				elapsedTime = age - oldAge
				self:updateEnergyIncome()
				for child in pairs(towers) do
					child:spendEnergy(elapsedTime)
				end
				self:energizeRecursiveSuffix(elapsedTime)
				self.energy = self.maxEnergy
			end
		end)
	)
end

function MainTower:destroy()
	Windows.defeat('le quartier général a été détruit')
end

function MainTower:connect()
	self:clearNetwork()
	EnergyTower.connect(self)
end

function MainTower:setEnergyIncome()
	self.energyIncome = self.energyPerSecond
	EnergyTower.setEnergyIncome(self)
end

function MainTower:getLinkShift()
	return 7, 30
end

return { MainTower, EnergyConfig }
