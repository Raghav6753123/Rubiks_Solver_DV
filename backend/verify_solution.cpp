#include <iostream>
#include <string>
#include <sstream>

using namespace std;

enum Move {
    Ux1, Ux2, Ux3, Dx1, Dx2, Dx3, Lx1, Lx2, Lx3,
    Rx1, Rx2, Rx3, Fx1, Fx2, Fx3, Bx1, Bx2, Bx3, None
};

string move_strings[18] = {
    "U", "U2", "U'", "D", "D2", "D'",
    "L", "L2", "L'", "R", "R2", "R'",
    "F", "F2", "F'", "B", "B2", "B'"
};

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

struct CubeState {
    int cp[8], co[8], ep[12], eo[12];
    CubeState() {
        for (int i = 0; i < 8; i++) { cp[i] = i; co[i] = 0; }
        for (int i = 0; i < 12; i++) { ep[i] = i; eo[i] = 0; }
    }
};

void cycle4(int* arr, int i1, int i2, int i3, int i4) {
    int temp = arr[i1];
    arr[i1] = arr[i4]; arr[i4] = arr[i3];
    arr[i3] = arr[i2]; arr[i2] = temp;
}

CubeState applyMove(const CubeState &s, Move m);

bool is_solved(const CubeState &s) {
    for (int i = 0; i < 8; i++) if (s.cp[i] != i || s.co[i] != 0) return false;
    for (int i = 0; i < 12; i++) if (s.ep[i] != i || s.eo[i] != 0) return false;
    return true;
}

Move parse_move(const string& s) {
    for (int i = 0; i < 18; i++) {
        if (move_strings[i] == s) return (Move)i;
    }
    return None;
}

CubeState applyMove(const CubeState &s, Move m) {
    CubeState n = s;
    int axis = m / 3, power = (m % 3) + 1;
    for (int k = 0; k < power; k++) {
        switch (axis) {
        case 0: // U
            cycle4(n.cp, URF, UBR, ULB, UFL);
            cycle4(n.co, URF, UBR, ULB, UFL);
            cycle4(n.ep, UR, UB, UL, UF);
            cycle4(n.eo, UR, UB, UL, UF);
            break;
        case 1: // D
            cycle4(n.cp, DFR, DLF, DBL, DRB);
            cycle4(n.co, DFR, DLF, DBL, DRB);
            cycle4(n.ep, DR, DF, DL, DB);
            cycle4(n.eo, DR, DF, DL, DB);
            break;
        case 2: // L
            cycle4(n.cp, UFL, ULB, DBL, DLF);
            { int tco = n.co[UFL]; n.co[UFL] = (n.co[DLF] + 1) % 3;
              n.co[DLF] = (n.co[DBL] + 2) % 3; n.co[DBL] = (n.co[ULB] + 1) % 3;
              n.co[ULB] = (tco + 2) % 3; }
            cycle4(n.ep, UL, BL, DL, FL);
            cycle4(n.eo, UL, BL, DL, FL);
            break;
        case 3: // R
            cycle4(n.cp, URF, DFR, DRB, UBR);
            { int tco = n.co[URF]; n.co[URF] = (n.co[UBR] + 1) % 3;
              n.co[UBR] = (n.co[DRB] + 2) % 3; n.co[DRB] = (n.co[DFR] + 1) % 3;
              n.co[DFR] = (tco + 2) % 3; }
            cycle4(n.ep, UR, FR, DR, BR);
            cycle4(n.eo, UR, FR, DR, BR);
            break;
        case 4: // F
            cycle4(n.cp, URF, UFL, DLF, DFR);
            { int tco = n.co[URF]; n.co[URF] = (n.co[DFR] + 1) % 3;
              n.co[DFR] = (n.co[DLF] + 2) % 3; n.co[DLF] = (n.co[UFL] + 1) % 3;
              n.co[UFL] = (tco + 2) % 3; }
            cycle4(n.ep, UF, FL, DF, FR);
            { int teo = n.eo[UF]; n.eo[UF] = (n.eo[FR] + 1) % 2;
              n.eo[FR] = (n.eo[DF] + 1) % 2; n.eo[DF] = (n.eo[FL] + 1) % 2;
              n.eo[FL] = (teo + 1) % 2; }
            break;
        case 5: // B
            cycle4(n.cp, UBR, DRB, DBL, ULB);
            { int tco = n.co[UBR]; n.co[UBR] = (n.co[ULB] + 1) % 3;
              n.co[ULB] = (n.co[DBL] + 2) % 3; n.co[DBL] = (n.co[DRB] + 1) % 3;
              n.co[DRB] = (tco + 2) % 3; }
            cycle4(n.ep, UB, BR, DB, BL);
            { int teo = n.eo[UB]; n.eo[UB] = (n.eo[BL] + 1) % 2;
              n.eo[BL] = (n.eo[DB] + 1) % 2; n.eo[DB] = (n.eo[BR] + 1) % 2;
              n.eo[BR] = (teo + 1) % 2; }
            break;
        }
    }
    return n;
}

int main() {
    // Test 1: Apply R U R' U' scramble and verify it solves back
    CubeState state;
    cout << "Test 1: R U R' U' scramble" << endl;
    
    state = applyMove(state, Rx1);
    state = applyMove(state, Ux1);
    state = applyMove(state, Rx3);
    state = applyMove(state, Ux3);
    
    cout << "After scramble, is_solved: " << is_solved(state) << endl;
    
    // Apply solution from API test
    string solution = "F2 U' D' B2 R U2 R2 U D F2 U' L2 B2 R2 D' F2 D2";
    istringstream iss(solution);
    string move_str;
    
    while (iss >> move_str) {
        Move m = parse_move(move_str);
        if (m != None) {
            state = applyMove(state, m);
        }
    }
    
    cout << "After applying solution, is_solved: " << is_solved(state) << endl;
    
    return 0;
}
