class EditorCamera: Camera
{
	static float UPDATE_ACCUMULATOR_INTERVAL = 0.5;
	
	protected float m_ServerUpdateAccumulator;
	float FieldOfView = 1.0;
	protected vector m_LinearVelocity, m_AngularVelocity;
		
	protected vector m_ViewDragDirection;
	protected vector m_ViewDragUp;
	protected vector m_ViewDragAside;
	protected vector m_ViewDragBasis[3];
	protected Object m_CursorObject;
	
	void EditorCamera()
	{
		SetEventMask(EntityEvent.FRAME);
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		if (GetDayZGame().IsLeftCtrlDown() || !IsActive()) {
			return;
		}
		
		Raycast raycast = PerformCursorRaycast();
		
		vector transform[4];
		GetTransform(transform);
		
		Input input = GetGame().GetInput();
	
		m_LinearVelocity += Vector(input.LocalValue_ID(UAMoveRight) 	- input.LocalValue_ID(UAMoveLeft), 
									input.LocalValue_ID(UAMoveUp) 		- input.LocalValue_ID(UAMoveDown), 
									input.LocalValue_ID(UAMoveForward) 	- input.LocalValue_ID(UAMoveBack)) * 18.5 * timeSlice * (1 + input.LocalValue_ID(UATurbo) * 2.5) * (1 - (input.LocalValue_ID(UALookAround) * 0.85));
		
		vector view_delta = vector.Forward;
		
			
		// Cursor is off, time to do regular camera things
		if (!GetGame().GetUIManager().IsCursorVisible()) {
			vector view = Vector(input.LocalValue_ID(UAAimRight) - input.LocalValue_ID(UAAimLeft), 
					input.LocalValue_ID(UAAimUp) - input.LocalValue_ID(UAAimDown), 
					0) * timeSlice * 5;
			
			// We need a delta vector created in camera coordinate space.
			// this is a vector that points forward towards the looking direction of the next frame
			view_delta = Vector(Math.Sin(view[0]), view[1], Math.Cos(view[0])).Normalized().Multiply3(transform);
			
			// Finally we construct a transform from our new view delta and move it by our linear velocity
			transform = { vector.Up * view_delta, vector.Up, view_delta, m_LinearVelocity.Multiply4(transform) };
		} 
		
		else if (input.LocalPress_ID(UATempRaiseWeapon)) {
			m_ViewDragAside = (GetGame().GetPointerDirection() * transform[1]).Normalized();
			m_ViewDragUp = transform[1];
			m_ViewDragDirection = GetGame().GetPointerDirection();
		}
		
		else if (input.LocalValue_ID(UATempRaiseWeapon)) {
			vector offset_matrix[3] = { m_ViewDragAside, m_ViewDragUp, m_ViewDragDirection };
			Math3D.MatrixOrthogonalize3(offset_matrix);
			
			vector pointer_dir = GetGame().GetPointerDirection().InvMultiply3(offset_matrix).Normalized();
			vector up_dir = transform[1].InvMultiply3(offset_matrix).Normalized();
			//Print(up_dir);
			
			vector test[3] = { pointer_dir, up_dir, up_dir * pointer_dir };			
			//Math3D.MatrixInvMultiply3(transform, test, transform);
			//view_delta = pointer_dir.Multiply3(transform);
			
			////Print(view_delta);
			//transform = { view_delta, vector.Up, view_delta * vector.Up, m_LinearVelocity.Multiply4(transform) };
			
			
			/*
			vector pointer_aside = GetGame().GetPointerDirection().InvMultiply3(offset_matrix2).Normalized();
			//Print(pointer_dir);
			Print(pointer_aside);
			//Print(pointer_aside * pointer_dir);
			vector movement_matrix[3] = { pointer_dir, vector.Up, pointer_dir * vector.Up };
			
			//Print(movement_matrix);
			Math3D.MatrixMultiply3(transform, movement_matrix, transform);*/
			
		} else {
			transform[3] = m_LinearVelocity.Multiply4(transform);
		}
		
		// Cannot allow camera to go through the floor
		transform[3][1] = Math.Max(transform[3][1], GetNearPlane() + GetGame().SurfaceY(transform[3][0], transform[3][2]));
		Math3D.MatrixOrthogonalize4(transform);		
		SetTransform(transform);	
		
		// Decay linear velocity
		m_LinearVelocity = m_LinearVelocity * 0.25;
		m_AngularVelocity = m_AngularVelocity * 0.5;
		
		// Set linear field of view
		float zoom_vel[1];		
		SetFOV(Math.SmoothCD(GetCurrentFOV(), FieldOfView, zoom_vel, 0.03, 1000.0, timeSlice));
	}
	
	Raycast PerformCursorRaycast(Object ignore = null)
	{		
		return GetCursorRay().PerformRaycast(ignore);
	}
	
	Ray GetCursorRay()
	{
		vector transform[4];
		GetTransform(transform);
		Ray ray = new Ray(transform[3], transform[2]);

		if (GetGame().GetUIManager().IsCursorVisible()) {
			ray.Direction = GetGame().GetPointerDirection();
		}
		
		return ray;
	}
	
	void LookAtMatrix(notnull Raycast raycast, out vector mat[4])
	{
		vector forward = ;
		vector aside = (vector.Up * forward);
		
	}
}