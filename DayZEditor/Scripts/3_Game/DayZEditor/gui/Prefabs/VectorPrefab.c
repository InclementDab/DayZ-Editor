class VectorPrefabController: PrefabBaseController<vector>
{
	StringEvaluater X, Y, Z;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "X":
			case "Y":
			case "Z": {
				Value = string.Format("%1 %2 %3", X.Parse(), Y.Parse(), Z.Parse()).ToVector();
				super.PropertyChanged("Value");
				break;
			}
		}
	}
}

class VectorPrefab: PrefabBase<vector>
{
	protected VectorPrefabController m_VectorPrefabController;
	
	void VectorPrefab(string caption, Class binding_context, string binding_name, vector default_value = DEFAULT_VALUE)
	{
		m_VectorPrefabController = VectorPrefabController.Cast(GetController());
		
		m_VectorPrefabController.X = default_value[0].ToString();
		m_VectorPrefabController.Y = default_value[1].ToString();
		m_VectorPrefabController.Z = default_value[2].ToString();
		m_VectorPrefabController.NotifyPropertyChanged("X");
		m_VectorPrefabController.NotifyPropertyChanged("Y");
		m_VectorPrefabController.NotifyPropertyChanged("Z");
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/VectorPrefab.layout";
	}
	
	override typename GetControllerType() {
		return VectorPrefabController;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel;
		motion *= 0.5;
		
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