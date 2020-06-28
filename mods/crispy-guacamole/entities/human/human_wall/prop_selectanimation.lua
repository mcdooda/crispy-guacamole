return function(tl, tr, bl, br)
    local name = 'human_wall'
    if bl == name and tr == name then
        return 'stand' -- thx Jon
    elseif tl == name and br == name then
        return 'build' -- ??
    end
    return 'cross'
end