#include <iostream>
#include <string>
using namespace std;

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

int cornerFacelet[8][3] = {
    {8, 9, 20},   {6, 18, 38},  {0, 36, 47},  {2, 45, 11},
    {29, 26, 15}, {27, 24, 44}, {33, 53, 42}, {35, 17, 51}
};

char cornerColor[8][3] = {
    {'U', 'R', 'F'}, {'U', 'F', 'L'}, {'U', 'L', 'B'}, {'U', 'B', 'R'},
    {'D', 'F', 'R'}, {'D', 'F', 'L'}, {'D', 'B', 'L'}, {'D', 'R', 'B'}
};

int edgeFacelet[12][2] = {
    {5, 10},  {7, 19},  {3, 37},  {1, 46},
    {32, 16}, {28, 25}, {30, 43}, {34, 52},
    {23, 12}, {21, 41}, {50, 39}, {48, 14}
};

char edgeColor[12][2] = {
    {'U', 'R'}, {'U', 'F'}, {'U', 'L'}, {'U', 'B'},
    {'D', 'R'}, {'D', 'F'}, {'D', 'L'}, {'D', 'B'},
    {'F', 'R'}, {'F', 'L'}, {'B', 'L'}, {'B', 'R'}
};

string cube_to_facelets(const CubeState& cube) {
    string f(54, '?');
    
    // Set center facelets
    f[4] = 'U'; f[13] = 'R'; f[22] = 'F';
    f[31] = 'D'; f[40] = 'L'; f[49] = 'B';
    
    // Set corner facelets
    for (int i = 0; i < 8; i++) {
        int piece = cube.cp[i];
        int ori = cube.co[i];
        
        for (int j = 0; j < 3; j++) {
            int faceletIdx = cornerFacelet[i][j];
            int colorIdx = (j - ori + 3) % 3;
            f[faceletIdx] = cornerColor[piece][colorIdx];
        }
    }
    
    // Set edge facelets  
    for (int i = 0; i < 12; i++) {
        int piece = cube.ep[i];
        int ori = cube.eo[i];
        
        f[edgeFacelet[i][0]] = edgeColor[piece][ori];
        f[edgeFacelet[i][1]] = edgeColor[piece][1-ori];
    }
    
    return f;
}

int main() {
    CubeState solved;
    string facelets = cube_to_facelets(solved);
    cout << "Result:   " << facelets << endl;
    cout << "Expected: UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB" << endl;
    
    if (facelets == "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB") {
        cout << "SUCCESS!" << endl;
    } else {
        cout << "MISMATCH" << endl;
        for (int i = 0; i < 54; i++) {
            if (facelets[i] != "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"[i]) {
                cout << "Position " << i << ": got '" << facelets[i] << "', expected '" 
                     << "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"[i] << "'" << endl;
            }
        }
    }
    
    return 0;
}
