import { useEffect } from 'react';
import { useCubeStore } from '../store/cubeStore';

export default function Controls() {
  const {
    solution,
    currentMoveIndex,
    isPlaying,
    playbackSpeed,
    nextMove,
    prevMove,
    togglePlayback,
    setPlaybackSpeed,
    resetPlayback,
  } = useCubeStore();

  const hasSolution = solution.length > 0;
  const canGoBack = currentMoveIndex >= 0;
  const canGoForward = currentMoveIndex < solution.length - 1;

  // Auto-play logic
  useEffect(() => {
    if (!isPlaying || !canGoForward) {
      if (isPlaying && !canGoForward) {
        togglePlayback(); // Stop when finished
      }
      return;
    }

    const timer = setTimeout(() => {
      nextMove();
    }, playbackSpeed);

    return () => clearTimeout(timer);
  }, [isPlaying, currentMoveIndex, playbackSpeed, canGoForward, nextMove, togglePlayback]);

  if (!hasSolution) return null;

  return (
    <div className="controls">
      <h3>Playback Controls</h3>
      
      <div className="control-buttons">
        <button
          className="control-btn"
          onClick={resetPlayback}
          disabled={currentMoveIndex === -1 && !isPlaying}
        >
          ⏮ Reset
        </button>
        
        <button
          className="control-btn"
          onClick={prevMove}
          disabled={!canGoBack}
        >
          ⏪ Previous
        </button>
        
        <button
          className="control-btn play-pause"
          onClick={togglePlayback}
          disabled={!canGoForward && !isPlaying}
        >
          {isPlaying ? '⏸ Pause' : '▶ Play'}
        </button>
        
        <button
          className="control-btn"
          onClick={nextMove}
          disabled={!canGoForward}
        >
          Next ⏩
        </button>
      </div>

      <div className="speed-control">
        <label htmlFor="speed-slider">
          Speed: {(1000 / playbackSpeed).toFixed(1)} moves/sec
        </label>
        <input
          id="speed-slider"
          type="range"
          min="200"
          max="2000"
          step="100"
          value={playbackSpeed}
          onChange={(e) => setPlaybackSpeed(Number(e.target.value))}
        />
      </div>

      <div className="move-counter">
        Move {currentMoveIndex + 1} / {solution.length}
      </div>
    </div>
  );
}
