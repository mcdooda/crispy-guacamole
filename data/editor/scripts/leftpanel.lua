local Path = require 'data/scripts/path'
local ModData = require 'data/editor/scripts/moddata'
local AssetIcon = require 'data/scripts/asseticon'
local UiSettings = require 'data/scripts/ui/uisettings'

local Slider = require 'data/scripts/ui/slider'
local TextInput = require 'data/scripts/ui/textinput'

local root = Widget.getRoot()
do
	local leftPanel = Widget.makeColumnFlow()
	leftPanel:setBackgroundColor(0x666666FF)
	leftPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	leftPanel:setSize(302, 0)
	leftPanel:setPadding(0)

	local content = Widget.makeColumnFlow()
	content:setSizePolicy(Widget.SizePolicy.EXPAND)
	content:setAllowScrollY(true)

	local contentLines = {}
	local iconPerLine = 3
	local iconSize = 90
	local iconMargin = 8

	local function addContent(widget)
		local contentLine
		if #contentLines == 0 or contentLines[#contentLines]:getChildrenCount() == iconPerLine then
			contentLine = Widget.makeLineFlow()
			contentLines[#contentLines + 1] = contentLine
			content:addChild(contentLine)
		else
			contentLine = contentLines[#contentLines]
		end
		contentLine:addChild(widget)
	end

	local function clearContent()
		for i = 1, #contentLines do
			contentLines[i]:removeFromParent()
		end
		contentLines = {}
	end

	do
		local padding = Widget.makeLineFlow()
		padding:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
		padding:setSize(128, 0)
		leftPanel:addChild(padding)
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
				local assetIcon = AssetIcon.tile(tileName, iconSize)
				assetIcon:setMargin(iconMargin, 0, 0, iconMargin)
				assetIcon:click(function()
					Editor.setTile(tileName)
				end)
				addContent(assetIcon)
			end
		end

		local function openPropsTab()
			Editor.setPropMode()
			Editor.setProp(ModData.props.names[1])
			for i = 1,  #ModData.props.names do
				local propName = ModData.props.names[i]
				local assetIcon = AssetIcon.prop(propName, iconSize)
				assetIcon:setMargin(iconMargin, 0, 0, iconMargin)
				assetIcon:click(function()
					Editor.setProp(propName)
				end)
				addContent(assetIcon)
			end
		end

		local function openEntitiesTab()
			Editor.setEntityMode()
			Editor.setEntity(ModData.entities.names[1])
			for i = 1, #ModData.entities.names do
				local entityName = ModData.entities.names[i]

				local assetIcon = AssetIcon.entity(entityName, iconSize)
				assetIcon:setMargin(iconMargin, 0, 0, iconMargin)
				assetIcon:click(function()
					Editor.setEntity(entityName)
				end)
				addContent(assetIcon)
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
				addZoneButton:addChild(icon)
				local text = Widget.makeText('Add Zone', table.unpack(UiSettings.defaultFont))
				addZoneButton:addChild(text)
				addZoneButton:setMargin(0, 0, 0, 7)
				addZoneButton:click(flat.ui.task(function()
					local newZoneName = flat.ui.prompt 'Zone name:'
					if newZoneName and newZoneName ~= '' then
						Editor.addZone(newZoneName)
						refreshCurrentTab()
						Editor.setZone(newZoneName)
					end
				end))
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