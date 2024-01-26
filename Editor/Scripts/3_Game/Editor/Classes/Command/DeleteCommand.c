[RegisterCommand(DeleteCommand, "Delete Selected Nodes", Symbols.TRASH_CAN)]
class DeleteCommand: Command
{
	override void Execute(bool state) 
	{
		Print("Cringe"); 
		foreach (EditorNode node: EditorNode.SelectedObjects) {
			Print(node);	
			delete node;
		}
	}
}