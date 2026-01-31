# Rubik's Cube Solver with 3D Visualization

A complete web-based Rubik's Cube Solver featuring a React + TypeScript frontend with 3D visualization using React Three Fiber, and a Python FastAPI backend powered by the proven kociemba library.

## Features

- **Interactive 2D Input**: Paint colors on a 2D net representation of the cube
- **Real-time 3D Visualization**: High-quality 3D rendering with React Three Fiber
- **Kociemba Solver**: Fast, correct solving using the muodov/kociemba library
- **Playback Controls**: Step through solutions with play/pause, next/previous, and speed control
- **Solution Display**: View the complete move sequence with current move highlighting
- **Validation**: Proper rejection of impossible cube states (parity errors, invalid configurations)

## Architecture

- **Frontend**: React + TypeScript + Vite + React Three Fiber + Zustand
- **Backend**: Python FastAPI + kociemba library
- **Solver**: Kociemba two-phase algorithm

## Project Structure

```
Rubiks_Solver_DV/
├── README.md
├── backend/
│   ├── main.py              # FastAPI server with kociemba solver
│   ├── requirements.txt     # Python dependencies
│   └── Dockerfile
├── frontend/
│   ├── src/
│   │   ├── components/      # React components
│   │   │   ├── ColorPicker.tsx
│   │   │   ├── CubeNet.tsx
│   │   │   ├── Cube3D.tsx
│   │   │   ├── Controls.tsx
│   │   │   └── SolutionPanel.tsx
│   │   ├── store/           # Zustand state management
│   │   │   └── cubeStore.ts
│   │   ├── utils/           # Helper functions
│   │   │   └── cubeUtils.ts
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

2. **Install and Run**:
   ```bash
   cd backend
   pip install -r requirements.txt
   python main.py
   ```

   The backend will start on `http://localhost:8000`

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

1. Open the frontend in your browser at `http://localhost:5173`
2. Click on the 2D cube net to paint colors using the color picker
3. Ensure each face has the correct colors (9 of each color)
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

### POST /api/solve
Solve cube from facelet string

**Request:**
```json
{
  "facelets": "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"
}
```

**Response (Success):**
```json
{
  "solution": "R U R' U'",
  "move_count": 4,
  "success": true,
  "error": ""
}
```

**Response (Already Solved):**
```json
{
  "solution": "",
  "move_count": 0,
  "success": true,
  "error": ""
}
```

**Response (Error):**
```json
{
  "solution": "",
  "move_count": 0,
  "success": false,
  "error": "Invalid cube: impossible configuration"
}
```

### GET /api/health
Health check endpoint

**Response:**
```json
{
  "status": "healthy",
  "solver": "kociemba"
}
```

## Solver Implementation

This project uses the **muodov/kociemba** Python library, a proven implementation of Herbert Kociemba's two-phase algorithm.

### Why kociemba Library?

- ✅ **Proven Correct**: 1M+ downloads, extensively tested in production
- ✅ **Fast**: Solves any valid cube in <1 second  
- ✅ **Reliable**: Proper validation of cube states (parity checking)
- ✅ **Maintained**: Active development with regular updates
- ✅ **Simple**: No compilation required, pure Python

### Test Cases

The solver must pass these key test cases:

1. **Solved State**: Returns empty solution (0 moves)
2. **Single Move**: Returns inverse move (e.g., R → R')
3. **Scrambled State**: Returns valid solution that solves the cube
4. **Invalid State**: Rejects impossible configurations

### References

- Original algorithm: http://kociemba.org/cube.htm
- Python library: https://github.com/muodov/kociemba

## Docker Deployment

```bash
cd backend
docker build -t rubiks-solver-backend .
docker run -p 8000:8000 rubiks-solver-backend
```

## Development Notes

### Facelet Mapping

The cube uses standard URFDLB notation with 54 facelets (0-53):

```
         U (0-8)
    L (36-44) F (18-26) R (9-17) B (45-53)
         D (27-35)
```

Each face is numbered left-to-right, top-to-bottom:
```
0 1 2
3 4 5
6 7 8
```

### Move Notation

- R, L, U, D, F, B: 90° clockwise
- R', L', U', D', F', B': 90° counter-clockwise  
- R2, L2, U2, D2, F2, B2: 180° rotation

## Future Enhancements

Once the library-based solver is working correctly, the plan is to:

1. ✅ Verify all test cases pass
2. ✅ Ensure 3D animation matches solution precisely
3. ✅ Confirm invalid cubes are properly rejected
4. 🔄 **Next**: Replace kociemba library with self-coded backend solver

## License

MIT License
