

class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected Widget m_ControllerFrame;

	
	private vector m_StartPosition;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		SetContent(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout"));
		
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "CancelCallback");
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
		
	
	void SaveCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::SaveCallback");

		CloseDialog();
	}
	
	void CancelCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");

		
		CloseDialog();
	}
	

}
