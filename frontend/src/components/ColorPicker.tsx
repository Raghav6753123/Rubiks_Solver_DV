import React from 'react';
import { useCubeStore, FACE_COLORS } from '../store/cubeStore';

export const ColorPicker: React.FC = () => {
  const { selectedColor, setSelectedColor } = useCubeStore();
  
  const colors = Object.entries(FACE_COLORS);
  
  return (
    <div style={{
      display: 'flex',
      gap: '8px',
      padding: '16px',
      backgroundColor: '#f5f5f5',
      borderRadius: '8px',
      marginBottom: '16px',
    }}>
      <div style={{ fontWeight: 'bold', marginRight: '8px' }}>Color:</div>
      {colors.map(([face, color]) => (
        <button
          key={face}
          onClick={() => setSelectedColor(face)}
          style={{
            width: '40px',
            height: '40px',
            backgroundColor: color,
            border: selectedColor === face ? '3px solid #000' : '1px solid #999',
            borderRadius: '4px',
            cursor: 'pointer',
            boxShadow: selectedColor === face ? '0 2px 4px rgba(0,0,0,0.3)' : 'none',
          }}
          title={face}
        />
      ))}
    </div>
  );
};
