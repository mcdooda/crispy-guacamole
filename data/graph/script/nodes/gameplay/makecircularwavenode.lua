local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'
local Wave = require 'data/game/scripts/wave'

local MakeCircularWaveNode = ScriptNode:inherit 'Make Circular Wave'

function MakeCircularWaveNode:buildPins()
    self.impulseInPin   = self:addInputPin(PinTypes.IMPULSE,           'In')
    self.positionInPin  = self:addInputPin(flat.types['flat.Vector2'], 'Position')
    self.radiusInPin    = self:addInputPin(flat.types.NUMBER,          'Radius')
    self.heightInPin    = self:addInputPin(flat.types.NUMBER,          'Height')
    self.durationInPin  = self:addInputPin(flat.types.NUMBER,          'Duration')

    self.impulseOutPin  = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.finishedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Finished')
end

function MakeCircularWaveNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local position = runtime:readPin(self.positionInPin)
    local radius   = runtime:readPin(self.radiusInPin)
    local height   = runtime:readPin(self.heightInPin)
    local duration = runtime:readPin(self.durationInPin)

    local function onEnd()
        runtime:impulse(self.finishedOutPin)
    end

    Wave.circular(position, radius, height, duration, onEnd)

    runtime:impulse(self.impulseOutPin)
end

return MakeCircularWaveNode