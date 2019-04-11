local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local IsFollowingPathNode = FunctionalScriptNode:inherit 'Is Following Path'

function IsFollowingPathNode:buildPins()
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')

    self.followingPathOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Following Path')
end

function IsFollowingPathNode:execute(runtime)
    local entity = runtime:readPin(self.entityInPin)

    local followingPath = entity:isFollowingPath()

    runtime:writePin(self.followingPathOutPin, followingPath)
end

return IsFollowingPathNode