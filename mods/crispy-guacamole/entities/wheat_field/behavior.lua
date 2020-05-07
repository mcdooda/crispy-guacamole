local ResourceBehavior = require 'data/scripts/componenthelpers/behaviors/resource'
local totalAmount = 10
local statesResources = ResourceBehavior.basicResource(0)
local growDuration = 8

local init = statesResources.init
function statesResources:init(wheatField)
    init(self, wheatField)
    local extraData = wheatField:getExtraData()
    extraData.emptied:on(function(amount)
        wheatField:enterState 'growing'
    end)
    return 'growing'
end
function statesResources:under_construction(wheatField)
    wheatField:enterState 'growing'
end

local function growProgressBar(wheatField, growDuration)
    local buildTimer = game.Timer()
    buildTimer:onUpdate(function(timer, elapsedTime)
        local t = elapsedTime / growDuration
        if 0 < t and t < 1 then
            wheatField:getExtraData().progressChanged(t)
        end
    end)

    buildTimer:onEnd(function()
        local extraData = wheatField:getExtraData()
        extraData.progressChanged(0)
    end)
    buildTimer:start(growDuration, false)
end

function statesResources:growing(wheatField)
    -- growProgressBar(wheatField, 8)
    wheatField:playAnimation 'fallow'
    wheatField:setCycleAnimation 'stand'
    local extraData = wheatField:getExtraData()
    extraData.amount = totalAmount
end

return statesResources