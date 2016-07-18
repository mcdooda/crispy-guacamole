local root = Widget.getRoot()

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
