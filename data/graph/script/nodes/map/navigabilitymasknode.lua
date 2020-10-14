local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local NavigabilityNode = FunctionalScriptNode:inherit 'Navigability'

function NavigabilityNode:buildPins()
    self.groundInPin = self:addInputPin(flat.types.BOOLEAN, 'Ground')
    self.waterInPin = self:addInputPin(flat.types.BOOLEAN, 'Water')

    self.navigabilityOutPin = self:addOutputPin(flat.types.NUMBER, 'Navigability')
end

function NavigabilityNode:execute(runtime)
    local ground = runtime:readPin(self.groundInPin)
    local water = runtime:readPin(self.waterInPin)

    local navigability = Map.Navigability.NONE;
    if ground then
        navigability = navigability | Map.Navigability.GROUND
    end
    if water then
        navigability = navigability | Map.Navigability.WATER
    end

    runtime:writePin(self.navigabilityOutPin, navigability)
end

return NavigabilityNode