local Inventory = require 'mods/dragons-lair/maps/dragons-lair/inventory'
local Items     = require 'mods/dragons-lair/maps/dragons-lair/items'

local User = {}

function User:computeDamages()
	local damages = 1
	for i = 1, #Inventory.items do
		damages = Inventory.items[i].effect(damages, Inventory.items[i].factor)
	end
	return damages
end

return User