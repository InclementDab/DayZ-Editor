class IntroSceneEditor: Managed
{
	const string BUILD_LOCATIONS = "DayZEditor\\Editor\\Builds";
	protected string m_BuildFile;
	protected EditorCamera m_EditorCamera;
	protected vector m_CameraMatrix[4];
	protected float m_MotionSicknessDt;
	
	protected ref array<Object> m_SpawnedObjects = {};

	void IntroSceneEditor(string map_name)
	{
		string build_directory = SystemPath.Combine(BUILD_LOCATIONS, map_name);
		array<string> files = Directory.EnumerateFiles(build_directory, "*.dze");

		m_EditorCamera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera_V2", vector.Zero, ECE_NONE));
		m_EditorCamera.SetActive(true);

		PPEffects.Init();
		PPEffects.SetBlur(0.621 * 0.621);
		PPEffects.SetVignette(0.621, 0, 0, 0, 255);

		if (files.Count() == 0) {
			return;
		}

		m_BuildFile = files.GetRandomElement();
		
		if (File.Exists(m_BuildFile)) {
			typename file_type_type = EditorDZEFile;
			EditorDZEFile file_type = EditorDZEFile.Cast(file_type_type.Spawn());
			EditorSaveData save_data = file_type.LoadBinFile(m_BuildFile);
			foreach (EditorObjectData object_data: save_data.EditorObjects) {
				Object spawned_object = GetGame().CreateObjectEx(object_data.Type, object_data.Position, ECE_NONE);
				if (spawned_object) {
					spawned_object.SetOrientation(object_data.Orientation);
					spawned_object.SetScale(object_data.Scale);
					m_SpawnedObjects.Insert(spawned_object);
				}
			}

			vector look_at_pos = save_data.EditorObjects.GetRandomElement().Position;
			vector direction = vector.Direction(save_data.CameraPosition, look_at_pos);
			vector aside = direction * vector.Up;
			vector mat[4] = {
				aside,
				aside * direction,
				direction,
				save_data.CameraPosition
			};

			m_EditorCamera.SetTransform(mat);
			copyarray(m_CameraMatrix, mat);
		}
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
		
		m_MotionSicknessDt += dt;
		if (m_MotionSicknessDt > (1 / 30)) {			
			const float PARALLAX_AMOUNT = 0.5;
	        float x_relative = ((mouse_x / screen_x) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2); // 5.0 is from my extra percent size / 2
	        float y_relative = ((mouse_y / screen_y) * PARALLAX_AMOUNT) - (PARALLAX_AMOUNT / 2);
	        
			vector ypr_mat[4], new_mat[4];
			Math3D.YawPitchRollMatrix(Vector(x_relative * 0.01, y_relative * 0.01, 0), ypr_mat);

			Math3D.MatrixMultiply4(ypr_mat, m_CameraMatrix, new_mat);
			m_EditorCamera.SetTransform(new_mat);

			m_MotionSicknessDt = 0;
		}
	}
}

modded class MissionMainMenu
{
	protected ref EditorMainMenu m_MainMenu;
	protected ref IntroSceneEditor m_IntroSceneEditor;

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (m_IntroSceneEditor) {
	    	m_IntroSceneEditor.OnUpdate(timeslice);
		}
	}
	
	override void OnInit()
	{
		// For reset cleanup
		//GetGame().GetUIManager().CloseAll();

		if (!m_NoCutscene) {
			m_IntroSceneEditor = new IntroSceneEditor("ChernarusPlus");
		}
		
		if (m_MainMenu) {
			m_MainMenu.Close();
		}
		
		m_MainMenu = new EditorMainMenu();
		m_mainmenu = m_MainMenu.GetUIScriptViewMenu();
		
		GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().GetUIManager().ShowCursor, 10, 0, 1);
	}
}