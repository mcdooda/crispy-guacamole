local math = math
local random = math.random

local states = {}

function states:init(boar)
	-- give them a chance to get an external order before wandering
	coroutine.yield()
	coroutine.yield()

	boar:enterState 'wander'
end

function states:idle(boar)
	boar:jump()
end

function states:wander(boar)
	local x, y = boar:getPosition()
	while true do
		
		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			boar:moveTo(rx, ry)
		end
		
	end
end

return states
