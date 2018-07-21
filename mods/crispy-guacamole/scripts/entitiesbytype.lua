local EntitiesByType = {
    types = {}
}

setmetatable(EntitiesByType.types, {
    __index = function(t, k)
        local entitiesOfType = {}
        t[k] = entitiesOfType
        return entitiesOfType
    end
})

function EntitiesByType:add(entity)
    assert(entity:isValid())
    local entitiesOfType = self.types[entity:getTemplateName()]
    entitiesOfType[#entitiesOfType + 1] = entity
end

function EntitiesByType:getAll(type)
    local entitiesOfType = self.types[type]

    -- clean invalid entities
    local c = #entitiesOfType
    local i = 1
    while i <= c do
        local entity = entitiesOfType[i]
        if not entity:isValid() then
            entitiesOfType[i] = entitiesOfType[c]
            entitiesOfType[c] = nil
            c = c - 1
        else
            i = i + 1
        end
    end

    return entitiesOfType
end

function EntitiesByType:getClosest(type, position)
    local entities = EntitiesByType:getAll(type)
    local closestEntity = nil
    local closestEntityDistance2 = math.huge
    for i = 1, #entities do
        local entity = entities[i]
        if entity:isValid() and entity:getTemplateName() == type then
            local entityPosition = entity:getPosition():toVector2()
            local distance2 = (entityPosition - position):length2()
            if distance2 < closestEntityDistance2 then
                closestEntityDistance2 = distance2
                closestEntity = entity
            end
        end
    end
    return closestEntity
end

return EntitiesByType