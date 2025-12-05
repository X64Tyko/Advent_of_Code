import math

def propermod(X, mod):
    return (X % mod + mod) % mod

def part1(lines, Value, includePasses):
    zCount = 0        
    for line in lines:
        dir = line[0] == "L"
        totalClicks = int(line[1:])
        fullTurns = math.floor(totalClicks / 100)
        
        lastRotClicks = totalClicks - (fullTurns * 100)
            
        if dir:
            lastRotClicks *= -1
        
        if Value != 0 and (Value + lastRotClicks < 0 or Value + lastRotClicks > 100):
            fullTurns += 1
        
        Value = propermod(Value + lastRotClicks, 100)
        if Value == 0 and lastRotClicks != 0:
            zCount += 1
            
        if includePasses:
            zCount += fullTurns
            
    return zCount
        

if __name__ == "__main__":
    with open("input.txt", "r") as file:
        lines = file.readlines()

    print(part1(lines, 50, False))
    print(part1(lines, 50, True))
