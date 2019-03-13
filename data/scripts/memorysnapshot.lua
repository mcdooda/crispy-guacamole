return function(addContainer, makeSeparator, font)
    local format = string.format
    local collectgarbage = collectgarbage

    local memorySnapshotContainer = addContainer 'Lua Memory'

    local usedMemoryLabel = Widget.makeText('X KB', table.unpack(font))
    usedMemoryLabel:setTextColor(0x000000FF)
    memorySnapshotContainer:addChild(usedMemoryLabel)

    local autoGC = false
    local autoGCLabel = Widget.makeText('Auto GC: off', table.unpack(font))
    autoGCLabel:setTextColor(0x000000FF)
    autoGCLabel:click(function()
        autoGC = not autoGC
        if autoGC then
            autoGCLabel:setText 'Auto GC: on'
        else
            autoGCLabel:setText 'Auto GC: off'
        end
    end)
    memorySnapshotContainer:addChild(autoGCLabel)
    
    local timer = flat.Timer()
    timer:onUpdate(function()
        if autoGC then
            collectgarbage 'collect'
        end
        usedMemoryLabel:setText(format('%.1f KB', collectgarbage 'count'))
    end)
    timer:start(0, true)

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