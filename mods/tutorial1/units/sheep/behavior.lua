local states = {}

function states:init(sheep)
	for i = 1, 10 do
		print(i, self, sheep)
		coroutine.yield()
	end
	--sheep:enterState(self.wander)
end

function states:idle()
	
end

function states:wander(sheep)
	local x, y = sheep:getPosition()
	local px1, py1 = x + 1, y - 1
	local px2, py2 = x - 1, y + 1
	while true do
		sheep:moveTo(px1, py1)
		sheep:moveTo(px2, py2)
	end
end

return states
