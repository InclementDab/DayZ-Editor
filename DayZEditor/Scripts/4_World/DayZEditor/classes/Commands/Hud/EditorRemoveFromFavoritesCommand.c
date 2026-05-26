class EditorRemoveFromFavoritesCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorPlaceableItem> data = Param1<EditorPlaceableItem>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		GetEditor().GetEditorHud().SetFavoriteState(data.param1, false);
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_REMOVE_FROM_FAVORITES";
	}
}