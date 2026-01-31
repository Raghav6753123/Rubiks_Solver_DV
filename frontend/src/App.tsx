import ColorPicker from './components/ColorPicker';
import CubeNet from './components/CubeNet';
import Cube3D from './components/Cube3D';
import Controls from './components/Controls';
import SolutionPanel from './components/SolutionPanel';
import './App.css';

function App() {
  return (
    <div className="app">
      <header className="app-header">
        <h1>🎲 Rubik's Cube Solver</h1>
        <p>Paint colors on the 2D net, then click "Calculate Solution"</p>
      </header>

      <div className="app-content">
        <div className="left-panel">
          <ColorPicker />
          <CubeNet />
          <SolutionPanel />
          <Controls />
        </div>

        <div className="right-panel">
          <h2>3D Visualization</h2>
          <Cube3D />
          <p className="hint">Drag to rotate • Scroll to zoom</p>
        </div>
      </div>
    </div>
  );
}

export default App;
