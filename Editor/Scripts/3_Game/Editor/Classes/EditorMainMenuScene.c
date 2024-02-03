class EditorMainMenuScene: Managed
{
	// camera data
	vector CameraPosition, CameraDirection;
	float CameraFov;
		
	static EditorMainMenuScene GetRandomEntry(string world_name)
	{
		// Int inclusive starts at 1 because we dont want Scene_Base
		return (GetEntry(world_name, Math.RandomIntInclusive(1, GetGame().ConfigGetChildrenCount(string.Format("CfgEditorScenes %1", world_name)) - 1)));
	}
	
	static EditorMainMenuScene GetEntry(string world_name, int index)
	{
		EditorMainMenuScene scene = new EditorMainMenuScene();
		
		string root = string.Format("CfgEditorScenes %1", world_name);		
		string child_name;
		GetGame().ConfigGetChildName(root, index, child_name);
		
		string scene_name = string.Format("%1 %2", root, child_name);
		scene.CameraPosition = GetGame().ConfigGetVector(scene_name + " camera_position");		
		scene.CameraDirection = GetGame().ConfigGetVector(scene_name + " camera_direction");
		scene.CameraFov = GetGame().ConfigGetFloat(scene_name + " camera_fov");
		
		return scene;
	}
	
	static void CopyFromCurrentPosition()
	{
		string copy;
		vector position = GetGame().GetCurrentCameraPosition();
		vector direction = GetGame().GetCurrentCameraDirection();
		copy += string.Format("\t\t\tcamera_position[]={%1, %2, %3};\n", position[0], position[1], position[2]);
		copy += string.Format("\t\t\tcamera_direction[]={%1, %2, %3};\n", direction[0], direction[1], direction[2]);
		GetGame().CopyToClipboard(copy);
	}
}