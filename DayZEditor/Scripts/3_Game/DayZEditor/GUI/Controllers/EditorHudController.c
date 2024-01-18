class EditorHudController: ViewController
{
	string Version = Editor.Version;
	
	string OnlineSessionId, OnlineEntityCount;
	int OnlineUserCount;
	
	string Search;
	
	StringEvaluater PrecisionLevel = "0.5";
	
	ref ObservableCollection<EditorTreeItem> LeftListItems = new ObservableCollection<EditorTreeItem>(this);
	ref ObservableCollection<EditorTreeItem> RightListItems = new ObservableCollection<EditorTreeItem>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
		
	// Camera Track
	protected Widget CameraTrackWrapper;
	protected ButtonWidget CameraTrackRunButton;
	protected Widget CameraTrackButtonOutline;
	
	// Preview for Object Readout
	TextWidget ObjectHoverSelectObjectReadout;
	
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>(this);

	float BrushRadius = 65;
	float BrushDensity = 0.25;
	
	bool BrushToggleButtonState;
	int BrushTypeSelection;
	string BrushToggleButtonText;
	
	bool ControlPlayerState;
	
	protected ImageWidget MagnetButton_Icon;
	protected ImageWidget GroundButton_Icon;
	protected ImageWidget SnapButton_Icon;
	protected ImageWidget CollisionButton_Icon;
	protected ImageWidget CameraLightButton_Icon;
	
	ButtonWidget BrushToggleButton;
		
	// Favorites
	protected ref array<string> m_FavoriteItems = {};
		
	void EditorHudController() 
	{
		// more hacking
		g_EditorPrecision = GetPrecisionLevel();
	}
			
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
		array<string> favorite_items = {};
		GetGame().GetProfileStringList("EditorFavoriteItems", favorite_items);
		
		float widest_x;				
		// Load Brushes		
		string brush_file = GetEditor().GeneralSettings.EditorBrushFile;
		if (brush_file.Contains("'")) {
			// bi wtf
			brush_file.Replace("'", "");
			brush_file.Replace("\"", "");
			GetEditor().GeneralSettings.EditorBrushFile = brush_file;
			GetEditor().GeneralSettings.Save();
		}
		
		if (!FileExist(GetEditor().GeneralSettings.EditorBrushFile)) {
			if (!CopyFile("DayZEditor/scripts/data/Defaults/Brushes.xml", GetEditor().GeneralSettings.EditorBrushFile)) {
				EditorLog.Error("Could not copy brush data to %1", GetEditor().GeneralSettings.EditorBrushFile);
				return;
			}
		}
				
		ReloadBrushes(GetEditor().GeneralSettings.EditorBrushFile);
	}
		
	float GetPrecisionLevel()
	{
		return PrecisionLevel.Parse();
	}
		
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Search": {
				for (int i = 0; i < LeftListItems.Count(); i++) {
					for (int j = 0; j < LeftListItems[i].GetTemplateController().Children.Count(); j++) {
						LeftListItems[i].GetTemplateController().Children[j].ApplyFilter(Search);
					}
					
					LeftListItems[i].ShowChildren(true);
				}
				
				break;
			}
		}
	}
	
	/*
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudController::OnMouseButtonDown");
		
		if (button == MouseState.RIGHT) {
		
			switch (w) {
				case LeftbarSearchBar: {
					SearchBarData = string.Empty;
					NotifyPropertyChanged("SearchBarData");
					break;
				}
				case PlacedSearchEditbox: {
					PlacedSearchBarData = string.Empty;
					NotifyPropertyChanged("PlacedSearchBarData");
					break;
				}
			}	
		}
	
		return super.OnMouseButtonDown(w, x, y, button);
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseEnter");

		switch (w) {
			case PlacementsTabButton: {
				GetEditor().GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetEditor().GetPlacedObjects().Count() + " #STR_EDITOR_PLACEMENTS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case DeletionsTabButton: {
				GetEditor().GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetEditor().GetDeletedObjects().Count() + " #STR_EDITOR_DELETIONS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case CinematicCameraButton: {
				GetEditor().GetEditorHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("#STR_EDITOR_CINEMATIC_CAMERA", w, TooltipPositions.TOP_LEFT));
				break;
			}
		}
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseLeave");
		delete GetEditor().GetEditorHud().CurrentTooltip;
		
		return super.OnMouseLeave(w, enterW, x, y);
	}*/
	
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (RecursiveGetParent(w, ScrollWidget)) {
			if (KeyState(KeyCode.KC_LCONTROL)) {
				ScrollWidget.Cast(w).VScrollStep(wheel * 10);
			}
		}
		
		return false;
	}
		
	// Brush Management
	void ReloadBrushes(string filename)
	{
		BrushToggleButtonState = false;
		NotifyPropertyChanged("BrushToggleButtonState");
		
		BrushTypeBoxData.Clear();
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes(BrushTypeBoxData);
		GetXMLApi().Read(filename, xml_brushes);
	}
}