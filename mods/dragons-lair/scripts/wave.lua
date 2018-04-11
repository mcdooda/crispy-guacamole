local sin = math.sin
local yield = coroutine.yield
local moveTileZBy = Map.moveTileZBy

local function waveShape(age, distance, progression, effect)
    return -sin(age * 10 + distance) * progression * effect
end

local function craterShape(age, distance, progression, effect)
    return age * 3 * (
        effect ^ 2
    )
end

local function new(position2d, radius, height, duration, onEnd, shape)
    local brush = Brush.sphere()
    brush:setRadius(radius)

    local tiles = brush:getTiles(position2d)
    local tilesDz = {}
    tiles:eachTile(function(tile)
        tilesDz[tile] = 0
    end)

    shape = shape or waveShape
    local timer = Timer.new()
    timer:onUpdate(coroutine.wrap(function(timer, age)
        local _
        local progression, prevProgression = 0, 0
        local prevAge = age
        while true do
            tiles:eachTile(function(tile, effect)
                local distance = effect * radius
                local dz = (shape(age, distance, progression, effect)
                          - shape(prevAge, distance, prevProgression, effect))
                          * height
                tilesDz[tile] = tilesDz[tile] + dz
                moveTileZBy(tile, dz)
            end)
            prevAge = age
            _, age = yield()
            prevProgression = progression
            progression = age / duration
            if progression > 0.5 then
                progression = 1 - progression
            end
        end
    end))
    timer:onEnd(function()
        -- reset tiles to their initial position
        for tile, tileDz in pairs(tilesDz) do
            moveTileZBy(tile, -tileDz)
        end

        if onEnd then
            onEnd()
        end
    end)
    timer:start(duration)
end

return {
    new = new
}