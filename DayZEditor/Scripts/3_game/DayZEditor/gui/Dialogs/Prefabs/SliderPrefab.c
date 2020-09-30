class SliderPrefabController: PrefabBaseController
{
	float Value;
}

class SliderPrefab: PrefabBase
{
	protected SliderPrefabController m_SliderPrefabController;
	protected float m_Min, m_Max;
	
	void SliderPrefab(Widget parent = null, string caption = "", Controller binding_context = null, string binding_name = "", float default_value = 0, float min = 0, float max = 100)
	{
		m_Min = min; m_Max = max;
		
		m_SliderPrefabController = SliderPrefabController.Cast(GetController());
		m_SliderPrefabController.Value = default_value / m_Max;
		m_SliderPrefabController.NotifyPropertyChanged("Value");
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_SliderPrefabController.Value * m_Max);
		m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/SliderPrefab.layout";
	}
	
	override typename GetControllerType() {
		return SliderPrefabController;
	}
}
