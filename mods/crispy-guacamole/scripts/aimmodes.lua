local FindInitialAbilityAimPositionEQSTemplate = require(Mod.getFilePath 'eqs/findinitialabilityaimposition')

-- Aim mode base
local AimModeBase = {}
function AimModeBase:inherit()
    return setmetatable({}, { __index = self })
end

-- None
local None = {
    computeInitialAimPosition = function(playerEntity, aimingEntities)
        return playerEntity:getPosition()
    end,

    computeAimPositions = function(playerEntity, currentMainPosition, aimingEntities)
        return {}
    end,

    useAbility = function(aimingEntities, aimPositions)
        for i = 1, #aimingEntities do
            local aimingEntity = aimingEntities[i]
            aimingEntity:enterState 'useAbility'
        end
    end,

    isValidTarget = function(playerEntity, otherEntity)
        return true
    end
}

-- Aim mode hostile base
local AimModeHostileBase = AimModeBase:inherit()

function AimModeHostileBase.computeInitialAimPosition(playerEntity, aimingEntities)
    local initialTargetEntityItem = flat.eqs.run(FindInitialAbilityAimPositionEQSTemplate, playerEntity, flat.eqs.RunMode.SingleBestItem)
    if initialTargetEntityItem then
        return initialTargetEntityItem.item:getPosition()
    else
        return playerEntity:getPosition() + playerEntity:getForward() * 2
    end
end

function AimModeHostileBase.isValidTarget(playerEntity, otherEntity)
    local isHostile = playerEntity:isHostile(otherEntity) or otherEntity:isHostile(playerEntity)
    local isAlive = otherEntity:isLiving() and otherEntity:isAlive()
    return isHostile and isAlive
end

-- Single unit
local SingleUnit = AimModeHostileBase:inherit()
-- TODO

-- All units single target
local AllUnitsSingleTarget = AimModeHostileBase:inherit()

function AllUnitsSingleTarget.computeAimPositions(playerEntity, currentMainPosition, aimingEntities)
    return { currentMainPosition }
end

function AllUnitsSingleTarget.useAbility(aimingEntities, aimPositions)
    for i = 1, #aimingEntities do
        local aimingEntity = aimingEntities[i]
        aimingEntity:getExtraData().abilityTargetPosition = aimPositions[1]
        aimingEntity:enterState 'useAbility'
    end
end

-- All units multiple targets base
local AllUnitsMultipleTargetsBase = AimModeHostileBase:inherit()

function AllUnitsMultipleTargetsBase.useAbility(aimingEntities, aimPositions)
    for i = 1, #aimPositions do
        local aimPosition = aimPositions[i]
        local aimingEntity = aimingEntities[i]
        if aimingEntity then
            aimingEntity:getExtraData().abilityTargetPosition = aimPosition
            aimingEntity:enterState 'useAbility'
        end
    end
end

-- All units multiple targets cone
local function rotate(x, y, a)
    local s = math.sin(a)
    local c = math.cos(a)
    return (c * x) - (s * y), (s * x) + (c * y)
end

local AllUnitsMultipleTargetsCone = AllUnitsMultipleTargetsBase:inherit()

function AllUnitsMultipleTargetsCone.computeAimPositions(playerEntity, currentMainPosition, aimingEntities)
    local numEntities = #aimingEntities
    local mainDirection = currentMainPosition - playerEntity:getPosition()
    mainDirection:z(0)
    local mainDistance = mainDirection:length()

    local totalAngle = math.pi * 0.5

    local aimPositions = {}
    if mainDistance > 0 then
        for i = 1, numEntities do
            local angle = (i - numEntities / 2 - 0.5) / numEntities * totalAngle
            local offsetX, offsetY = rotate(mainDirection:x(), mainDirection:y(), angle)
            local aimPosition = playerEntity:getPosition() + flat.Vector3(offsetX, offsetY, 0)
            aimPositions[#aimPositions + 1] = aimPosition
        end
    else
        for i = 1, numEntities do
            aimPositions[#aimPositions + 1] = playerEntity:getPosition()
        end
    end
    return aimPositions
end

-- All units multiple targets line
local AllUnitsMultipleTargetsLine = AllUnitsMultipleTargetsBase:inherit()

function AllUnitsMultipleTargetsLine.computeAimPositions(playerEntity, currentMainPosition, aimingEntities)
    local numEntities = #aimingEntities
    local mainDirection = currentMainPosition - playerEntity:getPosition()
    mainDirection:z(0)
    local sideVector = mainDirection:getNormalized():cross(flat.Vector3(0, 0, 1))
    local mainDistance = mainDirection:length()

    local aimPositions = {}
    if mainDistance > 0 then
        for i = 1, numEntities do
            local offset = (i - numEntities / 2 - 0.5) * 0.7
            local aimPosition = currentMainPosition + sideVector * offset
            aimPositions[#aimPositions + 1] = aimPosition
        end
    else
        for i = 1, numEntities do
            aimPositions[#aimPositions + 1] = playerEntity:getPosition()
        end
    end
    return aimPositions
end

return {
    None                        = None,
    SingleUnit                  = SingleUnit,
    AllUnitsSingleTarget        = AllUnitsSingleTarget,
    AllUnitsMultipleTargetsCone = AllUnitsMultipleTargetsCone,
    AllUnitsMultipleTargetsLine = AllUnitsMultipleTargetsLine
}