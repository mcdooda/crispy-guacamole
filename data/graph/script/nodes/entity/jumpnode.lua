local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local JumpNode = ScriptNode:inherit 'Jump'

function JumpNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.yieldInPin = self:addInputPin(flat.types.BOOLEAN, 'Yield')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function JumpNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local yield = runtime:readPin(self.yieldInPin)

    entity:jump(yield)

    runtime:impulse(self.impulseOutPin)
end

return JumpNode