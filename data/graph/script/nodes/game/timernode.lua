local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local TimerNode = ScriptNode:inherit 'Timer'

function TimerNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.durationInPin = self:addInputPin(flat.types.NUMBER, 'Duration')
    self.loopInPin = self:addInputPin(flat.types.BOOLEAN, 'Loop')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.onUpdateOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Update')
    self.progressionOutPin = self:addOutputPin(flat.types.NUMBER, 'Progression')
    self.onEndOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On End')
end

function TimerNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local duration = runtime:readPin(self.durationInPin)
    local loop = runtime:readPin(self.loopInPin)

    local timer = game.Timer()

    if self:isOutputPinPlugged(self.onUpdateOutPin) then
        timer:onUpdate(function(timer, elapsedTime)
            runtime:writePin(self.progressionOutPin, elapsedTime / duration)
            runtime:impulse(self.onUpdateOutPin)
        end)
    end

    if self:isOutputPinPlugged(self.onEndOutPin) then
        timer:onEnd(function()
            runtime:impulse(self.onEndOutPin)
        end)
    end

    timer:start(duration, loop)

    runtime:impulse(self.impulseOutPin)
end

return TimerNode