local math = math
local random = math.random

local function sleep(duration)
	local getTime = Time.getTime
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

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
			local heading = (i - 1) * 2 * math.pi / numZombies
			local zombie = Entity.spawn('zombie', x + math.cos(heading) * distance, y + math.sin(heading) * distance, z, heading)
			zombies[#zombies + 1] = zombie
		end
		sleep(6)
		for i = 1, numZombies do
			zombies[i]:despawn()
		end
		sleep(3)
	end
end

return states
