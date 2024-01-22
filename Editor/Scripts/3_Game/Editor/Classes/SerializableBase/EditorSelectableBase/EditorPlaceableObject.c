class EditorPlaceable: EditorNode
{
	Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		return null;
	}

	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			vector transform[4];
			Math3D.MatrixIdentity4(transform);
			
			
			Ray ray = GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast();
			transform[3] = ray.Position;
			
			GetDayZGame().GetEditor().Placing[CreateObject(transform)] = new EditorHandData();
		}
	}
}