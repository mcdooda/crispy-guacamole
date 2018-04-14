local Score = require 'mods/crispy-guacamole/maps/td/score'
local Money = require 'mods/crispy-guacamole/maps/td/money'
local MonstersData = require 'mods/crispy-guacamole/maps/td/monstersdata'

local mazeZone  = Map.getZone 'Cherry'
local startZone = Map.getZone 'Apple'
local endZone   = Map.getZone 'Lemon'

local startZonePosition = startZone:getCenter()
local endZonePosition = endZone:getCenter()

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
    makeWave('larva', 10),
    makeWave('sheep', 15),
    makeWave('boar', 20),
    makeWave('zombie', 25),
    makeWave('chaos_skeleton', 30),
    makeWave('devil', 15),
    makeWave('eye', 50),
    makeWave('larva', 5, 'eye', 5, 'larva', 5, 'eye', 5, 'larva', 5, 'eye', 5, 'larva', 5, 'eye', 5, 'devil', 20),
    makeWave('orc_warrior', 100)
}

local function despawnEntities()
    local endEntities = endZone:getEntities()
    for i = 1, #endEntities do
        local entity = endEntities[i]
        if entity:getExtraData().isWaveEntity then
            endEntities[i]:despawn()
            Score:addLeak()
        end
    end
end

for i = 1, #waves do
    local wave = waves[i]
    for j = 1, #wave do
        local entityTemplate = wave[j]
        for k = 1, 1 do
            local entity = Entity.spawn(
                entityTemplate,
                startZonePosition,
                nil, nil,
                Components.allExcept(Component.behavior)
            )
            entity:getExtraData().isWaveEntity = true
            entity:restrictToZone 'Cherry'
            entity:moveTo(endZonePosition, false)
            entity:died(function()
                Score:addKill()
                local monsterData = assert(MonstersData[entityTemplate])
                Money:add(monsterData.reward)
            end)
        
            local delay = 0.3
            local endTime = Game.getTime() + delay
            while Game.getTime() < endTime do
                despawnEntities()
                coroutine.yield()
            end
        end
    end

    repeat
        coroutine.yield()
        despawnEntities()
        local numEntities = mazeZone:getEntitiesCount()
    until numEntities == 0
end