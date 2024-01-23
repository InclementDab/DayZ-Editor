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
					
					//LeftListItems[i].Children.Show(true);
				}
				
				break;
			}
		}
	}
		
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