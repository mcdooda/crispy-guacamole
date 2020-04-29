local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetTileTemplateNode = ScriptNode:inherit 'Set Tile Template'

function SetTileTemplateNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.tileInPin = self:addInputPin(flat.types.NUMBER, 'Tile')
    self.templateNameInPin = self:addInputPin(flat.types.STRING, 'Template Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetTileTemplateNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local tile = runtime:readPin(self.tileInPin)
    local templateName = runtime:readPin(self.templateNameInPin)

    Map.setTileTemplate(tile, templateName)

    runtime:impulse(self.impulseOutPin)
end

return SetTileTemplateNode