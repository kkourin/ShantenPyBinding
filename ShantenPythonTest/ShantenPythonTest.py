
from ShantenLib import ShantenCalculator
import pprint
from random import shuffle
import time

def parseTenhouHand(hand):
    buffer = []
    output = [0 for _ in range(34)]
    offset = 0
    offsets = {'m':0, 'p':9, 's':18, 'z':27}
    for c in hand:
        if c in offsets:
            offset = offsets[c]
            for b in buffer:
                output[b-1+offset] += 1
            buffer = []
        else:
            if c == '0':
                c = '5'
            buffer.append(int(c))
    return output
def subtractHand(hand1, hand2):
    return [p[0] - p[1] for p in zip(hand1, hand2)]
def tileToString(i):
    assert 0 <= i <= 33
    d = {0: "m", 1: "p", 2:"s", 3:"z"}
    return str((i%9) + 1) + d[i//9]
def printResult(r):
    for k,v in r.items():
        print("{0}:\t{1}".format(tileToString(k), v))
def printResultList(r):
    for k,v in r.items():
        print("{0} {1}:\t{2}\t{3}".format(tileToString(k[0]),tileToString(k[1]), v, makeReadableList(v.Pairs)))
def makeReadableList(pairs):
    return [(tileToString(i), tileToString(j), w) for i,j,w in pairs]
def runTests():
    
    hand6 = parseTenhouHand("455788m307p23378s")
    s = ShantenCalculator()

    wall = [
      4,4,4,4,4,4,4,4,4,
      4,4,4,4,4,4,4,4,4,
      4,4,4,4,4,4,4,4,4,
      4,4,4,4,4,4,4
    ]
    hand1 = [
      1,1,1,0,0,0,0,0,0,
      0,1,0,1,1,0,2,0,1,
      0,0,0,0,0,0,0,0,0,
      1,0,1,0,3,0,0
    ]
    hand2 = [
      1,1,1,1,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,
      0,0,0,0,1,0,0
    ]
    hand3 = [
      1,0,0,0,0,1,1,1,0,
      0,1,0,2,0,0,1,0,0,
      1,1,0,0,0,1,0,2,1,
      0,0,0,0,0,0,0
    ]
    hand4 = [
      0,0,1,0,0,1,0,0,0,
      0,0,0,1,2,1,0,1,0,
      0,2,0,1,1,0,2,0,1,
      0,0,0,0,0,0,0
    ]
    hand5 = [
      0,0,0,1,2,0,0,0,0,
      0,3,1,0,1,0,1,0,1,
      0,0,1,0,1,0,1,0,1,
      0,0,0,0,0,0,0
    ]
    hand6 = parseTenhouHand("99m257779p345669s")
    hand7 = parseTenhouHand("238m079p144668s37z")

    #printResult(s.GetTwoStepCounts(hand1, subtractHand(wall, hand1)))
    #printResult(s.GetTwoStepCounts(hand2, subtractHand(wall, hand2)))
    #printResult(s.GetTwoStepCounts(hand3, subtractHand(wall, hand3)))
    #printResult(s.GetTwoStepCounts(hand4, subtractHand(wall, hand4)))
    #printResult(s.GetTwoStepCounts(hand5, subtractHand(wall, hand5)))


    #pprint.PrettyPrinter(indent=4).pprint(s.GetTwoStepCounts(hand6, subtractHand(wall, hand6)))
    #printResult(s.GetTwoShantenCounts(hand6, subtractHand(wall, hand6)))
    #printResult(s.GetOneShantenCounts(hand6, subtractHand(wall, hand6)))
    #printResultList(s.CalculateTwoStepList(hand6, subtractHand(wall, hand6)))

    #l = s.GetThreeStepCounts(hand6, subtractHand(wall, hand6))
    #print("test");
    #printResult(s.GetOneShantenCounts(hand6, subtractHand(wall, hand6)))
    #printResult(s.GetTwoStepCounts(hand6, subtractHand(wall, hand6)))
    print("hi")
    start = time.time_ns();
    for _ in range(1):
        printResult(s.GetThreeStepOnlyCountsThreaded(hand7, subtractHand(wall, hand7), 8))
    end = time.time_ns();
    print("time: {0}".format((end - start) / 1000000))
    start = time.time_ns();
    for _ in range(1):
        printResult(s.GetThreeStepOnlyCounts(hand7, subtractHand(wall, hand7)))
    end = time.time_ns();
    print("time: {0}".format((end - start) / 1000000))

    start = time.time_ns();
    for _ in range(1):
        printResult(s.GetThreeStepRecur(hand7, subtractHand(wall, hand7)))
    end = time.time_ns();
    print("time: {0}".format((end - start) / 1000000))

    
def checkDiff(s, hand, wall):
    shanten = s.GetShanten(hand) - 1
    if (shanten != 1):
        return
    one = s.GetOneShantenCounts(hand, wall)
    two = s.GetTwoStepCounts(hand, wall)
    f1 = max(one.items(), key = lambda kv: kv[1].One)
    f2 = max(two.items(), key = lambda kv: kv[1].Two)
    if (f1[0] not in two):
        return
    if (f2[0] not in one):
        return

    compared = two[f1[0]]

    compared2 = one[f2[0]]

    if compared.Two == f2[1].Two:
        return
    print(compared2.One, f1[1].One)
    #if compared2.One == f1[1].One:
    #    return;
    ratio = f2[1].Two/compared.Two
    
    print("Found hand: {} Shanten: {}".format(convertToTenhouNotation(hand), s.GetShanten(hand) - 1))
    print("DiscardOne: {} DiscardTwo: {} One shanten: {} Two shanten: {} Ratio: {}".format(tileToString(f1[0]), tileToString(f2[0]), compared.Two, f2[1].Two, ratio))
def convertToTenhouNotation(hand):
    d = {0: "m", 1: "p", 2:"s", 3:"z"}
    tot = [[],[],[],[]]
    for i,v in enumerate(hand):
        tot[i//9].extend([(i%9) + 1]*v)
    totStr = [''.join(map(str,l)) for l in tot]
    return "{}m{}p{}s{}z".format(totStr[0],totStr[1],totStr[2],totStr[3])
def convertToHand(tiles):
    hand = [0 for _ in range(34)]
    for t in tiles:
        hand[t] += 1
    return hand
def makeHands():
    while True:
        f = list(range(34))*4
        shuffle(f)
        for i in range(0, len(f), 14):
            h = f[i:i+14]
            if len(h) == 14:
                yield convertToHand(h)
if __name__ == "__main__":
    
    #wall = [
    #  4,4,4,4,4,4,4,4,4,
    #  4,4,4,4,4,4,4,4,4,
    #  4,4,4,4,4,4,4,4,4,
    #  4,4,4,4,4,4,4
    #]
    #print(convertToTenhouNotation(wall))
    #s = ShantenCalculator()
    #t = makeHands()
#
#    for _ in range(50000):
#        hand = next(t)
#        checkDiff(s, hand, subtractHand(wall, hand))
    runTests()