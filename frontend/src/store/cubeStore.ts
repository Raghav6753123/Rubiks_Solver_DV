import { create } from 'zustand';

// Color mapping for each face
export const FACE_COLORS: Record<string, string> = {
  U: '#FFFFFF', // White
  R: '#FF0000', // Red
  F: '#00FF00', // Green
  D: '#FFFF00', // Yellow
  L: '#FFA500', // Orange
  B: '#0000FF', // Blue
};

// Solved cube state
export const SOLVED_CUBE = 'UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB';

export interface CubeState {
  // Cube state (54 facelets)
  facelets: string;
  
  // Selected color for painting
  selectedColor: string;
  
  // Solution
  solution: string[];
  solutionString: string;
  moveCount: number;
  
  // Playback
  currentMoveIndex: number;
  isPlaying: boolean;
  playbackSpeed: number;
  
  // UI state
  isSolving: boolean;
  error: string;
  
  // Actions
  setFacelet: (index: number, color: string) => void;
  setSelectedColor: (color: string) => void;
  resetCube: () => void;
  solveCube: () => Promise<void>;
  setCurrentMoveIndex: (index: number) => void;
  setIsPlaying: (playing: boolean) => void;
  setPlaybackSpeed: (speed: number) => void;
  nextMove: () => void;
  prevMove: () => void;
}

export const useCubeStore = create<CubeState>((set, get) => ({
  // Initial state
  facelets: SOLVED_CUBE,
  selectedColor: 'U',
  solution: [],
  solutionString: '',
  moveCount: 0,
  currentMoveIndex: 0,
  isPlaying: false,
  playbackSpeed: 1,
  isSolving: false,
  error: '',
  
  // Set a facelet color
  setFacelet: (index: number, color: string) => {
    const facelets = get().facelets.split('');
    facelets[index] = color;
    set({ 
      facelets: facelets.join(''),
      solution: [],
      solutionString: '',
      moveCount: 0,
      currentMoveIndex: 0,
      error: '',
    });
  },
  
  // Set selected color
  setSelectedColor: (color: string) => set({ selectedColor: color }),
  
  // Reset to solved state
  resetCube: () => set({
    facelets: SOLVED_CUBE,
    solution: [],
    solutionString: '',
    moveCount: 0,
    currentMoveIndex: 0,
    error: '',
    isPlaying: false,
  }),
  
  // Solve the cube
  solveCube: async () => {
    const { facelets } = get();
    set({ isSolving: true, error: '' });
    
    try {
      const response = await fetch('/api/solve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ facelets }),
      });
      
      const data = await response.json();
      
      if (data.success) {
        const moves = data.solution ? data.solution.split(' ') : [];
        set({
          solution: moves,
          solutionString: data.solution || 'Already solved!',
          moveCount: data.move_count,
          currentMoveIndex: 0,
          isSolving: false,
          error: '',
        });
      } else {
        set({
          error: data.error,
          solution: [],
          solutionString: '',
          moveCount: 0,
          isSolving: false,
        });
      }
    } catch (error) {
      set({
        error: 'Failed to connect to solver',
        solution: [],
        solutionString: '',
        moveCount: 0,
        isSolving: false,
      });
    }
  },
  
  // Playback controls
  setCurrentMoveIndex: (index: number) => set({ currentMoveIndex: index }),
  setIsPlaying: (playing: boolean) => set({ isPlaying: playing }),
  setPlaybackSpeed: (speed: number) => set({ playbackSpeed: speed }),
  
  nextMove: () => {
    const { currentMoveIndex, solution } = get();
    if (currentMoveIndex < solution.length) {
      set({ currentMoveIndex: currentMoveIndex + 1 });
    }
  },
  
  prevMove: () => {
    const { currentMoveIndex } = get();
    if (currentMoveIndex > 0) {
      set({ currentMoveIndex: currentMoveIndex - 1 });
    }
  },
}));
