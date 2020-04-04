local LevelTasks = require 'data/scripts/leveltasks'

dofile 'mods/crispy-guacamole/scripts/common.lua'
dofile 'mods/crispy-guacamole/maps/sandbox/ui.lua'

Map.setFogType(Map.FogType.SOFT)

local townHalls = Map.getEntitiesOfType 'human_town_hall'
if townHalls[1] then
    game.setCameraCenter(townHalls[1]:getPosition():toVector2())
end

LevelTasks:run(
    
)
