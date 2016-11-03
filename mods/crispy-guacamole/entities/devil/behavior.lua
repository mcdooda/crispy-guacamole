local states = {}

function states:init(devil)
	
end

function states:idle(devil)
	local r = math.random(-math.pi, math.pi)
	devil:setHeading(r)
	devil:jump()
	local x, y, z = devil:getPosition()
	local heading = devil:getHeading()
	local radius = 0.25 + 0.1 + 0.01
	Entity.spawn('arrow', x + math.cos(heading) * radius, y + math.sin(heading) * radius, z + 1, heading)
end

return states
