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

// =================================================================================================
// --- ENUMS & CONSTANTS ---
// =================================================================================================

enum Move {
    Ux1, Ux2, Ux3,
    Dx1, Dx2, Dx3,
    Lx1, Lx2, Lx3,
    Rx1, Rx2, Rx3,
    Fx1, Fx2, Fx3,
    Bx1, Bx2, Bx3,
    None
};

string move_strings[18] = {
    "U", "U2", "U'", "D", "D2", "D'",
    "L", "L2", "L'", "R", "R2", "R'",
    "F", "F2", "F'", "B", "B2", "B'"
};

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

int cornerFacelet[8][3] = {
    {8, 9, 20},   {6, 18, 38}, {0, 36, 47}, {2, 45, 11},
    {29, 26, 15}, {27, 44, 24}, {33, 53, 42}, {35, 17, 51}
};

int edgeFacelet[12][2] = {
    {5, 10}, {7, 19}, {3, 37}, {1, 46},
    {32, 16}, {28, 25}, {30, 43}, {34, 52},
    {23, 12}, {21, 41}, {50, 39}, {48, 14}
};

char cornerColor[8][3] = {
    {'U', 'R', 'F'}, {'U', 'F', 'L'}, {'U', 'L', 'B'}, {'U', 'B', 'R'},
    {'D', 'F', 'R'}, {'D', 'L', 'F'}, {'D', 'B', 'L'}, {'D', 'R', 'B'}
};

char edgeColor[12][2] = {
    {'U', 'R'}, {'U', 'F'}, {'U', 'L'}, {'U', 'B'},
    {'D', 'R'}, {'D', 'F'}, {'D', 'L'}, {'D', 'B'},
    {'F', 'R'}, {'F', 'L'}, {'B', 'L'}, {'B', 'R'}
};

// =================================================================================================
// --- CUBE STATE ---
// =================================================================================================

struct CubeState {
    int cp[8];
    int co[8];
    int ep[12];
    int eo[12];

    CubeState() {
        for (int i = 0; i < 8; i++) { cp[i] = i; co[i] = 0; }
        for (int i = 0; i < 12; i++) { ep[i] = i; eo[i] = 0; }
    }

    bool operator==(const CubeState &b) const {
        for (int i = 0; i < 8; ++i) if (cp[i] != b.cp[i] || co[i] != b.co[i]) return false;
        for (int i = 0; i < 12; ++i) if (ep[i] != b.ep[i] || eo[i] != b.eo[i]) return false;
        return true;
    }
};

// =================================================================================================
// --- MOVE LOGIC (FIXED) ---
// =================================================================================================

// FIXED: Clockwise cycle direction
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
        case 0: // U
            cycle4(n.cp, URF, UFL, ULB, UBR);
            cycle4(n.co, URF, UFL, ULB, UBR);
            cycle4(n.ep, UR, UF, UL, UB);
            cycle4(n.eo, UR, UF, UL, UB);
            break;
        case 1: // D
            cycle4(n.cp, DFR, DRB, DBL, DLF);
            cycle4(n.co, DFR, DRB, DBL, DLF);
            cycle4(n.ep, DR, DB, DL, DF);
            cycle4(n.eo, DR, DB, DL, DF);
            break;
        case 2: // L - FIXED
            cycle4(n.cp, UFL, ULB, DBL, DLF);
            {
                int tco = n.co[UFL];
                n.co[UFL] = (n.co[DLF] + 1) % 3;
                n.co[DLF] = (n.co[DBL] + 2) % 3;
                n.co[DBL] = (n.co[ULB] + 1) % 3;
                n.co[ULB] = (tco + 2) % 3;
            }
            cycle4(n.ep, UL, BL, DL, FL);
            cycle4(n.eo, UL, BL, DL, FL);
            break;
        case 3: // R
            cycle4(n.cp, URF, DFR, DRB, UBR);
            {
                int tco = n.co[URF];
                n.co[URF] = (n.co[UBR] + 1) % 3;
                n.co[UBR] = (n.co[DRB] + 2) % 3;
                n.co[DRB] = (n.co[DFR] + 1) % 3;
                n.co[DFR] = (tco + 2) % 3;
            }
            cycle4(n.ep, UR, FR, DR, BR);
            cycle4(n.eo, UR, FR, DR, BR);
            break;
        case 4: // F
            cycle4(n.cp, URF, UFL, DLF, DFR);
            {
                int tco = n.co[URF];
                n.co[URF] = (n.co[DFR] + 1) % 3;
                n.co[DFR] = (n.co[DLF] + 2) % 3;
                n.co[DLF] = (n.co[UFL] + 1) % 3;
                n.co[UFL] = (tco + 2) % 3;
            }
            cycle4(n.ep, UF, FL, DF, FR);
            {
                int teo = n.eo[UF];
                n.eo[UF] = (n.eo[FR] + 1) % 2;
                n.eo[FR] = (n.eo[DF] + 1) % 2;
                n.eo[DF] = (n.eo[FL] + 1) % 2;
                n.eo[FL] = (teo + 1) % 2;
            }
            break;
        case 5: // B
            cycle4(n.cp, UBR, DRB, DBL, ULB);
            {
                int tco = n.co[UBR];
                n.co[UBR] = (n.co[ULB] + 1) % 3;
                n.co[ULB] = (n.co[DBL] + 2) % 3;
                n.co[DBL] = (n.co[DRB] + 1) % 3;
                n.co[DRB] = (tco + 2) % 3;
            }
            cycle4(n.ep, UB, BR, DB, BL);
            {
                int teo = n.eo[UB];
                n.eo[UB] = (n.eo[BL] + 1) % 2;
                n.eo[BL] = (n.eo[DB] + 1) % 2;
                n.eo[DB] = (n.eo[BR] + 1) % 2;
                n.eo[BR] = (teo + 1) % 2;
            }
            break;
        }
    }
    return n;
}

