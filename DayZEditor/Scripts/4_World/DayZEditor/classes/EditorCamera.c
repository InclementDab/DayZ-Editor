


// make option Q and E go up and down no matter orientation
class EditorCamera: Camera
{
	static float CAMERA_SPEED = 60;
	static float CAMERA_BOOST_MULT = 6.5;
	static float CAMERA_VELDRAG = 0.05;
	static float CAMERA_MSENS = 35.0;
	
	float SendUpdateAccumalator = 0.0;
	
	bool LookEnabled = true;
	bool MoveEnabled = true;

	Object SelectedTarget;
	vector TargetPosition;
	vector linearVelocity;
	vector angularVelocity;
	vector orientation;
	
	
	void EditorCamera()
	{
		EditorLog.Trace("EditorCamera");
		SetEventMask(EntityEvent.FRAME);
		SelectTarget(null);

	}

	void ~EditorCamera()
	{
		SelectTarget(null);
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
		if ( SendUpdateAccumalator > 0.5 ){
			GetGame().UpdateSpectatorPosition(GetPosition());

			SendUpdateAccumalator = 0;
		}
			
		SendUpdateAccumalator = SendUpdateAccumalator + timeSlice;
		
		vector transform[4];
		GetTransform(transform);

		Input input = GetGame().GetInput();
		if (!KeyState(KeyCode.KC_LCONTROL)) {
			float forward = input.LocalValue("EditorCameraForward") - input.LocalValue("EditorCameraBack");
			float strafe = input.LocalValue("EditorCameraRight") - input.LocalValue("EditorCameraLeft");
			float altitude = input.LocalValue("EditorCameraUp") - input.LocalValue("EditorCameraDown");
		}
		
		float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
		float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");

		float speedInc = input.LocalValue("UACameraToolSpeedIncrease" ) - input.LocalValue("UACameraToolSpeedDecrease");

		float zoomAmt = input.LocalValue("UANextAction") - input.LocalValue("UAPrevAction");
		
		vector current_position = GetPosition();
		float current_altitude = current_position[1] - GetGame().SurfaceY(current_position[0], current_position[2]);
		
		if (zoomAmt != 0)
			speedInc = 0;

		bool shouldRoll = false;
		bool decreaseSpeeds = input.LocalValue("EditorCameraSlow");
		bool increaseSpeeds = input.LocalValue("EditorCameraTurbo");
		

		if (MoveEnabled) {
			
			float cam_speed = CAMERA_SPEED;
			
			if (CAMERA_BOOST_MULT > 0) {
				CAMERA_SPEED += Math.Clamp( timeSlice * 40.0 * CAMERA_SPEED * speedInc / CAMERA_BOOST_MULT, -CAMERA_BOOST_MULT, CAMERA_BOOST_MULT );
				
				if ( CAMERA_SPEED < 0.001 ) {
					CAMERA_SPEED = 0.001;
				}
				
				cam_speed = CAMERA_SPEED;// + Math.Clamp(current_altitude - 50, -10, 250);
				if (decreaseSpeeds) {
					cam_speed = cam_speed * 0.1;	
				}

				if (increaseSpeeds) {
					cam_speed = (cam_speed * CAMERA_BOOST_MULT) * (0.2 + (transform[3][1])/600) ;
				}
			}
			
			linearVelocity = linearVelocity * CAMERA_VELDRAG;

			linearVelocity = linearVelocity + ( transform[0] * strafe * cam_speed );
			linearVelocity = linearVelocity + ( transform[1] * altitude * cam_speed );
			linearVelocity = linearVelocity + ( transform[2] * forward * cam_speed );

			transform[3] = transform[3] + ( linearVelocity * timeSlice );

		}

		SetTransform(transform);
		
		orientation = GetOrientation();
		if ((input.LocalValue("UATempRaiseWeapon") || !GetGame().GetUIManager().IsCursorVisible()) && LookEnabled) {
			//SelectTarget(null);
			angularVelocity = vector.Zero;
			angularVelocity[0] = angularVelocity[0] + ( yawDiff * CAMERA_MSENS * 10 );
			angularVelocity[1] = angularVelocity[1] + ( pitchDiff * CAMERA_MSENS * 10);
			
			if (shouldRoll) {
				angularVelocity[2] = angularVelocity[2] + ( speedInc * CAMERA_MSENS * 10);
			}

			orientation[0] = orientation[0] - ( angularVelocity[0] * timeSlice );
			orientation[1] = orientation[1] - ( angularVelocity[1] * timeSlice );
			orientation[2] = orientation[2] - ( angularVelocity[2] * timeSlice );

			if ( orientation[1] <= -90 ) {
				angularVelocity[1] = Math.Min( angularVelocity[1], 0 );
				orientation[1] = -89.9;
			} else if ( orientation[1] >= 90 ) {
				orientation[1] = 89.9; // lazy
				angularVelocity[1] = Math.Max( angularVelocity[1], 0 );
			}

			orientation[0] = Math.NormalizeAngle( orientation[0] );
			orientation[2] = Math.NormalizeAngle( orientation[2] );

			SetOrientation( orientation );
		}

		if (IsTargeting) {
			LookAt(TargetPosition);
		}
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
	
	float GetCameraSpeed() {
		return CAMERA_SPEED;
	}
}



class DSLRCamera: EntityAI
{
	void DSLRCamera()
	{
		EditorLog.Trace("DSLRCamera");
	}	
}



