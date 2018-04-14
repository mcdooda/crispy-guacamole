local yield = coroutine.yield

local states = {}

function states:init(arrow)

    local fire = Entity.spawn('fire', arrow:getPosition())
    fire:getExtraData().attachedEntity = arrow
    fire:enterState 'updatePosition'
end

return states
