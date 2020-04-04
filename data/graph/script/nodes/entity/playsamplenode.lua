local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PlaySampleNode = ScriptNode:inherit 'Play Sample'

function PlaySampleNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.sampleNameInPin = self:addInputPin(flat.types.STRING, 'Sample Name')
    self.loopCountInPin = self:addInputPin(flat.types.NUMBER, 'Loop Count')
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PlaySampleNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local sampleName = runtime:readPin(self.sampleNameInPin)
    local loopCount = runtime:readPin(self.loopCountInPin)

    entity:playSample(sampleName, loopCount)

    runtime:impulse(self.impulseOutPin)
end

return PlaySampleNode