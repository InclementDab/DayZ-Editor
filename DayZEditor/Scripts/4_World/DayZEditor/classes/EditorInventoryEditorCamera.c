class EditorInventoryEditorCamera: Camera
{
	protected ref Timer m_LerpTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected Object m_Target;
	
	void EditorInventoryEditorCamera()
	{
		SetEventMask(EntityEvent.FRAME);
	}
	
	void SetTarget(Object target)
	{
		m_Target = target;
	}
		
	override void EOnFrame(IEntity other, float timeSlice)
	{
		if (!m_Target) {
			return;
		}
		
		vector target_pos = m_Target.GetPosition();
		
		Input input = GetGame().GetInput();
		float zoom = input.LocalValue("EditorCameraForward") - input.LocalValue("EditorCameraBack");
		float strafe = input.LocalValue("EditorCameraLeft") - input.LocalValue("EditorCameraRight");
		
		vector dir = vector.Direction(GetPosition(), target_pos);
		dir.Normalize();
		SetPosition(GetPosition() + (dir * zoom * timeSlice));
				
		vector position = EditorMath.RotateAroundPoint(target_pos, GetPosition(), vector.Up, Math.Cos(strafe * timeSlice), Math.Sin(strafe * timeSlice));
		
		SetPosition(position);
		
		// Last thing
		LookAt(target_pos + "0 1 0");
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