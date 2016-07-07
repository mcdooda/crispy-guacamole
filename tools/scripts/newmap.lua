titlize 'Creating new map'

print 'name:'
local name = io.read '*l'

print 'width:'
local mapWidth = io.read '*n'

print 'height:'
local mapHeight = io.read '*n'

io.read '*l' -- skipping previous line end

print 'default tile:'
local defaultTile = io.read '*l'

print 'forest doodad:'
local forestDoodad = io.read '*l'

print()
mkdir('mods/' .. name)
mkdir('mods/' .. name .. '/abstracts')
mkdir('mods/' .. name .. '/buildings')
mkdir('mods/' .. name .. '/doodads')
mkdir('mods/' .. name .. '/fonts')
mkdir('mods/' .. name .. '/interface')
mkdir('mods/' .. name .. '/music')
mkdir('mods/' .. name .. '/samples')
mkdir('mods/' .. name .. '/scripts')
mkdir('mods/' .. name .. '/tiles')
mkdir('mods/' .. name .. '/units')

mkfile('mods/' .. name .. '/scripts/config.lua',
[=========================[
-- do not change this file after saving the map with the editor!
return {
	mapWidth  = %d,
	mapHeight = %d,
	defaultTile = '%s',
	forestDoodad = '%s',
	resourcePath = 'data/game'
}
]=========================],
mapWidth, mapHeight, defaultTile, forestDoodad)

mkfile('mods/' .. name .. '/scripts/game.lua',
[=========================[
-- insert game initialization and logic here

]=========================])

mkfile('mods/' .. name .. '/scripts/buildings.lua',
[=========================[
-- insert building definitions here

]=========================])

mkfile('mods/' .. name .. '/scripts/units.lua',
[=========================[
-- insert unit definitions here

]=========================])

mkfile('mods/' .. name .. '/scripts/abstracts.lua',
[=========================[
-- insert abstract entity definitions here

]=========================])

