


class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected Widget m_ViewModelFrame;
	protected EditorPropertiesViewModel m_ViewModel;
			
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		SetContent(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout"));
		m_ViewModelFrame = m_ContentWrapper.FindAnyWidget("EditorPropertiesViewModel");
		m_ViewModelFrame.GetScript(m_ViewModel);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
	

}
