class EditorHudToolbarController: Controller
{

	
	
	
	void EditorHudToolbarController()
	{
		EditorUIManager.CurrentEditorHudToolbarController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		
		

	}
	
	
	
	
	override void PropertyChanged(string property_name)
	{
		
		switch (property_name) {
			
		}
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
		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseLeave %1", w.GetName());
		
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
	
	
}
