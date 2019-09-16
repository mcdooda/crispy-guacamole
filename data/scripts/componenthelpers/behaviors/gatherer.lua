local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'
local Money = require 'mods/crispy-guacamole/scripts/money'

local function basicGatherer(buildingName, resourceName)
	local function getClosestBuilding(gatherer)
		return EntitiesByType:getClosests(buildingName, gatherer:getPosition():toVector2())[1]
	end
	
	local function getClosestResource(gatherer)
		local building = getClosestBuilding(gatherer)
		if building then
			local gathererPosition = gatherer:getPosition()
			local closestMineral
			local closestEntityDistance2 = math.huge
			local closestResources = EntitiesByType:getClosests(resourceName, building:getPosition():toVector2())
			for i = 1, #closestResources do
				local mineral = closestResources[i]
				local distance2 = (gathererPosition - mineral:getPosition()):length2()
				if distance2 < closestEntityDistance2 then
					closestMineral = mineral
					closestEntityDistance2 = distance2
				end
			end
			return closestMineral
		else
			return EntitiesByType:getClosests(resourceName, gatherer:getPosition():toVector2())[1]
		end
	end

	local states = BehaviorHelper.customAttacker(
		BehaviorHelper.doNothing,
		BehaviorHelper.findClosestTarget,
		BehaviorHelper.isValidHostileAttackTarget
	)

	local init = states.init
	function states:init(gatherer)
		gatherer:setCycleAnimation 'move'
		gatherer:getExtraData().resourcesAmount = 0
		init(self, gatherer)
	end

	function states:gatherResources(gatherer)
		local resources = gatherer:getInteractionEntity()

		local extraData = gatherer:getExtraData()
		local resourcesChanged = false
		if extraData.resources ~= resources then
			extraData.resources = resources
			resourcesChanged = true
		end

		local resourceData = resources:getExtraData()
		local collected = resourceData:withdraw(1)
		gatherer:setCycleAnimation 'gather'
		extraData.resourcesAmount = collected
		if resourceData.amount == 0 then
			resources:despawn()
		end

		local building = not resourcesChanged and gatherer:getExtraData().building or nil

		if not building or not building:isValid() then
			building = getClosestBuilding(gatherer)
			extraData.building = building
		end

		if building then
			gatherer:interactWith(building)
		end
	end

	function states:addResourceAndGetToNext(gatherer)
		local building = gatherer:getInteractionEntity()
		local extraData = gatherer:getExtraData()

		extraData.building = building
		if extraData.resourcesAmount > 0 then
			Money:add(extraData.resourcesAmount)
			gatherer:setCycleAnimation 'move'
			extraData.resourcesAmount = 0
		end
		local resources = extraData.resources
		if not resources or not resources:isValid() then
			return nil
		else 
			return resources
		end
	end

	function states:backToWork(gatherer)
		local resources = states:addResourceAndGetToNext(gatherer)
		if not resources then
			resources = getClosestResource(gatherer)
		end 
		if resources then
			gatherer:interactWith(resources)
		end
	end

	function states:missingInteractionEntity(gatherer)
		gatherer:clearPath()
		gatherer:getExtraData().building = nil
		local interactionStateName = gatherer:getInteractionStateName()
		if interactionStateName == 'gatherResources' then
			local resources = getClosestResource(gatherer)
			if resources then
				gatherer:interactWith(resources)
			else
				local building = getClosestBuilding(gatherer)
				if building then
					gatherer:interactWith(building)
				end
			end
		elseif interactionStateName == 'backToWork' then
			local building = getClosestBuilding(gatherer)
			if building then
				gatherer:interactWith(building)
			end
		else
			error('missing interaction entity for state: ' .. interactionStateName)
		end
	end

	function states:build(gatherer)
		local building = gatherer:getInteractionEntity()
		local extraData = building:getExtraData()

		if not building:isValid() then
			return
		elseif not extraData.buildingInProgress then
			gatherer:interactWith(building)
			return
		end

		gatherer:playAnimation 'build'
		gatherer:sleep(0.5)

		if not building:isValid() then
			return
		elseif not extraData.buildingInProgress then
			gatherer:interactWith(building)
			return
		end
		
		local health = math.min(building:getHealth() + 1, building:getMaxHealth())
		building:setHealth(health)
		gatherer:interactWith(building)
	end

	return states
end

return { 
	basicGatherer = basicGatherer 
} 