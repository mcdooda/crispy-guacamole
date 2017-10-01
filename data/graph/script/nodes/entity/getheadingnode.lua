local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetHeadingNode = FunctionalScriptNode:inherit 'Get Heading'

function GetHeadingNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.headingOutPin = self:addOutputPin(flat.types.NUMBER, 'Heading')
end

function GetHeadingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local heading = entity:getHeading()

    runtime:writePin(self.headingOutPin, heading)
end

return GetHeadingNode