local Container = require 'data/scripts/container'

-- window

local Window = {}

function Window:new(title)
	local window = Container:new(
		'data/editor/interface/selection',
		nil,
		Widget.CENTERX + Widget.CENTERY,
		{ width = 500, height = 300 },
		16
	)
end

return Window
