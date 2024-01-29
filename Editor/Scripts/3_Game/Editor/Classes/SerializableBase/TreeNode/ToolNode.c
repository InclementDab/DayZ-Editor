class ToolNode: TreeNode
{
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		if (state) {
			GetDayZGame().GetEditor().Tool = this;
		} else {
			GetDayZGame().GetEditor().Tool = null;
		}
	}
	
	// Called in editor when running, return TRUE when you want editor to continue processing inputs
	bool Update(float dt, Raycast raycast)
	{		
		if (raycast.Bounce != Raycast.INVALID) {
			raycast.Debug();
			Shape.CreateSphere(COLOR_GREEN, ShapeFlags.ONCE, raycast.Bounce.Position, 0.5);
		}
		
		return true;
	}
}