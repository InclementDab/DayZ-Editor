



class EditorCamera: Camera
{
	static float CAMERA_SPEED = 3.0;
	static float CAMERA_BOOST_MULT = 5.0;
	static float CAMERA_VELDRAG = 0.9;
	static float CAMERA_MSENS = 35.0;
	
	float SendUpdateAccumalator = 0.0;
	
	bool LookFreeze;
	bool MoveFreeze;

	Object SelectedTarget;
	vector TargetPosition;

	vector linearVelocity;
	vector angularVelocity;

	vector orientation;
	
	void EditorCamera()
	{
		SetEventMask(EntityEvent.FRAME);
		SelectedTarget(NULL);
	}

	void ~EditorCamera()
	{
		SelectedTarget(NULL);
	}

	void OnTargetSelected( Object target )
	{
		Print("OnTargetSelected");
	}

	void OnTargetDeselected( Object target )
	{
	}

	void SelectedTarget(Object target)
	{
		if (target != SelectedTarget) {
			TargetPosition = target.GetPosition();
			MoveFreeze = true;
			LookFreeze = true;

			OnTargetSelected(target);
		} else if ( target == NULL && SelectedTarget ) {
			TargetPosition = "0 0 0";
			MoveFreeze = false;
			LookFreeze = false;

			OnTargetDeselected(SelectedTarget);
		}

		SelectedTarget = target;
	}

	override void EOnFrame( IEntity other, float timeSlice )
	{
		if ( SendUpdateAccumalator > 0.5 ){
			GetGame().UpdateSpectatorPosition(GetPosition());

			SendUpdateAccumalator = 0;
		}
			
		SendUpdateAccumalator = SendUpdateAccumalator + timeSlice;
		
		vector transform[4];
		GetTransform(transform);

		Input input = GetGame().GetInput();

		float forward = input.LocalValue("UAMoveForward") - input.LocalValue("UAMoveBack");
		float strafe = input.LocalValue("UAMoveRight") - input.LocalValue("UAMoveLeft");
		float altitude = input.LocalValue("UAMoveUp") - input.LocalValue("UAMoveDown");

		float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
		float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");

		float speedInc = input.LocalValue("UACameraToolSpeedIncrease" ) - input.LocalValue("UACameraToolSpeedDecrease");

		float zoomAmt = input.LocalValue("UANextAction") - input.LocalValue("UAPrevAction");
		
		
		if ( zoomAmt != 0 )
			speedInc = 0;

		bool shouldRoll = input.LocalValue("UALookAround");
		bool increaseSpeeds = input.LocalValue("UATurbo");

		if (!MoveFreeze) {
			
			float cam_speed = CAMERA_SPEED;
			if ( !shouldRoll && CAMERA_BOOST_MULT > 0 ){
				CAMERA_SPEED += Math.Clamp( timeSlice * 40.0 * CAMERA_SPEED * speedInc / CAMERA_BOOST_MULT, -CAMERA_BOOST_MULT, CAMERA_BOOST_MULT );
				
				if ( CAMERA_SPEED < 0.001 ) {
					CAMERA_SPEED = 0.001;
				}
				
				cam_speed = CAMERA_SPEED;

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

		SetTransform( transform );
			
		if (input.LocalValue("UATempRaiseWeapon") || !LookFreeze)
		{
			angularVelocity = vector.Zero;

			angularVelocity[0] = angularVelocity[0] + ( yawDiff * CAMERA_MSENS * 10 );
			angularVelocity[1] = angularVelocity[1] + ( pitchDiff * CAMERA_MSENS * 10);
			
			if ( shouldRoll ) {
				angularVelocity[2] = angularVelocity[2] + ( speedInc * CAMERA_MSENS * 10);
			}

			orientation[0] = orientation[0] - ( angularVelocity[0] * timeSlice );
			orientation[1] = orientation[1] - ( angularVelocity[1] * timeSlice );
			orientation[2] = orientation[2] - ( angularVelocity[2] * timeSlice );

			if ( orientation[1] <= -90 ) {
				angularVelocity[1] = Math.Min( angularVelocity[1], 0 );
				orientation[1] = -90;
			} else if ( orientation[1] >= 90 ) {
				orientation[1] = 90;
				angularVelocity[1] = Math.Max( angularVelocity[1], 0 );
			}

			orientation[0] = Math.NormalizeAngle( orientation[0] );
			orientation[2] = Math.NormalizeAngle( orientation[2] );

			SetOrientation( orientation );
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
}



