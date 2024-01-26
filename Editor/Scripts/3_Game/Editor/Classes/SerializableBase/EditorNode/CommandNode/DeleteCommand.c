[RegisterCommand(DeleteCommand, "Delete Selected Nodes", Symbols.TRASH_CAN)]
class DeleteCommand: EditorNode
{
	override void SetSelected(bool state) 
	{
		Print("Cringe"); 
		foreach (EditorNode node: EditorNode.SelectedObjects) {
			Print(node);	
			delete node;
		}
	}
}