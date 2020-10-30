local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local EqualEntityNode = FunctionalScriptNode:inherit 'Equal (Entity)'

function EqualEntityNode:buildPins()
    self.entity1InPin = self:addInputPin(flat.types['CG.Entity'], 'Entity 1')
    self.entity2InPin = self:addInputPin(flat.types['CG.Entity'], 'Entity 2')

    self.resultOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Result')
end

function EqualEntityNode:execute(runtime)
    local entity1 = runtime:readOptionalPin(self.entity1InPin)
    local entity2 = runtime:readOptionalPin(self.entity2InPin)

    local result = entity1 == entity2

    runtime:writePin(self.resultOutPin, result)
end

return EqualEntityNode