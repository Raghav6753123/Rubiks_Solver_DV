#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <fstream>
#include <sstream>

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

void cycle4(int* arr, int i1, int i2, int i3, int i4) {
    int temp = arr[i1];
    arr[i1] = arr[i4];
    arr[i4] = arr[i3];
    arr[i3] = arr[i2];
    arr[i2] = temp;
}

CubeState applyMove(const CubeState &s, Move m) {
    CubeState n = s;
    int axis = m / 3;
    int power = (m % 3) + 1;

    for (int k = 0; k < power; k++) {
        switch (axis) {
        case 0: // U - Corners: URF <- UFL <- ULB <- UBR <- URF
            cycle4(n.cp, URF, UBR, ULB, UFL);
            cycle4(n.co, URF, UBR, ULB, UFL);
            // Edges: UR <- UF <- UL <- UB <- UR
            cycle4(n.ep, UR, UB, UL, UF);
            cycle4(n.eo, UR, UB, UL, UF);
            break;
        case 1: // D - Corners: DFR <- DLF <- DBL <- DRB <- DFR
            cycle4(n.cp, DFR, DLF, DBL, DRB);
            cycle4(n.co, DFR, DLF, DBL, DRB);
            // Edges: DR <- DF <- DL <- DB <- DR
            cycle4(n.ep, DR, DF, DL, DB);
            cycle4(n.eo, DR, DF, DL, DB);
            break;
        case 3: // R - Corners: URF <- UBR <- DRB <- DFR <- URF
            cycle4(n.cp, URF, DFR, DRB, UBR);
            {
                int tco = n.co[URF];
                n.co[URF] = (n.co[UBR] + 1) % 3;
                n.co[UBR] = (n.co[DRB] + 2) % 3;
                n.co[DRB] = (n.co[DFR] + 1) % 3;
                n.co[DFR] = (tco + 2) % 3;
            }
            // Edges: UR <- BR <- DR <- FR <- UR
            cycle4(n.ep, UR, FR, DR, BR);
            cycle4(n.eo, UR, FR, DR, BR);
            break;
        }
    }
    return n;
}

bool is_solved(const CubeState &s) {
    for (int i = 0; i < 8; i++) {
        if (s.cp[i] != i || s.co[i] != 0) return false;
    }
    for (int i = 0; i < 12; i++) {
        if (s.ep[i] != i || s.eo[i] != 0) return false;
    }
    return true;
}

int main() {
    // Start with solved state
    CubeState solved;
    cout << "Solved state: " << is_solved(solved) << endl;
    
    // Apply R move
    CubeState after_r = applyMove(solved, Rx1);
    cout << "After R: " << is_solved(after_r) << endl;
    
    // Apply R' (R three times)
    CubeState after_r_inv = applyMove(after_r, Rx3);
    cout << "After R R': " << is_solved(after_r_inv) << endl;
    
    // Apply R R R R (should return to solved)
    CubeState after_4r = applyMove(applyMove(applyMove(after_r, Rx1), Rx1), Rx1);
    cout << "After R R R R: " << is_solved(after_4r) << endl;
    
    return 0;
}
