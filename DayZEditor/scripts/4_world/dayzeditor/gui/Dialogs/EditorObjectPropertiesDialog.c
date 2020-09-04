




class EditorPropertiesPrefabController: Controller
{
	
	protected EditorObject m_EditorObject;
	
	string name;
	vector pos;
	vector rot;
	
	void EditorPropertiesPrefabController(EditorObject editor_object) 
	{
		EditorLog.Trace("EditorPropertiesPrefabController");
		m_EditorObject = editor_object;
		
		pos = m_EditorObject.GetPosition();
		rot = m_EditorObject.GetOrientation();		
		name = m_EditorObject.GetDisplayName();
	}
	
	override void MVCOnMouseWheel(Widget target, int direction, int x, int y)
	{
		EditorLog.Trace("EditorPropertiesController::OnMouseWheel %1", target.GetName());
		string w_name = target.GetName();
		
		StringEvaluater w_eval;
		EnScript.GetClassVar(this, w_name, 0, w_eval);
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			direction *= 10;
		}
		
		EnScript.SetClassVar(this, w_name, 0, (w_eval.Parse() + direction).ToString());
		NotifyPropertyChanged(w_name);
	}

	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorPropertiesPrefabController::PropertyChanged");
		switch (property_name) {
			
			case "pos": {
				m_EditorObject.SetPosition(pos);
				break;
			}
			
			case "rot": {
				m_EditorObject.SetOrientation(rot);
				break;
			}
			
			case "name": {
				m_EditorObject.SetDisplayName(name);
				break;
			}
		}
	}
}

class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected ref EditorPropertiesPrefabController m_EditorPropertiesController;
	
	private string m_StartName;
	private vector m_StartPosition;
	private vector m_StartOrientation;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		m_EditorPropertiesController = new EditorPropertiesPrefabController(m_EditorObject);
		
		EditorPrefabGroup info_group = new EditorPrefabGroup("Object Info");
		info_group.AddPrefab(new EditorPrefabEditText("Name", "name"));
		info_group.AddPrefab(new EditorPrefabPosition("Position", "pos"));
		info_group.AddPrefab(new EditorPrefabPosition("Rotation", "rot"));
		info_group.SetController(m_EditorPropertiesController);
		
		m_StartName = m_EditorObject.GetDisplayName();
		m_StartPosition = m_EditorObject.GetPosition();
		m_StartOrientation = m_EditorObject.GetOrientation();
		
		SetTitle("Edit: Object Properties");
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "CancelCallback");
		AddContent(info_group);
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
		m_EditorObject.SetDisplayName(m_StartName);
		m_EditorObject.SetPosition(m_StartPosition);
		m_EditorObject.SetOrientation(m_StartOrientation);
		CloseDialog();
	}
	

	

}