bool is_move_allowed(Move last_move, Move curr_move) {
    if (last_move == None) return true;
    
    int last_face = last_move / 3;
    int curr_face = curr_move / 3;

    if (last_face == curr_face) return false;
    
    if (last_face / 2 == curr_face / 2) {
        if (last_face > curr_face) return false;
    }
    
    return true;
}

// =================================================================================================
// --- PARITY VALIDATION (NEW) ---
// =================================================================================================

bool validate_cube(const CubeState &c) {
    int co_sum = 0;
    for (int i = 0; i < 8; i++) co_sum += c.co[i];
    if (co_sum % 3 != 0) return false;
    
    int eo_sum = 0;
    for (int i = 0; i < 12; i++) eo_sum += c.eo[i];
    if (eo_sum % 2 != 0) return false;
    
    auto count_inversions = [](const int* arr, int n) {
        int inv = 0;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (arr[i] > arr[j]) inv++;
        return inv % 2;
    };
    
    if (count_inversions(c.cp, 8) != count_inversions(c.ep, 12))
        return false;
    
    bool cp_seen[8] = {false};
    bool ep_seen[12] = {false};
    for (int i = 0; i < 8; i++) {
        if (c.cp[i] < 0 || c.cp[i] >= 8) return false;
        if (cp_seen[c.cp[i]]) return false;
        cp_seen[c.cp[i]] = true;
    }
    for (int i = 0; i < 12; i++) {
        if (c.ep[i] < 0 || c.ep[i] >= 12) return false;
        if (ep_seen[c.ep[i]]) return false;
        ep_seen[c.ep[i]] = true;
    }
    
    return true;
}

// =================================================================================================
// --- COORDINATES & HELPERS ---
// =================================================================================================

int C(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0) return 1;
    if (k > n / 2) k = n - k;
    long long res = 1;
    for (int i = 1; i <= k; ++i) res = res * (n - i + 1) / i;
    return (int)res;
}

int get_co_coord(const CubeState &s) {
    int coord = 0;
    for (int i = 0; i < 7; i++) coord = coord * 3 + s.co[i];
    return coord;
}

void set_co_coord(CubeState &s, int coord) {
    int parity = 0;
    for (int i = 6; i >= 0; i--) {
        s.co[i] = coord % 3;
        parity += s.co[i];
        coord /= 3;
    }
    s.co[7] = (3 - (parity % 3)) % 3;
}

int get_eo_coord(const CubeState &s) {
    int coord = 0;
    for (int i = 0; i < 11; i++) coord = coord * 2 + s.eo[i];
    return coord;
}

void set_eo_coord(CubeState &s, int coord) {
    int parity = 0;
    for (int i = 10; i >= 0; i--) {
        s.eo[i] = coord % 2;
        parity += s.eo[i];
        coord /= 2;
    }
    s.eo[11] = parity % 2;
}

