local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LeftStickDirectionNode = ScriptNode:inherit 'Left Stick Direction'

function LeftStickDirectionNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.gamepadIdInPin = self:addInputPin(flat.types.NUMBER, 'Gamepad Id')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.stickDirectionOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Direction')
end

function LeftStickDirectionNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local gamepadId = runtime:readPin(self.gamepadIdInPin)

    local rawX, rawY = Gamepads.getLeftStickValue(gamepadId)

    local function rotate(x, y, a)
        local s = math.sin(a);
        local c = math.cos(a);
        return (c * x) - (s * y), (s * x) + (c * y);
    end

    local x, y = rotate(-rawX, rawY, -math.pi / 4)

    runtime:writePin(self.stickDirectionOutPin, flat.Vector2(x, y))

    runtime:impulse(self.impulseOutPin)
end

return LeftStickDirectionNode