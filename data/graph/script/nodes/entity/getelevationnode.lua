local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetElevationNode = FunctionalScriptNode:inherit 'Get Elevation'

function GetElevationNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.elevationOutPin = self:addOutputPin(flat.types.NUMBER, 'Elevation')
end

function GetElevationNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local elevation = entity:getElevation()

    runtime:writePin(self.elevationOutPin, elevation)
end

return GetElevationNode