int get_slice_sorted_coord(const CubeState &s) {
    int k = 4;
    int coord = 0;
    for (int i = 11; i >= 0; i--) {
        if (s.ep[i] >= 8 && s.ep[i] <= 11) {
            coord += C(i, k--);
        }
    }
    return coord;
}

void set_slice_sorted_coord(CubeState &s, int coord) {
    int k = 4;
    int slice_idx = 0;
    int other_idx = 0;
    int slice_edges[] = {8,9,10,11};
    int other_edges[] = {0,1,2,3,4,5,6,7};
    
    for (int i = 11; i >= 0; i--) {
        if (k > 0 && coord >= C(i, k)) {
            coord -= C(i, k--);
            s.ep[i] = slice_edges[slice_idx++];
        } else {
            s.ep[i] = other_edges[other_idx++];
        }
        s.eo[i] = 0;
    }
}

long long factorial[13];
void init_fact() {
    factorial[0] = 1;
    for(int i=1; i<=12; i++) factorial[i] = factorial[i-1]*i;
}

int get_cp_coord(const CubeState &s) {
    int coord = 0;
    for (int i = 0; i < 8; i++) {
        int count = 0;
        for (int j = i + 1; j < 8; j++) {
            if (s.cp[j] < s.cp[i]) count++;
        }
        coord += count * factorial[7 - i];
    }
    return coord;
}

void set_cp_coord(CubeState &s, int coord) {
    vector<int> vals(8);
    iota(vals.begin(), vals.end(), 0);
    for (int i = 0; i < 8; i++) {
        int fact = factorial[7 - i];
        int idx = coord / fact;
        s.cp[i] = vals[idx];
        vals.erase(vals.begin() + idx);
        coord %= fact;
    }
}

int get_ud_ep_coord(const CubeState &s) {
    int coord = 0;
    vector<int> vals;
    for(int i=0; i<8; i++) vals.push_back(s.ep[i]);
    
    for (int i = 0; i < 8; i++) {
        int count = 0;
        for (int j = i + 1; j < 8; j++) {
            if (vals[j] < vals[i]) count++;
        }
        coord += count * factorial[7 - i];
    }
    return coord;
}

void set_ud_ep_coord(CubeState &s, int coord) {
    vector<int> vals(8);
    iota(vals.begin(), vals.end(), 0);
    for (int i = 0; i < 8; i++) {
        int fact = factorial[7 - i];
        int idx = coord / fact;
        s.ep[i] = vals[idx];
        vals.erase(vals.begin() + idx);
        coord %= fact;
    }
    for(int i=8; i<12; i++) s.ep[i] = i;
}

int get_slice_ep_coord(const CubeState &s) {
    int coord = 0;
    vector<int> vals;
    for(int i=8; i<12; i++) vals.push_back(s.ep[i]);
    
    for (int i = 0; i < 4; i++) {
        int count = 0;
        for (int j = i + 1; j < 4; j++) {
            if (vals[j] < vals[i]) count++;
        }
        coord += count * factorial[3 - i];
    }
    return coord;
}

void set_slice_ep_coord(CubeState &s, int coord) {
    vector<int> vals = {8, 9, 10, 11};
    for (int i = 8; i < 12; i++) {
        int fact = factorial[11 - i];
        int idx = coord / fact;
        s.ep[i] = vals[idx];
        vals.erase(vals.begin() + idx);
        coord %= fact;
    }
}

// =================================================================================================
// --- PATTERN DATABASE ---
// =================================================================================================

vector<int> co_pdb(2187, -1);
vector<int> eo_pdb(2048, -1);
vector<int> slice_pdb(495, -1);
vector<int> cp_pdb(40320, -1);
vector<int> ud_ep_pdb(40320, -1);
vector<int> slice_ep_pdb(24, -1);

void save_pdb(const string &filename, const vector<int> &pdb) {
    ofstream out(filename, ios::binary);
    out.write(reinterpret_cast<const char*>(pdb.data()), pdb.size() * sizeof(int));
}

bool load_pdb(const string &filename, vector<int> &pdb) {
    ifstream in(filename, ios::binary);
    if (!in) return false;
    in.read(reinterpret_cast<char*>(pdb.data()), pdb.size() * sizeof(int));
    return in.gcount() == (streamsize)(pdb.size() * sizeof(int));
}

