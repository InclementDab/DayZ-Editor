
class SliderPrefab: PrefabBase<float>
{
	protected float m_Min, m_Max;
	
	void SliderPrefab(string caption, Controller binding_context, string binding_name, float default_value = DEFAULT_VALUE, float min = 0, float max = 100)
	{
		m_Min = min; m_Max = max;
		
		m_PrefabBaseController.Value = default_value / m_Max;
		m_PrefabBaseController.NotifyPropertyChanged("Value");
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		m_PrefabBaseController.CalculatedValue = m_PrefabBaseController.Value * m_Max;
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue", false);
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.CalculatedValue);
		m_BindingContext.PropertyChanged(m_BindingName);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/SliderPrefab.layout";
	}
}
