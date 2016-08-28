local math = math
local min = math.min
local max = math.max

local Slider = {}
Slider.__index = Slider

local function triggerValueChanged(slider, value)
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
		value = newSliderX / maxSliderX
	else
		local maxSliderY = containerHeight - sliderHeight
		local newSliderY = min(max(0, y - sliderHeight / 2), maxSliderY)
		slider.slider:setPosition(0, newSliderY - maxSliderY)
		value = newSliderY / maxSliderY
	end
	triggerValueChanged(slider, value)
	return true
end

function Slider:new(width, height)
	assert(width > 0 and height > 0 and width ~= height)
	
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
	local slider = Widget.makeFixedSize(w, h)
	slider:setBackgroundColor(0x0000FFFF)
	
	container:addChild(slider)
	local o = setmetatable({
		container = container,
		slider = slider,
		valueChangedCallbacks = {}
	}, self)
	
	local click = function(c, x, y) containerClicked(o, x, y) end
	container:click(click)
	container:mouseMove(click)
	
	return o
end

function Slider:getValue()
	local containerWidth, containerHeight = self.container:getSize()
	local sliderWidth, sliderHeight = self.slider:getSize()
	local sliderX, sliderY = self.slider:getPosition()
	if containerWidth > containerHeight then
		local maxSliderX = containerWidth - sliderWidth
		return sliderX / maxSliderX
	else
		local maxSliderY = containerHeight - sliderHeight
		return sliderY / maxSliderY
	end
end

function Slider:setValue(value)
	assert(0 <= value and value <= 1)
	local containerWidth, containerHeight = self.container:getSize()
	local sliderWidth, sliderHeight = self.slider:getSize()
	local sliderX, sliderY = self.slider:getPosition()
	if containerWidth > containerHeight then
		local maxSliderX = containerWidth - sliderWidth
		self.slider:setPosition(maxSliderX * value, 0)
	else
		local maxSliderY = containerHeight - sliderHeight
		self.slider:setPosition(0, maxSliderY * value - maxSliderY)
	end
	triggerValueChanged(self, value)
end

return Slider