local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local TimerNode = ScriptNode:inherit 'Timer'

function TimerNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.durationInPin = self:addInputPin(flat.types.NUMBER, 'Duration')
    self.loopInPin = self:addInputPin(flat.types.BOOLEAN, 'Loop')
    self.contextInPin = self:addInputPin(PinTypes.ANY, 'Context', self.contextInPinPlugged, self.contextInPinUnplugged)

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.onUpdateOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Update')
    self.progressionOutPin = self:addOutputPin(flat.types.NUMBER, 'Progression')
    self.onEndOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On End')
    self.contextOutPin = self:addOutputPin(PinTypes.ANY, 'Context', self.contextOutPinPlugged, self.contextOutPinUnplugged)
end

function TimerNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local duration = runtime:readPin(self.durationInPin)
    local loop = runtime:readPin(self.loopInPin)
    local context = runtime:readPin(self.contextInPin)

    local timer = game.Timer()

    if self:isOutputPinPlugged(self.onUpdateOutPin) then
        timer:onUpdate(function(timer, elapsedTime)
            runtime:writePin(self.progressionOutPin, elapsedTime / duration)
            runtime:writePin(self.contextOutPin, context)
            runtime:impulse(self.onUpdateOutPin)
        end)
    end

    if self:isOutputPinPlugged(self.onEndOutPin) then
        timer:onEnd(function()
            runtime:writePin(self.contextOutPin, context)
            runtime:impulse(self.onEndOutPin)
        end)
    end

    timer:start(duration, loop)

    runtime:writePin(self.contextOutPin, context)
    runtime:impulse(self.impulseOutPin)
end

function TimerNode:contextInPinPlugged(pin)
    self:setOutputPinType(self.contextOutPin, pin.pinType)
    return true
end

function TimerNode:contextInPinUnplugged(pin)
    if not self:isOutputPinPlugged(self.contextOutPin) then
        self:setInputPinType(self.contextInPin, PinTypes.ANY)
        self:setOutputPinType(self.contextOutPin, PinTypes.ANY)
        return true
    end
end

function TimerNode:contextOutPinPlugged(pin)
    self:setInputPinType(self.contextInPin, pin.pinType)
    return true
end

function TimerNode:contextOutPinUnplugged(pin)
    if not self:isInputPinPlugged(self.contextInPin) and not self:isOutputPinPlugged(pin) then
        self:setInputPinType(self.contextInPin, PinTypes.ANY)
        self:setOutputPinType(self.contextOutPin, PinTypes.ANY)
        return true
    end
end

return TimerNode