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

local entityNamePerButton = {
    A = 'human_soldier',
    B = 'human_archer',
    X = 'human_crossbowman',
    Y = 'human_paladin',
}

return {
    getPlayers   = getPlayers,
    getBuildings = getBuildings,

    -- current game data
    entityNamePerButton = entityNamePerButton
}