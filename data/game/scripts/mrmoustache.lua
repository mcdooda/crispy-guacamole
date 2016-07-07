local splitLine = require 'data/scripts/splitline'
local Container = require 'data/scripts/container'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

local Widget = Widget
local Timer = Timer
local coroutine = coroutine
local math = math
local random = math.random

local animationTimer
local frameNumber
local speechTimer
local onEndFunc
local speaking
local voiceChannel

local MrMoustache = {}


local mrMoustacheContainer = Container:new(
	InterfaceConfig.container,
	nil,
	Widget.CENTERX + Widget.CENTERY,
	{ width = 470, height = 435 },
	16
)
mrMoustacheContainer:hide()

local w = Widget.new {
	anchor = Widget.CENTERX + Widget.TOP,
	parent = mrMoustacheContainer.widget,
	dimensions = { width = 200, height = 306 },
	position = { x = 0, y = -15 }
}

local faceWidget = Widget.new {
	image = 'data/game/interface/mrmoustache.png',
	parent = w,
	anchor = Widget.TOP + Widget.LEFT,
	dimensions = {
		width = 164,
		height = 269
	},
	position = {
		x = 20,
		y = -20
	}
}
Widget.setTextureCoords(faceWidget, 0, 1 / 2, 1 / 5, 0)

local foregroundWidget = Widget.new {
	image = 'data/game/interface/mrmoustacheforeground.png',
	parent = w,
	anchor = Widget.TOP + Widget.CENTERX
}

local speechContainer = Container:new(
	InterfaceConfig.container,
	mrMoustacheContainer.widget,
	Widget.CENTERX + Widget.BOTTOM,
	{ width = 425, height = 90 },
	16
)
speechContainer:setPosition { x = 0, y = 20 }
local linesPanel = Widget.new {
	parent = speechContainer.widget,
	anchor = Widget.LEFT + Widget.RIGHT + Widget.TOP + Widget.BOTTOM
}

local skipButtonContainer = Container:new(
	InterfaceConfig.container,
	mrMoustacheContainer.widget,
	Widget.CENTERX + Widget.CENTERY,
	{ width = 110, height = 30 },
	8
)
skipButtonContainer:setPosition { x = 157, y = -83 }
Widget.new {
	text = 'Passer tout',
	font = InterfaceConfig.font..'@14',
	color = InterfaceConfig.fontStrongColor,
	parent = skipButtonContainer.widget,
	anchor = Widget.CENTERY + Widget.LEFT,
	position = {
		x = 8,
		y = 0
	}
}
Widget.new {
	image = 'data/game/interface/mrmoustacheskip.png',
	parent = skipButtonContainer.widget,
	anchor = Widget.CENTERY + Widget.RIGHT,
	position = {
		x = -8,
		y = 0
	}
}
Widget.onClick(skipButtonContainer.widget, function()
	MrMoustache.skip()
end)

function MrMoustache.show()
	speaking = true
	mrMoustacheContainer:show()
	
	frameNumber = 0
	animationTimer = Timer.new(
		0.05,
		true,
		function(timer, age)
			frameNumber = (frameNumber + 1) % 10
			local line = math.floor(frameNumber / 5)
			local column = frameNumber % 5
			Widget.setTextureCoords(faceWidget, column / 5, (line + 1) / 2, (column + 1) / 5, line / 2)
		end
	)
	
	voiceChannel = Sound.playSample('voice.wav')
	Sound.setChannelVolume(voiceChannel, 0.5)
	Timer.new(
		30,
		true,
		function(timer)
			if speaking then
				voiceChannel = Sound.playSample('voice.wav')
				Sound.setChannelVolume(voiceChannel, 0.5)
			else
				Timer.setLoops(timer, false)
			end
		end
	)
end

function MrMoustache.hide()
	speaking = false
	mrMoustacheContainer:hide()
	Sound.fadeOutChannel(voiceChannel, 1)
end

function MrMoustache.say(text)
	Widget.clear(linesPanel)
	local y = -8
	local lines = splitLine(text, 28)
	for i = 1, #lines do
		local line = lines[i]
		local lineWidget = Widget.new {
			text = line,
			font = InterfaceConfig.font..'@20',
			color = InterfaceConfig.fontColor,
			parent = linesPanel,
			anchor = Widget.TOP + Widget.LEFT,
			position = {
				x = 10,
				y = y
			}
		}
		Widget.setOpacity(lineWidget, 0)
		Timer.new(
			i - 1,
			false,
			function()
				Timer.new(
					0.02,
					true,
					function(timer, age)
						if age < 1 then
							Widget.setOpacity(lineWidget, age)
						else
							Widget.setOpacity(lineWidget, 1)
							Timer.setLoops(timer, false)
						end
					end
				)
			end
		)
		local w, h = Widget.getDimensions(lineWidget)
		y = y - h
	end
end

function MrMoustache.speech(speech, onEnd)
	onEndFunc = onEnd
	local speechFunc = coroutine.wrap(function()
		Camera.lock()
		local timer
		for i = 1, #speech do
			MrMoustache.say(speech[i])
			timer = coroutine.yield()
		end
		Timer.setLoops(timer, false)
		MrMoustache.hide()
		Camera.unlock()
		if onEnd then
			onEnd()
		end
	end)
	
	MrMoustache.show()
	
	speechFunc()
	speechTimer = Timer.new(
		6,
		true,
		speechFunc
	)
end

function MrMoustache.skip()
	Timer.stop(speechTimer)
	MrMoustache.hide()
	Camera.unlock()
	if onEndFunc then
		onEndFunc()
	end
end

return MrMoustache
