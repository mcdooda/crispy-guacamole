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
local inputs = {}

function states:init(sheep)
	sheep:enterState 'wander'
	--sheep:playAnimation 'graze'
end

function states:idle(sheep)
	sheep:jump()
end

function states:wander(sheep)
	local x, y = sheep:getPosition()
	while true do

		do
			local rx = x + (random() * 2 - 1) * 2
			local ry = y + (random() * 2 - 1) * 2
			sheep:moveTo(rx, ry)
		end

		do
			while random() < 0.3 do
				sheep:playAnimation 'graze'
			end
		end

		do
			if random() < 0.1 then
				sheep:playAnimation 'crouch'
				local numLoops = random(3, 6)
				sheep:playAnimation('sleep', numLoops)
				sheep:playAnimation 'standup'
			end
		end
	end
end

function states:flee(sheep)
	local endFleeTime = Time.getTime() + 5
	while Time.getTime() < endFleeTime do
		local x, y = sheep:getPosition()
		local rx = x + (random() * 5 - 5) * 10
		local ry = y + (random() * 5 - 5) * 10
		sheep:moveTo(rx, ry)
	end
end

function states:onEntityEnteredVisionRange(sheep, entity)
	local isHostile = entity:getTemplateName() ~= 'sheep'
	if isHostile then
		return 'flee'
	end
end

return states