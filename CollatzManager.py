from sys import exit
from math import trunc
import numpy as np

class CollatzSolver:
    def __init__(self, *args):
        if len(args) < 3:
            raise Exception("Less than 3 args! Must be \"s1, q1, s2, ...\"")
        self.qs = list(args)
        self.bases = np.array(diophantSolution(*getCoeffs(*args))[::-1])
        self.temps = [abs(a) for a in getCoeffs(*args[:3])[:2]]
        
        self.cropBases()


    def cropBases(self):
        for i in range(len(self.bases)):
            if (self.bases[i] // self.temps[i] > 0):
                self.bases[i] -= self.temps[i]*(self.bases[i] // self.temps[i])

    def getM(self):
        return self.bases + self.temps

    def getSolution(self):
        sPatternItems = list()
        m = self.getM()
        for i in range(0, len(self.bases), 2):
            sPatternItems.append(2**self.qs[2*i] * m[i] - 1)
        return sPatternItems

    def getMStr(self):
        s = ""
        for i in range(len(self.bases)):
            s += "m" + str(i) + " = " + str(self.bases[i]) + " + " + str(self.temps[i]) + "t;   "
        return s

    def nextIteration(self, q, s):
        self.qs.extend([q, s])
        lastM = (self.bases[-1], self.temps[-1])
        diophCoeffs = getCoeffs(*self.qs[-3:])
        diophCoeffs[1] *= lastM[1]
        diophCoeffs[2] += 3**self.qs[-3]*lastM[0]
        
        solution = diophantSolution(*diophCoeffs)

        z, x = [solution[i] - trunc(solution[i]/diophCoeffs[1-i])*diophCoeffs[1-i] for i in range(len(solution))]
        self.bases = np.append(self.bases, z)

        zt, xt = [abs(a) for a in diophCoeffs[:-1][::-1]]
        self.temps = np.append(self.temps, zt)

        for i in range(len(self.bases)-1):
            self.bases[i] += self.temps[i]*x
            self.temps[i] *= xt

def getCoeffs(s1, q1, s2):
    return [2**( q1 + s2 ), -3**s1, 2**q1 - 1]

def diophantSolution(A, B, C = 1):
    def solve(A, B, coeffs = [1, 1]):
        if A == 0:
            return (0, 1)
        
        x, y = solve(B%A , A)
        return [(y - coeffs[0]*(B//A)*x), coeffs[1]*x]
    
    coeffs = [1, 1]
    if A < 0:
        coeffs[0] = -1
        A = abs(A)
    
    if B < 0:
        coeffs[1] = -1
        B = abs(B)
    
    return np.array(solve(A, B, coeffs))*C

def count2(v):
    if v%2 == 1:
        return 0
    return 1 + count2(v/2)

def collatz(base = 5, count=30):
    n = base
    print("Collatz:")
    print(n, end= " ")
    for i in range(count):
        if (n % 2 == 0 ):
            n //= 2
        else:
            n = 3*n + 1
        print(n, end="  ")


def main():
    q = [3, 2, 3]

    print("\nTest:", q)

    d = CollatzSolver(*q)
    print(d.getMStr())
    print(d.getSolution())
    # collatz(d.getSolution()[0])

    print()
    print()

    print("Test:", list(q) + [1, 3])
    d.nextIteration(1, 3)
    print(d.getMStr())
    print(d.getSolution())
    # print(collatz(d.getSolution()[0]))

if __name__ == "__main__":
    main()