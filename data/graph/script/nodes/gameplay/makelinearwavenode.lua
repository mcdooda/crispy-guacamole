local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'
local Wave = require 'mods/crispy-guacamole/scripts/wave'

local MakeLinearWaveNode = ScriptNode:inherit 'Make Linear Wave'

function MakeLinearWaveNode:buildPins()
    self.impulseInPin       = self:addInputPin(PinTypes.IMPULSE,           'In')
    self.startPositionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Start')
    self.endPositionInPin   = self:addInputPin(flat.types['flat.Vector2'], 'End')
    self.widthInPin         = self:addInputPin(flat.types.NUMBER,          'Width')
    self.heightInPin        = self:addInputPin(flat.types.NUMBER,          'Height')
    self.durationInPin      = self:addInputPin(flat.types.NUMBER,          'Duration')
    self.speedInPin         = self:addInputPin(flat.types.NUMBER,          'Speed')

    self.impulseOutPin  = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.finishedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Finished')
end

function MakeLinearWaveNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local startPosition = runtime:readPin(self.startPositionInPin)
    local endPosition   = runtime:readPin(self.endPositionInPin)
    local width         = runtime:readPin(self.widthInPin)
    local height        = runtime:readPin(self.heightInPin)
    local duration      = runtime:readPin(self.durationInPin)
    local speed         = runtime:readPin(self.speedInPin)

    local function onEnd()
        runtime:impulse(self.finishedOutPin)
    end

    Wave.linear(startPosition, endPosition, width, height, duration, speed, onEnd)

    runtime:impulse(self.impulseOutPin)
end

return MakeLinearWaveNode