local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local EnterStateNode = ScriptNode:inherit 'Enter State'

function EnterStateNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.stateNameInPin = self:addInputPin(flat.types.STRING, 'State Name')
    self.ignoreErrorsInPin = self:addInputPin(flat.types.BOOLEAN, 'Ignore Errors')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function EnterStateNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local stateName = runtime:readPin(self.stateNameInPin)
    local ignoreErrors = runtime:readPin(self.ignoreErrorsInPin)

    entity:enterState(stateName, ignoreErrors)

    runtime:impulse(self.impulseOutPin)
end

return EnterStateNode