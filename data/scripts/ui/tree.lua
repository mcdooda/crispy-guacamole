local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()

local button = require 'data/scripts/ui/button'
local Icon = require 'data/scripts/ui/icon'


local Branch = {}
Branch.__index = Branch

local Leaf = {}
Leaf.__index = Leaf

local Tree = {}
Tree.__index = Tree

local DEFAULT = 0
local CLOSED = 0
local OPEN = 1
local GRAY = 1
local HIGHLIGHT = 2
local SELECTED = 2


local function mouseEnter(c, o)
    if o.state == SELECTED then
        c:setBackgroundColor(0x6C6C6Cff)
    else
        c:setBackgroundColor(0x484848ff)
    end
end
local function mouseLeave(c, o)
    if o.state == SELECTED then
        c:setBackgroundColor(0x525252ff)
    else
        c:setBackgroundColor(0x3F3F3FFF)
    end
end
local function click(c, o)
    if o.state == CLOSED then
        o:open()
    else
        o:close()
    end
end

local function objectFactory(index, value, level)
    if type(value) == "string" then
        return Leaf:new(value, level)
    else
        return Branch:new(index, value, level)
    end
end

function Branch:new(name, childs, level)
    local container = Widget.makeColumnFlow()
    container:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    container:setBackgroundColor(0x3F3F3FFF)
    container:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.TOP)
    local childrenContainer = Widget.makeColumnFlow()
    childrenContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

    local nameContainer = Widget.makeLineFlow()
    nameContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    nameContainer:setSize(0,30)
    nameContainer:setPadding(0,0,0, level * 15)
    nameContainer:setMargin(0)

    local state = CLOSED
    local iconOpen = Icon:new(state == OPEN and "menuDown" or "menuRight", 12)
    iconOpen.container:setMargin(0,5,0,5)
    local icon = Icon:new(state == OPEN and "folderOpen" or "folderClosed", 12)
    icon.container:setMargin(0,5,0,0)
    local textWidget = Widget.makeText(name, table.unpack(UiSettings.defaultFont))
    textWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
    icon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
    iconOpen.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
    nameContainer:addChild(iconOpen.container)
    nameContainer:addChild(icon.container)
    nameContainer:addChild(textWidget)
    container:addChild(nameContainer)
    container:addChild(childrenContainer)
    local o = setmetatable({
        container = container,
        textWidget = textWidget,
        level = level,
        childs = childs,
        childrenContainer = childrenContainer,
        iconOpen = iconOpen,
        state = CLOSED,
        changeState = nil,
        iconWidget = icon
    }, self)

    if state == OPEN then
        for i,v in pairs(childs) do
            childrenContainer:addChild(objectFactory(i, v, level + 1).container)
        end
    end

    local mouseLeave = function(c) mouseLeave(c, o) end
    nameContainer:mouseLeave(mouseLeave)

    local mouseEnter = function(c) mouseEnter(c, o) end
    nameContainer:mouseEnter(mouseEnter)

    local click = function(c) click(c, o) end
    nameContainer:click(click)
    return o
end


function Branch:open()
    self.iconWidget:setIcon("folderOpen")
    self.iconOpen:setIcon("menuDown")
    self.state = OPEN
    for i,v in pairs(self.childs) do
        self.childrenContainer:addChild(objectFactory(i, v, self.level + 1).container)
    end

    if self.changeState ~= nil then
        self.changeState(self.state)
    end
end

function Branch:onStateChange(callback)
    self.changeState = callback
end
function Leaf:onStateChange(callback)
end
function Branch:close()
    self.iconWidget:setIcon("folderClosed")
    self.iconOpen:setIcon("menuRight")
    self.state = CLOSED
    if self.changeState ~= nil then
        self.changeState(self.state)
    end
    self.childrenContainer:removeAllChildren()
end

function Leaf:new(leaf, level)
    local container = Widget.makeColumnFlow()
    container:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    container:setBackgroundColor(0x3F3F3FFF)
    container:setPositionPolicy(Widget.PositionPolicy.LEFT + Widget.PositionPolicy.TOP)

    local nameContainer = Widget.makeLineFlow()
    nameContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    nameContainer:setSize(0,30)
    nameContainer:setPadding(0,0,0, level * 15 + 22)
    nameContainer:setMargin(0)
    local icon = Icon:new("new", 12)
    icon.container:setMargin(0,5,0,0)
    local textWidget = Widget.makeText(leaf, table.unpack(UiSettings.defaultFont))
    textWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
    icon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
    nameContainer:addChild(icon.container)
    nameContainer:addChild(textWidget)
    container:addChild(nameContainer)

    local o = setmetatable({
        container = container,
        textWidget = textWidget,
        level = level,
        state = DEFAULT,
        iconWidget = icon
    }, self)

    local mouseLeave = function(c) mouseLeave(c, o) end
    nameContainer:mouseLeave(mouseLeave)

    local mouseEnter = function(c) mouseEnter(c, o) end
    nameContainer:mouseEnter(mouseEnter)

    -- local click = function(c) click(c, o) end
    -- nameContainer:click(click)
    return o
end


function Tree:new(tree, title)
    local container = Widget.makeColumnFlow()
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setSize(200,300)
    container:setPadding(0,0,0,0)
    container:setMargin(100,0,0,0)

    local linesContainer = Widget.makeColumnFlow()
    linesContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.EXPAND_Y)
    linesContainer:setAllowScrollY(true)

    local titleContainer = Widget.makeLineFlow()
    titleContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    titleContainer:setSize(0,50)
    titleContainer:setPadding(8,5,15,5)
    titleContainer:setMargin(1,0,0,0)
    titleContainer:setBackgroundColor(0x313131FF)
    local icon = Icon:new("hashtag", 22)
    icon.container:setMargin(0,5,0,0)
    local textWidget = Widget.makeText(title, table.unpack(UiSettings.titleFont))


    titleContainer:addChild(icon.container)
    titleContainer:addChild(textWidget)
    titleContainer:mouseDown(function()
        container:drag()
    end)
    titleContainer:mouseUp(function()
        container:drop()
    end)
    container:addChild(titleContainer)
    container:addChild(linesContainer)

    print(tree)
    for i,v in pairs(tree) do
        print(v)
        local tempLine = objectFactory(i, v, 0)
        tempLine:onStateChange(function(state)
            tree[i] = state
        end)

        linesContainer:addChild(tempLine.container)
    end
    local o =  ({
        container  = container,
        currentState = 1,
        tree = tree,
        lineClick = nil,
        linesContainer  = linesContainer,
        lines = {}
    }, self)

    return o
end

return Tree