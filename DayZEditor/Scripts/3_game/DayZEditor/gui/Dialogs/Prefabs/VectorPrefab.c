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
	
	void VectorPrefab(Widget parent = null, string caption = "", Controller binding_context = null, string binding_name = "", vector default_value = "0 0 0")
	{
		m_VectorPrefabController = VectorPrefabController.Cast(GetController());
		
		m_VectorPrefabController.X = default_value[0];
		m_VectorPrefabController.Y = default_value[1];
		m_VectorPrefabController.Z = default_value[2];
		m_VectorPrefabController.NotifyPropertyChanged();
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/VectorPrefab.layout";
	}
	
	override typename GetControllerType() {
		return VectorPrefabController;
	}
}