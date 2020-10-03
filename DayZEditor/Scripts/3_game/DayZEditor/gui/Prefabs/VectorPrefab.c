class VectorPrefabController: PrefabBaseController<vector>
{
	float X, Y, Z;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "X":
			case "Y":
			case "Z": {
				Value = Vector(X, Y, Z);
				super.PropertyChanged("Value");
				break;
			}
		}
	}
}

class VectorPrefab: PrefabBase<vector>
{
	protected VectorPrefabController m_VectorPrefabController;
	
	void VectorPrefab(string caption, Controller binding_context, string binding_name, vector default_value = DEFAULT_VALUE)
	{
		m_VectorPrefabController = VectorPrefabController.Cast(GetController());
		
		m_VectorPrefabController.X = default_value[0];
		m_VectorPrefabController.Y = default_value[1];
		m_VectorPrefabController.Z = default_value[2];
		m_VectorPrefabController.NotifyPropertyChanged();
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
			motion *= 0.1;
		}
		
		switch (w.GetName()) {
			
			case "x_value": {
				m_VectorPrefabController.X += motion;
				m_VectorPrefabController.NotifyPropertyChanged("X");
				break;
			}
			
			case "y_value": {
				m_VectorPrefabController.Y += motion;
				m_VectorPrefabController.NotifyPropertyChanged("Y");
				break;
			}
			
			case "z_value": {
				m_VectorPrefabController.Z += motion;
				m_VectorPrefabController.NotifyPropertyChanged("Z");
				break;
			}
			
		}
		
		return false;
	}
}