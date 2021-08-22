local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AddSlotNode = ScriptNode:inherit 'Add Slot'

function AddSlotNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.slotNameInPin = self:addInputPin(flat.types.STRING, 'Slot Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function AddSlotNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local slotName = runtime:readPin(self.slotNameInPin)

    entity:getExtraData()[slotName] = flat.Slot:new()

    runtime:impulse(self.impulseOutPin)
end

return AddSlotNode