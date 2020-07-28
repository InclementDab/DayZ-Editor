

class EditorBrush
{
	protected hDecal m_RadiusDecal;
	
	void EditorBrush()
	{
		Print("EditorBrush");
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);
	}
	
	void ~EditorBrush()
	{
		Print("~EditorBrush");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}
	
	void UpdateBrush()
	{
		if (Editor.IsPlacing()) return;
		
		
		set<Object> o;
		vector CurrentMousePosition = MousePosToRay(o);
		
		Input input = GetGame().GetInput();
		
		if (input.LocalPress("UAFire")) {
			Particle.Play(ParticleList.BRUSH_PARTICLE, Editor.ActiveCamera, CurrentMousePosition);
			
		}
		
		
		
	}
	
	
}