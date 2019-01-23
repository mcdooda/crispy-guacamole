local entityName = ...

local modPath = Mod.getPath()
local entityPath = modPath .. '/entities/' .. entityName
local command = 'mkdir "' .. entityPath .. '"'
os.execute(command)

