
class EditorHudToolbarController: EditorControllerBase
{
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;

	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	// View Properties
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	protected ButtonWidget MenuBarEditor;
	
	protected ImageWidget MagnetButton_Icon;
	protected ImageWidget GroundButton_Icon;
	protected ImageWidget SnapButton_Icon;
	protected ImageWidget CollisionButton_Icon;
	
	protected ButtonWidget BrushToggleButton;
			
	void EditorHudToolbarController()
	{
		Debug_Logging = true;
		EditorUIManager.CurrentEditorHudToolbarController = this;
	}
	
	void ~EditorHudToolbarController()
	{
		delete BrushTypeBoxData;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>(this);

		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
#ifndef COMPONENT_SYSTEM
		// Load Brushes		
		if (!FileExist(m_Editor.EditorBrushFile)) {
			if (!CopyFile("DayZEditor/scripts/data/Defaults/EditorBrushes.xml", m_Editor.EditorBrushFile)) {
				EditorLog.Error("Could not copy brush data to %1", m_Editor.EditorBrushFile);
				return;
			}
		}
		
		ReloadBrushes(m_Editor.EditorBrushFile);
#endif		
	}
	
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorHudToolbarController::ReloadBrushes");
		BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(BrushTypeBoxData);
		GetXMLApi().Read(filename, xml_brushes);
	}
	
	override void PropertyChanged(string property_name)
	{
		//EditorLog.Trace("EditorHudToolbarController::PropertyChanged %1", property_name);
		switch (property_name) {
			
			case "BrushToggleButtonState":
			case "BrushTypeSelection": {
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
								
				m_Editor.CommandManager.BrushToggleCommand.Execute(this, new ButtonCommandArgs(BrushToggleButton, 0));

				break;
			}
			
			case "BrushRadius":
			case "BrushDensity": {
				EditorBrush.SetRadius(BrushRadius / 2);
				EditorBrush.SetDensity(BrushDensity);
				break;
			}
			
			case "m_Editor.MagnetMode": {
				
				if (m_Editor.MagnetMode) {
					MagnetButton_Icon.SetColor(COLOR_CANDY);
				} else {
					MagnetButton_Icon.SetColor(COLOR_WHITE);
				}
				break;
			}
			case "m_Editor.GroundMode": {
				if (m_Editor.GroundMode) {
					GroundButton_Icon.SetColor(COLOR_APPLE);
				} else {
					GroundButton_Icon.SetColor(COLOR_WHITE);
				}
				
				break;
			}
			case "m_Editor.SnappingMode": {
				if (m_Editor.SnappingMode) {
					SnapButton_Icon.SetColor(COLOR_JELLY);
				} else {
					SnapButton_Icon.SetColor(COLOR_WHITE);
				}
				
				break;
			}
			case "m_Editor.CollisionMode": {
				if (m_Editor.CollisionMode) {
					CollisionButton_Icon.SetColor(COLOR_PALE_B);
				} else {
					CollisionButton_Icon.SetColor(COLOR_WHITE);
				}
					
				break;
			}
		}
	}
	
	private int GetHighlightColor()
	{
		
	}
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		EditorLog.Trace("EditorHudToolbarController::CollectionChanged: " + collection_name);
		switch (collection_name) {
			
			case "BrushTypeBoxData": {
				
				m_Editor.CommandManager.BrushToggleCommand.SetCanExecute(args.Source.Count() > 0);
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
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
			return super.OnMouseEnter(w, x, y);
		}
		
		
		switch (w.GetTypeName()) {
			
			case "SliderWidget": {
				w.SetColor(COLOR_SALMON);
				break;
			}
		}
		
		ViewBinding view_binding = GetViewBinding(w);
		if (view_binding && !EditorUIManager.CurrentMenu) {
			EditorCommand editor_command;
			if (Class.CastTo(editor_command, view_binding.GetRelayCommand())) {
				
				float pos_x, pos_y, size_x, size_y;
				w.GetScreenPos(pos_x, pos_y);
				w.GetScreenSize(size_x, size_y);
								
				EditorCommandTooltip tooltip = new EditorCommandTooltip(editor_command, pos_x, pos_y + size_y);
				if (!editor_command.CanExecute()) {
					tooltip.GetLayoutRoot().SetAlpha(100);
				}
				
				EditorUIManager.SetCurrentTooltip(tooltip);
				
			}
		}
		
		switch (w) {
			
			case MenuBarFile:
			case MenuBarEdit:
			case MenuBarView:
			case MenuBarEditor: {
				
				if (EditorUIManager.CurrentMenu) {
					delete EditorUIManager.CurrentMenu;
					EditorUIManager.CurrentMenu = CreateToolbarMenu(w);
				}
				
				break;
			}	
			
		}
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseLeave %1", w.GetName());
		
		EditorUIManager.SetCurrentTooltip(null);
		
		switch (w.GetTypeName()) {
		
			case "SliderWidget": {
				w.SetColor(COLOR_WHITE_A);
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
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
	
	private ref EditorMenu CreateToolbarMenu(Widget toolbar_button)
	{
		EditorLog.Trace("EditorHudToolbarController::CreateToolbarMenu");	
				
		EditorMenu toolbar_menu;
		switch (toolbar_button) {
			
			case MenuBarFile: {
				toolbar_menu = new EditorFileMenu();
				break;
			}
			
			case MenuBarEdit: {
				toolbar_menu = new EditorEditMenu();
				break;
			}
			
			case MenuBarView: {
				toolbar_menu = new EditorViewMenu();
				break;
			}
			
			case MenuBarEditor: {
				toolbar_menu = new EditorEditorMenu();
				break;
			}
		}
		
		// Sets position to bottom of button
		float x, y, w, h;
		toolbar_button.GetScreenPos(x, y);
		toolbar_button.GetScreenSize(w, h);
		toolbar_menu.SetPos(x, y + h);
		
		return toolbar_menu;
	}
		
}
