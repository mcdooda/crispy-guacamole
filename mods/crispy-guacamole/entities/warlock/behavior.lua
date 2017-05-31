local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local pi = math.pi
local cos = math.cos
local sin = math.sin
local random = math.random

local states = {}

function states:init(warlock)
	
end

function states:idle(warlock)
	while true do
		local x, y, z = warlock:getPosition()
		local numZombies = 5
		local distance = 0.5
		local zombies = {}
		for i = 1, numZombies do
			local heading = (i - 1) * 2 * pi / numZombies
			local zombie = Entity.spawn('zombie', x + cos(heading) * distance, y + sin(heading) * distance, z, heading)
			zombies[#zombies + 1] = zombie
		end
		BehaviorHelper.sleep(6)
		for i = 1, numZombies do
			if zombies[i]:isValid() then
				zombies[i]:kill()
			end
		end
		BehaviorHelper.sleep(3)
	end
end

return states
