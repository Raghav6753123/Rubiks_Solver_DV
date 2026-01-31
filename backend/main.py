from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import subprocess
import os
import sys

app = FastAPI(title="Rubik's Cube Solver API")

# CORS configuration for frontend
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class SolveRequest(BaseModel):
    facelets: str

class SolveResponse(BaseModel):
    solution: str = ""
    move_count: int = 0
    success: bool = False
    error: str = ""

# Build solver executable on startup
def build_solver():
    backend_dir = os.path.dirname(os.path.abspath(__file__))
    solver_path = os.path.join(backend_dir, "solver")
    cpp_path = os.path.join(backend_dir, "solver.cpp")
    
    # Check if solver exists and is up to date
    if os.path.exists(solver_path):
        return solver_path
    
    # Compile solver
    print("Compiling solver...")
    try:
        result = subprocess.run(
            ["g++", "-O3", "-std=c++17", "-o", solver_path, cpp_path],
            capture_output=True,
            text=True,
            check=True
        )
        print("Solver compiled successfully")
        return solver_path
    except subprocess.CalledProcessError as e:
        print(f"Failed to compile solver: {e.stderr}")
        raise

SOLVER_PATH = None

@app.on_event("startup")
async def startup_event():
    global SOLVER_PATH
    SOLVER_PATH = build_solver()
    print(f"Solver ready at: {SOLVER_PATH}")

@app.get("/api/health")
async def health_check():
    return {"status": "healthy", "solver_available": SOLVER_PATH is not None}

@app.post("/api/solve", response_model=SolveResponse)
async def solve_cube(request: SolveRequest):
    if not SOLVER_PATH:
        raise HTTPException(status_code=500, detail="Solver not initialized")
    
    facelets = request.facelets.strip()
    
    # Validate input
    if len(facelets) != 54:
        return SolveResponse(
            success=False,
            error="Invalid input: must be exactly 54 characters"
        )
    
    # Check that we have the right number of each color
    valid_colors = set("URFDLB")
    if not all(c in valid_colors for c in facelets):
        return SolveResponse(
            success=False,
            error="Invalid input: only U, R, F, D, L, B characters allowed"
        )
    
    # Check color counts (each color should appear 9 times)
    color_counts = {}
    for c in facelets:
        color_counts[c] = color_counts.get(c, 0) + 1
    
    if any(count != 9 for count in color_counts.values()):
        return SolveResponse(
            success=False,
            error="Invalid input: each color must appear exactly 9 times"
        )
    
    # Call C++ solver
    try:
        result = subprocess.run(
            [SOLVER_PATH],
            input=facelets + "\n",
            capture_output=True,
            text=True,
            timeout=30
        )
        
        output = result.stdout.strip()
        
        # Parse output
        if "Result:" in output:
            solution = output.split("Result:")[1].strip()
            
            if solution.startswith("ERROR:"):
                return SolveResponse(
                    success=False,
                    error=solution
                )
            
            # Count moves
            moves = solution.split() if solution else []
            
            return SolveResponse(
                solution=solution,
                move_count=len(moves),
                success=True
            )
        else:
            return SolveResponse(
                success=False,
                error="Unexpected solver output"
            )
            
    except subprocess.TimeoutExpired:
        return SolveResponse(
            success=False,
            error="Solver timeout (>30 seconds)"
        )
    except Exception as e:
        return SolveResponse(
            success=False,
            error=f"Solver error: {str(e)}"
        )

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
