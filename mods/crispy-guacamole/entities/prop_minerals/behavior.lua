local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = {}

local function withdraw(extraData, value)
	local amount = extraData.amount
	amount = amount - value
	if amount < 0 then
		value = value + amount
		amount = 0
	end
	extraData.amount = amount
	extraData.amountChanged(amount)
	return value
end

function states:init(minerals)
	EntitiesByType:add(minerals)

	local extraData = minerals:getExtraData()
	extraData.amount = 1
	extraData.withdraw = withdraw
	extraData.amountChanged = flat.Slot:new()
end

return states
