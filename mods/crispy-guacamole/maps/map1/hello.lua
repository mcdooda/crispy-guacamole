local Wave = require 'mods/crispy-guacamole/scripts/wave'
for i = 1, 20 do
    print('Frame #' .. i .. ' (' .. tostring(_G) .. ',' .. tostring(_ENV) .. ')')
    coroutine.yield()
end

local function wave()
    Wave.new(flat.Vector2(30, -30), 15, 3, 1.5, wave)
end
wave()