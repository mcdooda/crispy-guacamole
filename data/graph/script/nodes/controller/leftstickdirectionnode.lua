local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LeftStickDirectionNode = FunctionalScriptNode:inherit 'Left Stick Direction'

function LeftStickDirectionNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.stickDirectionOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Direction')
end

function LeftStickDirectionNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local gamepadIndex = entity:getGamepadIndex()

    local rawX, rawY = Gamepads.getLeftStickValue(gamepadIndex)

    local function rotate(x, y, a)
        local s = math.sin(a);
        local c = math.cos(a);
        return (c * x) - (s * y), (s * x) + (c * y);
    end

    local x, y = rotate(-rawX, rawY, -math.pi / 4)

    runtime:writePin(self.stickDirectionOutPin, flat.Vector2(x, y))
end

return LeftStickDirectionNode