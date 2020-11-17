local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local StartAimingNode = ScriptNode:inherit 'Start Aiming'

function StartAimingNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.playerEntityInPin = self:addInputPin(flat.types['CG.Entity'], 'Player Entity')
    self.buttonNameInPin = self:addInputPin(flat.types.STRING, 'Button Name')
    
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function StartAimingNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local playerEntity = runtime:readPin(self.playerEntityInPin)
    local buttonName = runtime:readPin(self.buttonNameInPin)
    
    local extraData = playerEntity:getExtraData()

    local aimInitialPosition = playerEntity:getPosition()
    local aimPositionKey = 'aimPosition' .. buttonName
    extraData[aimPositionKey] = aimInitialPosition

    local aimEntityKey = 'aimEntity' .. buttonName
    extraData[aimEntityKey] = Entity.spawn('player_aim', aimInitialPosition)

    runtime:impulse(self.impulseOutPin)
end

return StartAimingNode