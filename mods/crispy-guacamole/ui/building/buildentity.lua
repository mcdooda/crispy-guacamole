local function buildEntity(building, entityToBuild)
    local buildTimer = game.Timer()

    buildTimer:onUpdate(function(timer, elapsedTime)
        local t = elapsedTime / entityToBuild.duration
        if 0 < t and t < 1 then
            building:getExtraData().progressChanged(t)
        end
    end)

    buildTimer:onEnd(function()
        local extraData = building:getExtraData()
        local buildingPosition = building:getPosition():toVector2()
        local position = flat.Vector2(buildingPosition:x(), buildingPosition:y())
        position:x(position:x() + 1 + (math.random() - 0.5) * 0.01)
        position:y(position:y() + 1 + (math.random() - 0.5) * 0.01)

        Entity.spawn(entityToBuild.entity, position)
        extraData.progressChanged(0)
        table.remove(extraData.queue,1)
        if #extraData.queue > 0 then
            buildEntity(building, extraData.queue[1])
        end
    end)
    buildTimer:start(entityToBuild.duration, false)
end

local function addToQueue(building, entityToBuild)
    local extraData = building:getExtraData()
    if extraData.queue == nil then
        extraData.queue = {}
    end
    local queue = extraData.queue
    queue[#queue + 1] = entityToBuild

    if #extraData.queue == 1 then
        buildEntity(building, entityToBuild)
    end
end

return {
    addToQueue = addToQueue
}