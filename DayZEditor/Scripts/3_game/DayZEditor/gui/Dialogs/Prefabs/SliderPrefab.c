class SliderPrefabController: PrefabBaseController
{
	float Value;
}

class SliderPrefab: PrefabBase
{
	protected SliderPrefabController m_SliderPrefabController;
	
	void SliderPrefab(Widget parent = null, string caption = "", Controller binding_context = null, string binding_name = "", int min = 0, int max = 100)
	{
		m_SliderPrefabController = SliderPrefabController.Cast(GetController());
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_SliderPrefabController.Value);
		m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/SliderPrefab.layout";
	}
	
	override typename GetControllerType() {
		return SliderPrefabController;
	}
}
