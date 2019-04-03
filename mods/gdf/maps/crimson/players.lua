local gdf = require 'mods/gdf/scripts/gdf'

local players = gdf.getPlayers()
for i = 1, #players do
    players[i]:setGamepadIndex(i)
end