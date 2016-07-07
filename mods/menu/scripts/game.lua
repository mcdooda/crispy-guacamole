local Menu = require 'data/scripts/menu'
local Alert = require 'data/scripts/alert'
local CameraTools = require 'data/scripts/cameratools'
local Fade = require 'data/scripts/fade'

CameraTools.lookAtCenter()
Camera.lock()

Sound.preloadMusic('music.mp3')

Sound.preloadSample('voice.wav')
Sound.preloadSample('build.wav')
Sound.preloadSample('explosion.wav')
Sound.preloadSample('loot.wav')

Sound.setMusicVolume(0.5)
Sound.fadeInMusic('menu.mp3', 5)

Input.onKeyDown(Input.ESCAPE, Game.exit)

local sin = math.sin
local mapTiles = Map.getTiles()
Timer.new(
	0.02,
	true,
	function(timer, age)
		for _, tile in pairs(mapTiles) do
			local x, y = Tile.getPosition(tile)
			local amplitude = math.exp(-age + 1) + 0.2
			Tile.setZ(tile, sin(age * 5 + x - y) * amplitude)
		end
	end
)

local function openMenu()
	local menu = Menu:new(nil,
		'Campagne', function()
			Fade.appear(2)
			Timer.new(
				2,
				false,
				function()
					Game.load('mods/tutorial1')
				end
			)
		end,
		--'Partie rapide', function() end,
		'Crédits', function()
			Alert:new(
				{
					'Auteurs de The Global Scourge :',
					'Programmation par Nicolas Dodelier',
					'Graphismes par Tom Rouillard et Jon Rouillard',
					'Musique libre par Boris Descombes Croset'
				},
				'Fermer',
				openMenu
			)
		end,
		'Quitter', Game.exit
	)
	menu:setPosition { x = 0, y = -150 }
end
openMenu()

Widget.new {
	image = 'data/game/interface/tgs.png',
	anchor = Widget.BOTTOM + Widget.RIGHT,
	pixelPerfect = true,
	position = {
		x = -10,
		y = 10
	}
}

Widget.new {
	image = 'data/game/interface/logo.png',
	anchor = Widget.CENTERX + Widget.CENTERY,
	pixelPerfect = true
}
