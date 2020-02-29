local LevelTasks = require 'data/scripts/leveltasks'

Map.setFogType(Map.FogType.SOFT)

local townHalls = Map.getEntitiesOfType 'human_town_hall'
if townHalls[1] then
    game.setCameraCenter(townHalls[1]:getPosition():toVector2())
end

LevelTasks:run(
    
)
