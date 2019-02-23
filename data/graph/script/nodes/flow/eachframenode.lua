local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local coyield = coroutine.yield

local EachFrameNode = ScriptNode:inherit 'Each Frame'

function EachFrameNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')

    self.impulseEachFramePin = self:addOutputPin(PinTypes.IMPULSE, 'Each Frame')
end

function EachFrameNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)

    while true do
        runtime:impulse(self.impulseEachFramePin)
        coyield()
    end
end

return EachFrameNode