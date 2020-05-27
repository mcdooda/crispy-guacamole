local UiSettings = require 'data/scripts/ui/uisettings'
local Icon = require 'data/scripts/ui/icon'

local root = Widget.getRoot()
do
	local leftPanel = Widget.makeColumnFlow()
	leftPanel:setBackgroundColor(0x666666FF)
	leftPanel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
	leftPanel:setSize(302, 0)
	leftPanel:setPadding(0)

	do
		local playButton = Widget.makeLineFlow()
		playButton:setBackgroundColor(0x848484FF)
		playButton:setMargin(6, 6, 0, 0)
		playButton:setPadding(0, 4, 0, 4)
		playButton:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
		do
			local playIcon = Icon:new('play', 18)
			playIcon.container:setPositionPolicy(Widget.PositionPolicy.CENTER)
			playButton:addChild(playIcon.container)
		end
		do
			-- Play button
			local playButtonText = Widget.makeText('Play', 'data/misc/fonts/gnuolane rg.ttf', 22)
			playButtonText:setPositionPolicy(Widget.PositionPolicy.CENTER)
			playButton:addChild(playButtonText)
		end
		playButton:click(flat.ui.task(function()
			if flat.ui.confirm 'Are you sure you want to open the map in play mode?' then
				game.openMap(Mod.getPath(), Mod.getCurrentMapPath())
			end
		end))

		leftPanel:addChild(playButton)
	end

	do
		local browserContainer = Widget.makeColumnFlow()
		browserContainer:setSizePolicy(Widget.SizePolicy.EXPAND)
		browserContainer:setAllowScrollY(true)
		leftPanel:addChild(browserContainer)

		local assetBrowser = flat.tool.asset.browser(browserContainer, Mod.getPath(), {
			allowSelection = true
		})
		assetBrowser:onSelectedAssetOfType('entity', function(asset)
			Editor.setEntity(asset:getPath())
		end)
		assetBrowser:onSelectedAssetOfType('tile', function(asset)
			Editor.setTile(asset:getPath())
		end)
		assetBrowser:onSelectedAssetOfType('prop', function(asset)
			Editor.setProp(asset:getPath())
		end)
		assetBrowser:onSelectionCleared(function()
			Editor.clearSelectedAsset()
		end)
	end

	root:addChild(leftPanel)
end