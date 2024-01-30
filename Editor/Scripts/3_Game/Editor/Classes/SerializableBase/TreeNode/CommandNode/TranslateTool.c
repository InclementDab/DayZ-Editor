class TranslateTool: CommandNode
{
	protected vector m_CursorAside = vector.Aside;
	protected ref Raycast m_StartPosition;
	
	override bool Update(float dt, Raycast raycast)
	{
		
	}
	
	override array<string> GetXorSelections()
	{
		return { "Scale", "Rotate" };
	}
}