local function getPlayers()
    local playerEntityNames = {'human_main_character'}
    local players = {}
    for i = 1, #playerEntityNames do
        local playersOfType = Map.getEntitiesOfType(playerEntityNames[i])
        for j = 1, #playersOfType do
            players[#players + 1] = playersOfType[j]
        end
    end
    return players
end

local function getBuildings()
    local buildingEntityNames = {'human_town_hall', 'human_barracks'}
    local buildings = {}
    for i = 1, #buildingEntityNames do
        local buildingsOfType = Map.getEntitiesOfType(buildingEntityNames[i])
        for j = 1, #buildingsOfType do
            buildings[#buildings + 1] = buildingsOfType[j]
        end
    end
    return buildings
end

local function getHostiles(queryInstance)
    local querier = queryInstance:getQuerier()
    local hostiles = {}
    for _, visibleEntity in querier:eachVisibleEntity() do
        local isHostile = querier:isHostile(visibleEntity) or visibleEntity:isHostile(querier)
        local isAlive = visibleEntity:isLiving() and visibleEntity:isAlive()
        if isHostile and isAlive then
            hostiles[#hostiles + 1] = visibleEntity
        end
    end
    return hostiles
end

local function generatePositionsAroundAttackTarget(queryInstance)
    local querier = queryInstance:getQuerier()
    local attackTarget = querier:getAttackTarget()
    if not attackTarget or not attackTarget:isValid() then
        return {}
    end

    local attackTargetPosition = attackTarget:getPosition()
    local targetX = math.floor(attackTargetPosition:x() + 0.5)
    local targetY = math.floor(attackTargetPosition:y() + 0.5)

    local attackRange = querier:getAttackRange()
    local attackRangeSquared = attackRange * attackRange
    local minX = math.floor(targetX - attackRange)
    local maxX = math.floor(targetX + attackRange)
    local minY = math.ceil(targetY - attackRange)
    local maxY = math.ceil(targetY + attackRange)

    local navigabilityMask = querier:getNavigabilityMask()

    local positions = {}
    for x = minX, maxX do
        for y = minY, maxY do
            local distanceToCenterSquared = (x - targetX) * (x - targetX) + (y - targetY) * (y - targetY)
            if distanceToCenterSquared <= attackRangeSquared then
                local tilePosition = flat.Vector2(x, y)
                local tile = Map.getTile(tilePosition)
                if Map.isTileNavigable(tile, navigabilityMask) then
                    positions[#positions + 1] = flat.Vector2(x, y)
                end
            end
        end
    end
    return positions
end

local entityNamePerButton = {
    A = 'human_soldier',
    B = 'human_archer',
    X = 'human_crossbowman',
    Y = 'human_paladin',
}

return {
    getPlayers   = getPlayers,
    getBuildings = getBuildings,
    getHostiles  = getHostiles,
    generatePositionsAroundAttackTarget = generatePositionsAroundAttackTarget,

    -- current game data
    entityNamePerButton = entityNamePerButton
}