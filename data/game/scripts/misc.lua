local coyield = coroutine.yield
local getTime = game.getTime

function game.sleep(duration)
    local endTime = getTime() + duration
    while getTime() < endTime do
        coyield()
    end
end