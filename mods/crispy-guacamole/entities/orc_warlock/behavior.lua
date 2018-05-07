local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local pi = math.pi
local cos = math.cos
local sin = math.sin
local random = math.random

local states = {}

function states:init(warlock)
	warlock:click(function(warlock) print(warlock, 'clicked') end)
end

function states:idle(warlock)
	while true do
		local position = warlock:getPosition()
		local numZombies = 5
		local distance = 0.5
		local zombies = {}
		for i = 1, numZombies do
			local heading = (i - 1) * 2 * pi / numZombies
			local zombiePosition = position + flat.Vector3(cos(heading) * distance, sin(heading) * distance, 0)
			local zombie = Entity.spawn('zombie', zombiePosition, heading)
			zombies[#zombies + 1] = zombie
		end
		warlock:sleep(6)
		for i = 1, numZombies do
			if zombies[i]:isValid() then
				zombies[i]:kill()
			end
		end
		warlock:sleep(3)
	end
end

return states
