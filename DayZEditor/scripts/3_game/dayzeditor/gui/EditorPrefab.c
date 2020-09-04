
class EditorPrefab
{
	protected ref Controller m_Controller;
	protected ref ViewBinding m_ViewBinding;
	
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { 
		return m_LayoutRoot;
	}
	
	
	
	
	void EditorPrefab(string caption = "", string binding_name = "") {
		EditorLog.Trace("EditorPrefab");
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(GetLayoutFile());
		
		if (m_LayoutRoot && binding_name != string.Empty) {
			m_LayoutRoot.GetScript(m_ViewBinding);
			m_ViewBinding.SetBindingName(binding_name);
		}
		
		if (m_LayoutRoot && GetLabelWidget() != string.Empty) {
			TextWidget.Cast(m_LayoutRoot.FindAnyWidget(GetLabelWidget())).SetText(caption);
		}
	}
	
	
	void SetController(Controller controller) {
		EditorLog.Trace("EditorPrefab::SetController");
		m_Controller = controller;
		
		if (m_ViewBinding && m_Controller) {
			m_ViewBinding.SetController(m_Controller);
			EnScript.SetClassVar(m_Controller, m_ViewBinding.GetBindingName(), 0, this);
		}
	}
	
	string GetLayoutFile();
	string GetLabelWidget();
}

class EditorPrefabGroup: EditorPrefab
{
	private ref array<ref EditorPrefab> m_EditorPrefabChildren = {};
	


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
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionSlider.layout";
	}
	
	override string GetLabelWidget() {
		return "EditorOptionSliderLabel";
	}
}


