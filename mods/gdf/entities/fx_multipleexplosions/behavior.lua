local yield = coroutine.yield
local math = math
local pi = math.pi
local random = math.random
local cos = math.cos
local sin = math.sin

local states = {}

function states:init(explosion)
    local position = explosion:getPosition()
    local range = 4
    local numExplosions = 30
    for i = 1, numExplosions do
        local distanceToCenter = random() * range
        local angle = random() * 2 * pi
        local explosionPosition = position + flat.Vector3(cos(angle) * distanceToCenter, sin(angle) * distanceToCenter, 1)
        Entity.spawn('fx_bigexplosion', explosionPosition)
    end
    explosion:despawn()
    yield() -- avoid entering init()
end

return states
