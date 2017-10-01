local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AttackComponentNode = FunctionalScriptNode:inherit 'Attack Component'

function AttackComponentNode:buildPins()
    self.rangeInPin = self:addInputPin(flat.types.NUMBER, 'Range')
    self.cooldownInPin = self:addInputPin(flat.types.NUMBER, 'Cooldown')
    self.autoAttackInPin = self:addInputPin(flat.types.BOOLEAN, 'Auto Attack')
    self.allowMoveInPin = self:addInputPin(flat.types.BOOLEAN, 'Allow Move')
    self.attackInPin = self:addInputPin(flat.types.FUNCTION, 'On Attack')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
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