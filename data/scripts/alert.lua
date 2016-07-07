local Container = require 'data/scripts/container'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

-- Alert

local Alert = {}

function Alert:new(text, okText, onOk)
	-- window
	local container = Container:new(
		InterfaceConfig.container,
		nil,
		Widget.CENTERX + Widget.CENTERY,
		{ width = 30, height = 30 },
		16
	)
	
	-- alert text
	if type(text) == 'string' then
		text = { text }
	end
	local width = 20
	local height = 60
	local y = -8
	for _, line in pairs(text) do
		local textWidget = Widget.new {
			text = line,
			font = InterfaceConfig.font..'@20',
			color = InterfaceConfig.fontColor,
			anchor = Widget.CENTERX + Widget.TOP,
			parent = container.widget,
			position = {
				x = 0,
				y = y
			}
		}
		local w, h = Widget.getDimensions(textWidget)
		if w + 20 > width then
			width = w + 20
		end
		height = height + h
		y = y - h
	end
	
	container:setDimensions{ width = width, height = height }
	
	-- ok button
	local okContainer = Container:new(
		InterfaceConfig.container,
		container.widget,
		Widget.CENTERX + Widget.BOTTOM,
		{ width = 30, height = 30 },
		5
	)
	local okTextWidget = Widget.new {
		text = okText,
		font = InterfaceConfig.font..'@20',
		color = InterfaceConfig.fontColor,
		anchor = Widget.CENTERX + Widget.CENTERY,
		parent = okContainer.widget
	}
	local w1, h1 = Widget.getDimensions(okTextWidget)
	
	Widget.onClick(okContainer.widget, function()
		if onOk then
			onOk()
		end
		container:destroy()
	end)
	
	-- resizing as needed
	okContainer:setDimensions{ width = w1 + 16, height = h1 + 10 }
	okContainer:setPosition{ x = 0, y = 10 }
	
	if width < w1 + 20 + 30 then
		container:setDimensions{ width = w1 + 20 + 30, height = height }
	end
	
	return container
end

return Alert
