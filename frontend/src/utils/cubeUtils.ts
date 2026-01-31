/**
 * Cube facelet mapping for 2D net
 * 
 * Layout:
 *          U (0-8)
 *     L (36-44) F (18-26) R (9-17) B (45-53)
 *          D (27-35)
 */

export const FACELET_LAYOUT = {
  U: [0, 1, 2, 3, 4, 5, 6, 7, 8],
  R: [9, 10, 11, 12, 13, 14, 15, 16, 17],
  F: [18, 19, 20, 21, 22, 23, 24, 25, 26],
  D: [27, 28, 29, 30, 31, 32, 33, 34, 35],
  L: [36, 37, 38, 39, 40, 41, 42, 43, 44],
  B: [45, 46, 47, 48, 49, 50, 51, 52, 53],
};

/**
 * Apply a move to a cube state
 */
export function applyMove(facelets: string, move: string): string {
  const cube = facelets.split('');
  
  // Parse move (e.g., "R", "R'", "R2")
  const face = move[0];
  const modifier = move.slice(1);
  
  let times = 1;
  if (modifier === "'") times = 3;
  else if (modifier === "2") times = 2;
  
  for (let i = 0; i < times; i++) {
    applyBasicMove(cube, face);
  }
  
  return cube.join('');
}

/**
 * Apply a single 90-degree clockwise rotation
 */
function applyBasicMove(cube: string[], face: string) {
  switch (face) {
    case 'U':
      rotateFace(cube, [0, 1, 2, 3, 4, 5, 6, 7, 8]);
      cycleEdges(cube, [
        [6, 7, 8],
        [45, 46, 47],
        [2, 1, 0],
        [18, 19, 20]
      ]);
      break;
    case 'R':
      rotateFace(cube, [9, 10, 11, 12, 13, 14, 15, 16, 17]);
      cycleEdges(cube, [
        [2, 5, 8],
        [20, 23, 26],
        [29, 32, 35],
        [53, 50, 47]
      ]);
      break;
    case 'F':
      rotateFace(cube, [18, 19, 20, 21, 22, 23, 24, 25, 26]);
      cycleEdges(cube, [
        [6, 7, 8],
        [9, 12, 15],
        [29, 28, 27],
        [44, 41, 38]
      ]);
      break;
    case 'D':
      rotateFace(cube, [27, 28, 29, 30, 31, 32, 33, 34, 35]);
      cycleEdges(cube, [
        [24, 25, 26],
        [15, 16, 17],
        [51, 52, 53],
        [42, 43, 44]
      ]);
      break;
    case 'L':
      rotateFace(cube, [36, 37, 38, 39, 40, 41, 42, 43, 44]);
      cycleEdges(cube, [
        [0, 3, 6],
        [47, 50, 53],
        [35, 32, 29],
        [18, 21, 24]
      ]);
      break;
    case 'B':
      rotateFace(cube, [45, 46, 47, 48, 49, 50, 51, 52, 53]);
      cycleEdges(cube, [
        [2, 1, 0],
        [38, 37, 36],
        [33, 34, 35],
        [11, 10, 9]
      ]);
      break;
  }
}

/**
 * Rotate a face 90 degrees clockwise
 */
function rotateFace(cube: string[], indices: number[]) {
  const [a, b, c, d, e, f, g, h, i] = indices.map(idx => cube[idx]);
  cube[indices[0]] = g;
  cube[indices[1]] = d;
  cube[indices[2]] = a;
  cube[indices[3]] = h;
  cube[indices[4]] = e;
  cube[indices[5]] = b;
  cube[indices[6]] = i;
  cube[indices[7]] = f;
  cube[indices[8]] = c;
}

/**
 * Cycle edge pieces between faces
 */
function cycleEdges(cube: string[], edges: number[][]) {
  const temp = edges[0].map(idx => cube[idx]);
  for (let i = 0; i < edges.length - 1; i++) {
    for (let j = 0; j < edges[i].length; j++) {
      cube[edges[i][j]] = cube[edges[i + 1][j]];
    }
  }
  for (let j = 0; j < temp.length; j++) {
    cube[edges[edges.length - 1][j]] = temp[j];
  }
}
