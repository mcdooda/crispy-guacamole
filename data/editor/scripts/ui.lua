dofile 'data/editor/scripts/tests.lua'
dofile 'data/scripts/stats.lua'

local dump = require 'data/scripts/dump'
local checkbox = require 'data/scripts/ui/checkbox'

local root = Widget.getRoot()
do
    local check1 = checkbox:new(true, 10, false)
    check1.container:setPosition(100,0)
    root:addChild(check1.container)
end