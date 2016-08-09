local function sleep(duration)
	local getTime = Time.getTime
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

local states = {}

function states:init(sheep)
	sheep:enterState('wander')
	print 'TEST'
end

function states:idle(sheep)
	--print '******* IDLE'
end

function states:wander(sheep)
	local x, y = sheep:getPosition()
	while true do
		local rx = x + (math.random() * 2 - 1) * 2
		local ry = y + (math.random() * 2 - 1) * 2
		sheep:moveTo(rx, ry)
		
		local duration = math.random() + 1
		sleep(duration)
	end
end

return states
