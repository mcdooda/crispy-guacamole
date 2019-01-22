local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local HeadingChangedNode = ScriptNode:inherit 'Heading Changed'

function HeadingChangedNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.headingChangedOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Heading Changed')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
    self.headingOutPin = self:addOutputPin(flat.types.NUMBER, 'Heading')
end

function HeadingChangedNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)

    entity:headingChanged(function()
        runtime:writePin(self.entityOutPin, entity)
        runtime:writePin(self.headingOutPin, entity:getHeading())
        runtime:impulse(self.headingChangedOutPin)
        return true
    end)

    local heading = entity:getHeading()

    runtime:impulse(self.impulseOutPin)
end

return HeadingChangedNode