local yield = coroutine.yield

local states = {}

function states:init(explosion)
    local position = explosion:getPosition()
    local nearbyEntities = Map.getEntitiesInRange(position:toVector2(), 1)
    for i = 1, #nearbyEntities do
        local entity = nearbyEntities[i]
        if entity:isLiving() then
            entity:dealDamage(100)
        end
    end
    explosion:playAnimation 'explode'
    explosion:despawn()
    yield() -- avoid entering init()
end

return states
