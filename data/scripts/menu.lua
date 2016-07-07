local Container = require 'data/scripts/container'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

-- Menu

local Menu = {}

function Menu:new(text, ...)
	-- window
	local container = Container:new(
		InterfaceConfig.container,
		nil,
		Widget.CENTERX + Widget.CENTERY,
		{ width = 200, height = 600 },
		16
	)
	
	local y = -10
	
	if text then
		-- header text
		container.textWidget = Widget.new {
			text = text,
			font = InterfaceConfig.font..'@28',
			color = InterfaceConfig.fontColor,
			anchor = Widget.CENTERX + Widget.TOP,
			parent = container.widget
		}
		Widget.setPosition(container.textWidget, { x = 0, y = y })
		y = y - 45
	end
	
	for i = 1, select('#', ...), 2 do
		local text, onClick = select(i, ...)
		local linkContainer = Container:new(
			InterfaceConfig.container,
			container.widget,
			Widget.CENTERX + Widget.TOP,
			{ width = 180, height = 40 },
			8
		)
		Widget.new {
			text = text,
			font = InterfaceConfig.font..'@20',
			color = InterfaceConfig.fontColor,
			anchor = Widget.CENTERX + Widget.CENTERY,
			parent = linkContainer.widget
		}
		linkContainer:setPosition { x = 0, y = y }
		Widget.onClick(linkContainer.widget, function()
			if onClick then
				onClick()
			end
			container:destroy()
		end)
		
		y = y - 45
	end
	
	container:setDimensions { width = 200, height = -y + 5 }
	
	return container
end

return Menu
