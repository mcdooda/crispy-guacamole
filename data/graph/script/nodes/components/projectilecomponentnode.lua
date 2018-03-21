local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ProjectileComponentNode = FunctionalScriptNode:inherit 'Projectile Component'

function ProjectileComponentNode:buildPins()
    self.weightInPin = self:addInputPin(flat.types.NUMBER, 'Weight')
    self.speedInPin = self:addInputPin(flat.types.NUMBER, 'Speed')
    self.rotateSpriteInPin = self:addInputPin(flat.types.BOOLEAN, 'Rotate Sprite')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.onAttackOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Collide')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
    self.collidedEntityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Collided Entity')
    self.normalOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Normal')
end

function ProjectileComponentNode:execute(runtime)
    local weight = runtime:readPin(self.weightInPin)
    local speed = runtime:readPin(self.speedInPin)
    local rotateSprite = runtime:readPin(self.rotateSpriteInPin)

    local component = {
        weight = weight,
        speed = speed,
        rotateSprite = rotateSprite,
        collidedCallback = function(entity, collidedEntity, normal)
            runtime:writePin(self.entityOutPin, entity)
            if collidedEntity then
                runtime:writePin(self.collidedEntityOutPin, collidedEntity)
            end
            runtime:writePin(self.normalOutPin, normal)
            runtime:impulse(self.onAttackOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return ProjectileComponentNode