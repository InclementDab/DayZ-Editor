[RegisterEditorCommand(EditorCopyPlaceableToClipboard)]
class EditorCopyPlaceableToClipboard: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorPlaceableListItem> data = Param1<EditorPlaceableListItem>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		GetGame().CopyToClipboard(data.param1.GetPlaceableItem().Type);
		return true;
	}
	
	override string GetName() 
	{
		return "Copy To Clipboard";
	}
}