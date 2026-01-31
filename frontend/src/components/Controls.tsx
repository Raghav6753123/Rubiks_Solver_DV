import React from 'react';
import { useCubeStore } from '../store/cubeStore';

export const Controls: React.FC = () => {
  const {
    isSolving,
    resetCube,
    solveCube,
    currentMoveIndex,
    solution,
    isPlaying,
    setIsPlaying,
    nextMove,
    prevMove,
    playbackSpeed,
    setPlaybackSpeed,
    setCurrentMoveIndex,
  } = useCubeStore();
  
  // Auto-play timer
  React.useEffect(() => {
    if (!isPlaying) return;
    
    const interval = setInterval(() => {
      if (currentMoveIndex < solution.length) {
        nextMove();
      } else {
        setIsPlaying(false);
      }
    }, 1000 / playbackSpeed);
    
    return () => clearInterval(interval);
  }, [isPlaying, currentMoveIndex, solution.length, playbackSpeed, nextMove, setIsPlaying]);
  
  return (
    <div style={{
      backgroundColor: '#fff',
      borderRadius: '8px',
      padding: '16px',
      marginBottom: '16px',
    }}>
      <div style={{ display: 'flex', gap: '8px', marginBottom: '16px' }}>
        <button
          onClick={resetCube}
          style={{
            padding: '10px 20px',
            backgroundColor: '#6c757d',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: 'pointer',
            fontSize: '14px',
            fontWeight: 'bold',
          }}
        >
          Reset Cube
        </button>
        
        <button
          onClick={solveCube}
          disabled={isSolving}
          style={{
            padding: '10px 20px',
            backgroundColor: isSolving ? '#ccc' : '#007bff',
            color: 'white',
            border: 'none',
            borderRadius: '4px',
            cursor: isSolving ? 'not-allowed' : 'pointer',
            fontSize: '14px',
            fontWeight: 'bold',
          }}
        >
          {isSolving ? 'Solving...' : 'Calculate Solution'}
        </button>
      </div>
      
      {solution.length > 0 && (
        <div>
          <div style={{ display: 'flex', gap: '8px', marginBottom: '8px' }}>
            <button
              onClick={prevMove}
              disabled={currentMoveIndex === 0}
              style={{
                padding: '8px 16px',
                backgroundColor: currentMoveIndex === 0 ? '#ccc' : '#28a745',
                color: 'white',
                border: 'none',
                borderRadius: '4px',
                cursor: currentMoveIndex === 0 ? 'not-allowed' : 'pointer',
              }}
            >
              ◀ Previous
            </button>
            
            <button
              onClick={() => setIsPlaying(!isPlaying)}
              style={{
                padding: '8px 16px',
                backgroundColor: '#ffc107',
                color: '#333',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer',
                fontWeight: 'bold',
              }}
            >
              {isPlaying ? '⏸ Pause' : '▶ Play'}
            </button>
            
            <button
              onClick={nextMove}
              disabled={currentMoveIndex >= solution.length}
              style={{
                padding: '8px 16px',
                backgroundColor: currentMoveIndex >= solution.length ? '#ccc' : '#28a745',
                color: 'white',
                border: 'none',
                borderRadius: '4px',
                cursor: currentMoveIndex >= solution.length ? 'not-allowed' : 'pointer',
              }}
            >
              Next ▶
            </button>
            
            <button
              onClick={() => setCurrentMoveIndex(0)}
              style={{
                padding: '8px 16px',
                backgroundColor: '#17a2b8',
                color: 'white',
                border: 'none',
                borderRadius: '4px',
                cursor: 'pointer',
              }}
            >
              ⏮ Reset
            </button>
          </div>
          
          <div style={{ marginTop: '8px' }}>
            <label>
              Speed: {playbackSpeed}x
              <input
                type="range"
                min="0.5"
                max="3"
                step="0.5"
                value={playbackSpeed}
                onChange={(e) => setPlaybackSpeed(parseFloat(e.target.value))}
                style={{ marginLeft: '8px', width: '150px' }}
              />
            </label>
          </div>
        </div>
      )}
    </div>
  );
};
