local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local GetTemplateNameNode = FunctionalScriptNode:inherit 'Get Template Name'

function GetTemplateNameNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.nameOutPin = self:addOutputPin(flat.types.STRING, 'Name')
end

function GetTemplateNameNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local name = entity:getTemplatePath()

    runtime:writePin(self.nameOutPin, name)
end

return GetTemplateNameNode