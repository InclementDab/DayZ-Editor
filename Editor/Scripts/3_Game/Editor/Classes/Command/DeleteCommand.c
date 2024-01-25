[RegisterCommand(DeleteCommand)]
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