class EditorAddObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		EditorPlaceableObjectData placeable_object = GetEditor().GetReplaceableItem(Editor.ObjectUnderCursor);
		if (placeable_object) {
			GetEditor().ClearHand();
			EditorHandMap objects_in_hand = GetEditor().AddInHand(placeable_object);
			foreach (Object object_in_hand, EditorHandData hand_data: objects_in_hand) {
				object_in_hand.SetOrientation(Editor.ObjectUnderCursor.GetOrientation());
			}
			
			return true;
		}
		
		return false;
	}
	
	override string GetName()
	{
		return "Add Object";
	}
}