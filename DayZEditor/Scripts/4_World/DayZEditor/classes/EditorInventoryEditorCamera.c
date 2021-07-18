class EditorInventoryEditorCamera: Camera
{
	protected ref Timer m_LerpTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected Object m_Target;
	
	void EditorInventoryEditorCamera()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(OnFrame);
	}
	
	void ~EditorInventoryEditorCamera()
	{
		if (GetGame()) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(OnFrame);
		}
	}
	
	void SetTarget(Object target)
	{
		m_Target = target;
	}
	
	void OnFrame()
	{
		if (m_Target) {
			LookAt(m_Target.GetPosition() + "0 1 0");
		}
	}
	
	void LerpToPosition(vector target_position, float animation_time)
	{
		vector current_pos = GetGame().GetCurrentCameraPosition();
		m_LerpTimer.Run(0.01, this, "RunPositionLerp", new Param4<vector, vector, float, int>(current_pos, target_position, animation_time, GetGame().GetTime()), true);
	}	
	
	private void RunPositionLerp(vector start, vector finish, float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;
		SetPosition(Math.SmoothLerpVector(start, finish, tfactor));
		Update();		
		
		if (tfactor >= 1) {
			m_LerpTimer.Stop();
		}
	}
}