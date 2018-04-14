local mapName = ...

local modPath = Mod.getPath()
local mapPath = modPath .. '/maps/' .. mapName
local command = 'mkdir "' .. mapPath .. '"'
print(command)
os.execute(command)

do
    local mapFile = [==========[
return {
    axes = {
        x = {-20, 10},
        y = { 20, 10},
        z = {  0,-20}
    }
}
]==========]

    local f = io.open(mapPath .. '/map.lua', 'w')
    f:write(mapFile)
    f:close()
end

do
    local setupFile = [==========[
local LevelTasks = require 'data/scripts/leveltasks'

LevelTasks:run(
    
)
]==========]

    local f = io.open(mapPath .. '/setup.lua', 'w')
    f:write(setupFile)
    f:close()
end
