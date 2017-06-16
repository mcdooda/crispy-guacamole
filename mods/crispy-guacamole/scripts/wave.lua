local sin = math.sin
local yield = coroutine.yield
local moveTileZBy = Map.moveTileZBy

local function new(x, y, radius, height, duration, onEnd)
    local brush = Brush.cone()
    brush:setRadius(radius)

    local tiles = brush:getTiles(x, y)
    local tilesDz = {}
    tiles:eachTile(function(tile)
        tilesDz[tile] = 0
    end)
    
    Timer.start(
        duration,
        coroutine.wrap(function(timer, age)
            local _
            local progression, prevProgression = 0, 0
            local prevAge = age
            while true do
                tiles:eachTile(function(tile, effect)
                    local distance = effect * radius
                    local dz = ((-sin(    age * 10 + distance) * progression    )
                              - (-sin(prevAge * 10 + distance) * prevProgression))
                             * height * effect
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
        end),
        function()
            -- reset tiles to their initial position
            for tile, tileDz in pairs(tilesDz) do
                moveTileZBy(tile, -tileDz)
            end

            if onEnd then
                onEnd()
            end
        end
    )
end

return {
    new = new
}