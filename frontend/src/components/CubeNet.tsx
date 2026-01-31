import React from 'react';
import { useCubeStore, FACE_COLORS } from '../store/cubeStore';
import { FACELET_LAYOUT } from '../utils/cubeUtils';

export const CubeNet: React.FC = () => {
  const { facelets, selectedColor, setFacelet } = useCubeStore();
  
  const renderFace = (faceKey: string, offsetX: number, offsetY: number) => {
    const faceIndices = FACELET_LAYOUT[faceKey as keyof typeof FACELET_LAYOUT];
    
    return (
      <g key={faceKey}>
        {faceIndices.map((faceletIndex, i) => {
          const row = Math.floor(i / 3);
          const col = i % 3;
          const x = offsetX + col * 30;
          const y = offsetY + row * 30;
          const color = FACE_COLORS[facelets[faceletIndex]];
          
          return (
            <rect
              key={faceletIndex}
              x={x}
              y={y}
              width={28}
              height={28}
              fill={color}
              stroke="#333"
              strokeWidth="2"
              style={{ cursor: 'pointer' }}
              onClick={() => setFacelet(faceletIndex, selectedColor)}
            />
          );
        })}
        {/* Face label */}
        <text
          x={offsetX + 45}
          y={offsetY + 50}
          fontSize="20"
          fontWeight="bold"
          fill="#666"
          textAnchor="middle"
          pointerEvents="none"
        >
          {faceKey}
        </text>
      </g>
    );
  };
  
  return (
    <div style={{
      backgroundColor: '#fff',
      borderRadius: '8px',
      padding: '16px',
      marginBottom: '16px',
    }}>
      <h3 style={{ marginTop: 0 }}>Cube Input (2D Net)</h3>
      <svg width="390" height="300" style={{ display: 'block', margin: '0 auto' }}>
        {/* Layout: U on top, then L F R B in middle row, D on bottom */}
        {renderFace('U', 105, 10)}
        {renderFace('L', 10, 100)}
        {renderFace('F', 105, 100)}
        {renderFace('R', 200, 100)}
        {renderFace('B', 295, 100)}
        {renderFace('D', 105, 190)}
      </svg>
    </div>
  );
};
