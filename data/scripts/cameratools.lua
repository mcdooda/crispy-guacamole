local CameraTools = {}

function CameraTools.lookAtCenter()
	local w, h = Map.getDimensions()
	local centerX = w / 2
	local centerY = h / 2
	Camera.lookAt(centerX, centerY)
end

return CameraTools
