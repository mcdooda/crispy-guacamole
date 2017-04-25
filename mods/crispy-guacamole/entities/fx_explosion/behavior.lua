local yield = coroutine.yield

local states = {}

function states:init(explosion)
    local x, y = explosion:getPosition()
    local nearbyEntities = Map.getEntitiesInRange(x, y, 1)
    for i = 1, #nearbyEntities do
        local entity = nearbyEntities[i]
        if entity:isLiving() then
            entity:dealDamage(25)
        end
    end
    explosion:playAnimation 'explode'
    explosion:despawn()
    yield() -- avoid entering init()
end

return states
