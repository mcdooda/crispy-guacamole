local format = string.format

local root = Widget.getRoot()

-- top right debug info
do
	local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setMargin(5)
	debugContainer:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)
	debugContainer:setSizePolicy(Widget.SizePolicy.COMPRESS)
	debugContainer:setBackgroundColor(0xFFFFFFAA)

	-- brush position
	do
		local label = Widget.makeText('X,Y', unpack(font))
		label:setTextColor(0x000000FF)
		label:setMargin(5)
		debugContainer:addChild(label)
		
		local getBrushPosition = Editor.getBrushPosition
		Timer.start(0, function()
			local x, y = getBrushPosition()
			label:setText(format('%.1f,%.1f', x, y))
		end)
	end

	root:addChild(debugContainer)
end