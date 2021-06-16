class SliderPrefab: PrefabBase<float>
{
	static float BUTTON_INCREMENT_COEF = 0.1;
	protected float m_Min, m_Max;
	
	void SliderPrefab(string caption, Class binding_context, string binding_name, float default_value = DEFAULT_VALUE, float min = 0, float max = 100)
	{
		m_Min = min; m_Max = max;
		
		m_PrefabBaseController.Value = default_value / m_Max;
		m_PrefabBaseController.NotifyPropertyChanged("Value");
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Value": {
				m_PrefabBaseController.CalculatedValue = Math.Lerp(m_Min, m_Max, m_PrefabBaseController.Value);
				m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
				m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue", false);
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.CalculatedValue);
				g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
				break;
			}
			
			case "CalculatedValue": {
				m_PrefabBaseController.Value = Math.InverseLerp(m_Min, m_Max, m_PrefabBaseController.CalculatedValue);
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
		}
	}
	
	void OnButtonUp(ButtonCommandArgs args)
	{
		m_PrefabBaseController.CalculatedValue += GetIncrementValue();
		m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
	}
	
	void OnButtonDown(ButtonCommandArgs args)
	{
		m_PrefabBaseController.CalculatedValue -= GetIncrementValue();
		m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
	}
	
	float GetIncrementValue()
	{
		return (Math.AbsFloat(m_Max - m_Min) * BUTTON_INCREMENT_COEF);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/prefabs/SliderPrefab.layout";
	}
}
