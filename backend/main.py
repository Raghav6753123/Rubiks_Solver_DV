from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import subprocess
import os
import sys
import kociemba

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

@app.get("/api/health")
async def health_check():
    return {"status": "healthy", "solver": "kociemba-python"}

@app.post("/api/solve", response_model=SolveResponse)
async def solve_cube(request: SolveRequest):
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
    
    # Use kociemba library to solve
    try:
        # kociemba.solve() returns solution string or raises ValueError for invalid cubes
        solution = kociemba.solve(facelets)
        
        # Handle the known issue where kociemba returns garbage for solved cube
        # Check if already solved by trying to parse as moves
        is_already_solved = facelets == "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"
        if is_already_solved:
            solution = ""
        
        # Count moves
        moves = solution.split() if solution else []
        
        return SolveResponse(
            solution=solution,
            move_count=len(moves),
            success=True
        )
            
    except ValueError as e:
        # kociemba raises ValueError for impossible cube states
        return SolveResponse(
            success=False,
            error="Invalid cube: impossible configuration (check corner/edge parity)"
        )
    except Exception as e:
        return SolveResponse(
            success=False,
            error=f"Solver error: {str(e)}"
        )

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
