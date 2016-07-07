local MainTower = unpack(require 'data/game/scripts/maintower')
local EnergyTower = unpack(require 'data/game/scripts/energytower')
local DefenseTower = unpack(require 'data/game/scripts/defensetower')
local HarvesterTower = require 'data/game/scripts/harvestertower'

local towers = {
	HarvesterTower,
	DefenseTower,
	EnergyTower,
	MainTower
}

return towers
