local Container = require 'data/scripts/container'

return function()
	local fps = Container:new('data/editor/interface/selection', nil, Widget.RIGHT + Widget.TOP, { width = 30, height = 30 }, 16)
	fps:setPosition{ x = -5, y = -5 }

	local fpsText = Widget.new {
		text = 'X fps',
		font = 'data/misc/fonts/LucidaSansRegular.ttf@20',
		color = { 0, 0, 0 },
		anchor = Widget.CENTERX + Widget.CENTERY,
		parent = fps.widget
	}

	local getFrameRate = Time.getFrameRate
	local floor = math.floor
	local getDimensions = Widget.getDimensions
	Timer.new(function()
		Widget.setText(fpsText, floor(getFrameRate())..' fps')
		local w, h = getDimensions(fpsText)
		fps:setDimensions{ width = w + 20, height = h + 20 }
	end)
end
