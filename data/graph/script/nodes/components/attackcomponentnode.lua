local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AttackComponentNode = FunctionalScriptNode:inherit 'Attack Component'

function AttackComponentNode:buildPins()
    self.rangeInPin = self:addInputPin(PinTypes.NUMBER, 'Range')
    self.cooldownInPin = self:addInputPin(PinTypes.NUMBER, 'Cooldown')
    self.autoAttackInPin = self:addInputPin(PinTypes.BOOLEAN, 'Auto Attack')
    self.allowMoveInPin = self:addInputPin(PinTypes.BOOLEAN, 'Allow Move')
    self.attackInPin = self:addInputPin(PinTypes.FUNCTION, 'Attack')

    self.componentOutPin = self:addOutputPin(PinTypes.TABLE, 'Component')
end

function AttackComponentNode:execute(runtime)
    local attackRange = runtime:readPin(self.rangeInPin)
    local attackCooldown = runtime:readPin(self.cooldownInPin)
    local autoAttack = runtime:readPin(self.autoAttackInPin)
    local moveDuringAttack = runtime:readPin(self.allowMoveInPin)
    local attackFunction = runtime:readPin(self.attackInPin)

    local component = {
        attackRange = attackRange,
        attackCooldown = attackCooldown,
        autoAttack = autoAttack,
        moveDuringAttack = moveDuringAttack,
        attack = attackFunction
    }

    runtime:writePin(self.componentOutPin, component)
end

return AttackComponentNode