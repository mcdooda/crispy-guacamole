for i = 1, 20 do
    print('Frame #' .. i)
    coroutine.yield()
end
