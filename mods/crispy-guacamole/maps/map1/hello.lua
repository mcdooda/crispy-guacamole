for i = 1, 20 do
    print('Frame #' .. i .. ' (' .. tostring(_G) .. ',' .. tostring(_ENV) .. ')')
    coroutine.yield()
end
