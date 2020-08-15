

typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorUIManager
{
	// Members
	private ref EditorUI 		m_EditorUI;
	private ref EditorPlaceableListItemSet m_PlaceableObjects;
	
	// Getters
	EditorUI GetEditorUI() { return m_EditorUI; }
	
	void EditorUIManager()
	{
		EditorLog.Trace("EditorUIManager");		
		
		// Init UI
		m_EditorUI = new EditorUI();
		GetGame().GetUIManager().ShowScriptedMenu(m_EditorUI, GetGame().GetUIManager().GetMenu());
		m_EditorUI.GetNotificationFrame().GetPos(notification_start_x, notification_start_y);
			
	
		
		// Load PlaceableObjects
		m_PlaceableObjects = new EditorPlaceableListItemSet();
		EditorLog.Info(string.Format("Loaded %1 Placeable Objects", EditorSettings.GetPlaceableObjects(m_PlaceableObjects)));
		
		foreach (ref EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			m_EditorUI.InsertPlaceableObject(placeable_object);
		}	
				
		
		// Subscribe to events (and twitch.tv/InclementDab)
		//EditorEvents.OnObjectCreated.Insert(OnEditorObjectCreated);		

	}	
	
		
	//void SetEditorUI(EditorUI ui) { m_EditorUI = ui; }
		
	

	
	static void ShowCursor()
	{
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	static void HideCursor()
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
		
		EffectSound notif_sound = SEffectManager.PlaySound("Notification_SoundSet", GetEditor().GetCamera().GetPosition());
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
		EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		/*foreach (EditorObject editor_object: placed_objects) {
			editor_object.GetObjectMarker().Show(state);
		}*/
		
		m_Visibility = state;
	}
	
	
	bool GetVisibility() { return m_Visibility;	}
	
	
	// todo this is broke
	void OnPlaceableCategoryChanged(Class context, PlaceableObjectCategory category)
	{
		EditorLog.Trace("EditorUIManager::OnPlaceableCategoryChanged");

		foreach (EditorPlaceableListItem placeable_object: m_PlaceableObjects) {
			Widget root = placeable_object.GetRoot();
			root.Show(placeable_object.GetData().GetCategory() == category);
		}
	}
	
	

	// Modal Window Control
	private static ref EditorDialog m_CurrentModal;
	static void ModalSet(EditorDialog w)
	{
		Print("ModalSet");
		m_CurrentModal = w;
		SetModal(m_CurrentModal.GetRoot());
		ShowCursor();
	}
	
	static void ModalClose()
	{
		Print("ModalClose");
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		ShowCursor();
	}

	bool IsModalActive()
	{
		return GetGame().GetUIManager().IsModalVisible();
	}
	
	
	
}