#include <iostream>
#include <string>

using namespace std;

int main() {
    // Start with solved state
    string cube = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
    
    // Apply R move
    char temp[3];
    temp[0] = cube[2]; temp[1] = cube[5]; temp[2] = cube[8];
    cube[2] = cube[20]; cube[5] = cube[23]; cube[8] = cube[26];
    cube[20] = cube[29]; cube[23] = cube[32]; cube[26] = cube[35];
    cube[29] = cube[51]; cube[32] = cube[48]; cube[35] = cube[45];
    cube[51] = temp[2]; cube[48] = temp[1]; cube[45] = temp[0];
    
    // Rotate R face
    string r = cube.substr(9, 9);
    cube[9] = r[6]; cube[10] = r[3]; cube[11] = r[0];
    cube[12] = r[7]; cube[13] = r[4]; cube[14] = r[1];
    cube[15] = r[8]; cube[16] = r[5]; cube[17] = r[2];
    
    // Apply U move
    temp[0] = cube[20]; temp[1] = cube[19]; temp[2] = cube[18];
    cube[20] = cube[9]; cube[19] = cube[10]; cube[18] = cube[11];
    cube[9] = cube[36]; cube[10] = cube[37]; cube[11] = cube[38];
    cube[36] = cube[45]; cube[37] = cube[46]; cube[38] = cube[47];
    cube[45] = temp[0]; cube[46] = temp[1]; cube[47] = temp[2];
    
    // Rotate U face
    string u = cube.substr(0, 9);
    cube[0] = u[6]; cube[1] = u[3]; cube[2] = u[0];
    cube[3] = u[7]; cube[4] = u[4]; cube[5] = u[1];
    cube[6] = u[8]; cube[7] = u[5]; cube[8] = u[2];
    
    cout << cube << endl;
    return 0;
}
