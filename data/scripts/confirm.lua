local Container = require 'data/scripts/container'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

-- Confirm

local Confirm = {}

function Confirm:new(text, okText, onOk, cancelText, onCancel)
	-- window
	local container = Container:new(
		InterfaceConfig.container,
		nil,
		Widget.CENTERX + Widget.CENTERY,
		{ width = 30, height = 30 },
		16
	)
	
	-- confirm text
	local textWidget = Widget.new {
		text = text,
		font = InterfaceConfig.font..'@20',
		color = InterfaceConfig.fontColor,
		anchor = Widget.CENTERX + Widget.TOP,
		parent = container.widget
	}
	Widget.setPosition(textWidget, { x = 0, y = -8 })
	local w, h = Widget.getDimensions(textWidget)
	container:setDimensions{ width = w + 20, height = h + 60 }
	
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
	
	-- cancel button
	local cancelContainer = Container:new(
		InterfaceConfig.container,
		container.widget,
		Widget.CENTERX + Widget.BOTTOM,
		{ width = 30, height = 30 },
		5
	)
	local cancelTextWidget = Widget.new {
		text = cancelText,
		font = InterfaceConfig.font..'@20',
		color = InterfaceConfig.fontColor,
		anchor = Widget.CENTERX + Widget.CENTERY,
		parent = cancelContainer.widget
	}
	local w2, h2 = Widget.getDimensions(cancelTextWidget)
	
	Widget.onClick(cancelContainer.widget, function()
		if onCancel then
			onCancel()
		end
		container:destroy()
	end)
	
	-- resizing as needed
	local maxW = math.max(w1, w2)
	
	okContainer:setDimensions{ width = maxW + 16, height = h1 + 10 }
	okContainer:setPosition{ x = -(maxW + 20) / 2 - 5, y = 10 }
	
	cancelContainer:setDimensions{ width = maxW + 16, height = h2 + 10 }
	cancelContainer:setPosition{ x = (maxW + 20) / 2 + 5, y = 10 }
	
	if w + 20 < maxW + 20 + maxW + 20 + 30 then
		container:setDimensions{ width = maxW + 20 + maxW + 20 + 30, height = h + 60 }
	end
	
	return container
end

return Confirm
