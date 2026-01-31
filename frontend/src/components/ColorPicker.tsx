import { useCubeStore, Color } from '../store/cubeStore';
import { COLOR_MAP, COLOR_NAMES } from '../utils/cubeUtils';

const COLORS: Color[] = ['U', 'R', 'F', 'D', 'L', 'B'];

export default function ColorPicker() {
  const { selectedColor, setSelectedColor } = useCubeStore();

  return (
    <div className="color-picker">
      <h3>Color Picker</h3>
      <div className="color-grid">
        {COLORS.map((color) => (
          <button
            key={color}
            className={`color-button ${selectedColor === color ? 'selected' : ''}`}
            style={{ backgroundColor: COLOR_MAP[color] }}
            onClick={() => setSelectedColor(color)}
            title={COLOR_NAMES[color]}
          >
            {selectedColor === color && <span className="checkmark">✓</span>}
          </button>
        ))}
      </div>
      <div className="color-legend">
        {COLORS.map((color) => (
          <div key={color} className="legend-item">
            <span className="legend-color" style={{ backgroundColor: COLOR_MAP[color] }}></span>
            <span className="legend-text">{color} = {COLOR_NAMES[color]}</span>
          </div>
        ))}
      </div>
    </div>
  );
}
