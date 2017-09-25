local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetAttackTargetNode = FunctionalScriptNode:inherit 'Get Attack Target'

function GetAttackTargetNode:buildPins()
    self.entityInPin = self:addInputPin(PinTypes.USERDATA, 'Entity')

    self.attackTargetOutPin = self:addOutputPin(PinTypes.USERDATA, 'Attack Target')
end

function GetAttackTargetNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local attackTarget = entity:getAttackTarget()

    runtime:writePin(self.attackTargetOutPin, attackTarget)
end

return GetAttackTargetNode