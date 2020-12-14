local LevelTasks = require 'data/scripts/leveltasks'

dofile(Mod.getFilePath 'scripts/common.lua')
dofile(Mod.getFilePath 'maps/sandbox/ui.lua')

Map.setFogType(Map.FogType.SOFT)

LevelTasks:run(
    'initgamepadsleveltask',
    'barkleveltask',
    'cameraleveltask'
)
