local root = Widget.getRoot()

-- top right debug info
do
	local font = { 'data/misc/fonts/LucidaSansRegular.ttf', 12 }
	local debugContainer = Widget.makeColumnFlow()
	debugContainer:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

	-- editor label
	do
		local label = Widget.makeText('Editor mode', unpack(font))
		debugContainer:addChild(label)
	end
	
	-- fps
	do
		local label = Widget.makeText('X fps', unpack(font))
		debugContainer:addChild(label)
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

	root:addChild(toolbar)
end
