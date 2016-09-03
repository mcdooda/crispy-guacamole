local states = {}

function states:init(devil)
	
end

function states:idle(devil)
	devil:jump()
end

return states
