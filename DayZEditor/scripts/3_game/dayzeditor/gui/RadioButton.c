

/* Script Params:
* 0: Binding_Name (Name of Binding in Controller)
* 1: Binding_Index (Index of Binding in Array)
* 2: Two_Way_Binding (Whether or not this is a two way bind or not)
* 3: Group_Name (Name of group for RadioButtons)
		NOTE: This also corresponds to the name of the variable in the Controller to store the selected RadioButton
*/

typedef ref map<int, ref RadioButton> RadioButtonHashMap;


class RadioButtonGroup: ViewBinding
{
	protected int m_ActiveButton;
	protected ref RadioButtonHashMap m_RadioButtonHashMap = new RadioButtonHashMap();
	
	ref ScriptInvoker OnRadioButtonActivate = new ScriptInvoker();
	ref ScriptInvoker OnRadioButtonDeactivate = new ScriptInvoker();
	
	override void SetController(Controller controller)
	{
		EditorLog.Trace("RadioButtonGroup::SetController");
		m_Controller = controller;
		m_PropertyConverter = MVC.GetTypeConversion(int);
		UpdateView();
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("RadioButtonGroup::Init");
		super.OnWidgetScriptInit(w);	
		
		// Load Child RadioButtons
		EditorLog.Info(string.Format("Loaded %1 Radio Buttons", LoadChildButtons(m_LayoutRoot, m_RadioButtonHashMap)));
	}
	
	override void UpdateModel()
	{
		EditorLog.Trace("RadioButtonGroup::UpdateModel");		
		EditorLog.Debug(string.Format("[%1] Updating Model...", m_LayoutRoot.Type()));

		m_PropertyConverter.SetInt(m_ActiveButton);
		m_PropertyConverter.SetToController(m_Controller, Binding_Name, 0);
		m_Controller.NotifyPropertyChanged(Binding_Name);
	}
	
	override void UpdateView()
	{
		super.UpdateView();
		SetActiveRadioButton(m_PropertyConverter.GetInt());
	}
	
	private int LoadChildButtons(Widget w, out RadioButtonHashMap button_hashmap)
	{
		RadioButton radio_button;
		w.GetScript(radio_button);
		
		if (radio_button && (radio_button.IsInherited(RadioButton))) {
			button_hashmap.Insert(radio_button.GetID(), radio_button);
			radio_button.SetGroup(this);
		}
		
		if (w.GetChildren() != null)
			LoadChildButtons(w.GetChildren(), button_hashmap);
		
		
		if (w.GetSibling() != null)
			LoadChildButtons(w.GetSibling(), button_hashmap);
	
		
		return button_hashmap.Count();
	}
	
	
	private int current_id;
	private RadioButton current_btn;
	
	void SetActiveRadioButton(int button_id)
	{
		EditorLog.Trace("RadioButtonGroup::SetActiveRadioButton");
		
		OnRadioButtonDeactivate.Invoke(m_RadioButtonHashMap.Get(m_ActiveButton));
		m_ActiveButton = button_id;
		OnRadioButtonActivate.Invoke(m_RadioButtonHashMap.Get(m_ActiveButton));
		
		foreach (int id, ref RadioButton radio_button: m_RadioButtonHashMap) {	
			
			switch (radio_button.GetRoot().Type()) {
				
				case ButtonWidget: {
					ButtonWidget.Cast(radio_button.GetRoot()).SetState(id == button_id);
					break;
				}
				
				case CheckBoxWidget: {
					CheckBoxWidget.Cast(radio_button.GetRoot()).SetChecked(id == button_id);
					break;
				}
				
				default: {
					EditorLog.Error("RadioButtonGroup::SetActiveRadioButton: Invalid Type %1", radio_button.GetRoot().Type().ToString());
				}
			}
		}
	}
	

}


class RadioButton: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	Widget GetRoot() 
		return m_LayoutRoot; 
	
	protected RadioButtonGroup m_RadioButtonGroup;
	
	protected reference int Button_ID;
	int GetID() { return Button_ID; }
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("RadioButton::Init");
		m_LayoutRoot = w;
		
		if ((m_LayoutRoot.Type() != ButtonWidget) && (m_LayoutRoot.Type() != CheckBoxWidget)) {
			EditorLog.Error("Invalid type for RadioButton: %1", m_LayoutRoot.Type().ToString());
			return;
		}
		
		m_LayoutRoot.SetHandler(this);
	}
	
	void SetGroup(RadioButtonGroup button_group)
	{
		EditorLog.Trace("RadioButton::SetGroup");
		m_RadioButtonGroup = button_group;
	}	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("RadioButton::OnClick " + w.Type());
		if (button != 0) return false;
		m_RadioButtonGroup.SetActiveRadioButton(Button_ID);
		
		return false;
	}

}

