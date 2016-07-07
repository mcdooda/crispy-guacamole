local MrMoustache = require 'data/game/scripts/mrmoustache'
local DefenseTower = unpack(require 'data/game/scripts/defensetower')
local TumorCreep = unpack(require 'data/game/scripts/tumorcreep')
local Money = require 'data/game/scripts/money'
local Windows = require 'data/game/scripts/windows'
local buildings = require 'data/game/scripts/buildings'
local Alert = require 'data/scripts/alert'

DefenseTower.description = 'Détruit le virus dans une large zone pour 200£ et 60 d\'énergie.'

dofile 'data/game/scripts/setup.lua'
dofile 'data/game/scripts/pausemenu.lua'
dofile 'data/game/scripts/interface.lua'
dofile 'data/game/scripts/rules.lua'

function checkVictory()
	if TumorCreep.allStoped() and TumorCreep.getRatio() < 0.1 then
		Game.over()
		Alert:new(
			{
				'Bravo !',
				'La menace a été anéantie et la population ne risque plus rien.',
				' ',
				'Merci d\'avoir joué !'
			},
			'Retour à l\'accueil', function() Game.load('mods/menu') end
		)
	end
end

function checkDefeat()
	
end

local creep = TumorCreep:new(11.5, 20.5, 0.6, 80)

MrMoustache.speech({
	
	'Général MacTyer au rapport...',
	'Nous nous approchons de la météorite.',
	'Le comportement du fléau semble ... différent mais nous devons en finir une bonne fois pour toute.',
	'Nos tours de défense ont été équipées de puissantes attaques radio.',
	'N\'oubliez pas ... il ne doit plus rien rester !',
	'A vous de jouer !'
	
}, function()
	dofile 'data/game/scripts/loot.lua'
	Money.setAmount(5000)
	creep:start()
end)
