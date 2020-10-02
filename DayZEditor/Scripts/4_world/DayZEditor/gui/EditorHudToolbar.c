class EditorHudToolbar: ScriptView
{
	// View Properties
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	
	// Relay Commands
	void MenuBarExecute(ButtonCommandArgs args) 
	{		
		EditorLog.Trace("EditorHudToolbarController::MenuBarExecute");
		if (!EditorUIManager.CurrentMenu) { //  GetMenu().Type() != GetBoundMenu(args.GetButtonWidget()) removed cause GetBoundMenu is gone
			EditorUIManager.CurrentMenu = CreateToolbarMenu(args.Source);
		} else {
			delete EditorUIManager.CurrentMenu;
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView: {
				
				if (EditorUIManager.CurrentMenu) {
					delete EditorUIManager.CurrentMenu;
					EditorUIManager.CurrentMenu = CreateToolbarMenu(w);
				}
				break;
			}	
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	private EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudToolbarController::CreateToolbarMenu");	
		
		EditorMenu toolbar_menu;
		switch (toolbar_button) {
			
			case MenuBarFile: {
				toolbar_menu = new EditorFileMenu(toolbar_button);
				break;
			}
			
			case MenuBarEdit: {
				toolbar_menu = new EditorEditMenu(toolbar_button);
				break;
			}
			
			case MenuBarView: {
				toolbar_menu = new EditorViewMenu(toolbar_button);
				break;
			}
		}
		
		// Sets position to bottom of button
		float w, h;
		toolbar_button.GetScreenSize(w, h);
		toolbar_menu.SetPosition(0, h);

		return EditorUIManager.CurrentMenu;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/hud/EditorHudToolbar.layout";
	}
	
	override typename GetControllerType() {
		return EditorHudToolbarController;
	}
}