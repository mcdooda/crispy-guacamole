local currentTimer
local function lockAndMoveTo(worldDestination, duration, easing)
	if currentTimer then
		currentTimer:stop()
	end
	if duration == 0 then
		local destination = game.convertToCameraPosition(worldDestination)
		game.setCameraCenter(destination)
		return
	end
	game.lockCamera()
	duration = duration or 1
	easing = easing or flat.easing.easeOut(flat.easing.cubic)
	local origin = game.getCameraCenter()
	local destination = game.convertToCameraPosition(worldDestination)
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

local random = math.random
local shakeTimer
local function shake(duration)
	duration = duration or 0.1
	if shakeTimer then
		shakeTimer:stop()
	end
	if duration == 0 then
		return
	end
	local position = game.getCameraCenter()
	shakeTimer = game.Timer()
	shakeTimer:onUpdate(function(timer, elapsedTime)
		local newPosition = flat.Vector2(position:x() + random(3), position:y() + random(3))
		game.setCameraCenter(newPosition)
	end)
	shakeTimer:onEnd(function()
		shakeTimer = nil
	end)
	shakeTimer:start(duration, false)
end

return {
	lockAndMoveTo = lockAndMoveTo,
	shake = shake
}
