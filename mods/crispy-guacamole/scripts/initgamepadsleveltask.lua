local cg = require(Mod.getFilePath 'scripts/cg')

local players = cg.getPlayers()
for i = 1, #players do
    players[i]:setGamepadIndex(i)
end