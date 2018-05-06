local Money  = require 'mods/dragons-lair/maps/dragons-lair/money'
local User   = require 'mods/dragons-lair/maps/dragons-lair/user'
local states = {}

local currencies = { 
	{ animation = 'coin',        value = 1 },
	{ animation = 'green rupee', value = 10},
	{ animation = 'red rupee',   value = 25},
	{ animation = 'blue rupee',  value = 50}
}

local function spawnCurrency(position, currencyType)
	local heading = math.random() * 2 * math.pi
	local elevation = math.random() * math.pi / 8 + (math.pi / 2 - math.pi / 8)
	local currency = Entity.spawn('currency', position, heading, elevation)
	currency:playAnimation(currencyType, -1, false)
end

local function spawnAmount(position, amount)
	local currencyPosition = position + flat.Vector3(0, 0, 1)
	local maximumIndex = #currencies
	while amount > 0 do
		while amount < currencies[maximumIndex].value do
			maximumIndex = maximumIndex - 1
		end
		local index = math.random(maximumIndex)
		local value = currencies[index].value
		amount = amount - value
		spawnCurrency(currencyPosition, currencies[index].animation)
	end
end

function states:init(dragon)
	dragon:click(function(dragon) 			
		local position = dragon:getPosition()
		local damages = math.floor(User:computeDamages())
		spawnAmount(dragon:getPosition(), damages)
		dragon:playAnimation('poked', 1, false)
		Money:add(damages)
	end)
end

function states:idle(dragon)	
end

return states
