#include <iostream>
#include <string>
using namespace std;

int main() {
    // Solved state
    string cube = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
    
    // Apply R move (clockwise)
    // This affects indices: 2,5,8 (U), 11,14,17 (R), 20,23,26 (F), 29,32,35 (D), 45,48,51 (B)
    
    // Save U right column
    char u2 = cube[2], u5 = cube[5], u8 = cube[8];
    
    // U right <- F right
    cube[2] = cube[20];
    cube[5] = cube[23];
    cube[8] = cube[26];
    
    // F right <- D right
    cube[20] = cube[29];
    cube[23] = cube[32];
    cube[26] = cube[35];
    
    // D right <- B left (reversed)
    cube[29] = cube[51];
    cube[32] = cube[48];
    cube[35] = cube[45];
    
    // B left <- U right (reversed)
    cube[51] = u8;
    cube[48] = u5;
    cube[45] = u2;
    
    // Rotate R face clockwise
    string r = cube.substr(9, 9);
    cube[9] = r[6];
    cube[10] = r[3];
    cube[11] = r[0];
    cube[12] = r[7];
    cube[13] = r[4];
    cube[14] = r[1];
    cube[15] = r[8];
    cube[16] = r[5];
    cube[17] = r[2];
    
    cout << cube << endl;
    return 0;
}
