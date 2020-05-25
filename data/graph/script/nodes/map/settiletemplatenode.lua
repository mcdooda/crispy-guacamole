local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local SetTileTemplateNode = ScriptNode:inherit 'Set Tile Template'

function SetTileTemplateNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.tileInPin = self:addInputPin(flat.types.NUMBER, 'Tile')
    self.templatePathInPin = self:addInputPin(flat.types.STRING, 'Template Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function SetTileTemplateNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local tile = runtime:readPin(self.tileInPin)
    local templatePath = runtime:readPin(self.templatePathInPin)

    Map.setTileTemplate(tile, templatePath)

    runtime:impulse(self.impulseOutPin)
end

return SetTileTemplateNode