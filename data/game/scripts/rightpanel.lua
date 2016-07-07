local Container = require 'data/scripts/container'
local InterfaceConfig = require 'data/scripts/interfaceconfig'
local Money = require 'data/game/scripts/money'
local towers = require 'data/game/scripts/towers'
local buildings = require 'data/game/scripts/buildings'
local TileEffect = require 'data/game/scripts/tileeffect'
local splitLine = require 'data/scripts/splitline'

local Map = Map
local Widget = Widget
local math = math

local rightPanel = Widget.new {
	anchor = Widget.RIGHT + Widget.CENTERY
}

local moneyContainer = Container:new(
	InterfaceConfig.container,
	rightPanel,
	Widget.TOP + Widget.LEFT + Widget.RIGHT,
	{ width = 0, height = 30 },
	16
)
local moneyWidget = Widget.new {
	text = '0£',
	font = InterfaceConfig.font..'@20',
	color = InterfaceConfig.fontColor,
	parent = moneyContainer.widget,
	anchor = Widget.CENTERX + Widget.CENTERY,
}

local buildingsContainer = Container:new(
	InterfaceConfig.container,
	rightPanel,
	Widget.TOP + Widget.LEFT + Widget.RIGHT,
	{ width = 0, height = 0 },
	16
)

local containerWidth = 0
local containerHeight = 0

local padding = 40
local costPosition = 5

local buildingPanels = {}
local y = 0
for _, Tower in pairs(towers) do
	if Tower.cost > 0 then
		local model = EntityModel.getByName(Tower.buildingModel)
	
		local widget = Widget.new {
			parent = buildingsContainer.widget,
			anchor = Widget.TOP + Widget.LEFT + Widget.RIGHT,
			position = {
				x = 0,
				y = y
			}
		}
		
		if Tower.description then
			local popup = Container:new(
				InterfaceConfig.container,
				widget,
				Widget.LEFT + Widget.CENTERY,
				{ width = 0, height = 0 },
				8
			)
			local maxW = 0
			local y = -5
			local lines = splitLine(Tower.description, 12)
			for i = 1, #lines do
				local line = lines[i]
				local popupText = Widget.new {
					parent = popup.widget,
					text = line,
					font = InterfaceConfig.font..'@14',
					color = InterfaceConfig.fontColor,
					anchor = Widget.LEFT + Widget.TOP,
					position = { x = 5, y = y }
				}
				local w, h = Widget.getDimensions(popupText)
				if w > maxW then
					maxW = w
				end
				y = y - h
			end
			
			Widget.setRequireMouseOverParent(popup.widget, true)
			popup:setDimensions{ width = maxW + 10, height = -y + 5 }
			popup:setPosition{ x = -maxW, y = 0 }
		end
		
		local icon = Widget.new {
			image = EntityModel.getTexturePath(model),
			anchor = Widget.CENTERX + Widget.CENTERY,
			parent = widget
		}
		
		local costWidget = Widget.new {
			text = Tower.cost..'£',
			font = InterfaceConfig.font..'@16',
			color = InterfaceConfig.fontColor,
			parent = widget,
			anchor = Widget.CENTERX + Widget.BOTTOM,
			position = { x = 0, y = costPosition }
		}
		
		local w, h = Widget.getDimensions(icon)
		Widget.setDimensions(icon, { width = w / 2, height = h / 2 })
		Widget.setDimensions(widget, { width = w / 2 + padding, height = h / 2 + padding })
		
		local function onClick()
			TileEffect.setHighlightColor{ 32, 192, 32 }
			Tower.highlightAdvisedTiles()
			Map.setGhostModel(model)
			Map.onBuild(function(m, x, y)
				if Tower.validPosition(x, y) then
					if Money.enough(Tower.cost) then
						Money.sub(Tower.cost)
						Entity.newBuilding(Tower.buildingModel, math.ceil(x), math.ceil(y))
						local tower = Tower:new(x, y)
						tower:start()
						Sound.playSample('build.wav')
						Tower.mainTower:connect()
						Tower.highlightAdvisedTiles()
					end
				end
			end)
		end
		Widget.onClick(widget, onClick)
	
		if containerWidth < w / 2 + padding then
			containerWidth = w / 2 + padding
		end
	
		y = y - h / 2 - padding
		containerHeight = containerHeight + h / 2 + padding
		
		buildingPanels[#buildingPanels + 1] = {
			onClick = onClick,
			panel = widget,
			costWidget = costWidget,
			Tower = Tower
		}
	end
end

Map.onGhostModelClear(TileEffect.clearHighlight)

if containerWidth < 100 then
	containerWidth = 100
end
moneyContainer:setDimensions { width = containerWidth, height = 40 }
buildingsContainer:setDimensions { width = containerWidth, height = containerHeight }
buildingsContainer:setPosition { x = 0, y = -50 }
Widget.setDimensions(rightPanel, { width = containerWidth, height = containerHeight + 50 })

function Money.updateWidget(amount)
	amount = amount or Money.getAmount()
	Widget.setText(moneyWidget, amount..'£')
	for _, buildingPanel in pairs(buildingPanels) do
		local cost = buildingPanel.Tower.cost
		Widget.setText(buildingPanel.costWidget, cost..'£')
		if amount < cost then
			Widget.setColor(buildingPanel.costWidget, InterfaceConfig.fontStrongColor)
			Widget.onClick(buildingPanel.panel, nil)
		else
			Widget.setColor(buildingPanel.costWidget, InterfaceConfig.fontColor)
			Widget.onClick(buildingPanel.panel, buildingPanel.onClick)
		end
	end
end

Money.setAmount(0)
