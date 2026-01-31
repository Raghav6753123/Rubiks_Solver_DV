# Rubik's Cube Solver with 3D Visualization

A complete web-based Rubik's Cube Solver featuring a React + TypeScript frontend with 3D visualization using React Three Fiber, and a Python FastAPI backend powered by the proven kociemba library.

## Features

- **Interactive 2D Input**: Paint colors on a 2D net representation of the cube
- **Real-time 3D Visualization**: High-quality 3D rendering with React Three Fiber
- **Kociemba Solver**: Fast, correct solving using the proven muodov/kociemba library
- **Playback Controls**: Step through solutions with play/pause, next/previous, and speed control
- **Solution Display**: View the complete move sequence with current move highlighting
- **Validation**: Proper rejection of impossible cube states (parity errors, invalid configurations)

## Architecture

- **Frontend**: React + TypeScript + Vite + React Three Fiber + Zustand
- **Backend**: Python FastAPI + kociemba library
- **Solver**: Kociemba two-phase algorithm (muodov/kociemba)

## Project Structure

```
Rubiks_Solver_DV/
├── README.md
├── IMPLEMENTATION_SUMMARY.md
├── backend/
│   ├── solver.cpp           # Reference C++ implementation (not used)
│   ├── main.py              # FastAPI server with kociemba solver
│   ├── requirements.txt     # Python dependencies including kociemba
│   └── Dockerfile
├── frontend/
│   ├── src/
│   │   ├── components/      # React components
│   │   ├── store/           # State management
│   │   ├── utils/           # Helper functions
│   │   ├── App.tsx
│   │   └── main.tsx
│   ├── package.json
│   ├── vite.config.ts
│   └── tsconfig.json
└── .gitignore
```

## Setup Instructions

### Backend Setup

1. **Prerequisites**:
   - Python 3.9+
   - C++ compiler (g++ or clang++)
   - CMake 3.15+
   - pybind11

2. **Build and Run**:
   ```bash
   cd backend
   pip install -r requirements.txt
   python main.py
   ```

   The backend will start on `http://localhost:8000`

   **Note**: No C++ compilation required! The solver uses the pure Python kociemba library.

### Frontend Setup

1. **Prerequisites**:
   - Node.js 18+
   - npm or yarn

2. **Install and Run**:
   ```bash
   cd frontend
   npm install
   npm run dev
   ```

   The frontend will start on `http://localhost:5173`

## Usage

1. Open the frontend in your browser
2. Click on the 2D cube net to paint colors using the color picker
3. Ensure each face has the correct colors
4. Click "Calculate Solution" to solve the cube
5. Use playback controls to step through the solution
6. Watch the 3D cube animate the solution

## Color Mapping

- **U (Up)**: White (#FFFFFF)
- **R (Right)**: Red (#FF0000)
- **F (Front)**: Green (#00FF00)
- **D (Down)**: Yellow (#FFFF00)
- **L (Left)**: Orange (#FFA500)
- **B (Back)**: Blue (#0000FF)

## API Endpoints

- `POST /api/solve`: Solve cube from facelet string
  - Input: `{facelets: "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"}`
  - Output: `{solution: "R U R' U'", move_count: 4, success: true}`
  - Returns empty solution for already-solved cubes
  - Returns error for impossible cube configurations
- `GET /api/health`: Health check endpoint
  - Returns: `{status: "healthy", solver: "kociemba-python"}`

## Solver Implementation

This project uses the **muodov/kociemba** Python library, a proven implementation of Herbert Kociemba's two-phase algorithm.

### Why kociemba Library?

- ✅ **Proven Correct**: 1M+ downloads, extensively tested in production
- ✅ **Fast**: Solves any valid cube in <1 second  
- ✅ **Reliable**: Proper validation of cube states (parity checking)
- ✅ **Maintained**: Active development with regular updates
- ✅ **Simple**: No compilation required, pure Python

### Test Results

All reference test cases pass:
- ✅ Solved cube returns empty solution (0 moves)
- ✅ Single R move returns R' (1 move)
- ✅ Single U move returns U' (1 move)  
- ✅ Complex scrambles solved in ≤21 moves
- ✅ Invalid states properly rejected

### References

- Original algorithm: http://kociemba.org/cube.htm
- Python library: https://github.com/muodov/kociemba
- Java reference: https://github.com/hkociemba/RubiksCube-TwophaseSolver

## Docker Deployment

```bash
cd backend
docker build -t rubiks-solver-backend .
docker run -p 8000:8000 rubiks-solver-backend
```

## License

MIT License