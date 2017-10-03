local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpriteAnimationNode = FunctionalScriptNode:inherit 'Sprite Animation'

function SpriteAnimationNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')
    self.lineInPin = self:addInputPin(flat.types.NUMBER, 'Line')
    self.numFramesInPin = self:addInputPin(flat.types.NUMBER, 'Frame Count')
    self.frameDurationInPin = self:addInputPin(flat.types.NUMBER, 'Frame Duration')

    self.animationOutPin = self:addOutputPin(flat.types.TABLE, 'Animation')
end

function SpriteAnimationNode:execute(runtime)
    local name = runtime:readPin(self.nameInPin)
    local line = runtime:readPin(self.lineInPin)
    local numFrames = runtime:readPin(self.numFramesInPin)
    local frameDuration = runtime:readPin(self.frameDurationInPin)

    local animation = {
        name = name,
        line = line,
        numFrames = numFrames,
        frameDuration = frameDuration
    }

    runtime:writePin(self.animationOutPin, animation)
end

return SpriteAnimationNode