local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SleepNode = ScriptNode:inherit 'Sleep'

function SleepNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.durationInPin = self:addInputPin(flat.types.NUMBER, 'Duration')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SleepNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local duration = runtime:readPin(self.durationInPin)

    game.sleep(duration)

    runtime:impulse(self.impulseOutPin)
end

return SleepNode