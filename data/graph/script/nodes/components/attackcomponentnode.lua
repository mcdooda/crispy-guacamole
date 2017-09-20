local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AttackComponentNode = ScriptNode:inherit 'Attack Component'

function AttackComponentNode:buildPins()
    self.rangeInPin = self:addInputPin(PinTypes.NUMBER, 'Range')
    self.cooldownInPin = self:addInputPin(PinTypes.NUMBER, 'Cooldown')
    self.autoAttackInPin = self:addInputPin(PinTypes.BOOL, 'Auto Attack')
    self.allowMoveInPin = self:addInputPin(PinTypes.BOOL, 'Allow Move')

    self.componentOutPin = self:addOutputPin(PinTypes.TABLE, 'Component')
end

function AttackComponentNode:execute(runtime)
    local attackRange = runtime:readPin(self.rangeInPin)
    local attackCooldown = runtime:readPin(self.cooldownInPin)
    local autoAttack = runtime:readPin(self.autoAttackInPin)
    local moveDuringAttack = runtime:readPin(self.allowMoveInPin)

    local component = {
        attackRange = attackRange,
        attackCooldown = attackCooldown,
        autoAttack = autoAttack,
        moveDuringAttack = moveDuringAttack
    }

    runtime:writePin(self.componentOutPin, component)
end

return AttackComponentNode