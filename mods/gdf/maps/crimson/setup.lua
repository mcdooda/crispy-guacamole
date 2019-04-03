local LevelTasks = require 'data/scripts/leveltasks'

game.setCameraZoom(1)

LevelTasks:run(
    'players',
    'spawn',
    'camera'
)