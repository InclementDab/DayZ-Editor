class EditorHudToolbarController: Controller
{	
	// View Properties
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>("BrushTypeBoxData", this);
		
	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	
	protected Widget BrushRadiusFrame;
	protected Widget BrushDensityFrame;
	
	bool MagnetButton;
	bool GroundButton;
	bool SnapButton;
	
	void EditorHudToolbarController()
	{
		EditorUIManager.CurrentEditorHudToolbarController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
				
#ifndef COMPONENT_SYSTEM		
		// Load Brushes
		ReloadBrushes(GetEditor().EditorBrushFile);
#endif
	}
	
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorHudToolbarController::ReloadBrushes");
		BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes(BrushTypeBoxData);
		
		if (!FileExist(filename)) {
			EditorLog.Error("File not found: " + filename);
			return;
		}
	
		GetXMLApi().Read(filename, xml_brushes);
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorHudToolbarController::PropertyChanged %1", property_name);
		switch (property_name) {
			
			case "BrushToggleButtonState":
			case "BrushTypeSelection": {
				BrushRadiusFrame.Show(BrushToggleButtonState);
				BrushDensityFrame.Show(BrushToggleButtonState);
			
				BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
				NotifyPropertyChanged("BrushToggleButtonText");

				if (BrushToggleButtonState && GetEditor()) {
					GetEditor().SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
				} else {
					GetEditor().SetBrush(null);
				}
				break;
			}
			
			case "BrushRadius":
			case "BrushDensity": {
				EditorBrush.SetRadius(BrushRadius);
				EditorBrush.SetDensity(BrushDensity);
				break;
			}
		}
	}
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::CollectionChanged: " + collection_name);
		switch (collection_name) {
			
			case "BrushTypeBoxData": {
				BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
				NotifyPropertyChanged("BrushToggleButtonText");
				break;
			}
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
				BrushRadius += direction * 2;
				BrushRadius = Math.Clamp(BrushRadius, 0, 100);
				NotifyPropertyChanged("BrushRadius");
				break;
			}
			
			case "BrushDensityText":
			case "BrushDensitySlider": {
				BrushDensity += direction * 0.05;
				BrushDensity = Math.Clamp(BrushDensity, 0, 1);
				NotifyPropertyChanged("BrushDensity");
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
					delete EditorUIManager.CurrentMenu;
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
	
		
	void BrushToggleButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::BrushToggleButtonExecute");
		
		switch (args.GetMouseButton()) {
			
			case 0: {
				bool button_state = args.GetButtonState();
				args.Source.FindAnyWidget("BrushToggleButtonText").SetPos(button_state * 1, button_state * 1);
				break;
			}
			
			case 1: {
				EditorBrushDialog brush_dialog();
				brush_dialog.SetEditorBrushData(BrushTypeBoxData[BrushTypeSelection]);
				brush_dialog.ShowDialog();
				break;
			}
		}
	}
	
	void CutButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::CutButtonExecute");
		EditorCutCommand cmd = new EditorCutCommand();
		cmd.Execute(this, null);
	}
	
	void CopyButtonExecute(ButtonCommandArgs args) 
	{
		EditorLog.Trace("EditorHudToolbarController::CopyButtonExecute");
		EditorCopyCommand cmd = new EditorCopyCommand();
		cmd.Execute(this, null);
	}
	
	void PasteButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::PasteButtonExecute");
		EditorPasteCommand cmd = new EditorPasteCommand();
		cmd.Execute(this, null);
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
	
}
