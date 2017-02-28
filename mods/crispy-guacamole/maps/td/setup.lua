dump = require 'data/scripts/dump'

local LevelTasks = require 'mods/crispy-guacamole/leveltasks'

local waves = assert(loadfile './mods/crispy-guacamole/maps/td/waves.lua')
LevelTasks.addTask(waves)

LevelTasks.loop()
