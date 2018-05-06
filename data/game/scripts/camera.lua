local function moveCameraTo(worldDestination, duration, easing)
	duration = duration or 1
	easing = easing or flat.easing.easeOut(flat.easing.cubic)
	local origin = Game.getCameraCenter()
	local destination = Game.convertToCameraPosition(worldDestination)
	local timer = Timer.new()
	timer:onUpdate(function(timer, elapsedTime)
		local t = elapsedTime / duration
		local result = flat.easing.lerp(t, origin, destination, easing)
		Game.setCameraCenter(result)
	end)
	timer:start(duration, false)
end

return {
	moveTo = moveCameraTo
}
