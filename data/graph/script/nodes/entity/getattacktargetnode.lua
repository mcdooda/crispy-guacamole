local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetAttackTargetNode = FunctionalScriptNode:inherit 'Get Attack Target'

function GetAttackTargetNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.attackTargetOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Attack Target')
end

function GetAttackTargetNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local attackTarget = entity:getAttackTarget()

    runtime:writeOptionalPin(self.attackTargetOutPin, attackTarget)
end

return GetAttackTargetNode