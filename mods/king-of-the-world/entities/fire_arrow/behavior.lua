local yield = coroutine.yield

local states = {}

function states:init(arrow)

    local fire = Entity.spawn('fire', arrow:getPosition())
    fire:getExtraData().attachedEntity = arrow
    return 'updatePosition'
end

return states
