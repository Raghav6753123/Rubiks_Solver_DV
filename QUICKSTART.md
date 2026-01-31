# Quick Start Guide

## Prerequisites

- Python 3.9 or higher
- Node.js 18 or higher
- npm or yarn

## Setup (5 minutes)

### 1. Start the Backend

```bash
cd backend
pip install -r requirements.txt
python main.py
```

You should see:
```
INFO:     Uvicorn running on http://0.0.0.0:8000 (Press CTRL+C to quit)
```

### 2. Start the Frontend (in a new terminal)

```bash
cd frontend
npm install
npm run dev
```

You should see:
```
VITE v5.x.x  ready in XXXms
➜  Local:   http://localhost:5173/
```

### 3. Open in Browser

Navigate to: http://localhost:5173

## Usage

1. **Select a Color**: Click one of the 6 colored buttons
2. **Paint Facelets**: Click on any square in the 2D cube net to paint it
3. **Solve**: Click "Calculate Solution" button
4. **Playback**: Use the playback controls to animate the solution

## Test the Solver

### Quick Test with curl

```bash
# Test health endpoint
curl http://localhost:8000/api/health

# Test solved cube (should return 0 moves)
curl -X POST http://localhost:8000/api/solve \
  -H "Content-Type: application/json" \
  -d '{"facelets": "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB"}'

# Test scrambled cube
curl -X POST http://localhost:8000/api/solve \
  -H "Content-Type: application/json" \
  -d '{"facelets": "BBURUDBFUFFFRRFUUFLULUFUDLRRDBBDBDBLUDDFLLRRBRLLLBRDDF"}'
```

## Color Mapping

- White (U) = #FFFFFF
- Red (R) = #FF0000
- Green (F) = #00FF00
- Yellow (D) = #FFFF00
- Orange (L) = #FFA500
- Blue (B) = #0000FF

## Troubleshooting

**Backend won't start:**
- Make sure Python 3.9+ is installed
- Try: `pip install --upgrade pip` then reinstall dependencies

**Frontend won't start:**
- Make sure Node.js 18+ is installed
- Delete `node_modules` and `package-lock.json`, then run `npm install` again

**"Failed to connect to solver":**
- Make sure the backend is running on port 8000
- Check firewall settings

**3D visualization not working:**
- Make sure WebGL is enabled in your browser
- Try a different browser (Chrome/Firefox recommended)

## Docker Deployment (Optional)

```bash
cd backend
docker build -t rubiks-solver-backend .
docker run -p 8000:8000 rubiks-solver-backend
```

## Next Steps

- Try scrambling the cube in the UI
- Test the playback controls
- Experiment with different scrambles
- Read IMPLEMENTATION_SUMMARY.md for technical details
