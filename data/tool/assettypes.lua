flat.tool.asset.addAssetType(
    'map',
    function()
        print 'New map'
    end,
    function()
        print 'Open map in game'
    end,
    {
        ['Edit'] = function()
            print 'Map editor'
        end
    }
)

flat.tool.asset.addAssetType(
    'entity',
    function()
        print 'New entity'
    end,
    function()
        print 'Entity editor'
    end
)
