local format = string.format

local root = Widget.getRoot()

-- top right debug info
do
	local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setMargin(5)
	debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
	debugContainer:setSizePolicy(Widget.SizePolicy.COMPRESS)
	debugContainer:setBackgroundColor(0xFFFFFF55)

	-- fps
	do
		local label = Widget.makeText('X fps', unpack(font))
		label:setMargin(5)
		debugContainer:addChild(label)
		
		local getFrameRate = Time.getFrameRate
		local medianNumFrames = 50
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
	
	-- entity count
	do
		local label = Widget.makeText('X entities', unpack(font))
		label:setMargin(5)
		debugContainer:addChild(label)
		
		Timer.start(0, function()
			label:setText(format('%d entities', Map.getNumEntities()))
		end)
	end

	root:addChild(debugContainer)
end

print 'STATS'