local Dialog  	 = require 'mods/dragons-lair/ui/entity/dialog'
local ShopKeeper = require 'mods/dragons-lair/maps/dragon\'s lair/shopkeeper'
local ItemIcon   = require 'mods/dragons-lair/maps/dragon\'s lair/itemicon'
local ui = setmetatable({}, { __index = Dialog })

local iconSize = 56
local margin = 10

local function buildUi(container, stock)
	container:removeAllChildren()
	for i = 1, #stock do
		local frame = Widget.makeFixedSize(iconSize, iconSize)
		frame:setMargin(0, 10, 0, 0)
		container:addChild(frame)
		ItemIcon:new(stock[i], frame)
	end
end

function ui.addedToMap(entity, widget)
	local widget = Dialog.addedToMap(entity, widget)
	local container = Widget.makeLineFlow()
	container:setMargin(margin)
	container:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.CENTER_Y)
	widget:addChild(container)

	entity:selected(function(entity)
		entity:setUiVisible(true)
	end)
	
	entity:deselected(function(entity) 			
		entity:setUiVisible(false)
	end)

	ShopKeeper:onStockChanged(function(stock)
		widget:setSize(#stock * (iconSize + margin) + 2 * margin, iconSize + 2 * margin)
		buildUi(container, stock)
	end)
end

return ui