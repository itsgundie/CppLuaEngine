N = 8

function isPlaceOk (a, n, c)
    for i = 1, n - 1 do
        if (a[i] == c) or
            (a[i] - i == c - n) or
            (a[i] + i == c + n) then
            return false
        end
    end
    return true
end

function printSolution (a)
    for i = 1, N do
        for j = 1, N do
            io.write(a[i] == j and "X" or "-", " ")
        end
        io.write("\n")
    end
    io.write("\n")
end

function addQueen(a, n)
    if n > N then
        printSolution(a)
    else
        for c = 1, N do
            if isPlaceOk(a, n, c) then
                a[n] = c
                addQueen(a, n + 1)
            end
        end
    end
end

addQueen({}, 1)
