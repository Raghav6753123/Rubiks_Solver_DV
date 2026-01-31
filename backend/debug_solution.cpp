#define PYBIND11_BUILD
#include "solver.cpp"
#include <sstream>

string to_facelets(const CubeState &c) {
    string f(54, '?');
    f[4] = 'U'; f[13] = 'R'; f[22] = 'F'; f[31] = 'D'; f[40] = 'L'; f[49] = 'B';
    
    for(int i=0; i<8; i++) {
        int piece = c.cp[i];
        int ori = c.co[i];
        for(int j=0; j<3; j++) {
            f[cornerFacelet[i][(j + ori) % 3]] = cornerColor[piece][j];
        }
    }
    
    for(int i=0; i<12; i++) {
        int piece = c.ep[i];
        int ori = c.eo[i];
        f[edgeFacelet[i][0]] = edgeColor[piece][ori];
        f[edgeFacelet[i][1]] = edgeColor[piece][1 - ori];
    }
    
    return f;
}

Move parseMove(const string& s) {
    char face = s[0];
    string mod = s.substr(1);
    int axis = -1;
    if (face == 'U') axis = 0;
    else if (face == 'D') axis = 1;
    else if (face == 'L') axis = 2;
    else if (face == 'R') axis = 3;
    else if (face == 'F') axis = 4;
    else if (face == 'B') axis = 5;
    
    int power = 0;
    if (mod == "") power = 0;
    else if (mod == "2") power = 1;
    else if (mod == "'") power = 2;
    
    return (Move)(axis * 3 + power);
}

int main() {
    initialize_solver("./pdb");
    
    // Start with R-scrambled cube
    CubeState solved;
    CubeState start_state = applyMove(solved, Rx1);
    string start_str = to_facelets(start_state);
    
    cout << "Start: " << start_str << endl;
    
    // Get solution from solver
    string solution = solve(start_str);
    cout << "Solution: " << solution << endl;
    
    // Apply solution move by move
    CubeState state = start_state;
    istringstream iss(solution);
    string move_str;
    int move_num = 0;
    while (iss >> move_str) {
        Move m = parseMove(move_str);
        state = applyMove(state, m);
        move_num++;
        if (move_num <= 5) {
            cout << "After move " << move_num << " (" << move_str << "): ";
            cout << to_facelets(state).substr(0, 20) << "..." << endl;
        }
    }
    
    string final_str = to_facelets(state);
    string expected = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
    
    cout << "\nFinal:    " << final_str << endl;
    cout << "Expected: " << expected << endl;
    cout << "Match: " << (final_str == expected ? "YES" : "NO") << endl;
    
    return 0;
}
