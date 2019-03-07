local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GameTimeNode = FunctionalScriptNode:inherit 'Game Time'

function GameTimeNode:buildPins()
    self.gameTimeOutPin = self:addOutputPin(flat.types.NUMBER, 'Game Time')
end

function GameTimeNode:execute(runtime)
    local time = game.getTime()
    runtime:writePin(self.gameTimeOutPin, time)
end

return GameTimeNode