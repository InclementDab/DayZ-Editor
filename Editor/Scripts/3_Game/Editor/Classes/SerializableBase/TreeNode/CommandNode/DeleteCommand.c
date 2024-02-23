class DeleteCommand: NamedNode
{
	override void OnInteract(NodeInteractType interact_type)
	{
		super.OnInteract(interact_type);
		
		if (interact_type & NodeInteractType.PRESS) {
			DaysBefore.GetEditor().InsertHistory(null, CreateCopy());
			GetParent().Remove(GetUUID());
			GetParent().SetSynchDirty();
			DaysBefore.GetEditor().PlaySound(EditorSounds.HIGHLIGHT);
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS;
	}
}