local UiSettings = require 'data/scripts/ui/uisettings'

local format = string.format

local root = Widget.getRoot()

-- top right debug info
do
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setMargin(5)
	debugContainer:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)
	debugContainer:setSizePolicy(Widget.SizePolicy.COMPRESS)
	debugContainer:setBackgroundColor(0xFFFFFFAA)

	-- brush position
	do
		local label = Widget.makeText('X,Y', table.unpack(UiSettings.defaultFont))
		label:setTextColor(0x000000FF)
		label:setMargin(5)
		debugContainer:addChild(label)
		
		local getBrushPosition = Editor.getBrushPosition
		Timer.start(0, nil, function()
			local x, y = getBrushPosition()
			label:setText(format('%.1f,%.1f', x, y))
		end, true)
	end

	root:addChild(debugContainer)
end