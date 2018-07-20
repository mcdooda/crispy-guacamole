local states = {}

function states:init(minerals)
	minerals:getExtraData().amount = 5
end

return states
