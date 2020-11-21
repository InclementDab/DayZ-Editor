


// make option Q and E go up and down no matter orientation
class EditorCamera: Camera
{
	float FOV = 1;
	float DOFDistance;
	float DOFBlur;
	float Blur;
	float Vignette;
	float Sharpness;
	float NearPlane;
	float Exposure;
	
	float Speed = 60;
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
	
	float ColorizeRed;
	float ColorizeGreen;
	float ColorizeBlue;
	
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
	
	private float cursor_store_x, cursor_store_y;
	
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
		float zoomAmt = input.LocalValue("EditorCameraZoomIn") - input.LocalValue("EditorCameraZoomOut");
		

		
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
		
		SetFOV(Math.Clamp(GetCurrentFOV() + zoomAmt, 0.01, 5));
		
		if (input.LocalValue("EditorCameraZoomReset")) {
			SetFOV(1);
		}
		
		Math.Clamp(Speed, 0.001, FLT_MAX);
		
		
		if (MoveEnabled) {
						
			if (Boost_Multiplier > 0) {
				Speed += Math.Clamp(timeSlice * 40.0 * Speed * speedInc / Boost_Multiplier, -Boost_Multiplier, Boost_Multiplier);
				
				float cam_speed = Speed;
				if (decreaseSpeeds) {
					cam_speed = cam_speed * 0.1;	
				}

				if (increaseSpeeds) {
					cam_speed = (cam_speed * Boost_Multiplier) * (0.2 + (transform[3][1])/600) ;
				}
			}
			
			linearVelocity = linearVelocity * Drag;

			linearVelocity = linearVelocity + (transform[0] * strafe * cam_speed);
			linearVelocity = linearVelocity + (transform[1] * altitude * cam_speed);
			linearVelocity = linearVelocity + (transform[2] * forward * cam_speed);

			transform[3] = transform[3] + ( linearVelocity * timeSlice );

		}

		SetTransform(transform);
		
		if (HideCursorOnDrag) {
			if (input.LocalPress("UATempRaiseWeapon")) {
				GetEditor().GetEditorHud().ShowCursor(false);
				
			}
			
			if (input.LocalRelease("UATempRaiseWeapon")) {
				GetEditor().GetEditorHud().ShowCursor(true);
			}
		}
		
		
		if ((input.LocalValue("UATempRaiseWeapon") || !GetGame().GetUIManager().IsCursorVisible()) && LookEnabled) {
			//SelectTarget(null);
			
			angularVelocity = vector.Zero;
			angularVelocity[0] = angularVelocity[0] + ( yawDiff * Mouse_Sens * 10 );
			angularVelocity[1] = angularVelocity[1] + ( pitchDiff * Mouse_Sens * 10);
			
			if (shouldRoll) {
				angularVelocity[2] = angularVelocity[2] + ( speedInc * Mouse_Sens * 10);
			}
			
			orientation = GetOrientation();
			orientation[0] = orientation[0] - (angularVelocity[0] * timeSlice);
			orientation[1] = orientation[1] - (angularVelocity[1] * timeSlice);
			orientation[2] = orientation[2] - (angularVelocity[2] * timeSlice);

			orientation[0] = Math.NormalizeAngle(orientation[0]);
			orientation[1] = Math.Clamp(orientation[1], -89.9, 89.9);
			orientation[2] = Math.NormalizeAngle(orientation[2]);

			SetOrientation(orientation);
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
			
			case "ColorizeRed":
			case "ColorizeGreen":
			case "ColorizeBlue": {
				PPEffects.SetColorizationNV(ColorizeRed, ColorizeGreen, ColorizeBlue);
				break;
			}
		}		
	}
}



class DSLRCamera: EntityAI
{
	void DSLRCamera()
	{
		EditorLog.Trace("DSLRCamera");
	}	
}



