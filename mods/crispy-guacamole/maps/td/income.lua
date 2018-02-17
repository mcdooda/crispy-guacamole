local LevelTasks = require 'data/scripts/leveltasks'

local Money = require 'mods/crispy-guacamole/maps/td/money'

while true do
    LevelTasks:sleep(1)
    Money:add(100)
end

