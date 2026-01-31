#include <iostream>
using namespace std;

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };

struct CubeState {
    int cp[8];
    int co[8];
    
    CubeState() {
        for (int i = 0; i < 8; i++) { cp[i] = i; co[i] = 0; }
    }
};

// Method 1: cycle4(URF, UFL, ULB, UBR)
void method1(CubeState &n) {
    int temp = n.cp[URF];
    n.cp[URF] = n.cp[UBR];
    n.cp[UBR] = n.cp[ULB];
    n.cp[ULB] = n.cp[UFL];
    n.cp[UFL] = temp;
}

// Method 2: cycle4(URF, UBR, ULB, UFL)
void method2(CubeState &n) {
    int temp = n.cp[URF];
    n.cp[URF] = n.cp[UFL];
    n.cp[UFL] = n.cp[ULB];
    n.cp[ULB] = n.cp[UBR];
    n.cp[UBR] = temp;
}

void print_corners(const CubeState &s) {
    string names[] = {"URF", "UFL", "ULB", "UBR", "DFR", "DLF", "DBL", "DRB"};
    for (int i = 0; i < 8; i++) {
        cout << names[i] << ": " << names[s.cp[i]] << "  ";
    }
    cout << endl;
}

int main() {
    CubeState s1, s2;
    
    cout << "Initial state:" << endl;
    print_corners(s1);
    
    cout << "\nMethod 1 - cycle4(URF, UFL, ULB, UBR):" << endl;
    method1(s1);
    print_corners(s1);
    
    cout << "\nMethod 2 - cycle4(URF, UBR, ULB, UFL):" << endl;
    method2(s2);
    print_corners(s2);
    
    // For a U move, we expect:
    // URF position gets UFL piece (piece moves URF <- UFL)
    // UFL position gets ULB piece (piece moves UFL <- ULB)
    // ULB position gets UBR piece (piece moves ULB <- UBR)
    // UBR position gets URF piece (piece moves UBR <- URF)
    // This creates cycle: URF <- UFL <- ULB <- UBR <- URF
    
    cout << "\n\nExpected for U move (clockwise rotation of top):" << endl;
    cout << "URF: UFL  UFL: ULB  ULB: UBR  UBR: URF" << endl;
    
    return 0;
}
