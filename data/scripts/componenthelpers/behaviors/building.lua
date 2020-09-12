local EntitiesByType = require(Mod.getFilePath 'scripts/entitiesbytype')
local Building = require 'data/scripts/building'

local yield = coroutine.yield

local function basicBuilding()
	local states = {}

	local function init(building)
		building:setCycleAnimation('stand')
	end

	function states:init(building)
		EntitiesByType:add(building)
		coroutine.yield() -- give external code a chance to change state immediately after spawning
		init(building)
	end

	function states:under_construction(building)
		building:clearCycleAnimation()
		building:setInteractionState 'build'
		Building.start_construction(building)
		local extraData = building:getExtraData()
		extraData.buildingInProgress = true
		while building:getHealth() < building:getMaxHealth() do
			yield()
		end
		extraData.buildingInProgress = false

		building:resetInteractionState()
		init(building)
	end

	return states
end

return {
	basicBuilding = basicBuilding
}