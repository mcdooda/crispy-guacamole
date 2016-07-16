print 'UI.LUA'

local root = Widget.getRoot()

local logo = Widget.makeImage("data/game/interface/logo.png")
logo:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
root:addChild(logo)

logo:click(function(w)
	print '*click lua*'
	return true
end)
