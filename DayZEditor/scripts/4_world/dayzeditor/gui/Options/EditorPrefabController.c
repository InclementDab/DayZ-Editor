
class EditorPrefab
{
	protected ref ViewBinding m_ViewBinding;
	
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { 
		return m_LayoutRoot;
	}
	
	protected Controller m_Controller;
	
	void EditorPrefab(string caption = "", string binding_name = "") {
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
		m_Controller = controller;
		m_LayoutRoot.SetHandler(m_Controller);
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


