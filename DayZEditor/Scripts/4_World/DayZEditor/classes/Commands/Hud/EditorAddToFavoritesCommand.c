class EditorAddToFavoritesCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorPlaceableItem> data = Param1<EditorPlaceableItem>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		GetEditor().GetEditorHud().SetFavoriteState(data.param1, true);
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_ADD_TO_FAVORITES";
	}
}