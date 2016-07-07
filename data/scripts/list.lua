local List = {}

function List:new ()
	return setmetatable({first = 0, last = -1}, { __index = self })
end

function List:isEmpty()
	return self.first > self.last
end

function List:pushLeft(value)
	local first = self.first - 1
	self.first = first
	self[first] = value
end

function List:pushrRight(value)
	local last = self.last + 1
	self.last = last
	self[last] = value
end

function List:popLeft()
	local first = self.first
	if first > self.last then error("list is empty") end
	local value = self[first]
	self[first] = nil        -- to allow garbage collection
	self.first = first + 1
	return value
end

function List:popRight()
	local last = self.last
	if self.first > last then error("list is empty") end
	local value = self[last]
	self[last] = nil         -- to allow garbage collection
	self.last = last - 1
	return value
end

return List
