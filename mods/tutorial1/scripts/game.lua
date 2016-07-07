local MrMoustache = require 'data/game/scripts/mrmoustache'
local EnergyTower = unpack(require 'data/game/scripts/energytower')
local HarvesterTower = require 'data/game/scripts/harvestertower'
local DefenseTower = unpack(require 'data/game/scripts/defensetower')
local TumorCreep = unpack(require 'data/game/scripts/tumorcreep')
local Money = require 'data/game/scripts/money'
local Windows = require 'data/game/scripts/windows'
local buildings = require 'data/game/scripts/buildings'

EnergyTower.description = 'Relaie l\'énergie du QG aux autres bâtiments.'
HarvesterTower.cost = -1
DefenseTower.cost = -1
DefenseTower.upgrades = {}

dofile 'data/game/scripts/setup.lua'
dofile 'data/game/scripts/pausemenu.lua'
dofile 'data/game/scripts/interface.lua'
dofile 'data/game/scripts/rules.lua'

function checkVictory()
	if TumorCreep.allStoped() then
		Windows.victory('mods/tutorial2')
	end
end

function checkDefeat()
	if #Map.getEntities('defense1') == 0 then
		Windows.defeat('toutes les défenses ont été détruites')
	end
end

local creep = TumorCreep:new(2, 10, 0.6, 5)

MrMoustache.speech({
	
	'Général MacTyer au rapport...',
	'Je dois vous informer qu\'une météorite a récemment percuté la terre. Les dégâts matériels sont limités',
	'mais une forme de vie inconnue s\'est répandue sur terre lors de l\'entrée dans l\'atmosphère.',
	'Les creeptologues ont pu l\'analyser rapidement. Tout contact physique est à éviter.',
	'Nous avons détecté une faiblesse aux fortes ondes radio.',
	'Notre objectif sera d\'alimenter en énergie les tours radio déjà présentes sur le terrain.',
	'Nous relaierons l\'énergie depuis notre quartier général pour limiter la propagation.',
	'Notre énergie est limitée, nous devrons la répartir autant que possible.',
	'Il faudra ajuster le débit des tours relais si nécessaire.',
	'A vous de jouer !'
	
}, function()
	Money.setAmount(2000)
	creep:start()
end)
