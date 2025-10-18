class IntroSceneEditor: Managed
{		
	const float LOCATION_TIME_MAX = 45;
	const string BUILD_LOCATIONS = "DayZEditor\\Editor\\Builds";
	protected string m_BuildFile;
	protected Camera m_Camera;
	protected vector m_CameraMatrix[4];
	protected float m_MotionSicknessDt, m_TimeElapsedAtLocation;
	
	protected ref array<ref NamedLocation> m_MapLocations = {};
	protected ref array<NamedLocation> m_UnvisitedLocations = {};
	protected ref array<Object> m_SpawnedObjects = {};
	EditorMainMenu MainMenuView;
	
	void IntroSceneEditor(string map_name)
	{
		array<ref NamedLocation> all_locations = NamedLocation.EnumerateAllLocations();

		// Filter for cities and capitals
		foreach (NamedLocation named_location: all_locations) {
			if (named_location.GetImportance() > 0.65) {
				m_MapLocations.Insert(named_location);
				m_UnvisitedLocations.Insert(named_location);
			}
		}

		m_Camera = Camera.Cast(GetGame().CreateObjectEx("StaticCamera", vector.Zero, ECE_NONE));
		m_Camera.SetActive(true);

		PPEffects.Init();
		PPEffects.SetBlur(0.621 * 0.621 * 0.621);
		//PPEffects.SetVignette(0.621, 0, 0, 0, 255);
		
		// Call once after initialization
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetLocation, 12, 0, m_UnvisitedLocations.GetRandomElement());
	}
	
	void ~IntroSceneEditor()
	{
		foreach (auto spawned_object: m_SpawnedObjects) {
			if (spawned_object) {
				spawned_object.Delete();
			}
		}

		PPEffects.ResetAll();
	}	

	void OnUpdate(float dt)
	{
		int mouse_x, mouse_y, screen_x, screen_y;
	    GetMousePos(mouse_x, mouse_y);
	    GetScreenSize(screen_x, screen_y);
				
		if (!m_Camera) {
			return;
		}
		
		vector camera_matrix[4];
		m_Camera.GetTransform(camera_matrix);
		
		camera_matrix[3] = camera_matrix[3] + camera_matrix[2] * 1 * m_MotionSicknessDt;
		m_Camera.SetTransform(camera_matrix);
		
		float new_fov = m_Camera.GetCurrentFOV() + 0.005 * m_MotionSicknessDt;
		m_Camera.SetFOV(new_fov);

		Ray camera_ray = new Ray(camera_matrix[3], camera_matrix[2]);
		Raycast camera_raycast = camera_ray.PerformRaycast();
		vector camera_raycast_bounce_pos;
		if (camera_raycast) {
			if (camera_raycast.Hit) {
				camera_raycast_bounce_pos = camera_raycast.Hit.GetPosition();
			} else if (camera_raycast.Bounce) {
				camera_raycast_bounce_pos = camera_raycast.Bounce.Position;
			}
		}

		bool we_are_about_to_hit_something;
		if (vector.Distance(camera_matrix[3], camera_raycast_bounce_pos) < 10) {
			we_are_about_to_hit_something = true;
		}
		
		m_TimeElapsedAtLocation += dt;
		if (m_TimeElapsedAtLocation > LOCATION_TIME_MAX || we_are_about_to_hit_something) {
			if (m_UnvisitedLocations.Count() == 0) {
				// Reset the tape
				foreach (auto named_location: m_MapLocations) {
					m_UnvisitedLocations.Insert(named_location);
				}
			}

			if (m_UnvisitedLocations.Count()) {
				SetLocation(m_UnvisitedLocations.GetRandomElement());
			}

			m_TimeElapsedAtLocation = 0.0;
		}
	}

	protected void SetLocation(notnull NamedLocation location)
	{
		m_UnvisitedLocations.RemoveItem(location);
		GetGame().GetSoundScene().SetSoundVolume(0, 0);
		GetGame().GetSoundScene().SetSoundVolume(1.0, 2.4);
		if (MainMenuView) {
			WidgetAnimator.Animate(MainMenuView.GetLayoutRoot(), WidgetAnimatorProperty.COLOR_A, 1.0, 0.0, 2400);
		}

		vector camera_position = location.Position + vector.RandomDir2D() * (location.GetImportance() / 2) * (location.GetImportance() / 2) * 140;
		camera_position[1] = GetGame().SurfaceY(camera_position[0], camera_position[2]) + 40;

		// Reset
		GetGame().GetWeather().GetRain().Set(0);
		GetGame().GetWeather().GetSnowfall().Set(0);
		GetGame().GetWeather().GetFog().Set(0);
		
		float overcast = Math.RandomFloat01();
		overcast *= overcast;
		GetGame().GetWeather().GetOvercast().Set(overcast);
		if (overcast > 0.5) {
			if (Math.RandomInt(0, 1)) {
				GetGame().GetWeather().GetRain().Set(Math.RandomFloat01() * 0.4);
			} else {
				GetGame().GetWeather().GetSnowfall().Set(Math.RandomFloat01());
			}
		}
		
		vector look_at_pos = location.Position;
		vector direction = vector.Direction(camera_position, look_at_pos);
		vector aside = direction * vector.Up;
		vector mat[4] = {
			aside,
			aside * direction,
			direction,
			camera_position
		};
		
		if (m_Camera) {
			m_Camera.SetPosition(camera_position);
			m_Camera.LookAt(look_at_pos);
			m_Camera.SetFOV(EditorCamera.FOV_DEFAULT);
		}
		
		copyarray(m_CameraMatrix, mat);
	}
}