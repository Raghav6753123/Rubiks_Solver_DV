#include <iostream>
#include <string>
using namespace std;

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

// Facelet indices - exactly from solver.cpp
int cornerFacelet[8][3] = {
    {8, 9, 20},   // URF
    {6, 18, 38},  // UFL
    {0, 36, 47},  // ULB
    {2, 45, 11},  // UBR
    {29, 26, 15}, // DFR
    {27, 24, 44}, // DLF
    {33, 53, 42}, // DBL
    {35, 17, 51}  // DRB
};

int edgeFacelet[12][2] = {
    {5, 10},  // UR
    {7, 19},  // UF
    {3, 37},  // UL
    {1, 46},  // UB
    {32, 16}, // DR
    {28, 25}, // DF
    {30, 43}, // DL
    {34, 52}, // DB
    {23, 12}, // FR
    {21, 41}, // FL
    {50, 39}, // BL
    {48, 14}  // BR
};

char cornerColor[8][3] = {
    {'U', 'R', 'F'}, // URF
    {'U', 'F', 'L'}, // UFL
    {'U', 'L', 'B'}, // ULB
    {'U', 'B', 'R'}, // UBR
    {'D', 'F', 'R'}, // DFR
    {'D', 'F', 'L'}, // DLF
    {'D', 'B', 'L'}, // DBL
    {'D', 'R', 'B'}  // DRB
};

char edgeColor[12][2] = {
    {'U', 'R'}, // UR
    {'U', 'F'}, // UF
    {'U', 'L'}, // UL
    {'U', 'B'}, // UB
    {'D', 'R'}, // DR
    {'D', 'F'}, // DF
    {'D', 'L'}, // DL
    {'D', 'B'}, // DB
    {'F', 'R'}, // FR
    {'F', 'L'}, // FL
    {'B', 'L'}, // BL
    {'B', 'R'}  // BR
};

struct CubeState {
    int cp[8];
    int co[8];
    int ep[12];
    int eo[12];
    
    CubeState() {
        for (int i = 0; i < 8; i++) { cp[i] = -1; co[i] = -1; }
        for (int i = 0; i < 12; i++) { ep[i] = -1; eo[i] = -1; }
    }
};

bool parse_facelets(string f, CubeState &c) {
    cout << "Parsing: " << f << "\n\n";
    
    // Parse corners
    for(int i=0; i<8; i++) {
        cout << "Corner position " << i << ":\n";
        cout << "  Facelets: [" << cornerFacelet[i][0] << "," << cornerFacelet[i][1] << "," << cornerFacelet[i][2] << "]\n";
        cout << "  Colors: " << f[cornerFacelet[i][0]] << f[cornerFacelet[i][1]] << f[cornerFacelet[i][2]] << "\n";
        
        // Find orientation
        int ori = -1;
        for(int o=0; o<3; o++) {
            char col = f[cornerFacelet[i][o]];
            if(col == 'U' || col == 'D') { ori = o; break; }
        }
        cout << "  Orientation: " << ori << "\n";
        if(ori == -1) { cout << "  ERROR: No U/D face found!\n"; return false; }
        c.co[i] = ori;

        // Find which piece
        string colors = "";
        colors += f[cornerFacelet[i][0]];
        colors += f[cornerFacelet[i][1]];
        colors += f[cornerFacelet[i][2]];
        
        bool found = false;
        for(int target=0; target<8; target++) {
            int match = 0;
            for(char ch : colors) {
                for(int k=0; k<3; k++) if(cornerColor[target][k] == ch) match++;
            }
            if(match == 3) { 
                c.cp[i] = target; 
                found = true;
                cout << "  Piece: " << target << " (expected colors: " 
                     << cornerColor[target][0] << cornerColor[target][1] << cornerColor[target][2] << ")\n";
                break; 
            }
        }
        if (!found) { cout << "  ERROR: No matching piece!\n"; return false; }
        cout << "\n";
    }
    
    return true;
}

int main() {
    // Test with cube after R move
    string afterR = "UUFUUFUUFRRRRRRRRRFFDFFDFFDDDBDDBDDBLLLLLLLLLUBBUBBUBB";
    CubeState state;
    parse_facelets(afterR, state);
    
    cout << "\nFinal state:\n";
    cout << "CP: "; for(int i=0;i<8;i++) cout << state.cp[i] << " "; cout << "\n";
    cout << "CO: "; for(int i=0;i<8;i++) cout << state.co[i] << " "; cout << "\n";
    
    // Expected after R:
    cout << "\nExpected after R move:\n";
    cout << "CP: 4 1 2 0 7 5 6 3\n";
    cout << "CO: 2 0 0 1 1 0 0 2\n";
    
    return 0;
}
