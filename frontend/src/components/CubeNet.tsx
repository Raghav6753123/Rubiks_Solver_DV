import { useCubeStore } from '../store/cubeStore';
import { COLOR_MAP } from '../utils/cubeUtils';

export default function CubeNet() {
  const { displayFacelets, selectedColor, setFacelet } = useCubeStore();

  const handleFaceletClick = (index: number) => {
    // Don't allow changing center facelets
    const centers = [4, 13, 22, 31, 40, 49];
    if (centers.includes(index)) return;
    
    setFacelet(index, selectedColor);
  };

  const renderFace = (startIndex: number, label: string) => {
    const facelets = [];
    for (let i = 0; i < 9; i++) {
      const index = startIndex + i;
      const color = displayFacelets[index];
      const isCenter = i === 4;
      
      facelets.push(
        <button
          key={index}
          className={`facelet ${isCenter ? 'center' : ''}`}
          style={{ backgroundColor: COLOR_MAP[color] }}
          onClick={() => handleFaceletClick(index)}
          disabled={isCenter}
        />
      );
    }
    
    return (
      <div className="face">
        <div className="face-label">{label}</div>
        <div className="facelet-grid">{facelets}</div>
      </div>
    );
  };

  return (
    <div className="cube-net">
      <div className="net-row">
        <div className="face-spacer"></div>
        {renderFace(0, 'U')}
        <div className="face-spacer"></div>
        <div className="face-spacer"></div>
      </div>
      <div className="net-row">
        {renderFace(36, 'L')}
        {renderFace(18, 'F')}
        {renderFace(9, 'R')}
        {renderFace(45, 'B')}
      </div>
      <div className="net-row">
        <div className="face-spacer"></div>
        {renderFace(27, 'D')}
        <div className="face-spacer"></div>
        <div className="face-spacer"></div>
      </div>
    </div>
  );
}
