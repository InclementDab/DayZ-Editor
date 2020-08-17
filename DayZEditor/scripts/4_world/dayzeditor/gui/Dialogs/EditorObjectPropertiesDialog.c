


class EditorObjectPropertiesDialog: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { return m_LayoutRoot; }
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected TextWidget x_pos;
	protected EditorObject m_Object;
			
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout");
		m_LayoutRoot.SetHandler(this);
		
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		EditorLog.Trace("~EditorObjectPropertiesDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	

	
	void Update()
	{
		
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) return false;
		
		if (w == m_ApplyButton) {
			
			Print("Applied");
			return true;
		}
		if (w == m_CancelButton) {
			
			m_LayoutRoot.Show(false);
			m_LayoutRoot.Update();
			return true;
		}
		return false;
	}
	

}
