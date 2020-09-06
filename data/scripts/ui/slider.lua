local min = math.min
local max = math.max

local Slider = {}
Slider.__index = Slider

local function triggerValueChanged(slider)
	local value = slider:getValue()
	local callbacks = slider.valueChangedCallbacks
	for i = 1, #callbacks do
		callbacks[i](slider, value)
	end
end

local function containerClicked(slider, x, y)
	local containerWidth, containerHeight = slider.container:getSize()
	local sliderWidth, sliderHeight = slider.slider:getSize()
	local value
	if containerWidth > containerHeight then
		local maxSliderX = containerWidth - sliderWidth
		local newSliderX = min(max(0, x - sliderWidth / 2), maxSliderX)
		slider.slider:setPosition(newSliderX, 0)
	else
		local maxSliderY = containerHeight - sliderHeight
		local newSliderY = min(max(0, y - sliderHeight / 2), maxSliderY)
		slider.slider:setPosition(0, newSliderY - maxSliderY)
	end
	triggerValueChanged(slider)
	return true
end

function Slider:new(width, height, min, max, value)
	assert(width > 0 and height > 0 and width ~= height)
	assert((not min and not max) or min < max)
	
	local container = Widget.makeFixedSize(width, height)
	container:setBackgroundColor(0xFF0000FF)
	
	local w, h
	if width > height then
		w = height
		h = height
	else
		w = width
		h = width
	end

	min = min or 0
	max = max or 1

	local slider = Widget.makeFixedSize(w, h)
	slider:setBackgroundColor(0x0000FFFF)
	
	container:addChild(slider)
	local o = setmetatable({
		container = container,
		slider = slider,
		min = min,
		max = max,
		mouseDown = false,
		valueChangedCallbacks = {}
	}, self)
	
	local click = function(c, x, y) containerClicked(o, x, y) end
	container:click(click)
	container:mouseMove(function(...)
		if o.mouseDown then
			click(...)
		end
	end)
	container:mouseDown(function()
		o.mouseDown = true
	end)
	container:mouseUp(function()
		o.mouseDown = false
	end)

	if value then
		o:setValue(value)
	end
	
	return o
end

function Slider:getValue()
	local containerWidth, containerHeight = self.container:getSize()
	local sliderWidth, sliderHeight = self.slider:getSize()
	local sliderX, sliderY = self.slider:getPosition()
	local value01
	if containerWidth > containerHeight then
		local maxSliderX = containerWidth - sliderWidth
		value01 = sliderX / maxSliderX
	else
		local maxSliderY = containerHeight - sliderHeight
		value01 = sliderY / maxSliderY
	end
	return self.min + value01 * (self.max - self.min)
end

function Slider:setValue(value)
	assert(self.min <= value and value <= self.max)
	local containerWidth, containerHeight = self.container:getSize()
	local sliderWidth, sliderHeight = self.slider:getSize()
	local sliderX, sliderY = self.slider:getPosition()
	local value01 = (value - self.min) / (self.max - self.min)
	if containerWidth > containerHeight then
		local maxSliderX = containerWidth - sliderWidth
		self.slider:setPosition(maxSliderX * value01, 0)
	else
		local maxSliderY = containerHeight - sliderHeight
		self.slider:setPosition(0, maxSliderY * value01 - maxSliderY)
	end
	triggerValueChanged(self, value)
end

function Slider:valueChanged(callback)
	self.valueChangedCallbacks[#self.valueChangedCallbacks + 1] = callback
end

return Slider