

class DSLRCamera: EntityAI
{
	void DSLRCamera()
	{
		EditorLog.Trace("DSLRCamera");
		if (IsMissionClient()) {
			EditorCamera camera = GetEditor().GetCamera();
			camera.AddChild(this, -1, true);
		}
	}
	
	
}