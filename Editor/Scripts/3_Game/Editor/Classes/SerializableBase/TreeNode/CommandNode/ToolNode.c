class ToolNode: CommandNode
{
	// Called in editor when running, return TRUE when you want editor to continue processing inputs
	bool Update(float dt, Raycast raycast)
	{
		if (raycast && raycast.Bounce) {
			raycast.Debug();
			if (raycast.Hit) {
				TreeNode node = GetEditor().FindNodeFromObject(raycast.Hit);
				if (node) {
					Print(node);
				}
			}
		}	
		return true;
	}
	
	void OnEnterNode(notnull TreeNode node)
	{
		GetEditor().GetHud().SetCursor(m_Icon);
	}
	
	void OnExitNode(notnull TreeNode node)
	{
		GetEditor().GetHud().ClearCursor();
	}
}