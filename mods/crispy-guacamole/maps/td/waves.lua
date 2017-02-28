local mazeZone  = Map.getZone 'Cherry'
local startZone = Map.getZone 'Apple'
local endZone   = Map.getZone 'Lemon'

local startX, startY = startZone:getCenter()
local endX, endY = endZone:getCenter()

local function sleep(duration)
	local getTime = Time.getTime
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

local waves = {
    {'larva', 'larva', 'larva'},
    {'sheep', 'sheep', 'sheep'},
    {'boar', 'boar', 'boar'},
    {'zombie', 'zombie', 'zombie'},
    {'chaos_skeleton', 'chaos_skeleton', 'chaos_skeleton'},
    {'devil', 'devil', 'devil'},
    {'eye', 'eye', 'eye'},
}

for i = 1, #waves do
    --print('== wave ' .. i .. ' ==')
    local wave = waves[i]
    for j = 1, #wave do
        local entityTemplate = wave[j]
        --print('* spawning ' .. entityTemplate)
        local entity = Entity.spawn(
            entityTemplate,
            startX, startY, nil,
            nil, nil,
            Components.allExcept(Component.behavior)
        )
        entity:restrictToZone 'Cherry'
        entity:moveTo(endX, endY, false)
        sleep(0.3)
    end

    repeat
        coroutine.yield()
        local endEntities = endZone:getEntities()
        for i = 1, #endEntities do
            endEntities[i]:despawn()
        end
        local numEntities = mazeZone:getEntitiesCount()
        --print('Still ' .. numEntities .. ' entities ...')
    until numEntities == 0
end