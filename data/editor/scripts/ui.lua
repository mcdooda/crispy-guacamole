require 'data/scripts/dumpenv' ()

local root = Widget.getRoot()

-- top right debug info
do
	local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

	-- fps
	do
		local label = Widget.makeText('X fps', unpack(font))
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

	root:addChild(debugContainer)
end

-- icons toolbar
do
	local toolbar = Widget.makeLineFlow()
	toolbar:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)

	local icons = {
		{
			'data/editor/interface/brushsizedown.png',
			function(w)
				return true
			end
		},
		{
			'data/editor/interface/brushsizeup.png',
			function(w)
				return true
			end
		},
		{
			'data/editor/interface/cone.png',
			function(w)
				return true
			end
		},
		{
			'data/editor/interface/disc.png',
			function(w)
				return true
			end
		},
		{
			'data/editor/interface/pyramid.png',
			function(w)
				return true
			end
		},
		{
			'data/editor/interface/square.png',
			function(w)
				return true
			end
		}
	}

	for _, pair in pairs(icons) do
		local image, onClick = unpack(pair)
		local icon = Widget.makeImage(image)
		icon:click(onClick)
		toolbar:addChild(icon)
	end
	
	do
		local icon = Widget.makeFixedSize(64,64)
		icon:setBackground('data/editor/interface/square.png')
		icon:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
		icon:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
		root:addChild(icon)
		Timer.start(0, function()
			local x, y = Mouse.getPosition()
			local width, height = icon:getSize()
			icon:setPosition(x, y - height)
		end)
	end

	root:addChild(toolbar)
end
