local UiSettings = require 'data/scripts/ui/uisettings'

local root = Widget.getRoot()

local button = require 'data/scripts/ui/button'
local Icon = require 'data/scripts/ui/icon'

local History = {}
History.__index = History

local Line = {}
Line.__index = Line

local DEFAULT = 0
local GRAY = 1
local HIGHLIGHT = 2


local function mouseEnter(c, o)
    if o.state == DEFAULT then
        c:setBackgroundColor(0x6C6C6Cff)
    elseif o.state == GRAY then
        c:setBackgroundColor(0x484848ff)
    else
        c:setBackgroundColor(0x73A1CFFF)
    end
end
local function mouseLeave(c, o)
    if o.state == DEFAULT then
        c:setBackgroundColor(0x525252ff)
    elseif o.state == GRAY then
        c:setBackgroundColor(0x3F3F3FFF)
    else
        c:setBackgroundColor(0x4F6983ff)
    end
end
function Line:new(icon, text)
    local lineContainer = Widget.makeLineFlow()
    lineContainer:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    lineContainer:setSize(196,25)
    lineContainer:setPadding(4)
    lineContainer:setMargin(1,2,0,2)
    lineContainer:setBackgroundColor(0x525252ff)
    local icon = Icon:new(icon, 15)
    icon.container:setMargin(0,15,0,2)
    local textWidget = Widget.makeText(text, table.unpack(UiSettings.defaultFont))
    lineContainer:addChild(icon.container)
    lineContainer:addChild(textWidget)

    local o = setmetatable({
        container = lineContainer,
        textWidget = textWidget,
        state = DEFAULT,
        iconWidget = icon
    }, self)

    local mouseLeave = function(c) mouseLeave(c, o) end
    lineContainer:mouseLeave(mouseLeave)

    local mouseEnter = function(c) mouseEnter(c, o) end
    lineContainer:mouseEnter(mouseEnter)

    return o
end

function Line:highlight()
    self.container:setBackgroundColor(0x73A1CFFF)
    self.textWidget:setTextColor(0xFFFFFFFF)
    self.iconWidget:setColor(0xFFFFFFFF)
    self.state = HIGHLIGHT
end
function Line:reset()
    self.container:setBackgroundColor(0x525252ff)
    self.textWidget:setTextColor(0xFFFFFFFF)
    self.iconWidget:setColor(0xFFFFFFFF)
    self.state = DEFAULT
end
function Line:grayout()
    self.container:setBackgroundColor(0x3F3F3FFF)
    self.textWidget:setTextColor(0x979797FF)
    self.iconWidget:setColor(0x2C2C2CFF)
    self.state = GRAY
end

function History:addLine(icon, text)
    local lineWidget = Line:new(icon, text)
    self.lines[#self.lines+1] = lineWidget
    self.linesContainer:addChild(lineWidget.container)
    self.currentState = #self.lines
end

function History:onStateClick(callback)
    self.lineClick = callback
end

function History:updateTable()
    self.linesContainer:removeAllChildren()
    local lines = self.lines
    for i,line in ipairs(self.lines) do
        self.linesContainer:addChild(line.container)
        line.container:click(function(c)
            self.currentState = i
            line:highlight()
            for j=1,i-1 do
                lines[j]:reset()
            end
            if i < #lines then

                for j=i+1,#lines do
                    lines[j]:grayout()
                end
            end
            if self.lineClick ~= nil then
                self.lineClick(i)
            end
        end)
    end
    self.lines[#self.lines]:highlight()
end

function History:setHistory(history)
    self.lines = {}
    for i,v in ipairs(history) do
        local icon = v[1]
        local text = v[2]
        local lineWidget = Line:new(icon, text)
        self.lines[#self.lines+1] = lineWidget
    end
    self.currentState = #self.lines
    self.updateTable()
end

function History:new()
    local container = Widget.makeColumnFlow()
    container:setBackgroundColor(0x444444FF)
    container:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.FIXED_Y)
    container:setSize(200,300)
    container:setPadding(0,0,0,0)
    container:setMargin(100,0,0,0)

    local linesContainer = Widget.makeColumnFlow()
    linesContainer:setSizePolicy(Widget.SizePolicy.EXPAND)
    linesContainer:setAllowScrollY(true)

    local titleContainer = Widget.makeLineFlow()
    titleContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    titleContainer:setSize(0,50)
    titleContainer:setPadding(8,5,15,5)
    titleContainer:setMargin(1,0,0,0)
    titleContainer:setBackgroundColor(0x313131FF)
    local icon = Icon:new("menu", 22)
    icon.container:setMargin(0,5,0,0)
    local textWidget = Widget.makeText("History", table.unpack(UiSettings.titleFont))

    titleContainer:addChild(icon.container)
    titleContainer:addChild(textWidget)
    titleContainer:mouseDown(function()
        container:drag()
    end)
    titleContainer:mouseUp(function()
        container:drop()
    end)
    container:addChild(titleContainer)
    -- local border = Widget.makeLineFlow()
    -- border:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    -- border:setSize(0,1)
    -- border:setBackgroundColor(0xFFFFFF66)
    -- container:addChild(border)
    container:addChild(linesContainer)

    local o = setmetatable({
        container  = container,
        currentState = 1,
        lineClick = nil,
        linesContainer  = linesContainer,
        lines = {}
    }, self)

    return o
end

return History