void gen_p1_pdb() {
    queue<int> q;
    q.push(0); co_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = co_pdb[u];
        CubeState s; set_co_coord(s, u);
        for(int m=0; m<18; m++) {
            CubeState ns = applyMove(s, (Move)m);
            int v = get_co_coord(ns);
            if(co_pdb[v] == -1) { co_pdb[v] = dist+1; q.push(v); }
        }
    }
    
    q.push(0); eo_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = eo_pdb[u];
        CubeState s; set_eo_coord(s, u);
        for(int m=0; m<18; m++) {
            CubeState ns = applyMove(s, (Move)m);
            int v = get_eo_coord(ns);
            if(eo_pdb[v] == -1) { eo_pdb[v] = dist+1; q.push(v); }
        }
    }

    q.push(0); slice_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = slice_pdb[u];
        CubeState s; set_slice_sorted_coord(s, u);
        for(int m=0; m<18; m++) {
            CubeState ns = applyMove(s, (Move)m);
            int v = get_slice_sorted_coord(ns);
            if(slice_pdb[v] == -1) { slice_pdb[v] = dist+1; q.push(v); }
        }
    }
}

vector<Move> p2_moves = {Ux1, Ux2, Ux3, Dx1, Dx2, Dx3, Lx2, Rx2, Fx2, Bx2};

void gen_p2_pdb() {
    queue<int> q;
    
    q.push(0); cp_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = cp_pdb[u];
        CubeState s; set_cp_coord(s, u);
        for(Move m : p2_moves) {
            CubeState ns = applyMove(s, m);
            int v = get_cp_coord(ns);
            if(cp_pdb[v] == -1) { cp_pdb[v] = dist+1; q.push(v); }
        }
    }

    q.push(0); ud_ep_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = ud_ep_pdb[u];
        CubeState s; set_ud_ep_coord(s, u);
        for(Move m : p2_moves) {
            CubeState ns = applyMove(s, m);
            int v = get_ud_ep_coord(ns);
            if(ud_ep_pdb[v] == -1) { ud_ep_pdb[v] = dist+1; q.push(v); }
        }
    }

    q.push(0); slice_ep_pdb[0] = 0;
    while(!q.empty()){
        int u = q.front(); q.pop();
        int dist = slice_ep_pdb[u];
        CubeState s; set_slice_ep_coord(s, u);
        for(Move m : p2_moves) {
            CubeState ns = applyMove(s, m);
            int v = get_slice_ep_coord(ns);
            if(slice_ep_pdb[v] == -1) { slice_ep_pdb[v] = dist+1; q.push(v); }
        }
    }
}

// =================================================================================================
// --- SEARCH ---
// =================================================================================================

int h_p1(const CubeState &s) {
    return max({co_pdb[get_co_coord(s)], eo_pdb[get_eo_coord(s)], slice_pdb[get_slice_sorted_coord(s)]});
}

bool solve_p1(CubeState s, int g, int threshold, vector<Move>& path, Move lastMove) {
    int h = h_p1(s);
    if (h == 0) return true;
    if (g + h > threshold) return false;

    for (int i = 0; i < 18; i++) {
        Move m = (Move)i;
        if (is_move_allowed(lastMove, m)) {
            path.push_back(m);
            if (solve_p1(applyMove(s, m), g + 1, threshold, path, m)) return true;
            path.pop_back();
        }
    }
    return false;
}

int h_p2(const CubeState &s) {
    return max({cp_pdb[get_cp_coord(s)], ud_ep_pdb[get_ud_ep_coord(s)], slice_ep_pdb[get_slice_ep_coord(s)]});
}

bool solve_p2(CubeState s, int g, int threshold, vector<Move>& path, Move lastMove) {
    int h = h_p2(s);
    if (h == 0) return true;
    if (g + h > threshold) return false;

    for (Move m : p2_moves) {
        if (is_move_allowed(lastMove, m)) {
            path.push_back(m);
            if (solve_p2(applyMove(s, m), g + 1, threshold, path, m)) return true;
            path.pop_back();
        }
    }
    return false;
}

// =================================================================================================
// --- SOLVED STATE CHECK ---
// =================================================================================================

bool is_solved(const CubeState &s) {
    for (int i = 0; i < 8; i++) {
        if (s.cp[i] != i || s.co[i] != 0) return false;
    }
    for (int i = 0; i < 12; i++) {
        if (s.ep[i] != i || s.eo[i] != 0) return false;
    }
    return true;
}

// =================================================================================================
// --- PARSING ---
// =================================================================================================

