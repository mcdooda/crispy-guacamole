local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local TextureComponentNode = FunctionalScriptNode:inherit 'Texture Component'

function TextureComponentNode:buildPins()
    self.originInPin = self:addInputPin(flat.types['flat.Vector2'], 'Origin')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function TextureComponentNode:execute(runtime)
    local origin = runtime:readPin(self.originInPin)

    local component = {
        origin = origin
    }

    runtime:writePin(self.componentOutPin, component)
end

return TextureComponentNode