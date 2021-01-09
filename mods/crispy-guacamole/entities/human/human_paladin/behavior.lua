local BasicUnitBehavior = require(Mod.getFilePath 'scripts/basicunitbehavior')

local states = BasicUnitBehavior.basicUnit()

function states:useAbility(paladin)
    local extraData = paladin:getExtraData()
    paladin:playAnimation('cast', 1, false)

    local abilityTargetPosition = assert(extraData.abilityTargetPosition)
    extraData.abilityTargetPosition = nil
    do
        Entity.spawn('fx_fist', abilityTargetPosition)
    end
    return extraData.currentLoopingState
end

return states