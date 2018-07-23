local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = {}

local function withdraw(extraData, value)
	extraData.amount = extraData.amount - value
	assert(extraData.amount >= 0)
	extraData.amountChanged(extraData.amount)
end

function states:init(minerals)
	EntitiesByType:add(minerals)

	local extraData = minerals:getExtraData()
	extraData.amount = 5
	extraData.withdraw = withdraw
	extraData.amountChanged = flat.Slot:new()
end

return states
