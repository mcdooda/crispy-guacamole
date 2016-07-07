local Container = require 'data/scripts/container'
local buildings = require 'data/game/scripts/buildings'
local InterfaceConfig = require 'data/scripts/interfaceconfig'
local TileEffect = require 'data/game/scripts/tileeffect'
local Money = require 'data/game/scripts/money'

local container = Widget.new {
	image = 'data/game/interface/background.png',
	anchor = Widget.CENTERX + Widget.BOTTOM,
	pixelPerfect = true
}

local selectedEntitiesContainer = Container:new(
	InterfaceConfig.container,
	container,
	Widget.CENTERY + Widget.LEFT,
	{ width = 280, height = 87 },
	16
)
selectedEntitiesContainer:setPosition{ x = 25, y = 0 }

local dataContainer = Container:new(
	InterfaceConfig.container,
	container,
	Widget.CENTERY + Widget.LEFT,
	{ width = 353, height = 87 },
	16
)
dataContainer:setPosition{ x = 315, y = 0 }

local upgradesContainer = Container:new(
	InterfaceConfig.container,
	container,
	Widget.CENTERY + Widget.LEFT,
	{ width = 87, height = 87 },
	16
)
upgradesContainer:setPosition{ x = 678, y = 0 }

local selectedModelContainer = Container:new(
	InterfaceConfig.container,
	selectedEntitiesContainer.widget,
	Widget.TOP + Widget.LEFT,
	{ width = 50, height = 50 },
	8
)

local selectedEntitiesPanel = Widget.new {
	parent = selectedEntitiesContainer.widget,
	anchor = Widget.LEFT + Widget.TOP + Widget.RIGHT + Widget.BOTTOM
}

local dataPanel = Widget.new {
	parent = dataContainer.widget,
	anchor = Widget.LEFT + Widget.TOP + Widget.RIGHT + Widget.BOTTOM
}

local upgradesPanel = Widget.new {
	parent = upgradesContainer.widget,
	anchor = Widget.LEFT + Widget.TOP + Widget.RIGHT + Widget.BOTTOM
}

local function clearPanels()
	selectedModelContainer:hide()
	Widget.clear(dataPanel)
	Widget.clear(upgradesPanel)
	Widget.new {
		text = 'Aucune unité sélectionnée',
		font = InterfaceConfig.font..'@18',
		parent = selectedEntitiesPanel,
		color = InterfaceConfig.fontDarkColor,
		anchor = Widget.CENTERX + Widget.CENTERY
	}
end
clearPanels()

local updateFunc

Timer.new(
	0.2,
	true,
	function()
	if updateFunc then
		updateFunc()
	end
end)

