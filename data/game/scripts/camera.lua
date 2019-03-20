local currentTimer
local function lockAndMoveTo(worldDestination, duration, easing)
	game.lockCamera()
	duration = duration or 1
	easing = easing or flat.easing.easeOut(flat.easing.cubic)
	local origin = game.getCameraCenter()
	local destination = game.convertToCameraPosition(worldDestination)
	if currentTimer then
		currentTimer:stop()
	end
	currentTimer = game.Timer()
	currentTimer:onUpdate(function(timer, elapsedTime)
		local t = elapsedTime / duration
		local result = flat.easing.lerp(t, origin, destination, easing)
		game.setCameraCenter(result)
	end)
	currentTimer:onEnd(function()
		game.unlockCamera()
		currentTimer = nil
	end)
	currentTimer:start(duration, false)
end

return {
	lockAndMoveTo = lockAndMoveTo
}
