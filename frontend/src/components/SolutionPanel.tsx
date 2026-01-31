import React from 'react';
import { useCubeStore } from '../store/cubeStore';

export const SolutionPanel: React.FC = () => {
  const { solution, solutionString, moveCount, currentMoveIndex, error } = useCubeStore();
  
  if (error) {
    return (
      <div style={{
        backgroundColor: '#fff',
        borderRadius: '8px',
        padding: '16px',
        marginBottom: '16px',
      }}>
        <h3 style={{ marginTop: 0, color: '#dc3545' }}>Error</h3>
        <p style={{ color: '#dc3545', margin: 0 }}>{error}</p>
      </div>
    );
  }
  
  if (!solutionString) {
    return null;
  }
  
  return (
    <div style={{
      backgroundColor: '#fff',
      borderRadius: '8px',
      padding: '16px',
      marginBottom: '16px',
    }}>
      <h3 style={{ marginTop: 0 }}>Solution</h3>
      
      {solution.length === 0 ? (
        <div style={{
          padding: '16px',
          backgroundColor: '#d4edda',
          borderRadius: '4px',
          color: '#155724',
          fontSize: '18px',
          fontWeight: 'bold',
        }}>
          ✅ {solutionString}
        </div>
      ) : (
        <>
          <div style={{ marginBottom: '8px' }}>
            <strong>Moves: {moveCount}</strong>
            <span style={{ marginLeft: '16px', color: '#666' }}>
              Progress: {currentMoveIndex} / {solution.length}
            </span>
          </div>
          
          <div style={{
            display: 'flex',
            flexWrap: 'wrap',
            gap: '8px',
            padding: '12px',
            backgroundColor: '#f8f9fa',
            borderRadius: '4px',
          }}>
            {solution.map((move, index) => (
              <span
                key={index}
                style={{
                  padding: '6px 12px',
                  backgroundColor: index < currentMoveIndex ? '#28a745' : 
                                 index === currentMoveIndex ? '#ffc107' : '#e9ecef',
                  color: index <= currentMoveIndex ? '#fff' : '#333',
                  borderRadius: '4px',
                  fontWeight: 'bold',
                  fontSize: '16px',
                }}
              >
                {move}
              </span>
            ))}
          </div>
        </>
      )}
    </div>
  );
};
