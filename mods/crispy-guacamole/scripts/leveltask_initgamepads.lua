local players = Map.getEntitiesOfType 'human_main_character'
for i = 1, #players do
    players[i]:setGamepadIndex(i)
end