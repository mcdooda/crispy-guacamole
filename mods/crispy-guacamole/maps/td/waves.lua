local mazeZone  = Map.getZone 'Cherry'
local startZone = Map.getZone 'Apple'
local endZone   = Map.getZone 'Lemon'

local startZonePosition = startZone:getCenter()
local endZonePosition = endZone:getCenter()

local waves = {
    {'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva', 'larva'},
    {'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep', 'sheep'},
    {'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar', 'boar'},
    {'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie', 'zombie'},
    {'chaos_skeleton', 'chaos_skeleton', 'chaos_skeleton', 'chaos_skeleton', 'chaos_skeleton', 'chaos_skeleton'},
    {'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil', 'devil'},
    {'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye', 'eye'},
    {'larva', 'larva', 'larva', 'larva', 'larva', 'eye', 'eye', 'eye', 'eye', 'eye', 'larva', 'larva', 'larva', 'larva', 'larva', 'eye', 'eye', 'eye', 'eye', 'eye', 'larva', 'larva', 'larva', 'larva', 'larva', 'eye', 'eye', 'eye', 'eye', 'eye', 'larva', 'larva', 'larva', 'larva', 'larva', 'eye', 'eye', 'eye', 'eye', 'eye', 'larva', 'larva', 'larva', 'larva', 'larva', 'eye', 'eye', 'eye', 'eye', 'eye', 'devil', 'devil', 'devil'},
    {'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior', 'orc_warrior'}
}

local function despawnEntities()
    local endEntities = endZone:getEntities()
    for i = 1, #endEntities do
        endEntities[i]:despawn()
    end
end

for i = 1, #waves do
    --print('== wave ' .. i .. ' ==')
    local wave = waves[i]
    for j = 1, #wave do
        local entityTemplate = wave[j]
        --print('* spawning ' .. entityTemplate)
        for k = 1, 1 do
            local entity = Entity.spawn(
                entityTemplate,
                startZonePosition,
                nil, nil,
                Components.allExcept(Component.behavior)
            )
            entity:restrictToZone 'Cherry'
            entity:moveTo(endZonePosition, false)
        
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
        --print('Still ' .. numEntities .. ' entities ...')
    until numEntities == 0
end