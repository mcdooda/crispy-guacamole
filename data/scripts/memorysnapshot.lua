return function(addContainer, makeSeparator, font)
	local memorySnapshotContainer = addContainer 'Lua Memory'

    local snapshot

    local snapshotButton = Widget.makeText('Take snapshot', table.unpack(font))
    snapshotButton:setTextColor(0x000000FF)
    snapshotButton:click(function()
        collectgarbage 'collect'
        snapshot = flat.lua.snapshot.snapshot()
        flat.ui.info 'Lua memory snapshot taken'
    end)
    memorySnapshotContainer:addChild(snapshotButton)

    local diffButton = Widget.makeText('Save diff file', table.unpack(font))
    diffButton:setTextColor(0x000000FF)
    local diffNumber = 1
    diffButton:click(function()
        if snapshot then
            collectgarbage 'collect'
            local snapshot2 = flat.lua.snapshot.snapshot()
            local diffFile = 'snapshotdiff-' .. diffNumber
            flat.lua.snapshot.diff(snapshot, snapshot2, diffFile)
            diffNumber = diffNumber + 1
            flat.ui.info('Snapshot diff saved to ' .. diffFile)
        end
    end)
    memorySnapshotContainer:addChild(diffButton)
end