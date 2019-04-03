local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetForwardNode = FunctionalScriptNode:inherit 'Get Forward'

function GetForwardNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.forwardOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Forward')
end

function GetForwardNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local forward = entity:getForward()

    runtime:writePin(self.forwardOutPin, forward)
end

return GetForwardNode