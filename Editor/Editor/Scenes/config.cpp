class CfgPatches
{
	class Editor_Scenes
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"Editor"};
	};
};

class CfgEditorScenes
{
	class ChernarusPlus
	{
		class Scene_Base
		{
			camera_position[]={0, 0, 0};
			camera_direction[]={0, 0, 0};
			camera_fov=1.0;
		};

		class Settlement_Elektrozavodsk: Scene_Base
		{
			camera_position[]={10475.2, 10.8892, 2303.71};
			camera_direction[]={0.635631, -0.110866, 0.763991};
		};
	};
};