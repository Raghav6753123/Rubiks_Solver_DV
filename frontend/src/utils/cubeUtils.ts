// Apply a single move notation to a facelets array
export function applyMoveToFacelets(facelets: string[], notation: string): string[] {
  const newFacelets = [...facelets];
  
  const face = notation[0];
  const modifier = notation.slice(1);
  let turns = 1;
  
  if (modifier === '2') turns = 2;
  else if (modifier === "'") turns = 3;
  
  for (let i = 0; i < turns; i++) {
    applySingleTurn(newFacelets, face);
  }
  
  return newFacelets;
}

// Apply a single 90-degree clockwise turn
function applySingleTurn(facelets: string[], face: string): void {
  switch (face) {
    case 'U':
      applyU(facelets);
      break;
    case 'D':
      applyD(facelets);
      break;
    case 'L':
      applyL(facelets);
      break;
    case 'R':
      applyR(facelets);
      break;
    case 'F':
      applyF(facelets);
      break;
    case 'B':
      applyB(facelets);
      break;
  }
}

// Rotate a face clockwise
function rotateFace(facelets: string[], start: number): void {
  const temp = [
    facelets[start], facelets[start + 1], facelets[start + 2],
    facelets[start + 3], facelets[start + 4], facelets[start + 5],
    facelets[start + 6], facelets[start + 7], facelets[start + 8]
  ];
  
  facelets[start] = temp[6];
  facelets[start + 1] = temp[3];
  facelets[start + 2] = temp[0];
  facelets[start + 3] = temp[7];
  facelets[start + 4] = temp[4];
  facelets[start + 5] = temp[1];
  facelets[start + 6] = temp[8];
  facelets[start + 7] = temp[5];
  facelets[start + 8] = temp[2];
}

// Cycle 4 elements
function cycle4(facelets: string[], i1: number, i2: number, i3: number, i4: number): void {
  const temp = facelets[i1];
  facelets[i1] = facelets[i4];
  facelets[i4] = facelets[i3];
  facelets[i3] = facelets[i2];
  facelets[i2] = temp;
}

// U move (indices 0-8)
function applyU(facelets: string[]): void {
  rotateFace(facelets, 0);
  
  // Cycle edges: R top -> F top -> L top -> B top
  cycle4(facelets, 20, 18, 38, 47);
  cycle4(facelets, 19, 21, 37, 46);
  cycle4(facelets, 9, 24, 36, 45);
}

// D move (indices 27-35)
function applyD(facelets: string[]): void {
  rotateFace(facelets, 27);
  
  // Cycle edges: F bottom -> R bottom -> B bottom -> L bottom
  cycle4(facelets, 15, 26, 42, 51);
  cycle4(facelets, 16, 25, 43, 52);
  cycle4(facelets, 17, 14, 44, 53);
}

// L move (indices 36-44)
function applyL(facelets: string[]): void {
  rotateFace(facelets, 36);
  
  // Cycle edges: U left -> F left -> D left -> B right
  cycle4(facelets, 0, 18, 27, 53);
  cycle4(facelets, 3, 21, 30, 50);
  cycle4(facelets, 6, 24, 33, 47);
}

// R move (indices 9-17)
function applyR(facelets: string[]): void {
  rotateFace(facelets, 9);
  
  // Cycle edges: U right -> B left -> D right -> F right
  cycle4(facelets, 2, 45, 29, 20);
  cycle4(facelets, 5, 48, 32, 23);
  cycle4(facelets, 8, 51, 35, 26);
}

// F move (indices 18-26)
function applyF(facelets: string[]): void {
  rotateFace(facelets, 18);
  
  // Cycle edges: U bottom -> R left -> D top -> L right
  cycle4(facelets, 6, 9, 29, 44);
  cycle4(facelets, 7, 12, 28, 41);
  cycle4(facelets, 8, 15, 27, 38);
}

// B move (indices 45-53)
function applyB(facelets: string[]): void {
  rotateFace(facelets, 45);
  
  // Cycle edges: U top -> L left -> D bottom -> R right
  cycle4(facelets, 0, 36, 35, 17);
  cycle4(facelets, 1, 39, 34, 14);
  cycle4(facelets, 2, 42, 33, 11);
}

// Color mapping for 3D visualization
export const COLOR_MAP: Record<string, string> = {
  U: '#FFFFFF', // White
  R: '#FF0000', // Red
  F: '#00FF00', // Green
  D: '#FFFF00', // Yellow
  L: '#FFA500', // Orange
  B: '#0000FF', // Blue
};

// Get color name from letter
export const COLOR_NAMES: Record<string, string> = {
  U: 'White',
  R: 'Red',
  F: 'Green',
  D: 'Yellow',
  L: 'Orange',
  B: 'Blue',
};
