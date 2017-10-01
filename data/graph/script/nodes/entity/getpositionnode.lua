local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetPositionNode = FunctionalScriptNode:inherit 'Get Position'

function GetPositionNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.positionOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Position')
end

function GetPositionNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local position = entity:getPosition()

    runtime:writePin(self.positionOutPin, position)
end

return GetPositionNode