require 'data/scripts/dumpenv' ()

local dump = require 'data/scripts/dump'

local ModData = require 'data/editor/scripts/moddata'
--dump(ModData)

local Slider = require 'data/scripts/ui/slider'

local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }

local root = Widget.getRoot()

do
	local leftPanel = Widget.makeColumnFlow()
	leftPanel:setBackgroundColor(0x444444FF)
	leftPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	leftPanel:setSize(96,0)
	
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
		
		local function openTilesTab()
			Editor.setTileMode()
			Editor.setTile(ModData.tiles.names[1])
			for i = 1, #ModData.tiles.names do
				local tileName = ModData.tiles.names[i]
				local label = Widget.makeText(tileName, unpack(font))
				label:setMargin(0, 0, 0, 7)
				label:click(function()
					Editor.setTile(tileName)
				end)
				addContent(label)
			end
		end
		
		local function openPropsTab()
			Editor.setPropMode()
			Editor.setProp(ModData.props.names[1])
			for i = 1, #ModData.props.names do
				local propName = ModData.props.names[i]
				local label = Widget.makeText(propName, unpack(font))
				label:setMargin(0, 0, 0, 7)
				label:click(function()
					Editor.setProp(propName)
				end)
				addContent(label)
			end
		end
		
		local function openEntitiesTab()
			Editor.setEntityMode()
			Editor.setEntity(ModData.entities.names[1])
			for i = 1, #ModData.entities.names do
				local entityName = ModData.entities.names[i]
				local label = Widget.makeText(entityName, unpack(font))
				label:setMargin(0, 0, 0, 7)
				label:click(function()
					Editor.setEntity(entityName)
				end)
				addContent(label)
			end
		end
		
		local tabs = {
			{'tilestab', openTilesTab},
			{'propstab', openPropsTab},
			{'unitstab', openEntitiesTab}
		}
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
		
		for i = 1, #tabs do
			local tab = tabs[i][1]
			local icon = Widget.makeImage('data/editor/interface/' .. tab .. '/passive.png')
			icons[i] = icon
			icon:click(function()
				if i ~= currentTabIndex then
					disableTab(currentTabIndex)
					clearContent()
					enableTab(i)
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
		local b = Widget.makeFixedSize(5, 5)
		b:setBackgroundColor(0x00FF00FF)
		b:setPositionPolicy(positionPolicy)
		a:addChild(b)
	end
	
	do
		local c = Widget.makeFixedSize(32, 32)
		c:setBackground 'data/editor/interface/widgets/checkbox.png'
		c:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
		c:setBackgroundPosition(32, 0)
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
]]
