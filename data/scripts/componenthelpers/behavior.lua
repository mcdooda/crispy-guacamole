local Time = Time
local getTime = Time.getTime
local yield = coroutine.yield

local function sleep(duration)
	local endTime = getTime() + duration
	local yield = coroutine.yield
	while getTime() < endTime do
		yield()
	end
end

return {
    sleep = sleep
}