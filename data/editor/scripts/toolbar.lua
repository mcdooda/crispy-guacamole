local Confirm = require 'data/scripts/confirm'
local Alert = require 'data/scripts/alert'

-- bottom right toolbar

local Toolbar = {}

function Toolbar:new()
	local o = { buttons = {} }
	return setmetatable(o, { __index = self })
end

function Toolbar:add(image, action)
	self.buttons[#self.buttons + 1] = {
		image = image,
		action = action
	}
end

function Toolbar:getWidget()
	local tb = Widget.new {
		anchor = Widget.RIGHT + Widget.BOTTOM,
		dimensions = {
			width = #self.buttons * 32,
			height = 32
		}
	}
	Widget.hide(tb)
	Widget.new {
		parent = tb,
		image = 'data/editor/interface/toolbarleft.png',
		anchor = Widget.LEFT + Widget.CENTERY
	}
	local tbCenter = Widget.new {
		parent = tb,
		image = 'data/editor/interface/toolbarcenter.png',
		anchor = Widget.LEFT + Widget.CENTERY,
		position = {
			x = 5,
			y = 0
		}
	}
	Widget.setDimensions(tbCenter, {
		width = #self.buttons * 32 - 10,
		height = 32
	})
	Widget.new {
		parent = tb,
		image = 'data/editor/interface/toolbarright.png',
		anchor = Widget.RIGHT + Widget.CENTERY,
	}
	for i = 1, #self.buttons do
		local button = Widget.new {
			parent = tb,
			position = {
				x = (i - 1) * 32,
				y = 0
			},
			anchor = Widget.LEFT + Widget.TOP,
			image = self.buttons[i].image
		}
		local action = self.buttons[i].action
		if action then
			Widget.onClick(button, action)
		end
	end
	return tb
end

-----

local tb, tbWidget

tb = Toolbar:new()
tb:add('data/editor/interface/brushsizedown.png', EditorMap.brushSizeDown)
tb:add('data/editor/interface/brushsizeup.png', EditorMap.brushSizeUp)
tb:add('data/editor/interface/disc.png', EditorMap.brushShapeDisc)
tb:add('data/editor/interface/square.png', EditorMap.brushShapeSquare)
tb:add('data/editor/interface/cone.png', EditorMap.brushShapeCone)
tb:add('data/editor/interface/pyramid.png', EditorMap.brushShapePyramid)
tb:add('data/editor/interface/random.png', EditorMap.brushShapePeak)
tb:add('data/editor/interface/random.png', EditorMap.brushShapeSphere)
tb:add('data/editor/interface/random.png', EditorMap.brushShapeForest)
tb:add('data/editor/interface/save.png', function()
	Confirm:new(
		'Êtes-vous sur de vouloir sauvegarder la carte ?',
		'Oui', function()
			EditorMap.save()
			Alert:new('Carte sauvegardée', 'Ok')
		end,
		'Non', nil
	)
end)
tbWidget = tb:getWidget()

Widget.setPosition(tbWidget, {
	x = -10,
	y = 10
})
Widget.show(tbWidget)


