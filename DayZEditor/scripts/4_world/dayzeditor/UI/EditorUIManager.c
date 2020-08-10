

typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorUIManager: Managed // remove meeeee
{
	// Members
	private ref EditorUI 		m_EditorUI;
	private EditorCamera		m_EditorCamera;
	private ref ScriptInvoker 	m_UpdateInvoker;
	private UIManager			m_UIManager;
	private ref EditorPlaceableListItemSet m_PlaceableObjects;
	
	// Getters
	EditorUI GetEditorUI() { return m_EditorUI; }
	EditorCamera GetEditorCamera() { return m_EditorCamera; }
	ScriptInvoker GetUpdateInvoker() { return m_UpdateInvoker; }
	
	void EditorUIManager()
	{
		Print("EditorUIManager");
		m_UpdateInvoker = new ScriptInvoker();
		m_UIManager = GetGame().GetUIManager();
		
		// Init UI
		m_EditorUI = new EditorUI();
		m_UIManager.ShowScriptedMenu(m_EditorUI, GetGame().GetUIManager().GetMenu());
		m_EditorUI.GetNotificationFrame().GetPos(notification_start_x, notification_start_y);
		
		// Init Spawn Position
		TIntArray center_pos = new TIntArray();		
		string world_name;
		GetGame().GetWorldName(world_name);
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", world_name), center_pos);
		
		// Random cam position smile :)
		center_pos[0] = Math.RandomInt(3500, 8500);
		center_pos[1] = Math.RandomInt(3500, 8500);
		
		// Init Camera
		float y_level = 200 + GetGame().SurfaceY(center_pos[0], center_pos[1]);
		m_EditorCamera = EditorCamera.Cast(GetGame().CreateObject("EditorCamera", Vector(center_pos[0], y_level, center_pos[1]), false));
		m_EditorCamera.SetActive(true);
		
		// Init Camera Map Marker
		EditorCameraMapMarker CameraMapMarker = new EditorCameraMapMarker();
		Widget m_MapMarkerWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorCameraMapMarker.layout");
		m_MapMarkerWidget.GetScript(CameraMapMarker);
		CameraMapMarker.SetCamera(m_EditorCamera, m_EditorUI.GetMapWidget(), m_UpdateInvoker);
		m_EditorUI.InsertMapObject(m_MapMarkerWidget);
		m_EditorUI.GetMapWidget().SetMapPos(Vector(center_pos[0], y_level, center_pos[1]));
		
	
		
		// Load PlaceableObjects
		m_PlaceableObjects = new EditorPlaceableListItemSet();
		Print(string.Format("Loaded %1 Placeable Objects", EditorObjectManager.GetPlaceableObjects(m_PlaceableObjects)));
		
		foreach (ref EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			m_EditorUI.InsertPlaceableObject(placeable_object);
		}	
				
		
		// Subscribe to events (and twitch.tv/InclementDab)
		//EditorEvents.OnObjectCreated.Insert(OnEditorObjectCreated);		
		EditorEvents.OnPlaceableCategoryChanged.Insert(OnPlaceableCategoryChanged);	
		
		// Sets default
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
		
		//m_EditorUI.SetOrientationWidget(GetEditorCamera().GetOrientation());
	}
	
	void SetEditorUI(EditorUI ui)
	{
		m_EditorUI = ui;
	}
	
	
	void OnEditorObjectCreated(Class context, EditorObject obj)
	{
		EditorPrint("EditorUIManager::OnObjectCreated");		
	}
	
	
	
	void SetEditorCameraActive(bool state)
	{
		EditorPrint("EditorUIManager::SetEditorCameraActive");
		m_EditorCamera.SetActive(state);
	}
	
	void ShowCursor()
	{
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	void HideCursor()
	{
		GetGame().GetUIManager().ShowUICursor(false);
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
	
	private float notification_start_x, notification_start_y;
	void NotificationCreate(string text, int color = -4301218, float duration = 4)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationDestroy);
		
		Widget notif_frame = m_EditorUI.GetNotificationFrame();
		Widget notif_panel = notif_frame.FindAnyWidget("NotificationPanel");
		TextWidget notif_text = TextWidget.Cast(notif_frame.FindAnyWidget("NotificationText"));
		
		notif_panel.SetColor(color);
		notif_text.SetText(text);
		notif_frame.Show(true);
			
		float width, height;
		notif_frame.GetSize(width, height);
		
		EffectSound notif_sound = SEffectManager.PlaySound("Notification_SoundSet", GetEditorCamera().GetPosition());
		notif_sound.SetSoundAutodestroy(true);
		
		// Animate pulldown
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, notif_frame, GetGame().GetTime(), 0.25, notification_start_x, notification_start_x, notification_start_y, notification_start_y + height);
		
		// Call destroy after duration done
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationDestroy, duration * 1000, false, notif_frame, notification_start_x, notification_start_y);
	}
	
	
	
	
	private void NotificationAnimateFrame(Widget root, float anim_starttime, float duration, float start_x, float final_x, float start_y, float final_y)
	{
				
		float anim_frametime = GetGame().GetTime() - anim_starttime;
		anim_frametime /= 1000;
		
		float normalized_time = (1 / duration) * anim_frametime;
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		
		float pos_x = Math.Lerp(start_x, final_x, normalized_time);
		float pos_y = Math.Lerp(start_y, final_y, normalized_time);
		
		root.SetPos(pos_x, pos_y);
		
		if (normalized_time >= 1)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		
		
	}
	
	private void NotificationDestroy(Widget root, float start_x, float start_y)
	{
		float current_x, current_y;
		root.GetPos(current_x, current_y);
		
		float duration = 0.25;
		// Animate in reverse
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, root, GetGame().GetTime(), duration, current_x, start_x, current_y, start_y);
		
		// Hide Object
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(root.Show, duration * 1000, false, false);
	}


	
	
	private bool m_Visibility;
	void SetVisibility(bool state)
	{
		m_EditorUI.GetRoot().Show(state);
		EditorObjectSet placed_objects = GetEditor().GetObjectManager().GetPlacedObjects();
		/*foreach (EditorObject editor_object: placed_objects) {
			editor_object.GetObjectMarker().Show(state);
		}*/
		
		m_Visibility = state;
	}
	
	
	bool GetVisibility() { return m_Visibility;	}
	
	
		
	void OnPlaceableCategoryChanged(Class context, PlaceableObjectCategory category)
	{
		EditorPrint("EditorUIManager::OnPlaceableCategoryChanged");

		foreach (EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			Widget root = placeable_object.GetRoot();
			root.Show(placeable_object.GetData().GetCategory() == category);
		}
	}
	
	

	// Modal Window Control
	private ref EditorDialog m_CurrentModal;
	void ModalSet(EditorDialog w)
	{
		m_CurrentModal = w;
		SetModal(m_CurrentModal.GetRoot());
		ShowCursor();
	}
	
	void ModalClose()
	{
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		ShowCursor();
	}

	bool IsModalActive()
	{
		return m_UIManager.IsModalVisible();
	}
	
}