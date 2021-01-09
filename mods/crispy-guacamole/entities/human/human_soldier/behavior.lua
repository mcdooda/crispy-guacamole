local BasicUnitBehavior = require(Mod.getFilePath 'scripts/basicunitbehavior')

local states = BasicUnitBehavior.basicUnit()

function states:useAbility(soldier)
    local extraData = soldier:getExtraData()
    soldier:playAnimation('cast', 1, false)

    return extraData.currentLoopingState
end

return states