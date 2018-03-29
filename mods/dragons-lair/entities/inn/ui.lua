local Dialog  	 	= require 'mods/dragons-lair/ui/entity/dialog'
local ShopKeeper 	= require 'mods/dragons-lair/maps/dragon\'s lair/shopkeeper'
local ItemIcon   	= require 'mods/dragons-lair/maps/dragon\'s lair/itemicon'
local Theme 		= require 'mods/dragons-lair/ui/theme'
local EntityPreview	= require 'mods/dragons-lair/scripts/entitypreview'

local ui = setmetatable({}, { __index = Dialog })

local iconSize = 56
local margin = 20
local avatar = EntityPreview:new('shopkeeper', 2)
avatar.widget:mouseDown(function()
	avatar:startAnimation('no', 1.1)
end)

local items = {}

local function buildItemList(container, stock)
	container:removeAllChildren()
	for i = 1, #items do
		if items[i].preview.stopAnimation then
			items[i].preview:stopAnimation()
		end
	end
	items = {}

	for i = 1, #stock do
		local item = ItemIcon:new(stock[i], container)
		item.preview.widget:mouseDown(function()
			local result = ShopKeeper:buyItem(i)
			if not result then
				avatar:startAnimation('no', 1.1)
			end
		end)
		table.insert(items, item)
	end
end

local function buildUi(entity, content)
	-- icon and text
	do
		avatar:startAnimation('speaking', 2)
		local firstRow = Widget.makeLineFlow()
		firstRow:addChild(avatar.widget)
		local textLabel = Widget.makeText('Touche, c\'est achete.', table.unpack(Theme.DIALOG_FONT))
		textLabel:setPositionPolicy(Widget.PositionPolicy.CENTER)
		textLabel:setTextColor(Theme.DIALOG_TEXT_COLOR)
		firstRow:addChild(textLabel)
		content:addChild(firstRow)
	end
	
	-- item list
	local itemContainer = Widget.makeLineFlow()
	content:addChild(itemContainer)
	buildItemList(itemContainer, ShopKeeper:getStock())
end

function ui.addedToMap(entity, widget)
	local dialog = Dialog.addedToMap(entity, widget)
	local content = Widget.makeColumnFlow()
	content:setMargin(margin)
	dialog:addChild(content)
	entity:selected(function(entity)
		entity:setUiVisible(true)
		buildUi(entity, content)
	end)
	
	entity:deselected(function(entity)
		entity:setUiVisible(false)
		content:removeAllChildren()
	end)
end

return ui