bool parse_facelets(string f, CubeState &c) {
    if (f.size() != 54) return false;

    for(int i=0; i<8; i++) {
        int ori = -1;
        for(int o=0; o<3; o++) {
            char col = f[cornerFacelet[i][o]];
            if(col == 'U' || col == 'D') { ori = o; break; }
        }
        if(ori == -1) return false;
        c.co[i] = ori;

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
            if(match == 3) { c.cp[i] = target; found = true; break; }
        }
        if (!found) return false;
    }

    for(int i=0; i<12; i++) {
        char c1 = f[edgeFacelet[i][0]];
        char c2 = f[edgeFacelet[i][1]];
        
        bool found = false;
        for(int target=0; target<12; target++) {
            if(c1 == edgeColor[target][0] && c2 == edgeColor[target][1]) {
                c.ep[i] = target; c.eo[i] = 0; found = true; break;
            }
            if(c1 == edgeColor[target][1] && c2 == edgeColor[target][0]) {
                c.ep[i] = target; c.eo[i] = 1; found = true; break;
            }
        }
        if (!found) return false;
    }
    return true;
}

// =================================================================================================
// --- SOLVER API ---
// =================================================================================================

string pdb_path = "./pdb";
bool initialized = false;

void initialize_solver(const string& path) {
    if (initialized) return;
    pdb_path = path;
    
    init_fact();

    bool gen = false;
    if(!load_pdb(pdb_path + "/co.pdb", co_pdb)) gen=true;
    if(!load_pdb(pdb_path + "/eo.pdb", eo_pdb)) gen=true;
    if(!load_pdb(pdb_path + "/slice.pdb", slice_pdb)) gen=true;
    if(!load_pdb(pdb_path + "/cp.pdb", cp_pdb)) gen=true;
    if(!load_pdb(pdb_path + "/ud.pdb", ud_ep_pdb)) gen=true;
    if(!load_pdb(pdb_path + "/sep.pdb", slice_ep_pdb)) gen=true;

    if(gen) {
        gen_p1_pdb();
        save_pdb(pdb_path + "/co.pdb", co_pdb);
        save_pdb(pdb_path + "/eo.pdb", eo_pdb);
        save_pdb(pdb_path + "/slice.pdb", slice_pdb);

        gen_p2_pdb();
        save_pdb(pdb_path + "/cp.pdb", cp_pdb);
        save_pdb(pdb_path + "/ud.pdb", ud_ep_pdb);
        save_pdb(pdb_path + "/sep.pdb", slice_ep_pdb);
    }
    
    initialized = true;
}

string solve(const string& facelet_string) {
    if (!initialized) {
        return "ERROR: Solver not initialized";
    }
    
    if (facelet_string.length() != 54) {
        return "ERROR: Invalid input length";
    }
    
    CubeState start_state;
    if(!parse_facelets(facelet_string, start_state)) {
        return "ERROR: Invalid cube configuration";
    }
    
    if (!validate_cube(start_state)) {
        return "ERROR: Impossible cube state";
    }

    // Check if already solved
    if (is_solved(start_state)) {
        return "";  // Empty solution for solved cube
    }

    vector<Move> p1_sol;
    CubeState p1_end = start_state;
    
    int threshold = h_p1(start_state);
    while(true) {
        if(solve_p1(start_state, 0, threshold, p1_sol, None)) break;
        threshold++;
        if (threshold > 12) return "ERROR: Phase 1 exceeded depth limit";
    }

    for(Move m : p1_sol) {
        p1_end = applyMove(p1_end, m);
    }

    vector<Move> p2_sol;
    Move last_p1 = (p1_sol.empty() ? None : p1_sol.back());
    
    threshold = h_p2(p1_end);
    while(true) {
        if(solve_p2(p1_end, 0, threshold, p2_sol, last_p1)) break;
        threshold++;
        if (threshold > 18) return "ERROR: Phase 2 exceeded depth limit";
    }

    ostringstream result;
    for(size_t i = 0; i < p1_sol.size(); i++) {
        if(i > 0) result << " ";
        result << move_strings[p1_sol[i]];
    }
    for(size_t i = 0; i < p2_sol.size(); i++) {
        if(i > 0 || !p1_sol.empty()) result << " ";
        result << move_strings[p2_sol[i]];
    }
    
    return result.str();
}

#ifndef PYBIND11_BUILD
int main() {
    initialize_solver("./pdb");
    
    string input;
    cout << "Enter cube (54 chars, URFDLB order):" << endl;
    if (!(cin >> input)) return 0;

    string result = solve(input);
    cout << "Result: " << result << endl;

    return 0;
}
#endif
