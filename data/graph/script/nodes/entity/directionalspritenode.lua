local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local pi = math.pi

local DirectionalSprite = ScriptNode:inherit 'Directional Sprite'

function DirectionalSprite:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.numDirectionsInPin = self:addInputPin(flat.types.NUMBER, 'Directions')
    self.prefixInPin = self:addInputPin(flat.types.STRING, 'Animation prefix')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
    self.animationNameOutPin = self:addOutputPin(flat.types.STRING, 'Animation name')
    self.mirrorOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Mirror')
end

function DirectionalSprite:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local numDirections = runtime:readPin(self.numDirectionsInPin)
    local spriteName = runtime:readPin(self.prefixInPin)

    local heading = entity:getHeading()
    local f = ((heading + pi / numDirections - pi * 0.25) / (2 * pi)) % 1
    local index = math.floor(f * numDirections) + 1
    local mirror = false
    local totalAnimations = numDirections / 2 + 1

    if index > totalAnimations then
        index = 2 * totalAnimations - index
        mirror = true
    end
    local animationName = spriteName .. '_' .. math.floor(index + 0.5)

    runtime:writePin(self.entityOutPin, entity)
    runtime:writePin(self.animationNameOutPin, animationName)
    runtime:writePin(self.mirrorOutPin, mirror)
    runtime:impulse(self.impulseOutPin)
end



return DirectionalSprite