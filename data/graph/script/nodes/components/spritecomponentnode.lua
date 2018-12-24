local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpriteComponentNode = FunctionalScriptNode:inherit 'Sprite Component'

function SpriteComponentNode:buildPins()
    self.originInPin = self:addInputPin(flat.types['flat.Vector2'], 'Origin')
    self.sizeInPin = self:addInputPin(flat.types['flat.Vector2'], 'Size')
    self.animationsInPin = self:addInputPin(flat.types.TABLE, 'Animations')
    self.attachPointsInPin = self:addInputPin(flat.types.TABLE, 'Attach Points')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.initOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Init')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function SpriteComponentNode:execute(runtime)
    local origin = runtime:readPin(self.originInPin)
    local size = runtime:readPin(self.sizeInPin)
    local animations = runtime:readPin(self.animationsInPin)
    local attachPoints = runtime:readOptionalPin(self.attachPointsInPin)

    local animationsByName = {}
    for i = 1, #animations do
        local animation = animations[i]
        animationsByName[animation.name] = {
            line = animation.line,
            numFrames = animation.numFrames,
            frameDuration = animation.frameDuration
        }
    end

    local attachPointsByName
    if attachPoints then
        attachPointsByName = {}
        for i = 1, #attachPoints do
            local attachPoint = attachPoints[i]
            attachPointsByName[attachPoint.name] = attachPoint.position
        end
    end

    local component = {
        origin = origin,
        size = size,
        animations = animationsByName,
        attachPoints = attachPointsByName,
        init = function(entity)
            runtime:writePin(self.entityOutPin, entity)
            runtime:impulse(self.initOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return SpriteComponentNode