local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local CollisionComponentNode = FunctionalScriptNode:inherit 'Collision Component'

function CollisionComponentNode:buildPins()
    self.typeInPin = self:addInputPin(flat.types.STRING, 'Type')
    self.radiusInPin = self:addInputPin(flat.types.NUMBER, 'Radius')
    self.heightInPin = self:addInputPin(flat.types.NUMBER, 'Height')
    self.separateInPin = self:addInputPin(flat.types.BOOLEAN, 'Separate From Entities')
    self.separateSameTypeInPin = self:addInputPin(flat.types.BOOLEAN, 'Separate Same Type')
    self.separateFromTilesInPin = self:addInputPin(flat.types.BOOLEAN, 'Separate From Tiles')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function CollisionComponentNode:execute(runtime)
    local type = runtime:readPin(self.typeInPin)
    local radius = runtime:readPin(self.radiusInPin)
    local height = runtime:readOptionalPin(self.heightInPin)
    local separate = runtime:readPin(self.separateInPin)
    local separateSameType = runtime:readPin(self.separateSameTypeInPin)
    local separateFromTiles = runtime:readPin(self.separateFromTilesInPin)

    local component = {
        type = type,
        radius = radius,
        height = height,
        separate = separate,
        separateSameType = separateSameType,
        separateFromTiles = separateFromTiles
    }

    runtime:writePin(self.componentOutPin, component)
end

return CollisionComponentNode