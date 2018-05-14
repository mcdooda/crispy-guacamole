local function moveCameraTo(worldDestination, duration, easing)
	duration = duration or 1
	easing = easing or flat.easing.easeOut(flat.easing.cubic)
	local origin = game.getCameraCenter()
	local destination = game.convertToCameraPosition(worldDestination)
	local timer = game.Timer()
	timer:onUpdate(function(timer, elapsedTime)
		local t = elapsedTime / duration
		local result = flat.easing.lerp(t, origin, destination, easing)
		game.setCameraCenter(result)
	end)
	timer:start(duration, false)
end

return {
	moveTo = moveCameraTo
}
