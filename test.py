import numpy as np

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
    
    return solve(A, B, coeffs)

print(diophantSolution(16, 7, 15))