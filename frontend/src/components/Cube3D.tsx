import { useRef } from 'react';
import { Canvas } from '@react-three/fiber';
import { OrbitControls } from '@react-three/drei';
import * as THREE from 'three';
import { useCubeStore } from '../store/cubeStore';
import { COLOR_MAP } from '../utils/cubeUtils';

// Individual cubie component
function Cubie({ position, colors }: { position: [number, number, number]; colors: string[] }) {
  const meshRef = useRef<THREE.Mesh>(null);

  return (
    <mesh ref={meshRef} position={position}>
      <boxGeometry args={[0.95, 0.95, 0.95]} />
      {colors.map((color, i) => (
        <meshStandardMaterial key={i} attach={`material-${i}`} color={color} />
      ))}
    </mesh>
  );
}

// Main 3D cube visualization
export default function Cube3D() {
  const { displayFacelets } = useCubeStore();

  // Convert facelets to cubie data
  const cubies = mapFaceletsToCubies(displayFacelets);

  return (
    <div className="cube-3d">
      <Canvas camera={{ position: [4, 4, 4], fov: 50 }}>
        <ambientLight intensity={0.5} />
        <directionalLight position={[5, 5, 5]} intensity={1} />
        <directionalLight position={[-5, -5, -5]} intensity={0.3} />
        
        {cubies.map((cubie, i) => (
          <Cubie key={i} position={cubie.position} colors={cubie.colors} />
        ))}
        
        <OrbitControls enablePan={false} />
      </Canvas>
    </div>
  );
}

// Map 54 facelets to 26 cubies (excluding center cubie)
function mapFaceletsToCubies(facelets: string[]) {
  const cubies = [];
  const BLACK = '#1a1a1a';

  // Define cubie positions and their facelet indices
  // Each cubie has 6 faces: right, left, top, bottom, front, back
  
  // Corner cubies (8)
  const corners = [
    // URF
    { pos: [1, 1, 1] as [number, number, number], faces: [9, -1, 8, -1, 20, -1] },
    // UFL
    { pos: [-1, 1, 1] as [number, number, number], faces: [-1, 38, 6, -1, 18, -1] },
    // ULB
    { pos: [-1, 1, -1] as [number, number, number], faces: [-1, 36, 0, -1, -1, 47] },
    // UBR
    { pos: [1, 1, -1] as [number, number, number], faces: 11, -1, 2, -1, -1, 45] },
    // DFR
    { pos: [1, -1, 1] as [number, number, number], faces: [15, -1, -1, 29, 26, -1] },
    // DLF
    { pos: [-1, -1, 1] as [number, number, number], faces: [-1, 44, -1, 27, 24, -1] },
    // DBL
    { pos: [-1, -1, -1] as [number, number, number], faces: [-1, 42, -1, 33, -1, 53] },
    // DRB
    { pos: [1, -1, -1] as [number, number, number], faces: [17, -1, -1, 35, -1, 51] },
  ];

  // Edge cubies (12)
  const edges = [
    // UR
    { pos: [1, 1, 0] as [number, number, number], faces: [10, -1, 5, -1, -1, -1] },
    // UF
    { pos: [0, 1, 1] as [number, number, number], faces: [-1, -1, 7, -1, 19, -1] },
    // UL
    { pos: [-1, 1, 0] as [number, number, number], faces: [-1, 37, 3, -1, -1, -1] },
    // UB
    { pos: [0, 1, -1] as [number, number, number], faces: [-1, -1, 1, -1, -1, 46] },
    // DR
    { pos: [1, -1, 0] as [number, number, number], faces: [14, -1, -1, 32, -1, -1] },
    // DF
    { pos: [0, -1, 1] as [number, number, number], faces: [-1, -1, -1, 28, 25, -1] },
    // DL
    { pos: [-1, -1, 0] as [number, number, number], faces: [-1, 43, -1, 30, -1, -1] },
    // DB
    { pos: [0, -1, -1] as [number, number, number], faces: [-1, -1, -1, 34, -1, 52] },
    // FR
    { pos: [1, 0, 1] as [number, number, number], faces: [12, -1, -1, -1, 23, -1] },
    // FL
    { pos: [-1, 0, 1] as [number, number, number], faces: [-1, 41, -1, -1, 21, -1] },
    // BL
    { pos: [-1, 0, -1] as [number, number, number], faces: [-1, 39, -1, -1, -1, 50] },
    // BR
    { pos: [1, 0, -1] as [number, number, number], faces: [16, -1, -1, -1, -1, 48] },
  ];

  // Center cubies (6)
  const centers = [
    // R
    { pos: [1, 0, 0] as [number, number, number], faces: [13, -1, -1, -1, -1, -1] },
    // L
    { pos: [-1, 0, 0] as [number, number, number], faces: [-1, 40, -1, -1, -1, -1] },
    // U
    { pos: [0, 1, 0] as [number, number, number], faces: [-1, -1, 4, -1, -1, -1] },
    // D
    { pos: [0, -1, 0] as [number, number, number], faces: [-1, -1, -1, 31, -1, -1] },
    // F
    { pos: [0, 0, 1] as [number, number, number], faces: [-1, -1, -1, -1, 22, -1] },
    // B
    { pos: [0, 0, -1] as [number, number, number], faces: [-1, -1, -1, -1, -1, 49] },
  ];

  // Process all cubies
  [...corners, ...edges, ...centers].forEach(cubie => {
    const colors = cubie.faces.map(faceIndex => {
      if (faceIndex === -1) return BLACK;
      return COLOR_MAP[facelets[faceIndex]] || BLACK;
    });
    cubies.push({ position: cubie.pos, colors });
  });

  return cubies;
}
