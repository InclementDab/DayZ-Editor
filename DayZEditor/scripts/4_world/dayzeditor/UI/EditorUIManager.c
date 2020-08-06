



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
	
	
	void TriggerUINotification(string text, int color = -4301218, float duration = 4)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		Widget notif_frame = m_EditorUI.GetNotificationFrame();
		Print(notif_frame);
		Widget notif_panel = notif_frame.FindAnyWidget("NotificationPanel");
		TextWidget notif_text = TextWidget.Cast(notif_frame.FindAnyWidget("NotificationText"));
		
		notif_panel.SetColor(color);
		notif_text.SetText(text);
		notif_frame.Show(true);
			
		float start_x, start_y, width, height;
		notif_frame.GetPos(start_x, start_y);
		notif_frame.GetSize(width, height);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(CloseUINotification, duration * 1000, false, notif_frame, start_x, start_y);
		
		EffectSound notif_sound = SEffectManager.PlaySound("Notification_SoundSet", GetGame().GetCurrentCameraPosition());
		notif_sound.SetSoundAutodestroy(true);
		
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, notif_frame, GetGame().GetTime(), 0.25, start_x, start_x, start_y, start_y + height);
		
	}
	
	
	
	
	private void NotificationAnimateFrame(Widget root, float anim_starttime, float duration, float start_x, float final_x, float start_y, float final_y)
	{
				
		float anim_frametime = GetGame().GetTime() - anim_starttime;
		anim_frametime /= 1000;
		
		float normalized_time = (1 / duration) * anim_frametime;
		
		
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		Print(normalized_time);
		
		float pos_x = Math.Lerp(start_x, final_x, normalized_time);
		float pos_y = Math.Lerp(start_y, final_y, normalized_time);
		
		root.SetPos(pos_x, pos_y);
		
		if (normalized_time >= 1) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		}
		
	}
	
	private void CloseUINotification(Widget root, float start_x, float start_y)
	{
		float current_x, current_y;
		root.GetPos(current_x, current_y);
		
		float duration = 0.25;
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, root, GetGame().GetTime(), duration, current_x, start_x, current_y, start_y);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(root.Show, duration * 1000, false, false);
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