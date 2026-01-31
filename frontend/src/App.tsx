import React from 'react';
import { ColorPicker } from './components/ColorPicker';
import { CubeNet } from './components/CubeNet';
import Cube3D from './components/Cube3D';
import { Controls } from './components/Controls';
import { SolutionPanel } from './components/SolutionPanel';

function App() {
  return (
    <div style={{
      minHeight: '100vh',
      backgroundColor: '#e9ecef',
      padding: '20px',
    }}>
      <div style={{
        maxWidth: '1200px',
        margin: '0 auto',
      }}>
        <h1 style={{
          textAlign: 'center',
          color: '#333',
          marginBottom: '24px',
        }}>
          Rubik's Cube Solver
        </h1>
        
        <div style={{
          display: 'grid',
          gridTemplateColumns: '1fr 1fr',
          gap: '20px',
        }}>
          {/* Left column: Input and controls */}
          <div>
            <ColorPicker />
            <CubeNet />
            <Controls />
          </div>
          
          {/* Right column: 3D visualization and solution */}
          <div>
            <Cube3D />
            <SolutionPanel />
          </div>
        </div>
      </div>
    </div>
  );
}

export default App;
