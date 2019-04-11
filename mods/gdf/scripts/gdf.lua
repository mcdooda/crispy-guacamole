local function getPlayers()
    local playerEntityNames = {'player_ranger', 'player_wingdiver'}
    local players = {}
    for i = 1, #playerEntityNames do
        local playersOfType = Map.getEntitiesOfType(playerEntityNames[i])
        for j = 1, #playersOfType do
            players[#players + 1] = playersOfType[j]
        end
    end
    return players
end

return {
    getPlayers = getPlayers
}