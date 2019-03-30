local LevelTasks = require 'data/scripts/leveltasks'

game.setCameraZoom(3)

LevelTasks:run(
    'spawn',
    'camera'
)