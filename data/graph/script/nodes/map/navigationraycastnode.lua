local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local NavigationRaycastNode = ScriptNode:inherit 'Navigation Raycast'

function NavigationRaycastNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.startPositionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Start Position')
    self.directionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Direction')
    self.lengthInPin = self:addInputPin(flat.types.NUMBER, 'Length')
    self.jumpHeightInPin = self:addInputPin(flat.types.NUMBER, 'Jump Height')
    self.navigabilityMaskInPin = self:addInputPin(flat.types.NUMBER, 'Navigability Mask')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.hitOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Hit')
    self.endPositionOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'End Position')
end

function NavigationRaycastNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local startPosition = runtime:readPin(self.startPositionInPin)
    local direction = runtime:readPin(self.directionInPin)
    local length = runtime:readPin(self.lengthInPin)
    local jumpHeight = runtime:readPin(self.jumpHeightInPin)
    local navigabilityMask = runtime:readPin(self.navigabilityMaskInPin)

    local hit, endPosition = Map.navigationRaycast(startPosition, direction, length, jumpHeight, navigabilityMask)

    runtime:writePin(self.hitOutPin, hit)
    runtime:writePin(self.endPositionOutPin, endPosition)
    runtime:impulse(self.impulseOutPin)
end

return NavigationRaycastNode