enum ECameraLockFlag
{
	NONE = 0,

	LOCK_MOVE = 1,
	LOCK_LOOK = 2,

	INVERT_MOVE = 4,
	INVERT_LOOK = 8,
	
	PAN_LOOK = 16,

	LOCK = 3,
}

class EditorCameraLight: SpotLightBase
{
	void EditorCameraLight()
	{
		SetBrightnessTo(3);
		SetRadiusTo(400);
		SetSpotLightAngle(120);
		SetVisibleDuringDaylight(true);
		EnableSpecular(true);
		EnableLinear(true);
		SetCastShadow(GetEditor().GetSettings().DrawCameraLightShadows);
	}
}

class EditorCamera: Camera
{
	protected EditorCameraSettings m_EditorCameraSettings;
	protected EditorCameraLight m_EditorCameraLight;
	protected float m_CameraFovActual;

	const float FOV_MIN = 0.0174533; // 1 deg
	const float FOV_MAX = 2.44346; // 140 deg
	const float FOV_DEFAULT = 1.25664; // 75 deg
	const float FOV_ZOOM_AMT = -0.6544985; // -37.5 deg

	const float MAX_LOOK_Y_DEG = 87.0;
	const float MIN_LOOK_Y_DEG = -87.0;

	const float EXPOSURE_MIN = 0.0;
	const float EXPOSURE_MAX = 3.0;
	const float EXPOSURE_DEFAULT = 1.0;

	const float SMOOTHING_MIN = 0.0;
	const float SMOOTHING_MAX = 1.0;
	const float SMOOTHING_DEFAULT = 0.0;

	const int VIEW_DISTANCE_DEFAULT = 800;
	const int VIEW_DISTANCE_MIN = 120;
	const int VIEW_DISTANCE_MAX = 6000;

	const float TURBO_MULTIPLIER = 2.5;
	const float SLOW_MULTIPLIER = 0.2;
	
	const float TELEPORT_LERP_DISTANCE = 1000;
	
	const float SPEED_DEFAULT = 60;
	const float SPEED_MAX = 300;
	const float SPEED_MIN = 1;
	
	float FOV = 90, DOFDistance, DOFBlur, Blur, Vignette, Sharpness, NearPlane, Exposure, Tilt;
	int ColorCorrection = -1;
	
	protected bool m_LightState;

	void EditorCamera()
	{
		m_EditorCameraSettings = EditorCameraSettings.Cast(GetDayZGame().GetProfileSetting(EditorCameraSettings));
		m_CameraFovActual = m_EditorCameraSettings.FieldOfView2 * Math.DEG2RAD;

		NearPlane = GetNearPlane();
		Exposure = GetGame().GetWorld().GetEyeAccom();
		FOV = m_CameraFovActual * Math.RAD2DEG;
		SetFOV(m_CameraFovActual);
	}

	void SetLightState(bool state)
	{
		m_LightState = state;
		if (m_LightState) {
			m_EditorCameraLight = EditorCameraLight.Cast(ScriptedLightBase.CreateLight(EditorCameraLight, vector.Zero, 0.2));
		} else {
			GetGame().ObjectDelete(m_EditorCameraLight);
		}
	}
	
	bool GetLightState()
	{
		return m_LightState;
	}

	EditorCameraSettings GetSettings()
	{
		return m_EditorCameraSettings;
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "FOV": {
				m_CameraFovActual = FOV * Math.DEG2RAD;
				m_EditorCameraSettings.FieldOfView2 = FOV;
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

#ifndef COMPONENT_SYSTEM
[RegisterProfileSetting(EditorCameraSettings, "CAMERA")]
#endif
class EditorCameraSettings: ProfileSettings
{
#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingSlider("CAMERA", "ViewDistance", "View Distance", EditorCamera.VIEW_DISTANCE_MIN, EditorCamera.VIEW_DISTANCE_MAX)]
#endif
	float ViewDistance = EditorCamera.VIEW_DISTANCE_DEFAULT;
	
#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingSlider("CAMERA", "Speed", "Camera Speed (m/s)", EditorCamera.SPEED_MIN, EditorCamera.SPEED_MAX)]
#endif
	float Speed = EditorCamera.SPEED_DEFAULT;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingSlider("CAMERA", "FieldOfView2", "Field Of View", EditorCamera.FOV_MIN * Math.RAD2DEG, EditorCamera.FOV_MAX * Math.RAD2DEG)]
#endif
	float FieldOfView2 = EditorCamera.FOV_DEFAULT * Math.RAD2DEG;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingSlider("CAMERA", "ExposureLevel", "Exposure Level", EditorCamera.EXPOSURE_MIN, EditorCamera.EXPOSURE_MAX)]
#endif
	float ExposureLevel = EditorCamera.EXPOSURE_DEFAULT;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingSlider("CAMERA", "SmoothingLevel", "Smoothing Level", EditorCamera.SMOOTHING_MIN, EditorCamera.SMOOTHING_MAX)]
#endif
	float SmoothingLevel = EditorCamera.SMOOTHING_DEFAULT;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingMultistate("CAMERA", "AllowUnderEarth", "Camera Below Surface", {"NO", "YES"})]
