local Creep, CreepConfig = unpack(require 'data/game/scripts/creep')
local buildings = require 'data/game/scripts/buildings'
local TileEffect = require 'data/game/scripts/tileeffect'
local tileData = require 'data/game/scripts/tiledata'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

local EntityModel = EntityModel
local Entity = Entity
local Widget = Widget
local Brush = Brush
local Tile = Tile
local math = math
local sin = math.sin
local min = math.min
local ceil = math.ceil
local floor = math.floor
local sqrt = math.sqrt
local atan = math.atan
local random = math.random
local pi = math.pi

local updatesPerSecond = CreepConfig.updatesPerSecond
local updatePeriod = 1 / updatesPerSecond

local Tower = {
	name                  = 'Tower',
	limitWidth            = 1,
	energyUpdatePerSecond = 30,
	numChildren           = 0,
	numParents            = 0,
	energyIncome          = 0
}
Tower.__index = Tower

function Tower:getType()
	return getmetatable(self)
end

function Tower:explode()
	Sound.playSample('explosion.wav')
	buildings[self.building] = nil
	Entity.destroy(self.building)
	for i = 1, random(8, 12) do
		local smoke = Entity.newAbstract('smoke', self.x, self.y)
		local x = self.x
		local y = self.y
		local z = random() - 0.5
		local dx = (random() - 0.5)
		local dy = (random() - 0.5)
		local dz = random() * 3
		local duration = random() * 2 + 1
		Entity.setZ(smoke, z)
		local color = { 255, 255, 255, 255 }
		Timer.new(
			0.02,
			true,
			function(timer, age)
				if age < duration then
					local d = age / duration
					Entity.setPosition(smoke, x + dx * d, y + dy * d)
					Entity.setZ(smoke, z + dz * d)
					color[4] = 255 * (1 - d)
					Entity.setColor(smoke, color)
				else
					Entity.disableOverlay(smoke)
					Entity.destroy(smoke)
					Timer.setLoops(timer, false)
				end
			end
		)
	end
end

function Tower:setBuilding()
	local tile = Map.getTile(self.x, self.y)
	self.building = Tile.getBuilding(tile)
	if not self.building then
		self.building = Entity.newBuilding(self.buildingModel, self.x, self.y)
	end
	buildings[self.building] = self
end

function Tower:setOverlay()
	local model = EntityModel.getByName(self.buildingModel)
	local textureWidth, textureHeight = EntityModel.getTextureDimensions(model)
	local overlay = Entity.enableOverlay(self.building)
	local energyBarBackground = Widget.new {
		image = 'data/game/interface/energybarbackground.png',
		parent = overlay,
		anchor = Widget.CENTERX + Widget.CENTERY,
		position = {
			x = 0,
			y = textureHeight / 2 + 8
		}
	}
	local energyBar = Widget.new {
		image = 'data/game/interface/energybar.png',
		parent = energyBarBackground,
		anchor = Widget.TOP + Widget.LEFT,
		position = {
			x = 3,
			y = -4
		}
	}
	Widget.setDimensions(energyBar, { width = 0, height = 0 })
	self.energyBar = energyBar
end

function Tower:updateEnergyBar()
	Widget.setDimensions(self.energyBar, { width = 90 * self.energy / self.maxEnergy, height = 6 })
	Widget.setTextureCoords(self.energyBar, 0, 1, self.energy / self.maxEnergy, 1)
end

function Tower:getBrush()
	local brush = Brush.new('disc')
	brush:setSize(self.radius)
	brush:setLimitWidth(self.limitWidth)
	return brush
end

function Tower:removeResource()
	local tile = Map.getTile(ceil(self.x), ceil(self.y))
	local resource = tileData[tile].resource
	if resource then
		local color = { 255, 255, 255, 255 }
		Timer.new(
			0.02,
			true,
			function(timer, age)
				if age < 0.5 then
					color[4] = (1 - age * 2) * 255
					Entity.setColor(resource, color)
				else
					tileData[tile].resource = nil
					Entity.destroy(resource)
					Timer.setLoops(timer, false)
				end
			end
		)
	end
end

function Tower:addLink(child)
	local sX, sY = Entity.getAbsolutePosition(self.building)
	local dX, dY = Entity.getAbsolutePosition(child.building)
	local ssX, ssY = self:getLinkShift()
	local sdX, sdY = child:getLinkShift()
	sX = sX + ssX
	sY = sY + ssY
	dX = dX + sdX
	dY = dY + sdY
	local length = sqrt((sX - dX) ^ 2 + (sY - dY) ^ 2)
	local mapOverlay = Map.getOverlay()
	local w = Widget.new {
		image = 'data/game/interface/link.png',
		anchor = Widget.CENTERX + Widget.CENTERY,
		parent = mapOverlay,
		position = {
			x = (sX + dX) / 2,
			y = (sY + dY) / 2
		}
	}
	Widget.setDimensions(w, { width = length, height = 15 })
	local phi = atan((dY - sY) / (dX - sX));
	if dX - sX < 0 then
		phi = phi + pi
	end
	Widget.setAngle(w, phi)
	self.childrenLinks[child] = w
	child.parentLinks[self] = w
	Tower.mainTower:updateEnergyIncome()
end

function Tower:removeLink(child)
	Widget.destroy(self.childrenLinks[child])
	self.childrenLinks[child] = nil
	child.parentLinks[self] = nil
	Tower.mainTower:updateEnergyIncome()
end

function Tower:addChild(child)
	self.children[child] = true
	child.parents[self] = true
	self.numChildren = self.numChildren + 1
	child.numParents = child.numParents + 1
	self:addLink(child)
