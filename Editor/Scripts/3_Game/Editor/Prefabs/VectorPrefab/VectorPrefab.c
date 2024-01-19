float g_EditorPrecision;

modded class VectorPrefab
{
	// Override is for step size
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel * g_EditorPrecision;
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			// code sucks because it needs to be perfectly accurate, since floats get cut off at high numbers
			if (wheel == 1) { 
				motion = 0.1;
			} else {
				motion = -0.1;
			}
		}
				
		switch (w.GetName()) {
			
			case "x_value": {
				m_VectorPrefabController.X = string.ToString(m_VectorPrefabController.X.Parse() + motion);
				m_VectorPrefabController.NotifyPropertyChanged("X");
				break;
			}
			
			case "y_value": {
				m_VectorPrefabController.Y = string.ToString(m_VectorPrefabController.Y.Parse() + motion);
				m_VectorPrefabController.NotifyPropertyChanged("Y");
				break;
			}
			
			case "z_value": {
				m_VectorPrefabController.Z = string.ToString(m_VectorPrefabController.Z.Parse() + motion);
				m_VectorPrefabController.NotifyPropertyChanged("Z");
				break;
			}
		}
		
		return true;
	}
}