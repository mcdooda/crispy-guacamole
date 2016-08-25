require 'data/scripts/dumpenv' ()

local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }

local root = Widget.getRoot()
-- top right debug info
do
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setPosition(-10,-10)
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
	toolbar:setPosition(-10,10)
	toolbar:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)

	local icons = {
		{
			'data/editor/interface/brushsizedown.png',
			function(w)
				print 'click size -'
				return true
			end,
			function(w)
				print 'mouse enter size -'
			end,
			function(w)
				print 'mouse leave size -'
			end
		},
		{
			'data/editor/interface/brushsizeup.png',
			function(w)
				print 'click size +'
				return true
			end,
			function(w)
				print 'mouse enter size +'
			end,
			function(w)
				print 'mouse leave size +'
			end
		},
		{
			'data/editor/interface/cone.png',
			function(w)
				print 'click cone'
				return true
			end,
			function(w)
				print 'mouse enter cone'
			end,
			function(w)
				print 'mouse leave cone'
			end
		},
		{
			'data/editor/interface/disc.png',
			function(w)
				print 'click disc'
				return true
			end,
			function(w)
				print 'mouse enter disc'
			end,
			function(w)
				print 'mouse leave disc'
			end
		},
		{
			'data/editor/interface/pyramid.png',
			function(w)
				print 'click pyramid'
				return true
			end,
			function(w)
				print 'mouse enter pyramid'
			end,
			function(w)
				print 'mouse leave pyramid'
			end
		},
		{
			'data/editor/interface/square.png',
			function(w)
				print 'click square'
				return true
			end,
			function(w)
				print 'mouse enter square'
			end,
			function(w)
				print 'mouse leave square'
			end
		}
	}

	for _, pair in pairs(icons) do
		local image, onClick, onMouseEnter, onMouseLeave = unpack(pair)
		local icon = Widget.makeImage(image)
		icon:setPosition(math.random(-10, 10), math.random(-10, 10))
		icon:click(onClick)
		icon:mouseEnter(onMouseEnter)
		icon:mouseLeave(onMouseLeave)
		toolbar:addChild(icon)
	end
	
	root:addChild(toolbar)
	
	do
		local icon = Widget.makeFixedSize(64,64)
		icon:setBackground('data/editor/interface/square.png')
		icon:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
		icon:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
		icon:setBackgroundColor(0xFF0000FF)
		
		local label = Widget.makeText('LABEL', unpack(font))
		label:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
		
		do
			local child1 = Widget.makeFixedSize(30,30)
			child1:setPosition(10,10)
			child1:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
			child1:setBackgroundColor(0xFFFFFFFF)
			do
				local child2 = Widget.makeFixedSize(10,10)
				child2:setPosition(-5,5)
				child2:setBackgroundColor(0x0000FFFF)
				child2:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
				child1:addChild(child2)
			end
			do
				local child3 = Widget.makeFixedSize(10,10)
				child3:setPosition(5,5)
				child3:setBackgroundColor(0x00FF00FF)
				child3:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
				child1:addChild(child3)
			end
			do
				local child4 = Widget.makeFixedSize(10,10)
				child4:setPosition(-5,-5)
				child4:setBackgroundColor(0xFF0000FF)
				child4:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
				child1:addChild(child4)
			end
			do
				local child5 = Widget.makeFixedSize(10,10)
				child5:setPosition(5,-5)
				child5:setBackgroundColor(0xFF00FFFF)
				child5:setPositionPolicy(Widget.PositionPolicy.BOTTOM_RIGHT)
				child1:addChild(child5)
			end
			icon:addChild(child1)
		end
		root:addChild(icon)
		root:addChild(label)
		Timer.start(0, function()
			local x, y = Mouse.getPosition()
			local width, height = icon:getSize()
			icon:setPosition(x, y - height)
			label:setPosition(x, y)
		end)
	end
end
