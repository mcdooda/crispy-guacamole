local AssetIcon = require 'data/scripts/asseticon'
local Preview = require 'data/scripts/preview'

flat.tool.asset.addAssetType(
    'map',
    function(path)
        print('New map in ' .. path)
    end,
    function(asset, size, allowSelection)
        return AssetIcon:new(asset, nil, size, allowSelection)
    end,
    function(asset)
        MapEditor.openMap(Mod.getPath(), asset:getPath())
        print('Open map in game ' .. asset:getPath())
    end,
    {
        ['Edit'] = function(asset)
            print('Open map in editor ' .. asset:getPath())
        end
    }
)

flat.tool.asset.addAssetType(
    'entity',
    function(path)
        print('New entity in ' .. path)
    end,
    function(asset, size, allowSelection)
        local path = asset:getPath()
        local preview = Preview.entity(path, nil, false, 1)
        local width, height = preview:getSize()
        if width * 2 < size and height * 2 < size then
            preview = Preview.entity(path, nil, false, 2)
        end
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        preview:setPosition(0, 10)
        return AssetIcon:new(asset, preview, size, allowSelection)
    end,
    function(asset)
        EntityEditor.openEntity(Mod.getPath(), 'mods/crispy-guacamole/maps/sandbox', asset:getPath())
        print('Open entity in editor ' .. asset:getPath())
    end
)

flat.tool.asset.addAssetType(
    'tile',
    function(path)
        print('New tile in ' .. path)
    end,
    function(asset, size, allowSelection)
        local path = asset:getPath()
        local preview = Preview.tile(path, 1, false, 1)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
        preview:setPosition(0, -16)
        return AssetIcon:new(asset, preview, size, allowSelection)
    end,
    function(asset)
        print('Open tile in editor ' .. asset:getPath())
    end
)

flat.tool.asset.addAssetType(
    'prop',
    function(path)
        print('New prop in ' .. path)
    end,
    function(asset, size, allowSelection)
        local path = asset:getPath()
        local preview = Preview.prop(path, ModData.props.getHighest(path), 1)
        preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
        preview:setPosition(0, 10)
        return AssetIcon:new(asset, preview, size, allowSelection)
    end,
    function(asset)
        print('Open tile in editor ' .. asset:getPath())
    end
)