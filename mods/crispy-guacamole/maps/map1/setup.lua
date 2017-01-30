local LevelTasks = require 'mods/crispy-guacamole/leveltasks'

local hello = assert(loadfile('./mods/crispy-guacamole/maps/map1/hello.lua'))
LevelTasks.addTask(hello)

LevelTasks.loop()