local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local MovementComponentNode = FunctionalScriptNode:inherit 'Movement Component'

function MovementComponentNode:buildPins()
    self.speedInPin = self:addInputPin(flat.types.NUMBER, 'Speed')
    self.jumpForceInPin = self:addInputPin(flat.types.NUMBER, 'Jump Force')
    self.weightInPin = self:addInputPin(flat.types.NUMBER, 'Weight')
    self.snapToGroundInPin = self:addInputPin(flat.types.BOOLEAN, 'Snap To Ground')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.onWalkedOnTileOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Walked On Tile')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
    self.tileOutPin = self:addOutputPin(flat.types.NUMBER, 'Tile')
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
        snapToGround = snapToGround,
        walkedOnTile = function(entity, tile)
            runtime:writePin(self.entityOutPin, entity)
            runtime:writePin(self.tileOutPin, tile)
            runtime:impulse(self.onWalkedOnTileOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return MovementComponentNode