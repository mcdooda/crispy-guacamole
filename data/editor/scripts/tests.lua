local Path = require 'data/scripts/path'
local ModData = require 'data/editor/scripts/moddata'
local Preview = require 'data/scripts/preview'
local UiSettings = require 'data/scripts/ui/uisettings'

local Slider = require 'data/scripts/ui/slider'
local TextInput = require 'data/scripts/ui/textinput'

local root = Widget.getRoot()
do
	local leftPanel = Widget.makeColumnFlow()
	leftPanel:setBackgroundColor(0x444444FF)
	leftPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	leftPanel:setSize(128,0)

	local content = Widget.makeColumnFlow()
	content:setSizePolicy(Widget.SizePolicy.EXPAND)

	local contentChildren = {}

	local function addContent(widget)
		content:addChild(widget)
		contentChildren[#contentChildren + 1] = widget
	end

	local function clearContent()
		for i = 1, #contentChildren do
			contentChildren[i]:removeFromParent()
		end
		contentChildren = {}
	end

	do
		local modes = Widget.makeLineFlow()
		modes:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.COMPRESS_Y)

		local tabs = {}
		local icons = {}
		local currentTabIndex
		local function enableTab(i)
			currentTabIndex = i
			local tab = tabs[i][1]
			local callback = tabs[i][2]
			icons[i]:setBackground('data/editor/interface/' .. tab .. '/active.png')
			callback()
		end
		local function disableTab(i)
			assert(currentTabIndex == i)
			local tab = tabs[i][1]
			currentTabIndex = nil
			icons[i]:setBackground('data/editor/interface/' .. tab .. '/passive.png')
		end
		local function setCurrentTab(i)
			disableTab(currentTabIndex)
			clearContent()
			enableTab(i)
		end
		local function refreshCurrentTab()
			setCurrentTab(currentTabIndex)
		end

		local function openTilesTab()
			Editor.setTileMode()
			Editor.setTile(ModData.tiles.names[1])
			for i = 1, #ModData.tiles.names do
				local tileName = ModData.tiles.names[i]
				local tileTexturePath = Path.getTileFilePath(tileName, ModData.tiles.getHighest(tileName) .. '.png')
				local preview = Widget.makeImage(tileTexturePath)
				preview:setSize(40,30)
				preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
				preview:setMargin(10, 0, 0, 7)
				preview:click(function()
					Editor.setTile(tileName)
				end)
				addContent(preview)
			end
		end

		local function openPropsTab()
			Editor.setPropMode()
			Editor.setProp(ModData.props.names[1])
			for i = 1,  #ModData.props.names do
				local propName = ModData.props.names[i]
				local propTexturePath = Path.getPropFilePath(propName, ModData.props.getHighest(propName) .. '.png')
				local preview = Widget.makeImage(propTexturePath)
				preview:setMargin(10, 0, 0, 7)
				preview:click(function()
					Editor.setProp(propName)
				end)
				addContent(preview)
			end
		end

		local function openEntitiesTab()
			Editor.setEntityMode()
			Editor.setEntity(ModData.entities.names[1])
			for i = 1, #ModData.entities.names do
				local entityName = ModData.entities.names[i]

				local preview = Preview.entity(entityName)
				if preview then
					preview:setMargin(10, 0, 0, 7)
					preview:click(function()
						Editor.setEntity(entityName)
					end)
					addContent(preview)
				end
			end
		end

		local function openZonesTab()
			Editor.setZoneMode()
			local zoneNames = Editor.getZoneNames()
			if #zoneNames > 0 then
				Editor.setZone(zoneNames[1])
			end
			for i = 1, #zoneNames do
				local zoneName = zoneNames[i]
				local zoneLine = Widget.makeLineFlow()
				local icon = Widget.makeImage 'data/editor/interface/icons/cross.png'
				icon:setMargin(3, 3, 1, 5)
				icon:click(function()
					Editor.removeZone(zoneName)
					refreshCurrentTab()
				end)
				zoneLine:addChild(icon)
				local text = Widget.makeText(zoneName, table.unpack(UiSettings.defaultFont))
				text:click(function()
					Editor.setZone(zoneName)
				end)
				zoneLine:addChild(text)
				addContent(zoneLine)
			end
			do
				local addZoneButton = Widget.makeLineFlow()
				local icon = Widget.makeImage 'data/editor/interface/icons/plus.png'
				icon:setMargin(2)
				local text = Widget.makeText('Add new zone', table.unpack(UiSettings.defaultFont))
				addZoneButton:addChild(icon)
				addZoneButton:addChild(text)
				addZoneButton:setMargin(7, 0, 0, 7)
				addZoneButton:click(function()
					local newZoneNames = {
						'Cherry', 'Apple', 'Lemon', 'Blueberry', 'Jam', 'Cream', 'Rhubarb', 'Lime',
						'Butter', 'Grape', 'Pomegranate', 'Sugar', 'Cinnamon', 'Avocado', 'Honey'
					}
					local newZoneName = assert(newZoneNames[#zoneNames + 1])
					Editor.addZone(newZoneName)
					refreshCurrentTab()
					Editor.setZone(newZoneName)
				end)
				addContent(addZoneButton)
			end
			do
				local addZoneButton = Widget.makeLineFlow()
				
				local icon = Widget.makeImage 'data/editor/interface/icons/plus.png'
				icon:setMargin(2)
				addZoneButton:addChild(icon)

				local text = TextInput:new('TEST')
				addZoneButton:addChild(text.container)

				addZoneButton:setMargin(0, 0, 0, 7)

				addContent(addZoneButton)
			end
		end

		local function addTab(iconName, openTabFunc)
			tabs[#tabs + 1] = {iconName, openTabFunc}
		end

		addTab('tilestab', openTilesTab)
		addTab('propstab', openPropsTab)
		addTab('unitstab', openEntitiesTab)
		addTab('zonestab', openZonesTab)

		for i = 1, #tabs do
			local tab = tabs[i][1]
			local icon = Widget.makeImage('data/editor/interface/' .. tab .. '/passive.png')
			icons[i] = icon
			icon:click(function()
				if i ~= currentTabIndex then
					setCurrentTab(i)
				end
			end)
			modes:addChild(icon)
		end
		enableTab(1)

		leftPanel:addChild(modes)
	end

	leftPanel:addChild(content)

	root:addChild(leftPanel)
end

--[[
do
	local a = Widget.makeFixedSize(50, 50)
	a:setBackgroundColor(0xFF0000FF)
	a:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
	a:setPadding(2)

	local positionPolicies = {
		Widget.PositionPolicy.TOP_LEFT,
		Widget.PositionPolicy.CENTER_X     + Widget.PositionPolicy.TOP,
		Widget.PositionPolicy.TOP_RIGHT,
		Widget.PositionPolicy.RIGHT        + Widget.PositionPolicy.CENTER_Y,
		Widget.PositionPolicy.BOTTOM_RIGHT,
		Widget.PositionPolicy.CENTER_X     + Widget.PositionPolicy.BOTTOM,
		Widget.PositionPolicy.BOTTOM_LEFT,
		Widget.PositionPolicy.LEFT         + Widget.PositionPolicy.CENTER_Y
	}
	for i = 1, #positionPolicies do
		local positionPolicy = positionPolicies[i]
		local b = Widget.makeFixedSize(10, 10)
		b:setBackgroundColor(0x00FF00FF)
		b:setPositionPolicy(positionPolicy)
		a:addChild(b)
	end

	do
		local c = Widget.makeFixedSize(32, 32)
		c:setBackground 'data/editor/interface/widgets/checkbox.png'
		c:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
		c:setBackgroundPosition(0.5, 0)
		c:setPositionPolicy(Widget.PositionPolicy.CENTER)
		a:addChild(c)
	end

	local w, h = a:getSize()
	Timer.start(0, function()
		local x, y = Mouse.getPosition()
		x = x + 5
		y = y - h - 5
		a:setPosition(x, y)
	end)
	root:addChild(a)
end
--]]
