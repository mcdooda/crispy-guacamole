local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetHeadingNode = ScriptNode:inherit 'Set Heading'

function SetHeadingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.headingInPin = self:addInputPin(flat.types.NUMBER, 'Heading')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetHeadingNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local heading = runtime:readPin(self.headingInPin)

    entity:setHeading(heading)

    runtime:impulse(self.impulseOutPin)
end

return SetHeadingNode