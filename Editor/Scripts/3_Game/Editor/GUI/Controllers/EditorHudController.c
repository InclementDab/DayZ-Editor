class EditorHudController: ViewController
{
	string Version = Editor.Version;
	
	string OnlineSessionId;
	int OnlineUserCount, OnlineEntityCount;
	
	string Search;
	int JoinCode;
	string RotatingJoinCode;
	

	ref ObservableCollection<EditorNodeView> LeftListItems = new ObservableCollection<EditorNodeView>(this);
	ref ObservableCollection<EditorNodeView> RightListItems = new ObservableCollection<EditorNodeView>(this);
		
	ref ObservableCollection<ref EditorFileHeadView> OpenFiles = new ObservableCollection<ref EditorFileHeadView>(this);
		
	ScrollWidget LeftScroll;
	ScrollWidget RightScroll;	
	
	ref ObservableCollection<ref EditorBrushData> BrushTypeBoxData = new ObservableCollection<ref EditorBrushData>(this);

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		// Reload Placeables
		array<string> favorite_items = {};
		GetGame().GetProfileStringList("EditorFavoriteItems", favorite_items);
		
		float widest_x;				
		// Load Brushes		
		
		/*
		string brush_file = GetDayZGame().GetEditor().GetProfileSettings().EditorBrushFile;
		if (brush_file.Contains("'")) {
			// bi wtf
			brush_file.Replace("'", "");
			brush_file.Replace("\"", "");
			GetDayZGame().GetEditor().GetProfileSettings().EditorBrushFile = brush_file;
			GetDayZGame().GetEditor().GetProfileSettings().Save();
		}
		
		if (!FileExist(GetDayZGame().GetEditor().GetProfileSettings().EditorBrushFile)) {
			if (!CopyFile("Editor/scripts/data/Defaults/Brushes.xml", GetDayZGame().GetEditor().GetProfileSettings().EditorBrushFile)) {
				EditorLog.Error("Could not copy brush data to %1", GetDayZGame().GetEditor().GetProfileSettings().EditorBrushFile);
				return;
			}
		}*/
	}
				
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Search": {
				for (int i = 0; i < LeftListItems.Count(); i++) {
					for (int j = 0; j < LeftListItems[i].GetTemplateController().ChildrenItems.Count(); j++) {
						LeftListItems[i].GetTemplateController().ChildrenItems[j].ApplyFilter(Search);
					}
					
					LeftListItems[i].Children.Show(true);
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
				GetDayZGame().GetEditor().GetHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetDayZGame().GetEditor().GetPlacedObjects().Count() + " #STR_EDITOR_PLACEMENTS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case DeletionsTabButton: {
				GetDayZGame().GetEditor().GetHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("" + GetDayZGame().GetEditor().GetDeletedObjects().Count() + " #STR_EDITOR_DELETIONS", w, TooltipPositions.BOTTOM_LEFT));
				break;
			}
			
			case CinematicCameraButton: {
				GetDayZGame().GetEditor().GetHud().SetCurrentTooltip(EditorTooltip.CreateOnButton("#STR_EDITOR_CINEMATIC_CAMERA", w, TooltipPositions.TOP_LEFT));
				break;
			}
		}
				
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		//EditorLog.Trace("EditorHudController::OnMouseLeave");
		delete GetDayZGame().GetEditor().GetHud().CurrentTooltip;
		
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
}