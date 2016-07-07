local dump = require 'data/scripts/dump'
local TabPanel = require 'data/scripts/tabpanel'
local Container = require 'data/scripts/container'

local EntityModel = EntityModel
local EditorMap = EditorMap

-- left panel

local LeftPanel = {}

function LeftPanel:new()
	local o = { groups = {} }
	return setmetatable(o, { __index = self })
end

function LeftPanel:add(group, action, textureCoords, ratio)
	self.groups[#self.groups + 1] = {
		group = group,
		action = action,
		textureCoords = textureCoords,
		ratio = ratio
	}
end

function LeftPanel:getWidget()
	local lp = Widget.new {
		anchor = Widget.LEFT + Widget.BOTTOM + Widget.TOP,
		dimensions = {
			width = 150
		}
	}
	Widget.new {
		anchor = Widget.LEFT + Widget.TOP,
		parent = lp
	}
	local lpSelection = Container:new('data/editor/interface/selection', lp, Widget.LEFT + Widget.TOP, { width = 30, height = 30 }, 16)
	local function setSelection(button)
		if button then
			local x, y = Widget.getPosition(button)
			local w, h = Widget.getDimensions(button)
			lpSelection:setPosition {
				x = x - 3,
				y = y + 3
			}
			lpSelection:setDimensions {
				width = w + 6,
				height = h + 6
			}
		else
			lpSelection:hide()
		end
	end
	local y = -29
	local firstWidget
	for i = 1, #self.groups do
		local group = self.groups[i]
		local groupWidget = Widget.new {
			parent = lp,
			position = {
				x = 10,
				y = y
			},
			anchor = Widget.LEFT + Widget.TOP
		}
		if i == 1 then
			firstWidget = groupWidget
		end
		Widget.onClick(groupWidget, function(button)
			setSelection(button)
			group.action(button)
		end)
		local groupHeight = 0
		for j = 1, #group.group do
			local w = Widget.new {
				parent = groupWidget,
				image = group.group[j],
				anchor = Widget.LEFT + Widget.TOP
			}
			if group.textureCoords then
				Widget.setTextureCoords(w, unpack(group.textureCoords))
			end
			local width, height = Widget.getDimensions(w)
			width = width / 3
			height = height / 3
			if group.ratio then
				width = width * group.ratio
				height = height * group.ratio
			end
			local dx = (130 - width) / (#group.group - 1)
			if dx > width * 0.8 then
				dx = width * 0.8
			end
			local x = (j - 1) * dx
			Widget.setPosition(w, {
				x = x,
				y = 0
			})
			Widget.setDimensions(w, {
				width = width,
				height = height
			})
			if height > groupHeight then
				groupHeight = height
			end
		end
		Widget.setDimensions(groupWidget, {
			width = 130,
			height = groupHeight
		})
		y = y - 10 - groupHeight
	end
	setSelection(firstWidget)
	Widget.hide(lp)
	return lp
end

-----

local lp, tp

tp = TabPanel:new(
	'data/editor/interface/leftpanel.png',
	'data/editor/interface/leftpaneltabs.png',
	nil, -- parent = screen
	Widget.TOP + Widget.LEFT + Widget.BOTTOM,
	{ x = 0, y = 0 },
	{ width = 150 }
)

-- tiles widget
lp = LeftPanel:new()

local f = File.new(Mod.getResourcePath("tiles"))
local tileGroups = f:read()

for groupPath, tileGroup in pairs(tileGroups) do
	local tiles = {}
	for tile, _ in pairs(tileGroup) do
		tiles[#tiles + 1] = tile
	end
	lp:add(tiles, function(button)
		EditorMap.setTilesGroup(groupPath)
	end)
end

EditorMap.setTilesGroup(next(tileGroups)) -- first enabled

tp:addTab(
	'tiles',
	'data/editor/interface/tilestab',
	lp:getWidget(),
	function()
		EditorMap.setMode(EditorMap.MODE_TILES)
	end
)

-- doodads widget
lp = LeftPanel:new()

local f = File.new(Mod.getResourcePath("doodads"))
local doodadGroups = f:read()

for groupPath, doodadGroup in pairs(doodadGroups) do
	local doodads = {}
	for doodad, _ in pairs(doodadGroup) do
		doodads[#doodads + 1] = doodad
	end
	lp:add(doodads, function(button)
		EditorMap.setDoodadsGroup(groupPath)
	end)
end

EditorMap.setDoodadsGroup(next(doodadGroups)) -- first enabled

tp:addTab(
	'doodads',
	'data/editor/interface/doodadstab',
	lp:getWidget(),
	function()
		EditorMap.setMode(EditorMap.MODE_DOODADS)
	end
)

-- LOADING MODELS
local unitModels = {}
local buildingModels = {}

for modelName, model in pairs(EntityModel.getAll()) do
	if EntityModel.isUnit(model) then
		unitModels[modelName] = model
	elseif EntityModel.isBuilding(model) then
		buildingModels[modelName] = model
	end
end

-- buildings widget
lp = LeftPanel:new()

for modelName, buildingModel in pairs(buildingModels) do
	lp:add({ EntityModel.getTexturePath(buildingModel) }, function(button)
		EditorMap.setBuildingModel(buildingModel)
	end)
end

tp:addTab(
	'buildings',
	'data/editor/interface/buildingstab',
	lp:getWidget(),
	function()
		EditorMap.setMode(EditorMap.MODE_BUILDINGS)
	end
)

if next(buildingModels) then
	EditorMap.setBuildingModel(buildingModels[next(buildingModels)]) -- first enabled
end

-- units widget
lp = LeftPanel:new()

for modelName, unitModel in pairs(unitModels) do
	lp:add({ EntityModel.getTexturePath(unitModel) }, function(button)
		EditorMap.setUnitModel(unitModel)
	end, {0, 1 / 8, 1 / 8, 0}, 1 / 4)
end

tp:addTab(
	'units',
	'data/editor/interface/unitstab',
	lp:getWidget(),
	function()
		EditorMap.setMode(EditorMap.MODE_UNITS)
	end
)

if next(unitModels) then
	EditorMap.setUnitModel(unitModels[next(unitModels)]) -- first enabled
end

-- zones widget
--[[
lp = LeftPanel:new()

tp:addTab(
	'zones',
	'data/editor/interface/zonestab',
	lp:getWidget(),
	function()
		EditorMap.setMode(EditorMap.MODE_ZONES)
	end
)
]]

-- opening the 1st tab
tp:openTab('tiles')
