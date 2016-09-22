local math = math
local random = math.random

local states = {}

function states:init(zombie)
	zombie:playAnimation 'spawn'
	zombie:enterState 'wander'
end

function states:idle(zombie)
	zombie:jump()
end

function states:wander(zombie)
	local x, y = zombie:getPosition()
	while true do
		
		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			zombie:moveTo(rx, ry)
		end
		
	end
end

return states
