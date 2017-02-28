local states = {}

function states:init(devil)
	
end

function states:idle(devil)
	--devil:jump()
	--[[
	local r = math.random() * math.pi * 2 - math.pi
	devil:setHeading(r)
	local x, y, z = devil:getPosition()
	local heading = devil:getHeading()
	local radius = 0.25 + 0.1 + 0.01
	Entity.spawn('arrow', x + math.cos(heading) * radius, y + math.sin(heading) * radius, z + 1, heading)
	coroutine.yield()
	]]
end

return states
