local states = {}

function states:init(landmine)
end

function states:idle(landmine)
    local entities = Map.getEntitiesInRange(landmine:getPosition():toVector2(), 1.5)
    for i = 1, #entities do
        local entity = entities[i]
        if entity ~= landmine and entity:isLiving() then
            return 'timeout'
        end
    end
end

function states:timeout(landmine)
    landmine:sleep(0.7)
    landmine:dealDamage(1)
end
function states:explode(landmine)
    Entity.spawn('super_explosion', landmine:getPosition())

    local entities = Map.getEntitiesInRange(landmine:getPosition():toVector2(), 1.5)
    for i = 1, #entities do
        local entity = entities[i]
        if  entity:isValid() and entity ~= landmine and entity:isLiving() then
            local fire = Entity.spawn('fire', entity:getPosition())
            fire:getExtraData().attachedEntity = entity
            return 'updatePosition'
        end
    end
end

return states