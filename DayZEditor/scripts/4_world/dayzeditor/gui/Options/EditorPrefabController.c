
enum EditorPrefabType {
	
	Dropdown,
	EditText,
	EditMultilineText,
	Position,
	Slider
};



class EditorPrefab
{
	
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { 
		return m_LayoutRoot;
	}
	
	void EditorPrefab() {
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets(GetLayoutFile());
	}
	
	
	string GetLayoutFile();
}

class EditorPrefabGroup: EditorPrefab
{
	private ref array<ref EditorPrefab> m_EditorPrefabChildren = {};
	
	void EditorPrefabGroup(string caption) {
		TextWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorDialogOptionTitle")).SetText(caption);
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
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOption.layout";
	}
}

class EditorPrefabDropdown: EditorPrefab
{
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/options/EditorDialogOptionDropdown.layout";
	}
}
