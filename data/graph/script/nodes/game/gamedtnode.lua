local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GameDTNode = FunctionalScriptNode:inherit 'Game DT'

function GameDTNode:buildPins()
    self.gameDTOutPin = self:addOutputPin(flat.types.NUMBER, 'Game DT')
end

function GameDTNode:execute(runtime)
    local dt = game.getDT()
    runtime:writePin(self.gameDTOutPin, dt)
end

return GameDTNode