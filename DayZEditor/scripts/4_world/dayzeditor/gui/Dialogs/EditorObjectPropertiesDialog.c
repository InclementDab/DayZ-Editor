




class EditorPropertiesPrefabController: Controller
{
	
	protected EditorObject m_EditorObject;
	
	string name;
	vector pos;
	vector rot;
	
	ref ObservableCollection<string> animations;
	string selected_animation;
	bool apply_animation;
	
	void EditorPropertiesPrefabController(EditorObject editor_object) 
	{
		EditorLog.Trace("EditorPropertiesPrefabController");
		m_EditorObject = editor_object;
		
		pos = m_EditorObject.GetPosition();
		rot = m_EditorObject.GetOrientation();		
		name = m_EditorObject.GetDisplayName();
		animations = new ObservableCollection<string>("animations", this);
		
		string value;
		GetGame().ConfigGetText("CfgVehicles " + editor_object.GetType() + " enfenimsys graphName", value);
		EditorLog.Info("EditorPropertiesPrefabController::Loading GraphName %1", value);
	}
	
	override void MVCOnMouseWheel(Widget target, int direction, int x, int y)
	{
		EditorLog.Trace("EditorPropertiesController::OnMouseWheel %1", target.GetName());
		string w_name = target.GetName();
		
		switch (w_name) {
			
			case "pos":
			case "rot": {
			
				StringEvaluater w_eval;
				EnScript.GetClassVar(this, w_name, 0, w_eval);
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					direction *= 10;
				}
				
				EnScript.SetClassVar(this, w_name, 0, (w_eval.Parse() + direction).ToString());
				NotifyPropertyChanged(w_name);
			}
		}
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
				m_EditorObject.ResetAnimation();
				break;
			}
			
			case "name": {
				m_EditorObject.SetDisplayName(name);
				break;
			}
			
			case "apply_animation": {
				m_EditorObject.SetAnimation("CMD_Climb");
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
		

		AddContent(info_group);
		
		if (editor_object.GetWorldObject().IsMan()) {
			
			EditorPrefabGroup character_group = new EditorPrefabGroup("Character Control");
			character_group.AddPrefab(new EditorPrefabDropdown("Animation", "animations", 0, "selected_animation"));
			character_group.AddPrefab(new EditorPrefabButton("Apply Animation", "apply_animation"));
			character_group.SetController(m_EditorPropertiesController);
			AddContent(character_group);
		}
		
		
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
		m_EditorObject.SetDisplayName(m_StartName);
		m_EditorObject.SetPosition(m_StartPosition);
		m_EditorObject.SetOrientation(m_StartOrientation);
		CloseDialog();
	}
	

	

}
