class EditorPlaceable: EditorNode
{
	Object CreateObject(vector transform[4], int flags = ECE_LOCAL)
	{
		Object object;
		if (m_UUID.Contains("\\") || m_UUID.Contains("/")) {
			object = GetGame().CreateStaticObjectUsingP3D(m_UUID, transform[3], transform[2].VectorToAngles(), 1.0, true);
		} else {
			object = GetGame().CreateObjectEx(m_UUID, transform[3], ECE_LOCAL);
		}
		
		
		
		object.SetTransform(transform);
		object.Update();
		return object;
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