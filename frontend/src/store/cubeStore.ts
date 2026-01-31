import { create } from 'zustand';
import { applyMoveToFacelets } from '../utils/cubeUtils';

export type Color = 'U' | 'R' | 'F' | 'D' | 'L' | 'B';

export interface Move {
  notation: string;
  face: string;
  turns: number;
}

interface CubeStore {
  // Initial cube state (54 facelets)
  facelets: string[];
  
  // Display facelets (with moves applied)
  displayFacelets: string[];
  
  // Selected color for painting
  selectedColor: Color;
  
  // Solution state
  solution: Move[];
  solutionString: string;
  isLoading: boolean;
  error: string | null;
  
  // Playback state
  currentMoveIndex: number;
  isPlaying: boolean;
  playbackSpeed: number; // milliseconds per move
  
  // Actions
  setFacelet: (index: number, color: Color) => void;
  setSelectedColor: (color: Color) => void;
  resetCube: () => void;
  solveCube: () => Promise<void>;
  setCurrentMove: (index: number) => void;
  nextMove: () => void;
  prevMove: () => void;
  togglePlayback: () => void;
  setPlaybackSpeed: (speed: number) => void;
  resetPlayback: () => void;
}

// Solved cube state (URFDLB order)
const SOLVED_STATE = 
  'UUUUUUUUU' + // U face (white)
  'RRRRRRRRR' + // R face (red)
  'FFFFFFFFF' + // F face (green)
  'DDDDDDDDD' + // D face (yellow)
  'LLLLLLLLL' + // L face (orange)
  'BBBBBBBBB';  // B face (blue)

export const useCubeStore = create<CubeStore>((set, get) => ({
  facelets: SOLVED_STATE.split(''),
  displayFacelets: SOLVED_STATE.split(''),
  selectedColor: 'U',
  solution: [],
  solutionString: '',
  isLoading: false,
  error: null,
  currentMoveIndex: -1,
  isPlaying: false,
  playbackSpeed: 500,

  setFacelet: (index: number, color: Color) => {
    const newFacelets = [...get().facelets];
    newFacelets[index] = color;
    set({ 
      facelets: newFacelets,
      displayFacelets: newFacelets,
      solution: [],
      solutionString: '',
      currentMoveIndex: -1,
      error: null,
    });
  },

  setSelectedColor: (color: Color) => {
    set({ selectedColor: color });
  },

  resetCube: () => {
    set({
      facelets: SOLVED_STATE.split(''),
      displayFacelets: SOLVED_STATE.split(''),
      solution: [],
      solutionString: '',
      currentMoveIndex: -1,
      isPlaying: false,
      error: null,
    });
  },

  solveCube: async () => {
    const { facelets } = get();
    set({ isLoading: true, error: null });

    try {
      const response = await fetch('http://localhost:8000/api/solve', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ facelets: facelets.join('') }),
      });

      const data = await response.json();

      if (!data.success) {
        set({ error: data.error || 'Failed to solve cube', isLoading: false });
        return;
      }

      // Parse solution string
      const moves = parseSolution(data.solution);
      
      set({
        solution: moves,
        solutionString: data.solution,
        isLoading: false,
        currentMoveIndex: -1,
        displayFacelets: [...facelets],
      });
    } catch (err) {
      set({ 
        error: err instanceof Error ? err.message : 'Network error',
        isLoading: false 
      });
    }
  },

  setCurrentMove: (index: number) => {
    const { facelets, solution } = get();
    const clampedIndex = Math.max(-1, Math.min(index, solution.length - 1));
    
    // Apply moves up to the current index
    let displayFacelets = [...facelets];
    for (let i = 0; i <= clampedIndex; i++) {
      displayFacelets = applyMoveToFacelets(displayFacelets, solution[i].notation);
    }
    
    set({ currentMoveIndex: clampedIndex, displayFacelets });
  },

  nextMove: () => {
    const { currentMoveIndex, solution } = get();
    if (currentMoveIndex < solution.length - 1) {
      get().setCurrentMove(currentMoveIndex + 1);
    }
  },

  prevMove: () => {
    const { currentMoveIndex } = get();
    if (currentMoveIndex >= 0) {
      get().setCurrentMove(currentMoveIndex - 1);
    }
  },

  togglePlayback: () => {
    const { isPlaying } = get();
    set({ isPlaying: !isPlaying });
  },

  setPlaybackSpeed: (speed: number) => {
    set({ playbackSpeed: speed });
  },

  resetPlayback: () => {
    const { facelets } = get();
    set({ 
      currentMoveIndex: -1, 
      isPlaying: false,
      displayFacelets: [...facelets],
    });
  },
}));

// Helper function to parse solution string
function parseSolution(solutionStr: string): Move[] {
  if (!solutionStr) return [];
  
  return solutionStr.split(' ').map(notation => {
    const face = notation[0];
    const modifier = notation.slice(1);
    let turns = 1;
    
    if (modifier === '2') turns = 2;
    else if (modifier === "'") turns = 3;
    
    return { notation, face, turns };
  });
}
