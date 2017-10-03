local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SpriteAttachPointNode = FunctionalScriptNode:inherit 'Sprite Attach Point'

function SpriteAttachPointNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Position')

    self.attachPointOutPin = self:addOutputPin(flat.types.TABLE, 'Attach Point')
end

function SpriteAttachPointNode:execute(runtime)
    local name = runtime:readPin(self.nameInPin)
    local position = runtime:readPin(self.positionInPin)

    local attachPoint = {
        name = name,
        position = position
    }

    runtime:writePin(self.attachPointOutPin, attachPoint)
end

return SpriteAttachPointNode