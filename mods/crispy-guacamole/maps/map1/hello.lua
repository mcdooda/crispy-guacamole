local sin = math.sin

for i = 1, 20 do
    print('Frame #' .. i .. ' (' .. tostring(_G) .. ',' .. tostring(_ENV) .. ')')
    coroutine.yield()
end

local wavePeriod = 1.5
local waveDuration = 1.4
local waveHeight = 3

local function wave(x, y, radius)
    local brush = Brush.cone()
    brush:setRadius(radius)

    local tiles = brush:getTiles(x, y)
    Timer.start(
        wavePeriod,
        nil,
        function()
            local tilesDz = {}
            tiles:eachTile(function(tile)
                tilesDz[tile] = 0
            end)
            Timer.start(
                waveDuration,
                coroutine.wrap(function(timer, duration)
                    local _
                    local progression, prevProgression = 0, 0
                    local prevDuration = duration
                    while true do
                        tiles:eachTile(function(tile, effect)
                            local distance = effect * radius
                            local dz = ((-sin(    duration * 10 + distance) * progression    )
                                        - (-sin(prevDuration * 10 + distance) * prevProgression))
                                        * waveHeight * effect
                            tilesDz[tile] = tilesDz[tile] + dz
                            local tileZ = Map.getTileZ(tile)
                            Map.setTileZ(tile, tileZ + dz)
                        end)
                        prevDuration = duration
                        _, duration = coroutine.yield()
                        prevProgression = progression
                        progression = duration / waveDuration
                        if progression > 0.5 then
                            progression = 1 - progression
                        end
                    end
                end),
                function()
                    -- reset tiles to their initial position
                    for tile, tileDz in pairs(tilesDz) do
                        local tileZ = Map.getTileZ(tile)
                        Map.setTileZ(tile, tileZ - tileDz)
                    end
                end
            )
        end,
        true
    )
end

wave(30, -30, 5)
wave(30, -30, 10)
wave(30, -30, 15)
wave(30, -30, 20)