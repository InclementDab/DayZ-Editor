

class EditorSettings: JMModuleBase
{
	
	
	// shit thats gotta be changed
	static float VIEW_DISTANCE = 12000;
	static float OBJECT_VIEW_DISTANCE = 5000;
	
	static bool MAGNET_PLACEMENT = false;
	static bool MAINTAIN_HEIGHT = false;
	static bool SNAPPING_MODE = false;
	static bool COLLIDE_ON_DRAG = false;
	
	// Autosave timer in SECONDS!!!!!!!!!!!!!!!!
	static int AUTOSAVE_TIMER = 240;

	
	
	
	void EditorSettings()
	{
		EditorLog.Trace("EditorSettings");
	}
	
	
	override void OnInit()
	{
		EditorLog.Trace("EditorSettings::OnInit");
		// uhhh
		Load();
		m_EditorBrushTypes = new EditorBrushDataSet();
		m_CustomBrushList = new map<string, typename>()
	}
	
	static void Load()
	{
		GetGame().GetWorld().SetViewDistance(VIEW_DISTANCE);
		GetGame().GetWorld().SetObjectViewDistance(OBJECT_VIEW_DISTANCE);
	}
	
	
	static void Save()
	{
		// todo save settings to ini or something
	}

	
	// Brush Management
	// move this to EditorBrush? make static
	
	private ref EditorBrushDataSet 		m_EditorBrushTypes;
	private ref map<string, typename> 	m_CustomBrushList;
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorUIManager::ReloadBrushes");
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(SetBrushTypes);
		GetXMLApi().Read(filename, xml_brushes);
	}
	
	EditorBrushData GetLoadedBrushData(string name) { return m_EditorBrushTypes.Get(name); }
	
	
	void SetBrushTypes(EditorBrushDataSet brush_types)
	{
		GetEditor().GetEditorUI().ClearBrushBox();
		m_EditorBrushTypes = brush_types;

		foreach (EditorBrushData brush: m_EditorBrushTypes)
			GetEditor().GetEditorUI().InsertBrush(brush.Name);		
	}
		
	EditorBrush GetBrush(string brush_name)
	{
		EditorLog.Trace("EditorSettings::GetBrush " + brush_name);
		foreach (EditorBrushData settings: m_EditorBrushTypes) {
			if (settings.Name == brush_name) {
				foreach (string name, typename type: m_CustomBrushList) 
					if (name == brush_name)
						return type.Spawn();
					
				return new EditorBrush(settings);			
			}
		}
		
		EditorLog.Trace("EditorSettings::GetBrush Brush not found!");
		return null;
	}
	
	override bool IsServer() { return false; }
	

}

