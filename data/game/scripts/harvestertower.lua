local buildings = require 'data/game/scripts/buildings'
local Tower = require 'data/game/scripts/tower'
local Money = require 'data/game/scripts/money'
local tileData = require 'data/game/scripts/tiledata'
local TileEffect = require 'data/game/scripts/tileeffect'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

---------------------
local Brush = Brush
local Tile = Tile
local Map = Map
local Entity = Entity
local math = math
local coroutine = coroutine
---------------------

local HarvesterTower = setmetatable({
	name            = 'HarvesterTower',
	buildingModel   = 'harvester',
	cost            = 500
}, Tower)
HarvesterTower.__index = HarvesterTower

function HarvesterTower:new(x, y)
	local o = setmetatable({
		x = x,
		y = y,
		parents = {},
		children = {},
	}, self)
	o:setBuilding()
	o:setOverlay()
	self.cost = math.ceil(self.cost * 1.1)
	Money.updateWidget()
	return o
end

function HarvesterTower:setOverlay()
	local overlay = Entity.enableOverlay(self.building)
	local font = InterfaceConfig.font..'@18'
	local string = '+'..self:getHarvest()..'£'
	self.harvestAmountShadow = Widget.new {
		text = string,
		font = font,
		color = InterfaceConfig.black,
		parent = overlay,
		anchor = Widget.CENTERX + Widget.CENTERY,
		position = {
			x = 2,
			y = 28
		}
	}
	self.harvestAmount = Widget.new {
		text = string,
		font = font,
		color = InterfaceConfig.gold,
		parent = overlay,
		anchor = Widget.CENTERX + Widget.CENTERY,
		position = {
			x = 0,
			y = 30
		}
	}
	Widget.setOpacity(self.harvestAmount, 0)
	Widget.setOpacity(self.harvestAmountShadow, 0)
	Widget.hide(self.harvestAmount)
	Widget.hide(self.harvestAmountShadow)
end

function HarvesterTower:destroy()
	self:explode()
	self.harvestAmount = nil
	self.harvestAmountShadow = nil
	Money.removeHarvester(self)
end

function HarvesterTower:start()
	self:removeResource()
	Money.addHarvester(self)
end

function HarvesterTower:getBrush()
	local brush = Brush.new('disc')
	brush:setSize(1)
	brush:setLimitWidth(0)
	return brush
end

function HarvesterTower:getHarvest()
	return 20
end

function HarvesterTower:animateHarvest()
	local position = { x = 0, y = 30 }
	Widget.setPosition(self.harvestAmount, position)
	position.x = 2
	position.y = 28
	Widget.setPosition(self.harvestAmountShadow, position)
	Widget.show(self.harvestAmount)
	Widget.show(self.harvestAmountShadow)
	Timer.new(
		0.02,
		true,
		function(timer, age)
			if not self.harvestAmount then
				Timer.setLoops(timer, false)
			elseif age < 0.6 then
				position.x = 0
				position.y = 30 + age * 30
				Widget.setPosition(self.harvestAmount, position)
				position.x = 2
				position.y = 28 + age * 30
				Widget.setPosition(self.harvestAmountShadow, position)
				local opacity
				if age < 0.1 then
					opacity = age * 10
				else
					opacity = 1 - (age - 0.1) * 2
				end
				Widget.setOpacity(self.harvestAmount, opacity)
				Widget.setOpacity(self.harvestAmountShadow, opacity)
			else
				Widget.hide(self.harvestAmount)
				Widget.hide(self.harvestAmountShadow)
				Timer.setLoops(timer, false)
			end
		end
	)
end

function HarvesterTower.validPosition(x, y)
	local tile = Map.getTile(x, y)
	return tileData[tile].level == 0 and tileData[tile].resource ~= nil
end

function HarvesterTower.highlightAdvisedTiles()
	TileEffect.clearHighlight()
	for _, resource in pairs(Map.getEntities('resource')) do
		local x, y = Entity.getPosition(resource)
		local tile = Map.getTile(x, y)
		TileEffect.highlightTile(tile, 1)
	end
end

HarvesterTower.upgrades = {
	
}

HarvesterTower.fields = {
	{
		name = 'Récolte',
		valueMethod = HarvesterTower.getHarvest,
		unit = '£/s'
	}
}

return HarvesterTower
