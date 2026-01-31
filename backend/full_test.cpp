// Compile only the parts we need from solver.cpp to test
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;

enum Move {
    Ux1, Ux2, Ux3,
    Dx1, Dx2, Dx3,
    Lx1, Lx2, Lx3,
    Rx1, Rx2, Rx3,
    Fx1, Fx2, Fx3,
    Bx1, Bx2, Bx3,
    None
};

string move_strings[18] = {
    "U", "U2", "U'", "D", "D2", "D'",
    "L", "L2", "L'", "R", "R2", "R'",
    "F", "F2", "F'", "B", "B2", "B'"
};

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

struct CubeState {
    int cp[8];
    int co[8];
    int ep[12];
    int eo[12];
    
    CubeState() {
        for (int i = 0; i < 8; i++) { cp[i] = i; co[i] = 0; }
        for (int i = 0; i < 12; i++) { ep[i] = i; eo[i] = 0; }
    }
};

// Test if applying R' to the "after R" state gives us back the solved state
CubeState applyMove(const CubeState &s, Move m);  // Forward declaration

int main() {
    // Start with solved
    CubeState solved;
    
    // Apply R
    CubeState afterR = applyMove(solved, Rx1);
    cout << "After R move:\n";
    cout << "CP: "; for(int i=0;i<8;i++) cout << afterR.cp[i] << " "; cout << "\n";
    cout << "CO: "; for(int i=0;i<8;i++) cout << afterR.co[i] << " "; cout << "\n";
    
    // Apply R' (which is Rx3)
    CubeState afterRprime = applyMove(afterR, Rx3);
    cout << "\nAfter R then R':\n";
    cout << "CP: "; for(int i=0;i<8;i++) cout << afterRprime.cp[i] << " "; cout << "\n";
    cout << "CO: "; for(int i=0;i<8;i++) cout << afterRprime.co[i] << " "; cout << "\n";
    
    // Check if solved
    bool is_solved = true;
    for(int i=0;i<8;i++) if(afterRprime.cp[i] != i || afterRprime.co[i] != 0) is_solved = false;
    for(int i=0;i<12;i++) if(afterRprime.ep[i] != i || afterRprime.eo[i] != 0) is_solved = false;
    
    cout << "\nIs solved after R then R': " << (is_solved ? "YES" : "NO") << "\n";
    
    // Now test: what if we start with the "after R" state and try to solve it?
    // The solver should find R' as the solution
    cout << "\nThis means the solution to the 'after R' state should be R' (or equivalent)\n";
    cout << "But the solver returned: R U2 D2 L2 R2 U2 D2 L2 (8 moves)\n";
    cout << "\nThis suggests the solver's search algorithm is not finding the optimal solution.\n";
    
    return 0;
}

