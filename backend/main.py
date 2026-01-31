from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
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

# Constants
SOLVED_CUBE = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"

class SolveRequest(BaseModel):
    facelets: str

class SolveResponse(BaseModel):
    solution: str = ""
    move_count: int = 0
    success: bool = False
    error: str = ""

@app.get("/api/health")
async def health_check():
    """Health check endpoint"""
    return {"status": "healthy", "solver": "kociemba"}

@app.post("/api/solve", response_model=SolveResponse)
async def solve_cube(request: SolveRequest):
    """
    Solve a Rubik's Cube from a facelet string.
    
    Args:
        request: SolveRequest with facelets string (54 chars, URFDLB format)
    
    Returns:
        SolveResponse with solution moves or error
    """
    facelets = request.facelets.strip()
    
    # Validate length
    if len(facelets) != 54:
        return SolveResponse(
            success=False,
            error="Invalid input: must be exactly 54 characters"
        )
    
    # Validate characters
    valid_colors = set("URFDLB")
    if not all(c in valid_colors for c in facelets):
        return SolveResponse(
            success=False,
            error="Invalid input: only U, R, F, D, L, B characters allowed"
        )
    
    # Validate color counts (each color should appear exactly 9 times)
    color_counts = {}
    for c in facelets:
        color_counts[c] = color_counts.get(c, 0) + 1
    
    if any(count != 9 for count in color_counts.values()):
        return SolveResponse(
            success=False,
            error="Invalid input: each color must appear exactly 9 times"
        )
    
    # Check if already solved
    if facelets == SOLVED_CUBE:
        return SolveResponse(
            solution="",
            move_count=0,
            success=True
        )
    
    # Use kociemba library to solve
    try:
        solution = kociemba.solve(facelets)
        
        # Count moves
        moves = solution.split() if solution else []
        
        return SolveResponse(
            solution=solution,
            move_count=len(moves),
            success=True
        )
            
    except ValueError:
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
