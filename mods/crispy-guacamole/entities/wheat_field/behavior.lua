local ResourceBehavior = require 'data/scripts/componenthelpers/behaviors/resource'

local states = ResourceBehavior.basicResource(100)

local init = states.init
function states:init(wheatField)
    wheatField:setCycleAnimation 'idle'
    return init(self, wheatField)
end

return states