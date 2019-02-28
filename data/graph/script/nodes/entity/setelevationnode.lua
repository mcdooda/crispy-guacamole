local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetElevationNode = ScriptNode:inherit 'Set Elevation'

function SetElevationNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.elevationInPin = self:addInputPin(flat.types.NUMBER, 'Elevation')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetElevationNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local elevation = runtime:readPin(self.elevationInPin)

    entity:setElevation(elevation)

    runtime:impulse(self.impulseOutPin)
end

return SetElevationNode