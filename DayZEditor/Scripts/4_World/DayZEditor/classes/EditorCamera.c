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
		SetBrightnessTo(10);
		SetRadiusTo(150);
		SetSpotLightAngle(100);
	}
}

class EditorCamera: Camera
{
	protected EditorCameraSettings m_EditorCameraSettings;
	protected EditorCameraLight m_EditorCameraLight;

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
	const int VIEW_DISTANCE_MAX = 12000;

	const float TURBO_MULTIPLIER = 2.5;
	const float SLOW_MULTIPLIER = 0.2;
	
	const float TELEPORT_LERP_DISTANCE = 1000;

	void EditorCamera()
	{
		m_EditorCameraSettings = EditorCameraSettings.Cast(GetDayZGame().GetProfileSetting(EditorCameraSettings));
	}

	void SetLightState(bool state)
	{
		if (state) {
			m_EditorCameraLight = EditorCameraLight.Cast(ScriptedLightBase.CreateLight(EditorCameraLight, GetPosition(), 0.2));
			m_EditorCameraLight.SetDirection(GetDirection());
			m_EditorCameraLight.AttachOnObject(this);
		} else {
			GetGame().ObjectDelete(m_EditorCameraLight);
		}
	}

	EditorCameraSettings GetSettings()
	{
		return m_EditorCameraSettings;
	}
}

[RegisterProfileSetting(EditorCameraSettings, "EDITOR")]
class EditorCameraSettings: ProfileSettings
{
	[RegisterProfileSettingSlider("CAMERA", "ViewDistance", "View Distance", EditorCamera.VIEW_DISTANCE_MIN, EditorCamera.VIEW_DISTANCE_MAX)]
	float ViewDistance = EditorCamera.VIEW_DISTANCE_DEFAULT;

	[RegisterProfileSettingSlider("CAMERA", "FieldOfView", "Field Of View", EditorCamera.FOV_MIN, EditorCamera.FOV_MAX)]
	float FieldOfView = EditorCamera.FOV_DEFAULT;

	[RegisterProfileSettingSlider("CAMERA", "ExposureLevel", "Exposure Level", EditorCamera.EXPOSURE_MIN, EditorCamera.EXPOSURE_MAX)]
	float ExposureLevel = EditorCamera.EXPOSURE_DEFAULT;

	[RegisterProfileSettingSlider("CAMERA", "SmoothingLevel", "Smoothing Level", EditorCamera.SMOOTHING_MIN, EditorCamera.SMOOTHING_MAX)]
	float SmoothingLevel = EditorCamera.SMOOTHING_DEFAULT;

	[RegisterProfileSettingMultistate("CAMERA", "AllowUnderEarth", "Camera Below Surface", {"NO", "YES"})]
	bool AllowUnderEarth = false;

	[RegisterProfileSettingMultistate("CAMERA", "InvertCamera", "Inverted Camera", {"NO", "YES"})]
	bool InvertCamera = false;

	[RegisterProfileSettingMultistate("CAMERA", "LegacyCamera", "Classic Camera", {"DISABLED", "ENABLED"}, true)]
	bool LegacyCamera = false;
}

// make option Q and E go up and down no matter orientation
class EditorCamera_V2: EditorCamera
{	
	protected float m_CameraFovVelocity[1];
	protected float m_CameraFovActual;

	float DOFDistance;
	float DOFBlur;
	float Blur;
	float Vignette;
	float Sharpness;
		
	float Speed = GetEditor().Settings.CameraSpeed;
	float Boost_Multiplier = 6.5;
	float Drag = 0.05;
	const float Mouse_Sens = 35.0;
	
	float SendUpdateAccumalator = 0.0;
	
	protected vector m_LinearVelocity, m_AngularVelocity;
	
	int ColorCorrection = COLOR_WHITE;
	
	void EditorCamera_V2()
	{
		SetEventMask(EntityEvent.FRAME);
		
		m_CameraFovActual = m_EditorCameraSettings.FieldOfView;
	}
	
	override void EOnFrame(IEntity other, float timeSlice)
	{
		vector transform[4];
		GetTransform(transform);

		UAInputAPI input = GetUApi();
		
		if (!IsActive()) {
			return;
		}
		
		ECameraLockFlag camera_lock = GetEditor().GetCameraLockFlags();
		vector movement;
		if ((camera_lock & ECameraLockFlag.LOCK_MOVE) == 0) {
			float forward = input.GetInputByID(UAMoveForward).LocalValue() - input.GetInputByID(UAMoveBack).LocalValue();
			float strafe = input.GetInputByID(UAMoveRight).LocalValue() - input.GetInputByID(UAMoveLeft).LocalValue();
			float altitude = input.GetInputByID(UAMoveUp).LocalValue() - input.GetInputByID(UAMoveDown).LocalValue();
			movement = Vector(strafe, altitude, forward);

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
			teleport = input.GetInputByID(UAZoomIn).LocalPress();
		}
							
		float speed = input.GetInputByID(UATurbo).LocalValue() - input.GetInputByID(UALookAround).LocalValue();

		if (camera_lock & ECameraLockFlag.PAN_LOOK) {
			vector offset_matrix[3]; 
			Math3D.YawPitchRollMatrix(Vector(0, 270, 180), offset_matrix);

			movement = movement + rotation.Multiply3(offset_matrix);;
			rotation = vector.Zero;
		}
		
		if (fov != 0) {
			m_EditorCameraSettings.FieldOfView = Math.Clamp(m_EditorCameraSettings.FieldOfView + fov, FOV_MIN, FOV_MAX);
			m_EditorCameraSettings.Save();
		}

		if (teleport) {
			Ray cursor_ray = GetEditor().GetCursorRayModeSafe();
			Raycast cursor_ray_cast = GetEditor().GetCursorRaycastModeSafe();
			if (cursor_ray_cast) {
				transform[3] = cursor_ray_cast.Bounce.GetPoint(100.0);
			} else if (cursor_ray) {
				transform[3] = cursor_ray.GetPoint(m_EditorCameraSettings.ViewDistance);
			}
		}
		
		if (speed > 0) {
			speed *= TURBO_MULTIPLIER;
		} else if (speed < 0) {
			speed /= -TURBO_MULTIPLIER;
		} else {
			speed = 1.0;
		}

		// m/s
		speed *= 120.0;

		// Process Angular Velocity, use angle addition. hope YawPitchRollMatrix normalizes it
		vector orientation = GetOrientation();
		
		m_AngularVelocity = m_AngularVelocity + (rotation * Math.RAD2DEG * 2);
		orientation = orientation - m_AngularVelocity * timeSlice;
		orientation[1] = Math.Clamp(orientation[1], MIN_LOOK_Y_DEG, MAX_LOOK_Y_DEG);

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
		m_CameraFovActual = Math.SmoothCD(m_CameraFovActual, m_EditorCameraSettings.FieldOfView + zoom * FOV_ZOOM_AMT, p, 0.05, 800 * (m_EditorCameraSettings.SmoothingLevel + 0.5), timeSlice);

		// Apply
		SetFOV(m_CameraFovActual);
		SetTransform(transform);
		GetGame().GetWorld().SetEyeAccom(m_EditorCameraSettings.ExposureLevel);
		GetGame().GetWorld().SetViewDistance(m_EditorCameraSettings.ViewDistance);
		GetGame().GetWorld().SetObjectViewDistance(m_EditorCameraSettings.ViewDistance);

		GetEditor().Statistics.EditorDistanceFlown += timeSlice * speed;
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