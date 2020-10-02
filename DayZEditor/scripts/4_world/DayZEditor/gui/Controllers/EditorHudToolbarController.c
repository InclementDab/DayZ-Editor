class EditorHudToolbarController: EditorControllerBase
{	
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData;

	float BrushRadius = 50;
	float BrushDensity = 0.5;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	bool MagnetButton;
	bool GroundButton;
	bool SnapButton;
	
	protected EditorCommand m_NewCommand;
	protected EditorCommand m_OpenCommand;
	protected EditorCommand m_SaveCommand;
	protected EditorCommand m_SaveAsCommand;
	protected EditorCommand m_CloseCommand;
	
	protected EditorCommand m_UndoCommand;
	protected EditorCommand m_RedoCommand;
	
	protected EditorCommand m_CutCommand;
	protected EditorCommand m_CopyCommand;
	protected EditorCommand m_PasteCommand;
	
	// View Properties
	protected ButtonWidget MenuBarFile;
	protected ButtonWidget MenuBarEdit;
	protected ButtonWidget MenuBarView;
	
	protected Widget BrushRadiusFrame;
	protected Widget BrushDensityFrame;
	
	void EditorHudToolbarController()
	{
		EditorUIManager.CurrentEditorHudToolbarController = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>("BrushTypeBoxData", this);

//#ifndef COMPONENT_SYSTEM
		// Load Brushes
		ReloadBrushes("$profile:Editor/EditorBrushes.xml");
//#endif
		
		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
		if (m_Editor) {
			m_NewCommand = m_Editor.NewCommand;
			m_OpenCommand = m_Editor.OpenCommand;
			m_SaveCommand = m_Editor.SaveCommand;
			m_SaveAsCommand = m_Editor.SaveAsCommand;
			m_CloseCommand = m_Editor.CloseCommand;
			
			m_UndoCommand = m_Editor.UndoCommand;
			m_RedoCommand = m_Editor.RedoCommand;
			
			m_CutCommand = m_Editor.CutCommand;
			m_CopyCommand = m_Editor.CopyCommand;
			m_PasteCommand = m_Editor.PasteCommand;
		}
	}
	
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorHudToolbarController::ReloadBrushes");
		BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(BrushTypeBoxData);
		
		if (!FileExist(filename)) {
			EditorLog.Error("File not found: " + filename);
			return;
		}
	
		GetXMLApi().Read(filename, xml_brushes);
	}
	
	override void PropertyChanged(string property_name)
	{
		//EditorLog.Trace("EditorHudToolbarController::PropertyChanged %1", property_name);
		switch (property_name) {
			
			case "BrushToggleButtonState":
			case "BrushTypeSelection": {
				BrushRadiusFrame.Show(BrushToggleButtonState);
				BrushDensityFrame.Show(BrushToggleButtonState);
				
				if (BrushTypeSelection < BrushTypeBoxData.Count()) {
					BrushToggleButtonText = BrushTypeBoxData[BrushTypeSelection].Name;
					NotifyPropertyChanged("BrushToggleButtonText", false);
				}
				
				if (m_Editor) {
					if (BrushToggleButtonState) {
						m_Editor.SetBrush(EditorBrush.Create(BrushTypeBoxData[BrushTypeSelection]));
					} else {
						m_Editor.SetBrush(null);
					}
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
	
	private int BUTTON_COLOR = -4235425;
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseEnter %1", w.GetName());		
		
		if (EditorUIManager.CurrentDialog && !EditorUIManager.IsDialogCommand(w)) {
			return super.OnMouseEnter(w, x, y);
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
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudToolbarController::OnMouseLeave %1", w.GetName());
		
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
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
				EditorBrushPropertiesCommand cmd = new EditorBrushPropertiesCommand();
				cmd.Execute(this, null);
				break;
			}
		}
	}
	
	
	private EditorMenu CreateToolbarMenu(Widget toolbar_button)
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
		}
		
		toolbar_menu.SetParent(toolbar_button);
		
		// Sets position to bottom of button
		float w, h;
		toolbar_button.GetScreenSize(w, h);
		toolbar_menu.SetPosition(0, h);

		return toolbar_menu;
	}
		
}
