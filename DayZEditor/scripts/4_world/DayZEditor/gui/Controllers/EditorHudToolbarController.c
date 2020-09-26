class EditorHudToolbarController: Controller
{	
		
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	
	
	void EditorHudToolbarController()
	{
		EditorUIManager.CurrentEditorHudToolbarController = this;
	}
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		EditorLog.Trace("EditorHudToolbarController::OnMouseWheel");
		
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
				break;
			}
			
			case "BrushRadiusText":
			case "BrushRadiusSlider": {
				EditorUIManager.CurrentEditorHudController.BrushRadius += direction * 2;
				EditorUIManager.CurrentEditorHudController.BrushRadius = Math.Clamp(EditorUIManager.CurrentEditorHudController.BrushRadius, 0, 100);
				EditorUIManager.CurrentEditorHudController.NotifyPropertyChanged("BrushRadius");
				break;
			}
			
			case "BrushDensityText":
			case "BrushDensitySlider": {
				EditorUIManager.CurrentEditorHudController.BrushDensity += direction * 0.05;
				EditorUIManager.CurrentEditorHudController.BrushDensity = Math.Clamp(EditorUIManager.CurrentEditorHudController.BrushDensity, 0, 1);
				EditorUIManager.CurrentEditorHudController.NotifyPropertyChanged("BrushDensity");
				break;
			}			
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseEnter %1", w.GetName());		
		
		if (EditorUIManager.CurrentDialog && !EditorUIManager.IsDialogCommand(w)) {
			return false;
		}
		
		switch (w.GetName()) {
			
			case "UndoButton": {
				//EditorTooltip tooltip = new EditorTooltip(w);
				//tooltip.SetTitle("Undo");
				//EditorUIManager.CurrentTooltip = tooltip;
				break;
			}
			
		}
		
		switch (w.GetTypeName()) {
			
			case "ButtonWidget": {
				w.SetColor(COLOR_SALMON);				
				break;
			}
			
			case "SliderWidget": {
				w.SetColor(COLOR_SALMON);
				break;
			}
		}
		
		switch (w) {
			
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView: {
				
				if (EditorUIManager.CurrentMenu) {
					EditorUIManager.CurrentMenu = CreateToolbarMenu(w);
				}
				break;
			}	
		}
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseLeave %1", w.GetName());
		
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
		switch (w.GetTypeName()) {
			
			case "ButtonWidget": {
				if (!ButtonWidget.Cast(w).GetState())
					w.SetColor(COLOR_EMPTY);
				
				break;
			}

			case "SliderWidget": {
				w.SetColor(COLOR_WHITE_A);
				break;
			}
		}
		
		return false;
			
	}
	
		
	void MenuBarExecute(ButtonCommandArgs args) 
	{		
		EditorLog.Trace("EditorHudController::MenuBarExecute");
		if (!EditorUIManager.CurrentMenu) { //  GetMenu().Type() != GetBoundMenu(args.GetButtonWidget()) removed cause GetBoundMenu is gone
			EditorUIManager.CurrentMenu = CreateToolbarMenu(args.GetButtonWidget());
		} else {
			delete EditorUIManager.CurrentMenu;
		}
	}
	
	private EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudController::CreateToolbarMenu");	
		
		ref EditorMenu toolbar_menu;
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
	
}
