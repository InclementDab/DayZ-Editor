


class EditorCommandManager: CommandManager
{
	ref RoutedUICommand NewCommand = new RoutedUICommand("New", "New", { KeyCode.KC_LCONTROL, KeyCode.KC_X, KeyCode.KC_LMENU, KeyCode.KC_LSHIFT, KeyCode.KC_RSHIFT });
	ref RoutedUICommand OpenCommand = new RoutedUICommand("Open", "Open", { KeyCode.KC_LCONTROL, KeyCode.KC_O });
}

class EditorHud: ScriptView
{	
	// Layout Elements
	protected Widget NotificationFrame;
	protected Widget MapContainer;
	
	protected ref EditorMenu m_EditorFileMenu;
	
	protected ref EditorMap m_EditorMap;
	EditorMap GetEditorMap() {
		return m_EditorMap;
	}
	
	void EditorHud(ScriptView parent = null)
	{	
		EditorLog.Trace("EditorHud");
		m_EditorMap = new EditorMap(parent);
		m_EditorMap.EditorMapWidget.Show(false);
		
		m_EditorFileMenu = new EditorFileMenu(this);
		
		EditorCommandManager.Cast(m_CommandManager).NewCommand.Execute.Insert(EditorCommand_NewExecute);
	}
	
	void EditorCommand_NewExecute(RoutedUICommandArgs args)
	{
		GetEditor().New();
	}
	
	void ~EditorHud() 
	{
		delete m_EditorMap;
	}
	
	void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
		
		EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject editor_object: placed_objects) {
			editor_object.Show(show);
		}
	}
	
	bool IsVisible() {
		return m_LayoutRoot.IsVisible();
	}
	
	void ToggleCursor() {
		ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	void ShowCursor(bool state) {
		GetGame().GetUIManager().ShowCursor(state);
	}
		
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		//EditorNotification notification = new EditorNotification(NotificationFrame, text, color);
		//notification.Play(duration);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{		
		return false;
	}

	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorUI.layout";
	}
	
	override typename GetControllerType() {
		return EditorHudController;
	}
	
	EditorHudController GetEditorHudController() { 
		return EditorHudController.Cast(GetController());
	}
	
	override typename GetCommandManagerType() {
		return EditorCommandManager;
	}
}