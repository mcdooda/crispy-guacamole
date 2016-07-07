local Widget = Widget
local Timer = Timer

local Fade = {}

function Fade.appear(duration)
	local w = Widget.new {
		image = 'data/game/interface/black.png',
		anchor = Widget.TOP + Widget.BOTTOM + Widget.LEFT + Widget.RIGHT
	}
	Widget.setOpacity(w, 0)
	Timer.new(
		0.02,
		true,
		function(timer, age)
			if age < duration then
				Widget.setOpacity(w, age / duration)
			else
				Widget.setOpacity(w, 1)
				Timer.setLoops(w, false)
			end
		end
	)
end

function Fade.disappear(duration)
	local w = Widget.new {
		image = 'data/game/interface/black.png',
		anchor = Widget.TOP + Widget.BOTTOM + Widget.LEFT + Widget.RIGHT
	}
	Widget.setOpacity(w, 1)
	Timer.new(
		0.02,
		true,
		function(timer, age)
			if age < duration then
				Widget.setOpacity(w, 1 - age / duration)
			else
				Widget.hide(w)
				Timer.setLoops(w, false)
			end
		end
	)
end

return Fade
