local ResourceBehavior = require 'data/scripts/componenthelpers/behaviors/resource'
local totalAmount = 10
local states = ResourceBehavior.basicResource(0)

local init = states.init
function states:init(wheatField)
    init(self, wheatField)    
    local extraData = wheatField:getExtraData()
    extraData.emptied:on(function(amount)
        wheatField:enterState 'growing'
    end)
    return 'growing'
end

function states:growing(wheatField)
    wheatField:setCycleAnimation 'fallow'
    wheatField:sleep(2)
    wheatField:setCycleAnimation 'grow_1'
    wheatField:sleep(2)
    wheatField:setCycleAnimation 'grow_2'
    wheatField:sleep(2)
    wheatField:setCycleAnimation 'grow_3'
    wheatField:sleep(2)
    wheatField:setCycleAnimation 'idle'
    local extraData = wheatField:getExtraData()
    extraData.amount = totalAmount
end

return states