

class EditorSettingsDialogController: Controller
{
	
	float ViewDistance;
	float ObjectViewDistance;
	float MarkerViewDistance;
	
	int AutoSaveTimer;
	
	void SetSettings(EditorSettings settings)
	{
		EditorLog.Trace("EditorSettingsDialogController");
		
		ViewDistance = settings.ViewDistance;
		ObjectViewDistance = settings.ObjectViewDistance;
		MarkerViewDistance = settings.MarkerViewDistance;
		AutoSaveTimer = settings.AutoSaveTimer;
	}
	
	EditorSettings GetSettings()
	{
		EditorSettings settings();
		settings.ViewDistance = ViewDistance;
		settings.ObjectViewDistance = ObjectViewDistance;
		settings.MarkerViewDistance = MarkerViewDistance;
		settings.AutoSaveTimer = AutoSaveTimer;
		
		return settings;
	}
		
}

/*
class EditorSettingsDialog: EditorDialog
{
	
	protected EditorSettings m_EditorSettings;
	protected ref EditorSettingsDialogController m_EditorSettingsDialogController;
	
	void EditorSettingsDialog(Widget parent = null) 
	{
		EditorLog.Trace("EditorSettingsDialog::Init");
		SetTitle("Editor Settings");
		AddButton("Save", SaveCallback);
		AddButton("Apply", ApplyCallback);
		AddButton("Close", CloseDialog);
	}
	
	void SetEditorSettings(EditorSettings settings)
	{
		m_EditorSettings = settings;
		
		m_EditorSettingsDialogController = new EditorSettingsDialogController();
		m_EditorSettingsDialogController.SetSettings(m_EditorSettings);
		/*
		EditorPrefabGroup settings_group = new EditorPrefabGroup();
		settings_group.AddPrefab(new EditorPrefabEditText("View Distance", "ViewDistance"));
		settings_group.AddPrefab(new EditorPrefabEditText("Object View Distance", "ObjectViewDistance"));
		settings_group.AddPrefab(new EditorPrefabEditText("Marker View Distance", "MarkerViewDistance"));
		settings_group.AddPrefab(new EditorPrefabEditText("Auto Save", "AutoSaveTimer"));
		settings_group.SetController(m_EditorSettingsDialogController);
		
		AddContent(settings_group);
	}
	
	void SaveCallback()
	{
		ApplyCallback();
		delete this;
	}
	
	void ApplyCallback()
	{
		GetEditor().SetSettings(m_EditorSettingsDialogController.GetSettings());
	}
}*/
