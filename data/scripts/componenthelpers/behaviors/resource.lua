local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local function basicResource(baseAmount)
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

    function states:init(resource)
        EntitiesByType:add(resource)

        local extraData = resource:getExtraData()
        extraData.amount = baseAmount
        extraData.withdraw = withdraw
        extraData.amountChanged = flat.Slot:new()
    end

    return states
end

return {
    basicResource = basicResource
}