local GathererBehavior = require 'data/scripts/componenthelpers/behaviors/gatherer'

local Money = require require(Mod.getFilePath 'scripts/money')
local EntitiesByType = require(Mod.getFilePath 'scripts/entitiesbytype')
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local states = GathererBehavior.basicGatherer('human_farm', 'wheat_field')

local lockResource, unlockResource

local function getClosestBuilding(gatherer)
    return EntitiesByType:getClosests('human_farm', gatherer:getPosition():toVector2())[1]
end
function states:gatherResources(gatherer)
    gatherer:setCycleAnimation 'gather'
    local extraData = gatherer:getExtraData()
    local building = gatherer:getExtraData().building or nil

    if not building or not building:isValid() then
        building = getClosestBuilding(gatherer)
        extraData.building = building
    end

    if building then
        gatherer:interactWith(building)
    end
end
local function hasAmountAndNoGatherer(wheatField)
    local extraData = wheatField:getExtraData()
    return extraData.amount > 0 and (extraData.gatherer == nil or not extraData.gatherer:isValid())
end

local function getClosestResource(gatherer)
    return EntitiesByType:getClosestsValid('wheat_field', gatherer:getPosition():toVector2(), hasAmountAndNoGatherer)[1]
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
    local targetResource = gatherer:getInteractionEntity()
    local targetResourceData = targetResource:getExtraData()
    local extraData = gatherer:getExtraData()

    if extraData.resources and extraData.resources ~= targetResource then
        unlockResource(gatherer)
    end
    if not targetResource:isValid() then
        return 'lookForWheat'
    end
    if hasAmountAndNoGatherer(targetResource) then
        lockResource(gatherer, targetResource)
    else
        if targetResourceData.gatherer ~= gatherer then
            if extraData.resourcesAmount > 0 then
                unlockResource(gatherer)
                return 'gatherResources'
            else
                return 'lookForWheat'
            end
        end
    end

    if targetResourceData.amount > 0 and extraData.resourcesAmount < 5 then
        gatherer:playAnimation 'reap'
        gatherer:sleep(0.2)
        local collected = targetResourceData:withdraw(1)
        extraData.resourcesAmount = extraData.resourcesAmount + collected
    else
        if targetResourceData.amount == 0 then
            unlockResource(gatherer)
        end
        return 'gatherResources'
    end
    gatherer:interactWith(targetResource)
end

function states:onPlayerMoveOrder(gatherer, destination, interactionEntity)
    BarkSystem:requestBark(Mod.getFilePath 'sounds/sound_test')
    unlockResource(gatherer)
    if interactionEntity == nil then
        return 'wander'
    end
end

function states:lookForWheat(gatherer)
    unlockResource(gatherer)
    local nextResource = getClosestResource(gatherer)
    if nextResource then
        lockResource(gatherer, nextResource)
        gatherer:interactWith(nextResource)
    else
        return 'lookForWheat'
    end
end

function lockResource(gatherer, resource)
    gatherer:getExtraData().resources = resource
    resource:getExtraData().gatherer = gatherer
end

function unlockResource(gatherer)
    local extraData = gatherer:getExtraData()
    if extraData.resources then
        if extraData.resources:getExtraData().gatherer == gatherer then
            extraData.resources:getExtraData().gatherer = nil
        end
        extraData.resources = nil
    end
end

return states