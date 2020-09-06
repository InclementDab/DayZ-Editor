
enum EditorPrefabButtonStyle {
	BUTTON_120x30,
	BUTTON_90x30,
	BUTTON_69x69,
};

class EditorPrefab
{
	protected ref EditorPrefabViewBinding m_ViewBinding;
	
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { 
		return m_LayoutRoot;
	}
	
	protected string m_BindingName;
	string GetBindingName() {
		return m_BindingName;
	}
	
	protected int m_BindingIndex;
	int GetBindingIndex() {
		return m_BindingIndex;
	}
	
	void EditorPrefab(string caption = "", string binding_name = "", int binding_index = 0) {
		EditorLog.Trace("EditorPrefab");
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(GetLayoutFile());
		m_BindingName = binding_name;
		m_BindingIndex = binding_index;
		
		if (m_LayoutRoot && m_BindingName != string.Empty) {
			m_LayoutRoot.GetScript(m_ViewBinding);
			m_ViewBinding.SetPrefab(this);
		}
		
		if (m_LayoutRoot && GetLabelWidget() != string.Empty) {
			TextWidget.Cast(m_LayoutRoot.FindAnyWidget(GetLabelWidget())).SetText(caption);
		}
	}
	
	
	void SetController(Controller controller) {
		EditorLog.Trace("EditorPrefab::SetController");		
		if (m_ViewBinding) {
			m_ViewBinding.SetController(controller);
		}
	}
	
	string GetLayoutFile();
	string GetLabelWidget();
}

class EditorPrefabGroup: EditorPrefab
{
	private ref array<ref EditorPrefab> m_EditorPrefabChildren = {};
	
	void AddPrefab(Widget child_fab)
	{
		m_LayoutRoot.FindAnyWidget("EditorDialogOptionContent").AddChild(child_fab);
	}
	
	void RemovePrefab(Widget child_fab)
	{
		m_LayoutRoot.FindAnyWidget("EditorDialogOptionContent").RemoveChild(child_fab);
	}
	
	void AddPrefab(EditorPrefab child_fab)
	{
		m_EditorPrefabChildren.Insert(child_fab);
		m_LayoutRoot.FindAnyWidget("EditorDialogOptionContent").AddChild(child_fab.GetLayoutRoot());
	}
	
	void RemovePrefab(EditorPrefab child_fab) 
	{
		m_EditorPrefabChildren.Remove(m_EditorPrefabChildren.Find(child_fab));
		m_LayoutRoot.FindAnyWidget("EditorDialogOptionContent").RemoveChild(child_fab.GetLayoutRoot());
	}
	
	void ClearPrefabs()
	{
		foreach (EditorPrefab child_fab: m_EditorPrefabChildren) {
			RemovePrefab(child_fab);
		}
	}
	
	override void SetController(Controller controller) {
		super.SetController(controller);
		foreach (EditorPrefab prefab: m_EditorPrefabChildren) {
			prefab.SetController(controller);
		}
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOption.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorDialogOptionTitle";
	}	
}

class EditorPrefabDropdown: EditorPrefab
{
	void EditorPrefabDropdown(string caption = "", string binding_name = "", int binding_index = 0, string binding_selected = "") {
		 
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionDropdown.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorDialogOptionDropdownLabel";
	}
}

class EditorPrefabEditText: EditorPrefab
{
	void SetText(string text) {
		EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorEditTextElementEditBox")).SetText(text);
	}
	
	string GetText() {
		return EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorEditTextElementEditBox")).GetText();
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionEditText.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorEditTextElementLabel";
	}
}

class EditorPrefabEditMultilineText: EditorPrefab
{
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionEditTextBox.layout";
	}

}

class EditorPrefabPosition: EditorPrefab
{

	
	void SetVector(vector value) {
		EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_x")).SetText(value[0].ToString());
		EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_y")).SetText(value[1].ToString());
		EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_z")).SetText(value[2].ToString());	
	}
	
	vector GetVector() {
		vector value;
		value[0] = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_x")).GetText().ToFloat();
		value[1] = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_y")).GetText().ToFloat();
		value[2] = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("pos_z")).GetText().ToFloat();
		return value;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionPosition.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorPosElementLabel";
	}
}

class EditorPrefabSlider: EditorPrefab
{
	
	float m_Min, m_Max;

	void EditorPrefabSlider(string caption = "", string binding_name = "", int binding_index = 0, float min = 0, float max = 100) {
		m_Min = min; m_Max = max;
	}

	void SetFloat(float value) {
		SliderWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorOptionSliderSlider")).SetCurrent(value / m_Max);
	}
		
	float GetFloat() {
		return SliderWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorOptionSliderSlider")).GetCurrent() * m_Max;
	}
	
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionSlider.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorOptionSliderLabel";
	}
}



class EditorPrefabButton: EditorPrefab
{
	EditorPrefabButtonStyle m_Style;
	
	protected ButtonWidget m_ButtonWidget;
	
	void EditorPrefabButton(string caption = "", string binding_name = "", int binding_index = 0, EditorPrefabButtonStyle style = 0) {
		m_Style = style;
		
		m_ButtonWidget = GetButtonWidgetByStyle(m_Style);
		m_ButtonWidget.GetParent().GetParent().Show(true);
	}
	
	void SetButtonText(string text) {
		m_ButtonWidget.SetText(text);
	}
	
	void SetBool(bool value) {
		m_ButtonWidget.SetState(value);
	}
	
	bool GetBool() {
		return m_ButtonWidget.GetState();
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorPrefabButton.layout";
	}
	
	override string GetLabelWidget() {
		return string.Format("%1_Label", typename.EnumToString(EditorPrefabButtonStyle, m_Style));
	}
	
	ButtonWidget GetButtonWidgetByStyle(EditorPrefabButtonStyle style) {
		return ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget(typename.EnumToString(EditorPrefabButtonStyle, style)));
	}
}


