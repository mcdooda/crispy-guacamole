-- container

local Container = {}

local Widget = Widget

function Container:new(path, parent, anchor, dimensions, borderWidth)
	local w = Widget.new {
		parent = parent,
		anchor = anchor
	}
	
	local cw = Widget.new {
		image = path..'/c.png',
		parent = w,
		anchor = Widget.LEFT + Widget.TOP,
		position = {
			x = borderWidth,
			y = -borderWidth
		}
	}
	local tw = Widget.new {
		image = path..'/t.png',
		parent = w,
		anchor = Widget.LEFT + Widget.TOP,
		position = {
			x = borderWidth,
			y = 0
		}
	}
	local lw = Widget.new {
		image = path..'/l.png',
		parent = w,
		anchor = Widget.LEFT + Widget.TOP,
		position = {
			x = 0,
			y = -borderWidth
		}
	}
	local rw = Widget.new {
		image = path..'/r.png',
		parent = w,
		anchor = Widget.RIGHT + Widget.TOP,
		position = {
			x = 0,
			y = -borderWidth
		}
	}
	local bw = Widget.new {
		image = path..'/b.png',
		parent = w,
		anchor = Widget.LEFT + Widget.BOTTOM,
		position = {
			x = borderWidth,
			y = 0
		}
	}
	
	Widget.new {
		image = path..'/tl.png',
		parent = w,
		anchor = Widget.LEFT + Widget.TOP,
		dimensions = {
			width = borderWidth,
			height = borderWidth
		}
	}
	Widget.new {
		image = path..'/tr.png',
		parent = w,
		anchor = Widget.RIGHT + Widget.TOP,
		dimensions = {
			width = borderWidth,
			height = borderWidth
		}
	}
	Widget.new {
		image = path..'/bl.png',
		parent = w,
		anchor = Widget.LEFT + Widget.BOTTOM,
		dimensions = {
			width = borderWidth,
			height = borderWidth
		}
	}
	Widget.new {
		image = path..'/br.png',
		parent = w,
		anchor = Widget.RIGHT + Widget.BOTTOM,
		dimensions = {
			width = borderWidth,
			height = borderWidth
		}
	}
	
	local o = { borderWidth = borderWidth, widget = w, topWidget = tw, leftWidget = lw, rightWidget = rw, bottomWidget = bw, centerWidget = cw }
	setmetatable(o, { __index = self })
	o:setDimensions(dimensions)
	return o
end

function Container:setPosition(position)
	Widget.setPosition(self.widget, position)
end

function Container:setDimensions(dimensions)
	Widget.setDimensions(self.widget, dimensions)
	Widget.setDimensions(self.topWidget, {
		width = dimensions.width - self.borderWidth * 2,
		height = self.borderWidth
	})
	Widget.setDimensions(self.leftWidget, {
		width = self.borderWidth,
		height = dimensions.height - self.borderWidth * 2
	})
	Widget.setDimensions(self.rightWidget, {
		width = self.borderWidth,
		height = dimensions.height - self.borderWidth * 2
	})
	Widget.setDimensions(self.bottomWidget, {
		width = dimensions.width - self.borderWidth * 2,
		height = self.borderWidth
	})
	Widget.setDimensions(self.centerWidget, {
		width = dimensions.width - self.borderWidth * 2,
		height = dimensions.height - self.borderWidth * 2
	})
end

function Container:hide()
	Widget.hide(self.widget)
end

function Container:show()
	Widget.show(self.widget)
end

function Container:destroy()
	Widget.destroy(self.widget)
end

return Container