end

function Tower:removeChild(child)
	self.children[child] = nil
	child.parents[self] = nil
	self.numChildren = self.numChildren - 1
	child.numParents = child.numParents - 1
	self:removeLink(child)
	if child.numParents == 0 then
		child.energyIncome = 0
	end
end

function Tower:removeAllChildren()
	self.numChildren = 0
	for child in pairs(self.children) do
		self.children[child] = nil
		child.parents[self] = nil
		child.numParents = child.numParents - 1
		self:removeLink(child)
		if child.numParents == 0 then
			child.energyIncome = 0
		end
	end
end

function Tower:isChild(tower)
	return self.children and self.children[tower]
end

function Tower:addParent(parent)
	self.parents[parent] = true
	parent.children[self] = true
	parent.numChildren = parent.numChildren + 1
	self.numParents = self.numParents + 1
	parent:addLink(self)
end

function Tower:removeParent(parent)
	self.parents[parent] = nil
	parent.children[self] = nil
	parent.numChildren = parent.numChildren - 1
	self.numParents = self.numParents - 1
	parent:removeLink(self)
	if self.numParents == 0 then
		self.energyIncome = 0
	end
end

function Tower:removeAllParents()
	self.energyIncome = 0
	self.numParents = 0
	for parent in pairs(self.parents) do
		self.parents[parent] = nil
		parent.children[self] = nil
		parent.numChildren = parent.numChildren - 1
		parent:removeLink(self)
	end
end

function Tower:isParent(tower)
	return self.parents[tower]
end

function Tower:hasSameParent(tower)
	for parent in pairs(self.parents) do
		if tower:isParent(parent) then
			return true
		end
	end
	return false
end

function Tower:clearNetwork()
	if self.children then
		for child in pairs(self.children) do
			child:clearNetwork()
		end
		self:removeAllChildren()
	end
end

function Tower:getRadius()
	return self.radius
end

function Tower:getEnergyText()
	return floor(self.energy)..' / '..self.maxEnergy
end

function Tower:getEnergyIncomeText()
	return floor(self.energyIncome)
end

function Tower:getLinks()
	local numParents
	local numChildren
	
	if self.numParents == 1 then
		numParents = '1 fournisseuse'
	elseif self.numParents > 1 then
		numParents = self.numParents..' fournisseuses'
	end
	
	if self.numChildren == 1 then
		numChildren = '1 alimentée'
	elseif self.numChildren > 1 then
		numChildren = self.numChildren..' alimentées'
	end
	
	if numParents and numChildren then
		return numParents..', '..numChildren
	elseif numParents then
		return numParents
	elseif numChildren then
		return numChildren
	else
		return 'aucune'
	end
end

function Tower:updateEnergyIncome()
	self:clearEnergyIncome()
	self:setEnergyIncome()
	self:clearLinks()
	self:updateLinks()
end

function Tower:clearEnergyIncome()
	self.energyIncome = 0
	if self.children then
		for child in pairs(self.children) do
			child:clearEnergyIncome()
		end
	end
end

function Tower:getEnergyIncome()
	return self.energyIncome
end

function Tower:clearLinks()
	if self.childrenLinks then
		for child, childLink in pairs(self.childrenLinks) do
			Widget.setTexture(childLink, 'data/game/interface/link.png')
			child:clearLinks()
		end
	end
end

function Tower:updateLinks()
	if self.energy < self.maxEnergy and self.energyIncome < self:getRequiredEnergyIncome() then
		for parent, parentLink in pairs(self.parentLinks) do
			Widget.setTexture(parentLink, 'data/game/interface/deadlink.png')
		end
	end
	if self.childrenLinks then
		for child, childLink in pairs(self.childrenLinks) do
			child:updateLinks()
		end
	end
end

function Tower:wave(waveDuration, waveHeight, waveRadius)
	local sx = self.x
	local sy = self.y
	local waveTiles = {}
	local brush = Brush.new('disc')
	brush:setSize(8)
	local brushTiles = brush:getTiles(sx, sy)
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
			local oldAge = age
			local oldD
			local d = 0
			while age < waveDuration do
				oldD, d = d, age / waveDuration
				local brush = Brush.new('disc')
				brush:setSize(d * waveRadius)
				local brushTiles = brush:getTiles(sx, sy)
				for tile, effect in pairs(brushTiles) do
					local e = 1 - effect
					local tileContamination = tileData[tile]
					if e < tileContamination.level then
						tileContamination.level = e
						if e < tileContamination.max then
							tileContamination.max = e
						end
						TileEffect.updateColor(tile)
					end
				end
				if d > 0.5 then
					d = 1 - d
				end
				for k, t in pairs(waveTiles) do
					local distance = t.distance
					local dz = ((-sin(-age * 10 + distance) * d) - (-sin(-oldAge * 10 + distance) * oldD)) * waveHeight * t.effect * (distance / waveRadius)
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

function Tower.highlightAdvisedTiles()
	TileEffect.clearHighlight()
	for _, energy1 in pairs(Map.getEntities('energy1')) do
		if buildings[energy1] then
			TileEffect.highlight(buildings[energy1])
		end
	end
	TileEffect.highlight(Tower.mainTower)
	for _, resource in pairs(Map.getEntities('resource')) do
		local x, y = Entity.getPosition(resource)
		local tile = Map.getTile(x, y)
		TileEffect.setTileHighlighting(tile, 0)
	end
end

function Tower.validPosition(x, y)
	local tile = Map.getTile(x, y)
	return tileData[tile].level == 0
end

return Tower
