dofile('data/editor/scripts/toolbar.lua')
dofile('data/editor/scripts/leftpanel.lua')

require 'data/scripts/displayfps' ()

local CameraTools = require 'data/scripts/cameratools'

CameraTools.lookAtCenter()
Input.onKeyDown(Input.ESCAPE, Game.exit)
--[[
for _, e in pairs(Map.getEntities()) do
	if Entity.isUnit(e) then
		Entity.destroy(e)
	end
end
]]
