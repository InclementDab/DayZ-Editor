
/*
class EditorPropertiesPrefabController: DialogBaseController
{
	
	protected EditorObject m_EditorObject;
	
	string name;
	vector pos;
	vector rot;
	
	ref ObservableCollection<string> animations;
	string selected_animation;
	bool apply_animation;
	bool reset_animation;
	bool pause_simulation;
	
	bool edit_loot;
	
	void SetEditorObject(EditorObject editor_object) 
	{
		EditorLog.Trace("EditorPropertiesPrefabController::SetEditorObject");
		m_EditorObject = editor_object;
		
		pos = m_EditorObject.GetPosition();
		rot = m_EditorObject.GetOrientation();		
		name = m_EditorObject.GetDisplayName();
		animations = new ObservableCollection<string>("animations", this);
		animations.Insert("Test1");
		animations.Insert("Test2");
		
		string value;
		GetGame().ConfigGetText("CfgVehicles " + editor_object.GetType() + " enfenimsys graphName", value);
		EditorLog.Info("EditorPropertiesPrefabController::Loading GraphName %1", value);

	}

	// this isnt being called due to how we are dynamically generating ViewBindings in the controller
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		string w_name = w.GetName();
		float direction = wheel;
		switch (w_name) {
			
			case "pos":
			case "rot": {
			
				StringEvaluater w_eval;
				EnScript.GetClassVar(this, w_name, 0, w_eval);
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					direction *= 10;
				} else if (KeyState(KeyCode.KC_LMENU)) {
					direction /= 10;
				}
				
				EnScript.SetClassVar(this, w_name, 0, (w_eval.Parse() + direction).ToString());
				NotifyPropertyChanged(w_name);
			}
		}
		return false;
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
				m_EditorObject.ResetAnimation();
				m_EditorObject.SetAnimation(selected_animation);
				break;
			}
			
			case "reset_animation": {
				m_EditorObject.ResetAnimation();
				break;
			}
			
			case "pause_simulation": {
				pause_simulation = !pause_simulation;
				m_EditorObject.PauseSimulation(pause_simulation);
				break;
			}
			
			case "edit_loot": {
				GetEditor().EditLootSpawns(m_EditorObject.GetType());
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
	
	private string m_StartName;
	private vector m_StartPosition;
	private vector m_StartOrientation;
	
	protected ref EditorPropertiesPrefabController m_EditorPropertiesController;

	void EditorObjectPropertiesDialog(Widget parent = null) 
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		SetTitle("Edit: Object Properties");
		AddButton("Save", SaveCallback);
		AddButton("Close", CancelCallback);
	}
	
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		if (!m_EditorObject) {
			return;
		}
		
		m_EditorPropertiesController = EditorPropertiesPrefabController.Cast(GetController());
		m_EditorPropertiesController.SetEditorObject(m_EditorObject);
		
				
		m_StartName = m_EditorObject.GetDisplayName();
		m_StartPosition = m_EditorObject.GetPosition();
		m_StartOrientation = m_EditorObject.GetOrientation();
		/*
		EditorPrefabGroup info_group = new EditorPrefabGroup("Object Info");
		info_group.AddPrefab(new EditorPrefabEditText("Name", "name"));
		info_group.AddPrefab(new EditorPrefabPosition("Position", "pos"));
		info_group.AddPrefab(new EditorPrefabPosition("Rotation", "rot"));
		info_group.SetController(m_EditorPropertiesController);

		AddContent(info_group);
		
		if (editor_object.GetWorldObject().IsMan()) {
			
			EditorPrefabGroup character_group = new EditorPrefabGroup("Character Control");
			character_group.AddPrefab(new EditorPrefabDropdown("Animation", "animations", "selected_animation"));
			character_group.AddPrefab(new EditorPrefabEditText("Animation Name", "selected_animation"));
			character_group.AddPrefab(new EditorPrefabButton("Apply", "apply_animation", 0));
			character_group.AddPrefab(new EditorPrefabButton("Reset", "reset_animation", 0));
			character_group.AddPrefab(new EditorPrefabButton("Pause", "pause_simulation", 0));
			
			m_EditorPropertiesController.animations.Insert("Test1");
			character_group.SetController(m_EditorPropertiesController);
			AddContent(character_group);
		} else {
			
			EditorPrefabGroup loot_editor = new EditorPrefabGroup("Loot Editor");
			loot_editor.AddPrefab(new EditorPrefabButton("Edit Loot", "edit_loot"));
			
			loot_editor.SetController(m_EditorPropertiesController);
			AddContent(loot_editor);
		}
	}
	
	void SaveCallback() 
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::SaveCallback");
		CloseDialog();
	}
	
	void CancelCallback() 
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
		m_EditorObject.SetDisplayName(m_StartName);
		m_EditorObject.SetPosition(m_StartPosition);
		m_EditorObject.SetOrientation(m_StartOrientation);
		CloseDialog();
	}
	
	void ~EditorObjectPropertiesDialog() {
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
	
	override typename GetControllerType() {
		return EditorPropertiesPrefabController;
	}
}*/
