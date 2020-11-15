local Wave = require 'data/game/scripts/wave'
local Camera = require 'data/game/scripts/camera'

local yield = coroutine.yield

local states = {}

local random = math.random
function states:init(fist)
    fist:playAnimation 'spawn'
    fist:playAnimation 'close'
    fist:playAnimation 'shake'
    fist:playAnimation 'throw'
    local position = fist:getPosition()
    Camera.shake()
    Wave.circular(position:toVector2(), 2, 2, 0.3)
    for i = 0, 3, 1 do
		local newPosition = flat.Vector2(position:x() - 2 + random(4), position:y() - 2 + random(4))
        Entity.spawn('fx_big_smoke', newPosition)
    end
    fist:playAnimation 'hit'
    fist:despawn()
    yield() -- avoid entering init()
end

return states
