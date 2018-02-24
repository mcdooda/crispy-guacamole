local LevelTasks = require 'data/scripts/leveltasks'

local Money = require 'mods/crispy-guacamole/maps/td/money'

local period = 0.1

while true do
    LevelTasks:sleep(period)
    Money:add(Money.income * period)
end

