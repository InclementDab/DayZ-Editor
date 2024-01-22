class EditorCamera: ScriptedCamera
{
	static float UPDATE_ACCUMULATOR_INTERVAL = 0.5;

	protected float m_ServerUpdateAccumulator;
	protected vector m_LinearVelocity, m_AngularVelocity;
	
	protected vector m_ViewDragStartPosition;
	protected vector m_ViewDragBasis[4];
	
	void EditorCamera()
	{
		SetEventMask(EntityEvent.FRAME);
	}

	Ray PerformCursorRaycast(Object ignore = null)
	{
		Ray cursor_ray = GetCursorRay();
		Ray output_ray = new Ray();
		
		float fraction;
		if (!DayZPhysics.RayCastBullet(cursor_ray.Position, cursor_ray.Position + cursor_ray.Direction * 1000.0, PhxInteractionLayers.TERRAIN, ignore, null, output_ray.Position, output_ray.Direction, fraction)) {
			return Ray.INVALID;
		}
		
		return output_ray;
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
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		// We are holding control down, essentially
		if (GetDayZGame().GetEditor().IsProcessingCommand()) {
			return;
		}
		
		vector transform[4];
		GetTransform(transform);
		
		Input input = GetGame().GetInput();
		m_LinearVelocity += Vector(input.LocalValue_ID(UAMoveRight) 	- input.LocalValue_ID(UAMoveLeft), 
									input.LocalValue_ID(UAMoveUp) 		- input.LocalValue_ID(UAMoveDown), 
									input.LocalValue_ID(UAMoveForward) 	- input.LocalValue_ID(UAMoveBack)) * timeSlice * 5.0 * (1 + input.LocalValue_ID(UATurbo) * 5.0);
		
		vector view_delta = vector.Forward;
		
		Ray cursor_ray = GetCursorRay();
		
		// Cursor is off, time to do regular camera things
		if (!GetGame().GetUIManager().IsCursorVisible()) {
			vector view = Vector(input.LocalValue_ID(UAAimRight) - input.LocalValue_ID(UAAimLeft), 
					input.LocalValue_ID(UAAimUp) - input.LocalValue_ID(UAAimDown), 
					0) * timeSlice;
			
			// We need a delta vector created in camera coordinate space.
			// this is a vector that points forward towards the looking direction of the next frame
			view_delta = Vector(Math.Sin(view[0]), view[1], Math.Cos(view[0])).Normalized().Multiply3(transform);
			
			// Finally we construct a transform from our new view delta and move it by our linear velocity
			transform = { vector.Up * view_delta, vector.Up, view_delta, m_LinearVelocity.Multiply4(transform) };
		} 
		
		else if (input.LocalPress_ID(UATempRaiseWeapon)) {
			GetTransform(m_ViewDragBasis);
		}
		
		else if (input.LocalValue_ID(UATempRaiseWeapon)) {		
			vector start_matrix[4] = { m_ViewDragStartPosition * vector.Up, vector.Up, m_ViewDragStartPosition, Vector(0, 0, 1).Multiply4(transform) };
			
			vector pointer = GetGame().GetPointerDirection().InvMultiply3(m_ViewDragBasis);
			Shape.CreateSphere(COLOR_GREEN, ShapeFlags.ONCE, pointer + transform[3], 0.2);
			
			vector mid_matrix[4] = { transform[0], transform[1], transform[2], transform[3] };
			
			pointer = pointer.Multiply3(transform);
			pointer.Normalize();
			vector matrix[4] = { pointer * m_ViewDragBasis[1], m_ViewDragBasis[1], pointer, Vector(0, 0, 1).Multiply4(transform) };
			Math3D.MatrixOrthogonalize3(matrix);
			
			//Shape.CreateMatrix(start_matrix);
			Shape.CreateMatrix(matrix);
			
			
			
			vector camera_matrix[4];
			GetTransform(camera_matrix);
			camera_matrix[3] = Vector(0, 0, 1).Multiply4(camera_matrix);
			
			Math3D.MatrixMultiply3(camera_matrix, matrix, camera_matrix);
			//Math3D.MatrixMultiply3(camera_matrix, start_matrix, camera_matrix);
			
			Shape.CreateMatrix(camera_matrix);
			
			//Math3D.MatrixMultiply3(matrix, m_ViewDragStartMat, matrix);
			
			
			//pointer = pointer.InvMultiply3(transform);
			//Debug.DrawSphere(pointer, 0.2, COLOR_WHITE, ShapeFlags.ONCE);
			
		
			//
			//transform = { vector.Up * pointer, vector.Up, pointer, m_LinearVelocity.Multiply4(transform) };
			
			vector u[4] = { vector.Aside, vector.Up, vector.Forward, vector.Zero };
			
			//Shape.CreateMatrix(x);
			//Math3D.MatrixInvMultiply3(x, u, x);
			//Math3D.MatrixMultiply3(transform, x, transform);
			
			
		} else {
			transform[3] = m_LinearVelocity.Multiply4(transform);
		}
			
		
		transform[3][1] = Math.Max(transform[3][1], GetNearPlane() + GetGame().SurfaceY(transform[3][0], transform[3][2]));
		Math3D.MatrixOrthogonalize4(transform);		
		SetTransform(transform);	
		
		// Decay linear velocity
		m_LinearVelocity = m_LinearVelocity * Math.Pow(GetDayZGame().GetEditor().GetProfileSettings().Smoothing, 2);
		m_AngularVelocity = m_AngularVelocity * 0.5;
	}
}
/*
// make option Q and E go up and down no matter orientation
class EditorCamera: ScriptedCamera
{
	static const float TELEPORT_LERP_DISTANCE = 1000;
	
	//protected EditorCameraLight m_EditorCameraLight;
	
	float FOV = 1;
	float DOFDistance;
	float DOFBlur;
	float Blur;
	float Vignette;
	float Sharpness;
	float NearPlane;
	float Exposure;
	
	float Smoothing = 0;
	
	float Speed = GetDayZGame().GetEditor().GeneralSettings.CameraSpeed;
	float Boost_Multiplier = 6.5;
	float Drag = 0.05;
	float Mouse_Sens = 35.0;
	
	float SendUpdateAccumalator = 0.0;
	
	bool LookEnabled = true;
	bool MoveEnabled = true;

	Object SelectedTarget;
	vector TargetPosition;
	vector linearVelocity;
	vector angularVelocity;
	vector orientation;
	
	int ColorCorrection = COLOR_WHITE;
	
	bool HideCursorOnDrag;
	
	void EditorCamera()
	{
		EditorLog.Trace("EditorCamera");
		SetEventMask(EntityEvent.FRAME);
		SelectTarget(null);
		
		NearPlane = GetNearPlane();
		Exposure = GetGame().GetWorld().GetEyeAccom();
	}

	void ~EditorCamera()
	{
		SelectTarget(null);
	}
	
	void SetLightState(bool state)
	{
		if (state) {
			//m_EditorCameraLight = EditorCameraLight.Cast(ScriptedLightBase.CreateLight(EditorCameraLight, GetPosition(), 0.2));
			//m_EditorCameraLight.SetDirection(GetDirection());
			//m_EditorCameraLight.AttachOnObject(this);
		} else {
			//GetGame().ObjectDelete(m_EditorCameraLight);
		}
	}
	
	// Safe and easy position set, used for teleporting
	void SendToPosition(vector position)
	{
		if (vector.Distance(position, GetPosition()) > TELEPORT_LERP_DISTANCE) {
			SetPosition(position);
			Update();
		} else {
			LerpCameraPosition(position, 0.1);
		}
	}
	
	override void OnSelectCamera()
	{
		super.OnSelectCamera();
		
		MoveEnabled = true;
		LookEnabled = true;
		
		// literally just for startup
		if (GetDayZGame().GetEditor().GetHud()) {
			GetDayZGame().GetEditor().GetHud().Show(true);
		}
	}

	void OnTargetSelected( Object target )
	{
		EditorLog.Trace("OnTargetSelected");
	}

	void OnTargetDeselected(Object target)
	{
		EditorLog.Trace("OnTargetDeselected");
	}

	private bool IsTargeting = false;
	void SelectTarget(Object target)
	{
		EditorLog.Trace("ActiveCamera::SelectTarget");
		if (target != SelectedTarget && target != null) {
			TargetPosition = target.GetPosition();
			IsTargeting = true;
			OnTargetSelected(target);
			
		} else if (target == null) {
			TargetPosition = vector.Zero;
			IsTargeting = false;
			OnTargetDeselected(SelectedTarget);
			
		}

		SelectedTarget = target;
	}
		
	override void EOnFrame(IEntity other, float timeSlice)
	{
		if (GetDayZGame().GetEditor().GetCurrentControl() != this) {
			return;
		}
				
		vector original_position_unchanged;
		vector transform[4];
		GetTransform(transform);
		original_position_unchanged = transform[3];

		Input input = GetGame().GetInput();
		if (GetFocus() && GetFocus().IsInherited(EditBoxWidget)) {
			return;
		}
		
		// teleportation logic
		if (input.LocalPress("UAZoomIn")) {	
			vector mouse_pos = Vector(Editor.CurrentMousePosition[0], GetGame().SurfaceY(Editor.CurrentMousePosition[0], Editor.CurrentMousePosition[2]), Editor.CurrentMousePosition[2]);
			vector camera_current_pos = GetPosition();
			float camera_surface_y = GetGame().SurfaceY(camera_current_pos[0], camera_current_pos[2]);
			
			// check if water is under mouse, to stop from teleporting under water			
			if (GetDayZGame().GetEditor().IsSurfaceWater(mouse_pos)) {
				SendToPosition(Vector(mouse_pos[0],  camera_current_pos[1], mouse_pos[2]));
			} else {
				SendToPosition(Vector(mouse_pos[0],  mouse_pos[1] + camera_current_pos[1] - camera_surface_y, mouse_pos[2]));
			}
		}
		
		if (!KeyState(KeyCode.KC_LCONTROL)) {
			float forward = input.LocalValue("EditorCameraForward") - input.LocalValue("EditorCameraBack");
			float strafe = input.LocalValue("EditorCameraRight") - input.LocalValue("EditorCameraLeft");
			float altitude = input.LocalValue("EditorCameraUp") - input.LocalValue("EditorCameraDown");
		}
		
		float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
		float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");
		float zoomAmt = input.LocalValue("EditorCameraZoomIn") - input.LocalValue("EditorCameraZoomOut");
		
		if (KeyState(KeyCode.KC_C)) { //C = CameraSpeed modifier
			float speedInc = 0;
			
			if (input.LocalValue("EditorCameraToolSpeedIncrease" )) {
				speedInc = input.LocalValue("EditorCameraToolSpeedIncrease") * 0 + 10;
			}
			
			if (input.LocalValue("EditorCameraToolSpeedDecrease" )) {
				speedInc = input.LocalValue("EditorCameraToolSpeedDecrease") * 0 + -10;			
			}		
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			zoomAmt *= 0.1;
		} else {
			zoomAmt *= 0.01;
		}
		
		vector current_position = GetPosition();
		float current_altitude = current_position[1] - GetGame().SurfaceY(current_position[0], current_position[2]);
		
		if (zoomAmt != 0)
			speedInc = 0;

		bool shouldRoll = false;
		bool decreaseSpeeds = input.LocalValue("EditorCameraSlow");
		bool increaseSpeeds = input.LocalValue("EditorCameraTurbo");
		
		FOV = Math.Clamp(GetCurrentFOV() + zoomAmt, 0.01, 5);
		SetFOV(FOV);

		if (input.LocalValue("EditorCameraZoomReset")) {
			SetFOV(1);
		}
		
		if (MoveEnabled && !g_Game.GetMission().IsPaused()) {
						
			if (Boost_Multiplier > 0) {
				Speed += speedInc;
				
				//speed can become negative so stop that
				if (Speed < 0) {
					Speed = 2;
				}
				
				//speed has no uper limit
				if (Speed > 500) {
					Speed = 500;
				}
				
				float cam_speed = Speed;
				if (decreaseSpeeds) {
					cam_speed = cam_speed * 0.1;	
				}

				if (increaseSpeeds) {
					cam_speed = (cam_speed * Boost_Multiplier) * (0.2 + (transform[3][1])/600) ;
				}
			}
			
			linearVelocity = linearVelocity * Smoothing;
			float temp_cam_speed = Math.Lerp(cam_speed, cam_speed * 0.01, Smoothing);
			linearVelocity = linearVelocity + (transform[0] * strafe * temp_cam_speed);
			linearVelocity = linearVelocity + (transform[1] * altitude * temp_cam_speed);
			linearVelocity = linearVelocity + (transform[2] * forward * temp_cam_speed);

			transform[3] = transform[3] + ( linearVelocity * timeSlice );

		}

		if (!GetDayZGame().GetEditor().GeneralSettings.AllowBelowGround) {
			transform[3][1] = Math.Max(GetGame().SurfaceY(transform[3][0], transform[3][2]) + 0.1, transform[3][1]);
		}
				
		SetTransform(transform);
		GetDayZGame().GetEditor().Statistics.DistanceFlown += vector.Distance(transform[3], original_position_unchanged) / 1000; //km		
		
		orientation = GetOrientation();
		if ((input.LocalValue("UATempRaiseWeapon") || !GetGame().GetUIManager().IsCursorVisible()) && LookEnabled) {
			angularVelocity = angularVelocity * Smoothing;
			float temp_cam_rot_speed = Math.Lerp(Mouse_Sens, Mouse_Sens * 0.01, Smoothing);
			angularVelocity[0] = angularVelocity[0] + ( yawDiff * temp_cam_rot_speed * 10 );
			angularVelocity[1] = angularVelocity[1] + ( pitchDiff * temp_cam_rot_speed * 10);
			
			if (shouldRoll) {
				angularVelocity[2] = angularVelocity[2] + (speedInc * temp_cam_rot_speed * 10);
			}
			
			
			orientation[0] = orientation[0] - (angularVelocity[0] * timeSlice);
			orientation[1] = orientation[1] - (angularVelocity[1] * timeSlice);
			orientation[2] = orientation[2] - (angularVelocity[2] * timeSlice);

			orientation[0] = Math.NormalizeAngle(orientation[0]);
			orientation[1] = Math.Clamp(orientation[1], -89.9, 89.9);
		}
		
		orientation[2] = Math.NormalizeAngle(GetDayZGame().GetEditor().GeneralSettings.CameraTilt); //orientation[2]	
		SetOrientation(orientation);

		if (IsTargeting) {
			LookAt(TargetPosition);
		}
		
		
		if (GetDayZGame().GetEditor().GetCurrentOnlineSession()) {
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(GetPosition());
			rpc.Write(GetDirection());
			rpc.Send(null, EditorServerManager.RPC_REPORT_POSITION, false);
		}
		
		//EditorLog.Trace("EditorCamera::EOnFrame-");
	}

	void AngleToQuat( float angle, vector dir, out float d[4] )
	{
		float sin = Math.Sin( angle * 0.5 * Math.DEG2RAD );
		float cos = Math.Cos( angle * 0.5 * Math.DEG2RAD );

		d[3] = cos;
		d[2] = dir[2] * sin;
		d[1] = dir[1] * sin;
		d[0] = dir[0] * sin;
	}
	
	void LerpCameraPosition(vector targetpos, float time)
	{
		thread _LerpCameraPosition(GetPosition(), targetpos, time);
	}
	
	private void _LerpCameraPosition(vector startpos, vector targetpos, float time)
	{
		MoveEnabled = false;
		
		int td = 0;
		while (td < time * 1000) {
			float time_value = 1 / (time * 1000) * td;
			vector step = Math.SmoothLerpVector(startpos, targetpos, time_value);
			SetPosition(step);
			td += 10;
			Sleep(10);
		}
		
		MoveEnabled = true;
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "FOV": {
				SetFOV(FOV);
				break;
			}			
			
			case "NearPlane": {
				SetNearPlane(NearPlane);
				break;
			}			
			
			case "DOFBlur":
			case "DOFDistance": {
				SetFocus(DOFDistance, DOFBlur);
				break;
			}
			
			case "Vignette": {
				PPEffects.SetVignette(Vignette, 0, 0, 0, 255);
				break;
			}
			
			case "Blur": {
				PPEffects.SetBlur(Blur);
				break;
			}
			
			case "Sharpness": {	
				GetGame().GetWorld().GetMaterial("Graphics/Materials/postprocess/filmgrainNV").SetParam("Sharpness", Sharpness);
				break;
			}
			
			case "Exposure": {
				GetGame().GetWorld().SetEyeAccom(Exposure);
				break;
			}
			
			case "ColorCorrection": {
				float a, r, g, b;
				InverseARGBF(ColorCorrection, a, r, g, b);
				PPEffects.SetColorizationNV(r, g, b);
				break;
			}
		}		
	}
}
