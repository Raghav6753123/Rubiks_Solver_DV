# Rubik's Cube Solver with 3D Visualization

A complete web-based Rubik's Cube Solver featuring a React + TypeScript frontend with 3D visualization using React Three Fiber, and a Python FastAPI backend powered by the Kociemba two-phase algorithm implemented in C++.

## Features

- **Interactive 2D Input**: Paint colors on a 2D net representation of the cube
- **Real-time 3D Visualization**: High-quality 3D rendering with React Three Fiber
- **Kociemba Solver**: Fast optimal solving using the two-phase algorithm
- **Playback Controls**: Step through solutions with play/pause, next/previous, and speed control
- **Solution Display**: View the complete move sequence with current move highlighting

## Architecture

- **Frontend**: React + TypeScript + Vite + React Three Fiber + Zustand
- **Backend**: Python FastAPI + C++ Kociemba solver + pybind11
- **Solver**: Optimized C++ implementation with pattern databases

## Project Structure

```
Rubiks_Solver_DV/
├── README.md
├── backend/
│   ├── solver.cpp           # Kociemba algorithm implementation
│   ├── solver_wrapper.cpp   # pybind11 bindings
│   ├── main.py              # FastAPI server
│   ├── requirements.txt
│   ├── CMakeLists.txt
│   ├── Dockerfile
│   └── pdb/                 # Pattern databases (auto-generated)
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
   cmake .
   make
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
- `GET /api/health`: Health check endpoint

## Docker Deployment

```bash
cd backend
docker build -t rubiks-solver-backend .
docker run -p 8000:8000 rubiks-solver-backend
```

## License

MIT License