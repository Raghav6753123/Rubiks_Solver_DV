import React, { useMemo } from 'react';
import { Canvas } from '@react-three/fiber';
import { OrbitControls } from '@react-three/drei';
import * as THREE from 'three';
import { useCubeStore, FACE_COLORS, SOLVED_CUBE } from '../store/cubeStore';
import { applyMove } from '../utils/cubeUtils';

// Map facelet indices to 3D cube positions
const getFaceletPosition = (index: number): [number, number, number, string] => {
  // Returns [x, y, z, normal] for each facelet
  const positions: Record<number, [number, number, number, string]> = {
    // U face (top)
    0: [-1, 1.5, 1, 'up'], 1: [0, 1.5, 1, 'up'], 2: [1, 1.5, 1, 'up'],
    3: [-1, 1.5, 0, 'up'], 4: [0, 1.5, 0, 'up'], 5: [1, 1.5, 0, 'up'],
    6: [-1, 1.5, -1, 'up'], 7: [0, 1.5, -1, 'up'], 8: [1, 1.5, -1, 'up'],
    
    // R face (right)
    9: [1.5, 1, 1, 'right'], 10: [1.5, 1, 0, 'right'], 11: [1.5, 1, -1, 'right'],
    12: [1.5, 0, 1, 'right'], 13: [1.5, 0, 0, 'right'], 14: [1.5, 0, -1, 'right'],
    15: [1.5, -1, 1, 'right'], 16: [1.5, -1, 0, 'right'], 17: [1.5, -1, -1, 'right'],
    
    // F face (front)
    18: [-1, 1, 1.5, 'front'], 19: [0, 1, 1.5, 'front'], 20: [1, 1, 1.5, 'front'],
    21: [-1, 0, 1.5, 'front'], 22: [0, 0, 1.5, 'front'], 23: [1, 0, 1.5, 'front'],
    24: [-1, -1, 1.5, 'front'], 25: [0, -1, 1.5, 'front'], 26: [1, -1, 1.5, 'front'],
    
    // D face (bottom)
    27: [-1, -1.5, 1, 'down'], 28: [0, -1.5, 1, 'down'], 29: [1, -1.5, 1, 'down'],
    30: [-1, -1.5, 0, 'down'], 31: [0, -1.5, 0, 'down'], 32: [1, -1.5, 0, 'down'],
    33: [-1, -1.5, -1, 'down'], 34: [0, -1.5, -1, 'down'], 35: [1, -1.5, -1, 'down'],
    
    // L face (left)
    36: [-1.5, 1, -1, 'left'], 37: [-1.5, 1, 0, 'left'], 38: [-1.5, 1, 1, 'left'],
    39: [-1.5, 0, -1, 'left'], 40: [-1.5, 0, 0, 'left'], 41: [-1.5, 0, 1, 'left'],
    42: [-1.5, -1, -1, 'left'], 43: [-1.5, -1, 0, 'left'], 44: [-1.5, -1, 1, 'left'],
    
    // B face (back)
    45: [1, 1, -1.5, 'back'], 46: [0, 1, -1.5, 'back'], 47: [-1, 1, -1.5, 'back'],
    48: [1, 0, -1.5, 'back'], 49: [0, 0, -1.5, 'back'], 50: [-1, 0, -1.5, 'back'],
    51: [1, -1, -1.5, 'back'], 52: [0, -1, -1.5, 'back'], 53: [-1, -1, -1.5, 'back'],
  };
  
  return positions[index] || [0, 0, 0, 'up'];
};

const Facelet: React.FC<{ index: number; color: string }> = ({ index, color }) => {
  const [x, y, z, normal] = getFaceletPosition(index);
  
  // Rotation is constant for each facelet's normal orientation
  const rotation = useMemo(() => {
    switch (normal) {
      case 'up': return [0, 0, 0];
      case 'down': return [Math.PI, 0, 0];
      case 'front': return [-Math.PI / 2, 0, 0];
      case 'back': return [Math.PI / 2, 0, 0];
      case 'left': return [0, 0, Math.PI / 2];
      case 'right': return [0, 0, -Math.PI / 2];
      default: return [0, 0, 0];
    }
  }, [normal]);
  
  return (
    <mesh position={[x, y, z]} rotation={rotation as [number, number, number]}>
      <planeGeometry args={[0.9, 0.9]} />
      <meshStandardMaterial color={FACE_COLORS[color]} />
    </mesh>
  );
};

const Cube3D: React.FC = () => {
  const { facelets, solution, currentMoveIndex } = useCubeStore();
  
  // Apply moves up to current index
  const displayFacelets = useMemo(() => {
    let current = facelets;
    for (let i = 0; i < currentMoveIndex && i < solution.length; i++) {
      current = applyMove(current, solution[i]);
    }
    return current;
  }, [facelets, solution, currentMoveIndex]);
  
  return (
    <div style={{
      width: '100%',
      height: '400px',
      backgroundColor: '#f0f0f0',
      borderRadius: '8px',
      marginBottom: '16px',
    }}>
      <Canvas camera={{ position: [5, 5, 5], fov: 50 }}>
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        <OrbitControls />
        
        {/* Render all 54 facelets */}
        {displayFacelets.split('').map((color, index) => (
          <Facelet key={index} index={index} color={color} />
        ))}
      </Canvas>
    </div>
  );
};

export default Cube3D;
