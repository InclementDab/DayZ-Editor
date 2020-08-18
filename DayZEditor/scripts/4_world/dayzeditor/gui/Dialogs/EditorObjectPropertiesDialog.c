

class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected Widget m_ControllerFrame;
	protected ref EditorPropertiesController m_Controller;
	
	private vector m_StartPosition;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		SetContent(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout"));
		m_ControllerFrame = m_ContentWrapper.FindAnyWidget("EditorPropertiesController");
		m_ControllerFrame.GetScript(m_Controller);
		m_Controller.SetEditorObject(m_EditorObject);
		m_StartPosition = m_Controller.GetPosition();
		
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
		m_EditorObject.SetPosition(m_Controller.GetPosition());
		m_EditorObject.SetOrientation(m_Controller.GetOrientation());
		CloseDialog();
	}
	
	void CancelCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
		m_Controller.SetPosition(m_StartPosition);
		m_Controller.SetOrientation(m_StartPosition);
		
		CloseDialog();
	}
	

}
