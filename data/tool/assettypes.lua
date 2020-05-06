flat.tool.asset.addAssetType(
    'map',
    function(path)
        print('New map in ' .. path)
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
    function(asset)
        EntityEditor.openEntity(Mod.getPath(), 'sandbox', asset:getPath())
        print('Open entity in editor ' .. asset:getPath())
    end
)
