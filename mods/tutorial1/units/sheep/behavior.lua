local states = {}

function states:init(sheep)
	sheep:enterState(self.wander)
	coroutine.yield()
end

function states:idle()
	print 'idle'
end

function states:wander(sheep)
	while true do
		local x, y = sheep:getPosition()
		local rx = x + math.random() * 10 - 5
		local ry = y + math.random() * 10 - 5
		sheep:moveTo(rx, ry)
		coroutine.yield()
	end
end

return states
