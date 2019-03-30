local spawnZoneName  = 'Spawn'

local spawnZone  = Map.getZone(spawnZoneName)

local function makeWave(...)
    local wave = {}
    for i = 1, select('#', ...), 2 do
        local template = select(i, ...)
        local amount = select(i + 1, ...)
        for j = 1, amount do
            wave[#wave + 1] = template
        end
    end
    return wave
end

local waves = {
    makeWave('enemy_redant', 5),
    makeWave('enemy_redant', 10),
    makeWave('enemy_redant', 20),
    makeWave('enemy_redant', 40),
    makeWave('enemy_redant', 80),
    makeWave('enemy_redant', 160),
    makeWave('enemy_redant', 320),
    makeWave('enemy_blackant', 40),
    makeWave('enemy_redant', 40, 'enemy_blackant', 40),
    makeWave('enemy_blackant', 80)
}

local spawnedEntityCount = 0

for i = 1, #waves do
    local wave = waves[i]
    for j = 1, #wave do
        local entityTemplate = wave[j]
        local spawnTile = spawnZone:getRandomTile()
        assert(spawnTile)
        local x, y = Map.getTilePosition(spawnTile)
        local entity = Entity.spawn(
            entityTemplate,
            flat.Vector2(x, y)
        )
        spawnedEntityCount = spawnedEntityCount + 1
        entity:died(function()
            spawnedEntityCount = spawnedEntityCount - 1
        end)

        game.sleep(0.05)
    end

    repeat
        coroutine.yield()
    until spawnedEntityCount == 0

    game.sleep(2)
end