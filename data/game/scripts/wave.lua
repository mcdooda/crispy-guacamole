local sin = math.sin
local yield = coroutine.yield

local function waveShape(age, distance, progression, effect)
    return -sin(age * 10 + distance) * progression * effect
end

local function craterShape(age, distance, progression, effect)
    return age * 3 * (
        effect ^ 2
    )
end

local function circular(position2d, radius, height, duration, onEnd, shape)
    local brush = Brush.sphere()
    brush:setRadius(radius)

    local tiles = brush:getTiles(position2d)
    local tilesDz = {}
    tiles:eachTile(function(tile)
        tilesDz[tile] = 0
    end)

    shape = shape or waveShape
    local timer = game.Timer()
    timer:onUpdate(coroutine.wrap(function(timer, age)
        local moveTileZBy = Map.moveTileZBy
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
        local moveTileZBy = Map.moveTileZBy
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

local function linear(startPosition, endPosition, width, height, duration, speed, onEnd, shape)
    local length = (endPosition - startPosition):length()
    local direction = (endPosition - startPosition):getNormalized()
    local totalDuration = length / speed
    local timerStep = 1 / speed

    local distance = 0

    local timer = game.Timer()
    timer:onEnd(function()
        local position = startPosition + direction * distance
        circular(position, width, height, duration, nil, shape)

        distance = distance + 1
        if distance > length then
            if onEnd then
                onEnd()
            end
            timer:stop()
            return
        end
    end)
    timer:start(timerStep, true)
end

return {
    circular = circular,
    linear   = linear
}