Selection.onChange(function(entities)
	
	Widget.clear(selectedEntitiesPanel)
	updateFunc = nil
	
	-- counting the number of each entity type
	local entityTypes = {}
	for _, entity in pairs(entities) do
		local model = Entity.getModel(entity)
		if not entityTypes[model] then
			entityTypes[model] = { entity }
		else
			entityTypes[model][#entityTypes[model] + 1] = entity
		end
	end
	
	-- adding panel
	local panels = {}
	for model, entities in pairs(entityTypes) do
		local isUnit = EntityModel.isUnit(model)
		local numEntities = #entities
		local modelPanel = Widget.new {
			parent = selectedEntitiesPanel,
			anchor = Widget.LEFT + Widget.TOP + Widget.BOTTOM
		}
		-- entity image
		local image = Widget.new {
			image = EntityModel.getTexturePath(model),
			parent = modelPanel,
			anchor = Widget.CENTERX + Widget.CENTERY
		}
		if isUnit then
			Widget.setTextureCoords(image, 0, 1/8, 1/8, 0)
		end
		-- number of entities
		Widget.new {
			text = numEntities,
			font = InterfaceConfig.font..'@17',
			parent = modelPanel,
			color = InterfaceConfig.fontColor,
			anchor = Widget.TOP + Widget.LEFT,
			position = {
				x = 5,
				y = -5
			}
		}
		local w1, h1 = Widget.getDimensions(image)
		local w2, h2 = Widget.getDimensions(modelPanel)
		if isUnit then
			Widget.setDimensions(image, {
				width = w1 / 8,
				height = h1 / 8
			})
			Widget.setAnchor(modelPanel, Widget.TOP + Widget.LEFT)
			Widget.setDimensions(modelPanel, {
				width = w1 / 8 + 10,
				height = h2 - 10
			})
		else
			Widget.setDimensions(image, {
				width = w1 / 2.8,
				height = h1 / 2.8
			})
			Widget.setAnchor(modelPanel, Widget.TOP + Widget.LEFT)
			Widget.setDimensions(modelPanel, {
				width = w1 / 2.8 + 10,
				height = h2 - 10
			})
		end
		
		local tower = buildings[entities[1]]
		local upgrades, fields
		if tower then
			upgrades = tower.upgrades
			fields = tower.fields
		end
		
		local function displayData()
			Widget.clear(dataPanel)
			if fields then
				local y = -5
				for _, field in pairs(fields) do
					local minValue, minValueText, maxValue, maxValueText, lastValue
					for i = 1, numEntities do
						local value = field.valueMethod(buildings[entities[i]])
						lastValue = value
						local txtNumber = string.gsub(value, '^(%d+)', '%1')
						local numberValue = tonumber(txtNumber)
						if numberValue then
							if not minValue or numberValue < minValue then
								minValue = numberValue
								minValueText = value
							end
							if not maxValue or numberValue > maxValue then
								maxValue = numberValue
								maxValueText = value
							end
						end
					end
					local text
					if minValueText and (not field.displayBounds or minValueText == maxValueText) then
						text = field.name..' : '..minValueText
					elseif minValueText and maxValueText then
						text = field.name..' : '..minValueText..' à '..maxValueText
					else
						text = field.name..' : '..lastValue
					end
					if field.unit then
						text = text..' '..field.unit
					end
					local w = Widget.new {
						text = text,
						font = InterfaceConfig.font..'@15',
						parent = dataPanel,
						color = InterfaceConfig.fontColor,
						anchor = Widget.TOP + Widget.LEFT,
						position = {
							x = 10,
							y = y
						}
					}
					local w, h = Widget.getDimensions(w)
					y = y - h
				end
			end
		end
		
		local function onClick()
			updateFunc = displayData
			Widget.clear(upgradesPanel)
			displayData()
			
			local x, y = Widget.getPosition(modelPanel)
			local w, h = Widget.getDimensions(modelPanel)
			selectedModelContainer:setPosition{ x = x, y = y }
			selectedModelContainer:setDimensions{ width = w, height = h }
			selectedModelContainer:show()
			
			TileEffect.setHighlightColor { 192, 32, 32 }
			TileEffect.clearHighlight()
			for _, entity in pairs(entities) do
				if buildings[entity] then
					TileEffect.highlight(buildings[entity])
				end
			end
			
			local positions = {
				{0, 0}, {0, 1},
				--{1, 0}, {1, 1}
			}
			
			if upgrades then
				local i = 1
				for _, upgrade in pairs(upgrades) do
					local link = Widget.new {
						image = 'data/game/interface/largebutton.png',
						parent = upgradesPanel,
						anchor = Widget.TOP + Widget.LEFT,
						position = {
							x = positions[i][1] * 40 + 5,
							y = -positions[i][2] * 40 - 5
						}
					}
					local text = Widget.new {
						text = upgrade.name,
						font = InterfaceConfig.font..'@18',
						parent = link,
						color = InterfaceConfig.fontColor,
						anchor = Widget.CENTERX + Widget.CENTERY,
					}
					
					local popup
					local function updatePopup()
						if popup then
							popup:destroy()
						end
						popup = Container:new(
							InterfaceConfig.container,
							link,
							Widget.CENTERX + Widget.CENTERY,
							{ width = 150, height = 50 },
							8
						)
						popup:setPosition { x = 0, y = 50 }
						Widget.setRequireMouseOverParent(popup.widget, true)
						local minCost, maxCost
						local minEnergyCost, maxEnergyCost
						local minNewValue, maxNewValue
						for _, entity in pairs(entities) do
							local tower = buildings[entity]
							if tower then
								local cost = upgrade.costMethod(tower)
								if not minCost or cost < minCost then
									minCost = cost
								end
								if not maxCost or cost > maxCost then
									maxCost = cost
								end
								
								if upgrade.energyCostMethod then
									local energyCost = upgrade.energyCostMethod(tower)
									if not minEnergyCost or energyCost < minEnergyCost then
										minEnergyCost = energyCost
									end
									if not maxEnergyCost or energyCost > maxEnergyCost then
										maxEnergyCost = energyCost
									end
								end
								
								if upgrade.newValueMethod then
									local newValue = upgrade.newValueMethod(tower)
									if not minNewValue or newValue < minNewValue then
										minNewValue = newValue
									end
									if not maxNewValue or newValue > maxNewValue then
										maxNewValue = newValue
									end
								end
							end
						end
						local descriptionText, costText
						if maxCost and minCost ~= maxCost then
							costText = 'Coût : '..minCost..'£ à '..maxCost..'£'
						else
							costText = 'Coût : '..minCost..'£'
						end
						if upgrade.energyCostMethod then
							if maxEnergyCost and minEnergyCost ~= maxEnergyCost then
								costText = costText..' Energie : '..minEnergyCost..' à '..maxEnergyCost
							else
								costText = costText..' Energie : '..minEnergyCost
							end
						end
						if numEntities > 1 then
							costText = costText..' (x'..numEntities..')'
						end
						if upgrade.newValueMethod then
							if maxNewValue and minNewValue ~= maxNewValue then
								descriptionText = upgrade.description..' de '..minNewValue..' à '..maxNewValue
							else
								descriptionText = upgrade.description..' à '..minNewValue
							end
						else
							descriptionText = upgrade.description
						end
						Widget.new {
							text = costText,
							font = InterfaceConfig.font..'@18',
							parent = popup.widget,
							color = InterfaceConfig.fontColor,
							anchor = Widget.CENTERX + Widget.BOTTOM,
							position = { x = 0, y = 5 }
						}
						local descriptionWidget = Widget.new {
							text = descriptionText,
							font = InterfaceConfig.font..'@18',
							parent = popup.widget,
							color = InterfaceConfig.fontColor,
							anchor = Widget.CENTERX + Widget.TOP,
							position = { x = 0, y = -5 }
						}
						local w, h = Widget.getDimensions(descriptionWidget)
						popup:setDimensions { width = w + 20, height = 50 }
					end
					updatePopup()
					
					local function applyUpgrade()
						local towerUpgrades = {}
						for _, entity in pairs(entities) do
							local tower = buildings[entity]
							if tower then
								towerUpgrades[#towerUpgrades + 1] = {
									upgrade = upgrade,
									tower = tower
								}
							end
						end
						table.sort(towerUpgrades, function(a, b)
							return a.upgrade.costMethod(a.tower) < b.upgrade.costMethod(b.tower)
						end)
						for i = 1, #towerUpgrades do
							local towerUpgrade = towerUpgrades[i]
							local cost = towerUpgrade.upgrade.costMethod(towerUpgrade.tower)
							if Money.enough(cost) then
								if towerUpgrade.upgrade.energyCostMethod then
									local energyCost = towerUpgrade.upgrade.energyCostMethod(towerUpgrade.tower)
									if towerUpgrade.tower.energy >= energyCost then
										towerUpgrade.tower.energy = towerUpgrade.tower.energy - energyCost
										Money.sub(cost)
										towerUpgrade.upgrade.upgradeMethod(towerUpgrade.tower)
									end
								else
									Money.sub(cost)
									towerUpgrade.upgrade.upgradeMethod(towerUpgrade.tower)
								end
							else
								break
							end
						end
						TileEffect.updateHighlight()
						displayData()
						updatePopup()
					end
					Widget.onClick(link, applyUpgrade)
					i = i + 1
				end
			end
			
		end
		
		Widget.onClick(modelPanel, onClick)
		
		local width
		if isUnit then
			width = w1 / 8
		else
			width = w1 / 2.8
		end
		panels[#panels + 1] = {
			panel = modelPanel,
			width = width,
			onClick = onClick,
			numEntities = numEntities,
			isUnit = isUnit
		}
	end
	
	-- sorting panels by size
	table.sort(panels, function(p1, p2)
		if p1.isUnit and not p2.isUnit then
			return false
		elseif not p1.isUnit and p2.isUnit then
			return true
		else
			return p1.width > p2.width
		end
	end)
	
	local x = 5
	local maxNumEntitiesPanel
	local maxNumEntities = 0
	for i = 1, #panels do
		local p = panels[i]
		if p.numEntities > maxNumEntities then
			maxNumEntities = p.numEntities
			maxNumEntitiesPanel = p
		end
		Widget.setPosition(p.panel, {
			x = x,
			y = -5
		})
		x = x + p.width + 10
	end
	
	if maxNumEntitiesPanel then
		maxNumEntitiesPanel.onClick()
	else
		TileEffect.clearHighlight()
		clearPanels()
	end
	
end)