#endif
	bool AllowUnderEarth = false;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingMultistate("CAMERA", "InvertCamera", "Inverted Camera", {"NO", "YES"})]
#endif
	bool InvertCamera = false;

#ifndef COMPONENT_SYSTEM
	[RegisterProfileSettingMultistate("CAMERA", "LegacyCamera", "Classic Camera", {"DISABLED", "ENABLED"}, true)]
#endif
	bool LegacyCamera = false;
}

// make option Q and E go up and down no matter orientation
class EditorCamera_V2: EditorCamera
{	
	protected float m_CameraFovVelocity[1];
		
	float Speed;
	float Boost_Multiplier = 6.5;
	float Drag = 0.05;
	const float Mouse_Sens = 35.0;
	
	float SendUpdateAccumalator = 0.0;
	
	protected vector m_LinearVelocity, m_AngularVelocity;
	protected vector m_Impulse;
	
	void EditorCamera_V2()
	{
		SetEventMask(EntityEvent.FRAME);
		
		Speed = m_EditorCameraSettings.Speed;
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		vector transform[4];
		GetTransform(transform);

		UAInputAPI input = GetUApi();
		
		if (!IsActive()) {
			return;
		}
		
		ECameraLockFlag camera_lock = -1;
		if (GetEditor()) {
			camera_lock = GetEditor().GetCameraLockFlags();
		}
		
		vector movement;
		if ((camera_lock & ECameraLockFlag.LOCK_MOVE) == 0) {
			float forward = input.GetInputByID(UAMoveForward).LocalValue() - input.GetInputByID(UAMoveBack).LocalValue();
			float strafe = input.GetInputByID(UAMoveRight).LocalValue() - input.GetInputByID(UAMoveLeft).LocalValue();
			float altitude = input.GetInputByID(UAMoveUp).LocalValue() - input.GetInputByID(UAMoveDown).LocalValue();
			movement = Vector(strafe, altitude / 1.618, forward);
			if (camera_lock & ECameraLockFlag.INVERT_MOVE) {
				movement = -movement;
			}
		}
				
		vector rotation;
		float zoom;
		float fov;
		bool teleport;
		if ((camera_lock & ECameraLockFlag.LOCK_LOOK) == 0) {
			float yaw = input.GetInputByID(UAAimLeft).LocalValue() - input.GetInputByID(UAAimRight).LocalValue();
			float pitch = input.GetInputByID(UAAimDown).LocalValue() - input.GetInputByID(UAAimUp).LocalValue();
			float roll = 0;

			rotation = Vector(yaw, pitch, roll);
			zoom = input.GetInputByID(UAZoomIn).LocalValue() * !(camera_lock & ECameraLockFlag.LOCK_MOVE);
			fov = (Math.PI / 40) * (input.GetInputByID(UABuldZoomIn).LocalValue() - input.GetInputByID(UABuldZoomOut).LocalValue());
			teleport = input.GetInputByID(UAZoomIn).LocalPress() * (camera_lock & ECameraLockFlag.LOCK_MOVE);

			if (camera_lock & ECameraLockFlag.INVERT_LOOK) {
				rotation = -rotation;
			}
		} else {
			teleport = input.GetInputByID(UAZoomIn).LocalPress() && !GetDayZGame().IsLeftCtrlDown();
		}
							
		float speed = m_EditorCameraSettings.Speed;
		if (input.GetInputByID(UATurbo).LocalValue()) {
			speed *= TURBO_MULTIPLIER;
		}
		
		if (input.GetInputByID(UALookAround).LocalValue()) {
			speed /= (TURBO_MULTIPLIER * TURBO_MULTIPLIER);
		}

		if (camera_lock & ECameraLockFlag.PAN_LOOK) {
			vector offset_matrix[3]; 
			Math3D.YawPitchRollMatrix(Vector(0, 270, 180), offset_matrix);

			movement = movement + rotation.Multiply3(offset_matrix);;
			rotation = vector.Zero;
		}
		
		if (teleport) {
			Ray cursor_ray = GetEditor().GetCursorRayModeSafe();
			Raycast cursor_ray_cast = GetEditor().GetCursorRaycastModeSafe();

			float y_height = transform[3][1] - GetGame().SurfaceY(transform[3][0], transform[3][2]);
			vector target = cursor_ray.GetPoint(250);
			if (cursor_ray_cast) {
				target = cursor_ray_cast.Bounce.Position;
			}

			target[1] = GetGame().SurfaceY(target[0], target[2]) + y_height;

			m_Impulse = target - transform[3];
		}

		if (m_Impulse.Length() > 0.01) {
						
			vector impulse_amount = 5 * m_Impulse * timeSlice;
			transform[3] = transform[3] + impulse_amount;
			m_Impulse = m_Impulse - impulse_amount;
		}
				
		// Process Angular Velocity, use angle addition. hope YawPitchRollMatrix normalizes it
		vector orientation = GetOrientation();
		
		m_AngularVelocity = m_AngularVelocity + (rotation * Math.RAD2DEG * 2);
		orientation = orientation - m_AngularVelocity * timeSlice;
		orientation[1] = Math.Clamp(orientation[1], MIN_LOOK_Y_DEG, MAX_LOOK_Y_DEG);
		orientation[2] = Tilt;

		vector ypr_matrix[3];
		// normalize it
		Math3D.YawPitchRollMatrix(orientation, ypr_matrix);
		copyarray(transform, ypr_matrix);

		// Process linear velocity
		m_LinearVelocity = m_LinearVelocity + transform[0] * movement[0] * speed;
		m_LinearVelocity = m_LinearVelocity + transform[1] * movement[1] * speed;
		m_LinearVelocity = m_LinearVelocity + transform[2] * movement[2] * speed;

		transform[3] = transform[3] + (m_LinearVelocity * timeSlice);

		if (!m_EditorCameraSettings.AllowUnderEarth) {
			transform[3][1] = Math.Max(GetGame().SurfaceY(transform[3][0], transform[3][2]) + GetNearPlane() * 2, transform[3][1]);
		}

		m_AngularVelocity = m_AngularVelocity * m_EditorCameraSettings.SmoothingLevel;
		m_LinearVelocity = m_LinearVelocity * m_EditorCameraSettings.SmoothingLevel;
		
		// FOV velocity
		float p[1];
		copyarray(p, m_CameraFovVelocity);		
		m_CameraFovActual = Math.SmoothCD(m_CameraFovActual, m_EditorCameraSettings.FieldOfView2 * Math.DEG2RAD + zoom * FOV_ZOOM_AMT, p, 0.05, 800 * (m_EditorCameraSettings.SmoothingLevel + 0.5), timeSlice);

		// Apply
		SetFOV(m_CameraFovActual);
		SetTransform(transform);
		if (m_EditorCameraLight) {
			m_EditorCameraLight.SetTransform(transform);
		}
		
		//GetGame().GetWorld().SetEyeAccom(m_EditorCameraSettings.ExposureLevel);
		GetGame().GetWorld().SetViewDistance(m_EditorCameraSettings.ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(m_EditorCameraSettings.ViewDistance);
		
		if (GetEditor()) {
			GetEditor().GetStatistics().DistanceFlown += timeSlice * speed;
		}
	}

	void LerpCameraPosition(vector targetpos, float time)
	{
		thread _LerpCameraPosition(GetPosition(), targetpos, time);
	}
	
	private void _LerpCameraPosition(vector startpos, vector targetpos, float time)
	{
		GetEditor().SetCameraLockFlag(ECameraLockFlag.LOCK_MOVE);
		
		int td = 0;
		while (td < time * 1000) {
			float time_value = 1 / (time * 1000) * td;
			vector step = Math.SmoothLerpVector(startpos, targetpos, time_value);
			SetPosition(step);
			td += 10;
			Sleep(10);
		}
		
		GetEditor().ClearCameraLockFlag(ECameraLockFlag.LOCK_MOVE);
	}
}