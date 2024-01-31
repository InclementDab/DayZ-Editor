class PianoCommand: CommandNode
{
	protected Object m_PianoInstance;
	
	override void Update(float dt)
	{
		if (!m_TreeNodeState.IsActive()) {
			return;
		}
		
		Raycast raycast = GetEditor().GetCamera().PerformCursorRaycast();
		if (!raycast) {
			return;
		}
		
		
	}
	
	protected void SpawnPianoDrop(vector position)
	{
		GetEditor().PlaySound(EditorSounds.PIANO_DROP);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SpawnPiano, 3900, false, position + "0 30 0");
	}
	
	protected void SpawnPiano(vector position)
	{
		m_PianoInstance = GetGame().CreateStaticObjectUsingP3D("DZ\\structures\\furniture\\Decoration\\piano\\piano.p3d", position, vector.Zero, 1.0, true);
		
	}
		
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeInteract.PRESS;
	}
}