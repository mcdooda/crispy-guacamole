dofile 'data/editor/scripts/tests.lua'
dofile 'data/scripts/stats.lua'

local dump = require 'data/scripts/dump'
local radiobutton = require 'data/scripts/ui/radiobutton'

local root = Widget.getRoot()
do
    local check1 = radiobutton:new(true, 15, 'teeeeest', false)
    check1.container:setPosition(100,0)
    root:addChild(check1.container)
end