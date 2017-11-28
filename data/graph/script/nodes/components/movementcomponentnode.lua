local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local MovementComponentNode = FunctionalScriptNode:inherit 'Movement Component'

function MovementComponentNode:buildPins()
    self.speedInPin = self:addInputPin(flat.types.NUMBER, 'Speed')
    self.jumpForceInPin = self:addInputPin(flat.types.NUMBER, 'Jump Force')
    self.weightInPin = self:addInputPin(flat.types.NUMBER, 'Weight')
    self.snapToGroundInPin = self:addInputPin(flat.types.BOOLEAN, 'Snap To Ground')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function MovementComponentNode:execute(runtime)
    local speed = runtime:readPin(self.speedInPin)
    local jumpForce = runtime:readPin(self.jumpForceInPin)
    local weight = runtime:readPin(self.weightInPin)
    local snapToGround = runtime:readPin(self.snapToGroundInPin)

    local component = {
        speed = speed,
        jumpForce = jumpForce,
        weight = weight,
        snapToGround = snapToGround
    }

    runtime:writePin(self.componentOutPin, component)
end

return MovementComponentNode