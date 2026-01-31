#include <iostream>
#include <string>
using namespace std;

// Test if the solver's applyMove(R) matches our expectations
// Copy the exact code from solver.cpp

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

enum Move {
    Ux1, Ux2, Ux3,
    Dx1, Dx2, Dx3,
    Lx1, Lx2, Lx3,
    Rx1, Rx2, Rx3,
    Fx1, Fx2, Fx3,
    Bx1, Bx2, Bx3,
    None
};

CubeState applyMove(const CubeState &s, Move m) {
    CubeState n = s;
    int axis = m / 3;
    int power = (m % 3) + 1;

    for (int k = 0; k < power; k++) {
        CubeState temp = n;
        
        if (axis == 3) { // R move
            // URF←DFR←DRB←UBR←URF (orientation +2,+1,+2,+1)
            n.cp[URF] = temp.cp[DFR]; n.co[URF] = (temp.co[DFR] + 2) % 3;
            n.cp[DFR] = temp.cp[DRB]; n.co[DFR] = (temp.co[DRB] + 1) % 3;
            n.cp[DRB] = temp.cp[UBR]; n.co[DRB] = (temp.co[UBR] + 2) % 3;
            n.cp[UBR] = temp.cp[URF]; n.co[UBR] = (temp.co[URF] + 1) % 3;
            
            // UR←FR←DR←BR←UR
            n.ep[UR] = temp.ep[FR]; n.eo[UR] = temp.eo[FR];
            n.ep[FR] = temp.ep[DR]; n.eo[FR] = temp.eo[DR];
            n.ep[DR] = temp.ep[BR]; n.eo[DR] = temp.eo[BR];
            n.ep[BR] = temp.ep[UR]; n.eo[BR] = temp.eo[UR];
        }
    }
    return n;
}

void printState(const CubeState& s, const string& label) {
    cout << label << ":\n";
    cout << "  CP: "; for(int i=0;i<8;i++) cout << s.cp[i] << " "; cout << "\n";
    cout << "  CO: "; for(int i=0;i<8;i++) cout << s.co[i] << " "; cout << "\n";
    cout << "  EP: "; for(int i=0;i<12;i++) cout << s.ep[i] << " "; cout << "\n";
    cout << "  EO: "; for(int i=0;i<12;i++) cout << s.eo[i] << " "; cout << "\n";
}

int main() {
    CubeState solved;
    cout << "Testing R move:\n";
    printState(solved, "Solved");
    
    CubeState afterR = applyMove(solved, Rx1);
    printState(afterR, "After R");
    
    // Apply R three times (should give R')
    CubeState afterR3 = applyMove(afterR, Rx3);
    printState(afterR3, "After R then R' (should be solved)");
    
    // Check if back to solved
    bool solved_check = true;
    for(int i=0;i<8;i++) if(afterR3.cp[i] != i || afterR3.co[i] != 0) solved_check = false;
    for(int i=0;i<12;i++) if(afterR3.ep[i] != i || afterR3.eo[i] != 0) solved_check = false;
    
    cout << "\nR then R' returns to solved: " << (solved_check ? "YES" : "NO") << "\n";
    
    return 0;
}
