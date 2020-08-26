

/* Script Params:
* 0: Binding_Name (Name of Binding in Controller)
* 1: Binding_Index (Index of Binding in Array)
* 2: Two_Way_Binding (Whether or not this is a two way bind or not)
* 3: Group_Name (Name of group for RadioButtons)
		NOTE: This also corresponds to the name of the variable in the Controller to store the selected RadioButton
*/



typedef ref map<int, ref RadioButton> RadioButtonHashMap;

class RadioButtonData: ref map<string, ref RadioButtonHashMap>
{
	void InsertRadioButton(string key, ref RadioButton radio_button)
	{
		
		RadioButtonHashMap radio_hashmap = Get(key);
		if (!radio_hashmap) {
			radio_hashmap = new RadioButtonHashMap();
		}
		
		radio_hashmap.Set(radio_button.Button_ID, radio_button);
		Set(key, radio_hashmap);	
	}
}



ref RadioButtonData _RadioButtonData;


class RadioButtonGroup: ViewBinding
{
	
	
	
}


class RadioButton: ViewBinding
{
	reference string Group_Name;
	reference int Button_ID;
	
	protected typename m_RadioButtonPropertyDataType;
	protected ref TypeConverter m_RadioButtonPropertyDataConverter;
	
	override void SetController(Controller controller) 
	{ 
		EditorLog.Trace("RadioButton::SetController");	
		super.SetController(controller);
		
		m_RadioButtonPropertyDataType = m_Controller.GetPropertyType(Group_Name);
		m_RadioButtonPropertyDataConverter = MVC.GetTypeConversion(m_RadioButtonPropertyDataType);
	}
	
	void RadioButton()
	{
		if (!_RadioButtonData) {
			_RadioButtonData = new RadioButtonData();
		}
	}
	
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("RadioButton::Init");
		super.OnWidgetScriptInit(w);
		
		if ((m_LayoutRoot.Type() != ButtonWidget) && (m_LayoutRoot.Type() != CheckBoxWidget)) {
			MVC.ErrorDialog(string.Format("Invalid type for RadioButton: %1", m_LayoutRoot.Type()));
			return;
		}
		
		
		_RadioButtonData.InsertRadioButton(Group_Name, this);		
	}
	
	
	override void UpdateModel()
	{
		EditorLog.Trace("RadioButton::UpdateModel");
		super.UpdateModel();
		
		m_RadioButtonPropertyDataConverter.SetFloat(Button_ID);
		m_RadioButtonPropertyDataConverter.SetToController(m_Controller, Group_Name, 0);
		m_Controller.PropertyChanged(Group_Name);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("RadioButton::OnClick");
		
		ref RadioButtonHashMap radio_hashmap = _RadioButtonData.Get(Group_Name);
		foreach (int id, ref RadioButton radio_button: radio_hashmap) {
				
			ButtonWidget _ButtonWidget;
			CheckBoxWidget _CheckBoxWidget;
			if (Class.CastTo(_ButtonWidget, radio_button.m_LayoutRoot)) {
				_ButtonWidget.SetState(id == Button_ID);
			} 
			else if (Class.CastTo(_CheckBoxWidget, radio_button.m_LayoutRoot)) {
				_CheckBoxWidget.SetChecked(id == Button_ID);
			}
		}
		
		//UpdateModel();
		
		
		return true;
	}
	
	
}

