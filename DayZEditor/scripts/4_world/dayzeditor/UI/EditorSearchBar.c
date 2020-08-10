class PlaceableSearchBar: EditorWidgetEventHandler
{

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("PlaceableSearchBar::OnChange");
		
		
		WrapSpacerWidget spacer = WrapSpacerWidget.Cast(GetEditor().GetUIManager().GetEditorUI().GetRoot().FindAnyWidget("LeftbarSpacer"));
		string filter = EditBoxWidget.Cast(w).GetText();
		Widget child = spacer.GetChildren();
		while (child != null) {
			
			EditorListItem editor_list_item;
			child.GetScript(editor_list_item);
			
			if (editor_list_item != null) {
				string Config_Lower = editor_list_item.GetPlaceableObject().GetType();
				Config_Lower.ToLower();
				if (filter == "") return false;
	           	child.Show(Config_Lower.Contains(filter));				
			}
						
			child = child.GetSibling();

			
		}
		
		return true;
	}
	
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocus");
		
		GetEditor().GetUIManager().GetEditorCamera().SetMoveEnabled(false);
		
		return super.OnFocus(w, x, y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocusLost");
		
		GetEditor().GetUIManager().GetEditorCamera().SetMoveEnabled(true);
		
		return super.OnFocusLost(w, x, y);
	}
	
}
