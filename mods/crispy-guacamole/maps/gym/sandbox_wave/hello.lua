local Wave = require 'data/game/scripts/wave'
local function wave()
    Wave.circular(flat.Vector2(12.5, 31.5), 15, 3, 1.5, wave)
end
wave()