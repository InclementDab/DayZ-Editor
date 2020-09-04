


class EditorPropertiesPrefabController: Controller
{
	EditorPrefabEditText name;
	
	EditorPrefabPosition pos;
	EditorPrefabPosition rot;
}

class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected ref EditorPropertiesPrefabController m_EditorPropertiesController;
	
	private vector m_StartPosition;
	private vector m_StartOrientation;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		m_EditorPropertiesController = new EditorPropertiesPrefabController();
		
		EditorPrefabGroup info_group = new EditorPrefabGroup("Object Info");
		info_group.AddPrefab(new EditorPrefabEditText("Name", "name"));
		info_group.AddPrefab(new EditorPrefabPosition("Position", "pos"));
		info_group.AddPrefab(new EditorPrefabPosition("Rotation", "rot"));
		
		info_group.SetController(m_EditorPropertiesController);
		
		AddContent(info_group);
		
		/*
		m_EditorPropertiesController = AddContent("DayZEditor/gui/Layouts/options/EditorDialogOptionPropertiesPrefab.layout");
		m_EditorPropertiesController.SetEditorObject(m_EditorObject);
		m_StartPosition = m_EditorObject.GetPosition();
		m_StartOrientation = m_EditorObject.GetOrientation();
		*/
		SetTitle("Edit: Object Properties");
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "CancelCallback");
	}
	
	void ~EditorObjectPropertiesDialog() {
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
		
	
	void SaveCallback() {
		EditorLog.Trace("EditorObjectPropertiesDialog::SaveCallback");
		CloseDialog();
	}
	
	void CancelCallback() {
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
		m_EditorObject.SetPosition(m_StartPosition);
		m_EditorObject.SetOrientation(m_StartOrientation);
		CloseDialog();
	}
	

	

}
