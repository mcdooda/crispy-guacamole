local states = {}

function states:init()
	enter(self.wander)
end

function states:wander()
	local x, y, z = getPosition()
	local px1, py1 = x + 1, y - 1
	local px2, py2 = x - 1, y + 1
	while true do
		moveTo(px1, py1)
		moveTo(px2, py2)
	end
end

return states
