local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local GetTemplatePathNode = FunctionalScriptNode:inherit 'Get Template Name'

function GetTemplatePathNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.nameOutPin = self:addOutputPin(flat.types.STRING, 'Name')
end

function GetTemplatePathNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local name = entity:getTemplatePath()

    runtime:writePin(self.nameOutPin, name)
end

return GetTemplatePathNode