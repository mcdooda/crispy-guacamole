local UiSettings = require 'data/scripts/ui/uisettings'

local BORDER_COLOR = 0x848484FF
local BORDER_COLOR_FOCUS = 0xA2A2A2FF
local BACKGROUND_COLOR = 0x444444FF
local BACKGROUND_COLOR_FOCUS = 0x555555FF

local TextInput = {}
TextInput.__index = TextInput

function TextInput:new(value)
	local container = Widget.makeFixedSize(120, 1)
	container:setMargin(2)
	container:setBackgroundColor(BORDER_COLOR)

	local background = Widget.makeLineFlow()
	background:setSizePolicy(Widget.SizePolicy.EXPAND)
	background:setBackgroundColor(BACKGROUND_COLOR)
	background:setMargin(1)
	container:addChild(background)

	local textInputWidget = Widget.makeTextInput(table.unpack(UiSettings.defaultFont))
	local _, h = textInputWidget:getSize()
	textInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
	textInputWidget:setText(value)
	textInputWidget:setMargin(2, 4, 2, 4)
	container:setSize(120, h + 6)
	background:addChild(textInputWidget)

	textInputWidget:focus(function()
		container:setBackgroundColor(BORDER_COLOR_FOCUS)
		background:setBackgroundColor(BACKGROUND_COLOR_FOCUS)
	end)
	textInputWidget:blur(function()
		container:setBackgroundColor(BORDER_COLOR)
		background:setBackgroundColor(BACKGROUND_COLOR)
	end)

	local o = setmetatable({
		textInputWidget = textInputWidget,
		container = container
	}, self)

	return o
end

function TextInput:setValue(value)
	 self.textInputWidget:setText(value)
end

function TextInput:getValue()
	return self.textInputWidget:getText()
end

return TextInput