# Test to verify the facelet string for a solved cube
# URFDLB order means: U face (0-8), R face (9-17), F face (18-26), D face (27-35), L face (36-44), B face (45-53)

solved = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"
print(f"Length: {len(solved)}")
print(f"U count: {solved.count('U')}")
print(f"R count: {solved.count('R')}")
print(f"F count: {solved.count('F')}")
print(f"D count: {solved.count('D')}")
print(f"L count: {solved.count('L')}")
print(f"B count: {solved.count('B')}")
print(f"Total: {solved.count('U') + solved.count('R') + solved.count('F') + solved.count('D') + solved.count('L') + solved.count('B')}")
