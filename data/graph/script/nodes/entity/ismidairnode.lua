local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsMidairNode = FunctionalScriptNode:inherit 'Is Midair'

function IsMidairNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.midairPathOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Midair')
end

function IsMidairNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local midair = entity:isMidair()

    runtime:writePin(self.midairPathOutPin, midair)
end

return IsMidairNode