CubeState applyMove(const CubeState &s, Move m) {
    CubeState n = s;
    int axis = m / 3;
    int power = (m % 3) + 1;

    for (int k = 0; k < power; k++) {
        CubeState temp = n;
        
        switch (axis) {
        case 0: // U
            n.cp[UBR] = temp.cp[URF]; n.co[UBR] = temp.co[URF];
            n.cp[ULB] = temp.cp[UBR]; n.co[ULB] = temp.co[UBR];
            n.cp[UFL] = temp.cp[ULB]; n.co[UFL] = temp.co[ULB];
            n.cp[URF] = temp.cp[UFL]; n.co[URF] = temp.co[UFL];
            
            n.ep[UB] = temp.ep[UR]; n.eo[UB] = temp.eo[UR];
            n.ep[UL] = temp.ep[UB]; n.eo[UL] = temp.eo[UB];
            n.ep[UF] = temp.ep[UL]; n.eo[UF] = temp.eo[UL];
            n.ep[UR] = temp.ep[UF]; n.eo[UR] = temp.eo[UF];
            break;

        case 1: // D
            n.cp[DLF] = temp.cp[DFR]; n.co[DLF] = temp.co[DFR];
            n.cp[DBL] = temp.cp[DLF]; n.co[DBL] = temp.co[DLF];
            n.cp[DRB] = temp.cp[DBL]; n.co[DRB] = temp.co[DBL];
            n.cp[DFR] = temp.cp[DRB]; n.co[DFR] = temp.co[DRB];
            
            n.ep[DL] = temp.ep[DF]; n.eo[DL] = temp.eo[DF];
            n.ep[DB] = temp.ep[DL]; n.eo[DB] = temp.eo[DL];
            n.ep[DR] = temp.ep[DB]; n.eo[DR] = temp.eo[DB];
            n.ep[DF] = temp.ep[DR]; n.eo[DF] = temp.eo[DR];
            break;

        case 2: // L
            n.cp[UFL] = temp.cp[ULB]; n.co[UFL] = (temp.co[ULB] + 2) % 3;
            n.cp[ULB] = temp.cp[DBL]; n.co[ULB] = (temp.co[DBL] + 1) % 3;
            n.cp[DBL] = temp.cp[DLF]; n.co[DBL] = (temp.co[DLF] + 2) % 3;
            n.cp[DLF] = temp.cp[UFL]; n.co[DLF] = (temp.co[UFL] + 1) % 3;
            
            n.ep[UL] = temp.ep[BL]; n.eo[UL] = temp.eo[BL];
            n.ep[BL] = temp.ep[DL]; n.eo[BL] = temp.eo[DL];
            n.ep[DL] = temp.ep[FL]; n.eo[DL] = temp.eo[FL];
            n.ep[FL] = temp.ep[UL]; n.eo[FL] = temp.eo[UL];
            break;

        case 3: // R
            n.cp[URF] = temp.cp[DFR]; n.co[URF] = (temp.co[DFR] + 2) % 3;
            n.cp[DFR] = temp.cp[DRB]; n.co[DFR] = (temp.co[DRB] + 1) % 3;
            n.cp[DRB] = temp.cp[UBR]; n.co[DRB] = (temp.co[UBR] + 2) % 3;
            n.cp[UBR] = temp.cp[URF]; n.co[UBR] = (temp.co[URF] + 1) % 3;
            
            n.ep[UR] = temp.ep[FR]; n.eo[UR] = temp.eo[FR];
            n.ep[FR] = temp.ep[DR]; n.eo[FR] = temp.eo[DR];
            n.ep[DR] = temp.ep[BR]; n.eo[DR] = temp.eo[BR];
            n.ep[BR] = temp.ep[UR]; n.eo[BR] = temp.eo[UR];
            break;

        case 4: // F
            n.cp[URF] = temp.cp[UFL]; n.co[URF] = (temp.co[UFL] + 1) % 3;
            n.cp[UFL] = temp.cp[DLF]; n.co[UFL] = (temp.co[DLF] + 2) % 3;
            n.cp[DLF] = temp.cp[DFR]; n.co[DLF] = (temp.co[DFR] + 1) % 3;
            n.cp[DFR] = temp.cp[URF]; n.co[DFR] = (temp.co[URF] + 2) % 3;
            
            n.ep[UF] = temp.ep[FL]; n.eo[UF] = (temp.eo[FL] + 1) % 2;
            n.ep[FL] = temp.ep[DF]; n.eo[FL] = (temp.eo[DF] + 1) % 2;
            n.ep[DF] = temp.ep[FR]; n.eo[DF] = (temp.eo[FR] + 1) % 2;
            n.ep[FR] = temp.ep[UF]; n.eo[FR] = (temp.eo[UF] + 1) % 2;
            break;

        case 5: // B
            n.cp[UBR] = temp.cp[DRB]; n.co[UBR] = (temp.co[DRB] + 1) % 3;
            n.cp[DRB] = temp.cp[DBL]; n.co[DRB] = (temp.co[DBL] + 2) % 3;
            n.cp[DBL] = temp.cp[ULB]; n.co[DBL] = (temp.co[ULB] + 1) % 3;
            n.cp[ULB] = temp.cp[UBR]; n.co[ULB] = (temp.co[UBR] + 2) % 3;
            
            n.ep[UB] = temp.ep[BR]; n.eo[UB] = (temp.eo[BR] + 1) % 2;
            n.ep[BR] = temp.ep[DB]; n.eo[BR] = (temp.eo[DB] + 1) % 2;
            n.ep[DB] = temp.ep[BL]; n.eo[DB] = (temp.eo[BL] + 1) % 2;
            n.ep[BL] = temp.ep[UB]; n.eo[BL] = (temp.eo[UB] + 1) % 2;
            break;
        }
    }
    return n;
}
