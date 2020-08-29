local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetAttachPoint = FunctionalScriptNode:inherit 'Get Attach Point'

function GetAttachPoint:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.attachPointInPin = self:addInputPin(flat.types.STRING, 'Attach Point')

    self.positionOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Attach Point Position')
end

function GetAttachPoint:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)
    local attachPoint = runtime:readPin(self.attachPointInPin)

    local position = entity:getAttachPoint(attachPoint)

    runtime:writePin(self.positionOutPin, position)
end

return GetAttachPoint