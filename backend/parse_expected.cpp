#define PYBIND11_BUILD
#include "solver_new.cpp"

int main() {
    string expected = "UUFUUFUUFRRRRRRRRRFFDFFDFFDDDBDDBDDBLLLLLLLLLUBBUBBUBB";
    CubeState state;
    
    if (parse_facelets(expected, state)) {
        cout << "Successfully parsed expected R-move result:" << endl;
        cout << "Corner permutation: ";
        for(int i=0; i<8; i++) cout << state.cp[i] << " ";
        cout << "\nCorner orientation: ";
        for(int i=0; i<8; i++) cout << state.co[i] << " ";
        cout << "\nEdge permutation: ";
        for(int i=0; i<12; i++) cout << state.ep[i] << " ";
        cout << "\nEdge orientation: ";
        for(int i=0; i<12; i++) cout << state.eo[i] << " ";
        cout << endl;
    } else {
        cout << "Failed to parse" << endl;
    }
    
    return 0;
}
