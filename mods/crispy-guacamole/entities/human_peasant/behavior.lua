local GathererBehavior = require 'data/scripts/componenthelpers/behaviors/gatherer'

local Money = require 'mods/crispy-guacamole/scripts/money'
local EntitiesByType = require 'mods/crispy-guacamole/scripts/entitiesbytype'

local states = GathererBehavior.basicGatherer('human_farm', 'wheat_field')

local function getClosestBuilding(gatherer)
    return EntitiesByType:getClosests('human_farm', gatherer:getPosition():toVector2())[1]
end
function states:gatherResources(gatherer)
    gatherer:setCycleAnimation 'gather'
    local extraData = gatherer:getExtraData()
    local building = not extraData.resourcesChanged and gatherer:getExtraData().building or nil

    if not building or not building:isValid() then
        building = getClosestBuilding(gatherer)
        extraData.building = building
    end

    if building then
        gatherer:interactWith(building)
    end
end

local function getClosestResource(gatherer)
    return EntitiesByType:getClosestsValid('wheat_field', gatherer:getPosition():toVector2(), 
        function(entity)
            local extraData = entity:getExtraData()
            return extraData.amount > 0 and (extraData.gatherer == nil or not extraData.gatherer:isValid())
        end)[1]
end

function states:backToWork(gatherer)
    local resources = states:addResourceAndGetToNext(gatherer)
    if not resources or resources:getExtraData().amount == 0 then
        return 'lookForWheat'
    else
        gatherer:interactWith(resources)
    end
end

function states:reapResources(gatherer)
    local resources = gatherer:getInteractionEntity()
    local resourceData = resources:getExtraData()
    local extraData = gatherer:getExtraData()
    if not resources:isValid() then 
        return 'lookForWheat'
    end
    if extraData.resources and extraData.resources ~= resources then
        extraData.resources:getExtraData().gatherer = nil
    end

    if resourceData.amount == 0 and resourceData.gatherer ~= nil and resourceData.gatherer == gatherer then
        resourceData.gatherer = nil 
        if extraData.resourcesAmount > 0 then
            return 'gatherResources'
        else
            return 'lookForWheat'
        end
    end

    if (resourceData.gatherer ~= nil and resourceData.gatherer ~= gatherer) or resourceData.amount== 0 then
        return 'lookForWheat'
    end

    extraData.resourcesChanged = false
    if extraData.resources ~= resources then
        extraData.resources = resources
        extraData.resourcesChanged = true
    end
    
    resourceData.gatherer = gatherer
    if resourceData.amount > 0 and extraData.resourcesAmount < 5 then
        gatherer:playAnimation 'reap'
        gatherer:sleep(0.2)
        local collected = resourceData:withdraw(1)
        extraData.resourcesAmount = extraData.resourcesAmount + collected
    else 
        return 'gatherResources'
    end
    gatherer:interactWith(resources)
end

function states:lookForWheat(gatherer)
    local extraData = gatherer:getExtraData()
    if extraData.resources and extraData.resources:getExtraData().gatherer == gatherer then
        extraData.resources:getExtraData().gatherer = nil
    end
    if extraData.resources then
        extraData.resources = nil
    end
    local nextResource = getClosestResource(gatherer)
    if nextResource then
        nextResource:getExtraData().gatherer = gatherer
        gatherer:interactWith(nextResource)
    else
        return 'lookForWheat'
    end
end

return states