local states = {}

function states:init(devil)
	
end

function states:idle(devil)
	devil:jump()
	local x, y, z = devil:getPosition()
	local heading = devil:getHeading()
	Entity.spawn('arrow', x, y, z, heading)
end

return states
