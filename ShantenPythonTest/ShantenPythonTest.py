
from ShantenLib import ShantenCalculator
import pprint

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
if __name__ == "__main__":
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
    hand6 = parseTenhouHand("557m24889s23789p2s")


    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoShantenCounts(hand1, wall))
    pprint.PrettyPrinter(indent=4).pprint(s.GetOneShantenCounts(hand1, wall))

    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoShantenCounts(hand2, wall))
    pprint.PrettyPrinter(indent=4).pprint(s.GetOneShantenCounts(hand2, wall))

    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoShantenCounts(hand3, wall))
    pprint.PrettyPrinter(indent=4).pprint(s.GetOneShantenCounts(hand3, wall))

    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoShantenCounts(hand4, wall))
    pprint.PrettyPrinter(indent=4).pprint(s.GetOneShantenCounts(hand4, wall))

    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoShantenCounts(hand5, wall))
    pprint.PrettyPrinter(indent=4).pprint(s.GetOneShantenCounts(hand5, wall))

    pprint.PrettyPrinter(indent=4).pprint(s.GetTwoStepCounts(hand6, subtractHand(wall, hand6)))
    printResult(s.GetTwoShantenCounts(hand6, subtractHand(wall, hand6)))
    printResult(s.GetOneShantenCounts(hand6, subtractHand(wall, hand6)))
    printResultList(s.CalculateTwoStepList(hand6, subtractHand(wall, hand6)))