local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetStrafingNode = ScriptNode:inherit 'Set Strafing'

function SetStrafingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.strafingInPin = self:addInputPin(flat.types.BOOLEAN, 'Strafing')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetStrafingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local strafing = runtime:readPin(self.strafingInPin)

    entity:setIsStrafing(strafing)

    runtime:impulse(self.impulseOutPin)
end

return SetStrafingNode