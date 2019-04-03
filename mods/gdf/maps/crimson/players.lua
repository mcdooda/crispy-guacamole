local players = Map.getEntitiesOfType('player')

for i = 1, #players do
    players[i]:setGamepadIndex(i)
    print(players[i], '-> gamepad:', i)
end