



class EditorUIManager: Managed
{
	// Members
	private ref EditorUI 		m_EditorUI;
	private ref Widget 			m_EditorWidget;
	private EditorCamera		m_EditorCamera;
	private ref ScriptInvoker 		m_UpdateInvoker;
	
	// Getters
	EditorUI GetEditorUI() { return m_EditorUI; }
	EditorCamera GetEditorCamera() { return m_EditorCamera; }


	ScriptInvoker GetUpdateInvoker() { return m_UpdateInvoker; }
	
	void EditorUIManager()
	{
		Print("EditorUIManager");
		m_UpdateInvoker = new ScriptInvoker();
		
		// Init UI
		m_EditorUI = new EditorUI();
		m_EditorWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "Editor.layout");
		m_EditorWidget.GetScript(m_EditorUI);
		EntityAI translate = GetGame().CreateObjectEx("TranslationWidget", vector.Zero, ECE_NONE, RF_FRONT);
		m_EditorUI.m_OrientationWidget.SetItem(translate);
		m_EditorUI.m_OrientationWidget.SetView(0);
		
		// Init Spawn Position
		TIntArray center_pos = new TIntArray();		
		string world_name;
		GetGame().GetWorldName(world_name);
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", world_name), center_pos);
		
		// Init Camera
		float y_level = 200 + GetGame().SurfaceY(center_pos[0], center_pos[1]);
		m_EditorCamera = GetGame().CreateObject("EditorCamera", Vector(center_pos[0], y_level, center_pos[1]), false);
		m_EditorCamera.SetActive(true);
		
		// Init Camera Map Marker
		EditorCameraMapMarker CameraMapMarker = new EditorCameraMapMarker();
		Widget m_MapMarkerWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorCameraMapMarker.layout");
		m_MapMarkerWidget.GetScript(CameraMapMarker);
		CameraMapMarker.SetCamera(m_EditorCamera);
		
		MapWidget map_widget = m_EditorUI.GetMapWidget();
		map_widget.AddChild(m_MapMarkerWidget);
	
		
		// Load placeable objects
		ref array<ref PlaceableEditorObject> placeable_objects = new array<ref PlaceableEditorObject>();

		Print(string.Format("Loaded %1 Placeable Objects", EditorObjectManager.GetPlaceableObjects(placeable_objects)));
		foreach (PlaceableEditorObject placeable_object: placeable_objects)
			m_EditorUI.InsertPlaceableObject(placeable_object);
		
		
		// Subscribe to events (and twitch.tv/InclementDab)
		EditorEvents.OnObjectCreated.Insert(OnEditorObjectCreated);
		//EditorEvents.OnObjectCreated.Insert(O);
		
		
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorUIManager()
	{
		Print("~EditorUIManager");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	private int m_LastFrameTime;
	void Update()
	{
		float timeslice = (GetGame().GetTime() - m_LastFrameTime) / 1000;
		m_UpdateInvoker.Invoke(timeslice);
		m_LastFrameTime = GetGame().GetTime();
	}
	
	
	void OnEditorObjectCreated(Class context, EditorObject obj)
	{
		Print("EditorUIManager::OnObjectCreated");
		
		m_EditorUI.InsertPlacedObject(obj);
		m_EditorUI.GetMap().OnObjectCreated(null, obj); // subscribe mee
		
	}
	
	void SetEditorCameraActive(bool state)
	{
		Print("EditorUIManager::SetEditorCameraActive");
		m_EditorCamera.SetActive(state);
	}
	
	bool IsCursorOverUI()
	{
		float pos_x, pos_y, size_x, size_y;
		
		m_EditorUI.GetCenterSpaceFrame().GetScreenPos(pos_x, pos_y);
		m_EditorUI.GetCenterSpaceFrame().GetScreenSize(size_x, size_y);
		
		int cursor_x, cursor_y;
		GetCursorPos(cursor_x, cursor_y);
		
		return (cursor_x > size_x + pos_x) || (cursor_x < pos_x) || (cursor_y > size_y + pos_y) || (cursor_y < pos_y);
	}
	

	
	
	private bool m_Visibility;
	void SetVisibility(bool state)
	{
		m_EditorUI.GetRoot().Show(state);
		foreach (EditorObject editor_object: GetEditor().GetObjectManager().GetPlacedObjects()) {
			editor_object.GetObjectMarker().Show(state);
		}
		
		m_Visibility = state;
	}
	
	
	bool GetVisibility() { return m_Visibility;	}
	
	
	
	
}