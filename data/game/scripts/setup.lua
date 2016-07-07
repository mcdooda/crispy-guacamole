local CameraTools = require 'data/scripts/cameratools'
local Fade = require 'data/scripts/fade'

math.randomseed(Time.getTime() * 10000)

Map.enableOverlay()
CameraTools.lookAtCenter()
Sound.setMusicVolume(0.4)
Sound.fadeInMusic('music.mp3', 10)
Fade.disappear(2)
