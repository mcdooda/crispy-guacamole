local yield = coroutine.yield
local Wave = require 'mods/crispy-guacamole/scripts/wave'
local states = {}
local moveTileZBy = Map.moveTileZBy

local function moveTile(position2d, height)
    -- Wave.new(position2d, 0.5, height, 0.25)
end

local function dealDamage(position, damage)
    local nearbyEntities = Map.getEntitiesInRange(position, 0.1)
    for i = 1, #nearbyEntities do
        local entity = nearbyEntities[i]
        if entity:isValid() and entity:isLiving() then
            entity:dealDamage(10)
        end
    end
end

local function spreadExplosion(position, distance)
    local height = 0.8
    local damage = 1
    local pos1 = flat.Vector3(position:x() + distance, position:y(), position:z())
    Entity.spawn('fx_explosion', pos1)
    moveTile(pos1:toVector2(), height)
    dealDamage(pos1:toVector2(), damage)

    local pos2 = flat.Vector3(position:x(), position:y() + distance, position:z())
    Entity.spawn('fx_explosion', pos2)
    moveTile(pos2:toVector2(), height)
    dealDamage(pos2:toVector2(), damage)

    local pos3 = flat.Vector3(position:x(), position:y() - distance, position:z())
    Entity.spawn('fx_explosion', pos3)
    moveTile(pos3:toVector2(), height)
    dealDamage(pos3:toVector2(), damage)

    local pos4 = flat.Vector3(position:x() - distance, position:y(), position:z())
    Entity.spawn('fx_explosion', pos4)
    moveTile(pos4:toVector2(), height)
    dealDamage(pos4:toVector2(), damage)

end

function states:init(explosion)
    -- Wave.new(explosion:getPosition():toVector2(), 3,0.7, 0.3)

    spreadExplosion(explosion:getPosition(), 0.5)
    explosion:sleep(0.1)
    spreadExplosion(explosion:getPosition(), 0.5)
    explosion:sleep(0.1)
    spreadExplosion(explosion:getPosition(), 1)
    explosion:sleep(0.1)
    spreadExplosion(explosion:getPosition(), 1.5)
    explosion:despawn()
end

return states
