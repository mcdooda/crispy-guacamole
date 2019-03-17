local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local RightStickDirectionNode = FunctionalScriptNode:inherit 'Right Stick Direction'

function RightStickDirectionNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.stickDirectionOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Direction')
end

function RightStickDirectionNode:execute(runtime)
    local gamepadId = 0

    local rawX, rawY = Gamepads.getRightStickValue(gamepadId)

    local function rotate(x, y, a)
        local s = math.sin(a);
        local c = math.cos(a);
        return (c * x) - (s * y), (s * x) + (c * y);
    end

    local x, y = rotate(-rawX, rawY, -math.pi / 4)

    runtime:writePin(self.stickDirectionOutPin, flat.Vector2(x, y))
end

return RightStickDirectionNode