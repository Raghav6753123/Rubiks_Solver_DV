import { useCubeStore } from '../store/cubeStore';

export default function SolutionPanel() {
  const { solution, solutionString, currentMoveIndex, error, isLoading, solveCube, resetCube } = useCubeStore();

  const hasSolution = solution.length > 0;

  return (
    <div className="solution-panel">
      <h3>Solution</h3>
      
      <div className="action-buttons">
        <button
          className="solve-btn"
          onClick={solveCube}
          disabled={isLoading}
        >
          {isLoading ? 'Solving...' : 'Calculate Solution'}
        </button>
        
        <button
          className="reset-btn"
          onClick={resetCube}
        >
          Reset Cube
        </button>
      </div>

      {error && (
        <div className="error-message">
          <strong>Error:</strong> {error}
        </div>
      )}

      {solutionString && !error && (
        <div className="solution-display">
          {hasSolution ? (
            <>
              <div className="solution-stats">
                <span>Moves: {solution.length}</span>
              </div>
              
              <div className="solution-moves">
                {solution.map((move, index) => (
                  <span
                    key={index}
                    className={`move ${index === currentMoveIndex ? 'current' : ''} ${index < currentMoveIndex ? 'done' : ''}`}
                  >
                    {move.notation}
                  </span>
                ))}
              </div>
              
              <div className="solution-text">
                <strong>Solution:</strong> {solutionString}
              </div>
            </>
          ) : (
            <div className="solution-text">
              <strong>✅ {solutionString}</strong>
            </div>
          )}
        </div>
      )}
    </div>
  );
}
