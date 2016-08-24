local states = {}

function states:init(larva)
	
end

function states:idle(larva)
	larva:jump()
end

return states
