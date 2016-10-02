require 'data/scripts/dumpenv' ()

local dump = require 'data/scripts/dump'

local ModData = require 'data/editor/scripts/moddata'
dump(ModData)

local Slider = require 'data/scripts/ui/slider'

local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }

local root = Widget.getRoot()

do
	local leftPanel = Widget.makeColumnFlow()
	leftPanel:setPadding(10)
	leftPanel:setBackgroundColor(0xFF0000FF)
	leftPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	leftPanel:setSize(100, 0)
	
	do
		local label = Widget.makeText('Editor', unpack(font))
		label:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
		leftPanel:addChild(label)
	end
	
	do
		local modes = Widget.makeColumnFlow()
		modes:setBackgroundColor(0x00FF00FF)
		modes:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
		
		-- entity
		local entity = Widget.makeText('Entity', unpack(font))
		modes:addChild(entity)
		-- tile
		local tile = Widget.makeText('Tile', unpack(font))
		modes:addChild(tile)
		-- prop
		local prop = Widget.makeText('Prop', unpack(font))
		modes:addChild(prop)
		
		local w, h = prop:getSize()
		modes:setSize(0, h * 3)
		
		leftPanel:addChild(modes)
	end
	
	do
		local content = Widget.makeColumnFlow()
		content:setBackgroundColor(0x0000FFFF)
		content:setSizePolicy(Widget.SizePolicy.EXPAND)
		
		for i = 1, 10 do
			local item = Widget.makeText('Item ' .. i, unpack(font))
			content:addChild(item)
		end
		
		leftPanel:addChild(content)
	end
	
	root:addChild(leftPanel)
end

-- top right debug info
do
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
	debugContainer:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	debugContainer:setSize(100, 0)

	-- fps
	do
		local label = Widget.makeText('X fps', unpack(font))
		label:setMargin(10)
		debugContainer:addChild(label)
		
		local getFrameRate = Time.getFrameRate
		local format = string.format
		local medianNumFrames = 20
		local medianFps = {}
		for j = 1, medianNumFrames do
			medianFps[j] = getFrameRate()
		end
		local i = 1
		Timer.start(0, function()
			local fps = getFrameRate()
			medianFps[i] = nil
			medianFps[i + medianNumFrames] = fps
			local displayedFps = 0
			for j = i + 1, i + medianNumFrames do
				displayedFps = displayedFps + medianFps[j]
			end
			displayedFps = displayedFps / medianNumFrames
			label:setText(format('%.1f fps', displayedFps))
			i = i + 1
		end)
	end
	
	-- entities
	do
		local title = Widget.makeText('--- Entities ---', unpack(font))
		title:setMargin(10, 0, 5, 0)
		debugContainer:addChild(title)
		for i = 1, #ModData.entities.names do
			local entityName = ModData.entities.names[i]
			local label = Widget.makeText(entityName, unpack(font))
			label:setMargin(0, 0, 0, 7)
			debugContainer:addChild(label)
			label:click(function()
				Editor.setEntityMode()
				Editor.setEntity(entityName)
			end)
		end
	end
	
	-- tiles
	do
		local title = Widget.makeText('--- Tiles ---', unpack(font))
		title:setMargin(10, 0, 5, 0)
		debugContainer:addChild(title)
		for i = 1, #ModData.tiles.names do
			local tileName = ModData.tiles.names[i]
			local label = Widget.makeText(tileName, unpack(font))
			label:setMargin(0, 0, 0, 7)
			debugContainer:addChild(label)
			label:click(function()
				Editor.setTileMode()
				Editor.setTile(tileName)
			end)
		end
	end
	
	-- props
	do
		local title = Widget.makeText('--- Props ---', unpack(font))
		title:setMargin(10, 0, 5, 0)
		debugContainer:addChild(title)
		for i = 1, #ModData.props.names do
			local propName = ModData.props.names[i]
			local label = Widget.makeText(propName, unpack(font))
			label:setMargin(0, 0, 0, 7)
			debugContainer:addChild(label)
			label:click(function()
				Editor.setPropMode()
				Editor.setProp(propName)
			end)
		end
	end

	root:addChild(debugContainer)
end
Editor.setEntityMode()
Editor.setEntity(ModData.entities.names[1])

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