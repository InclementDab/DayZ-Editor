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
			camera_position[]={217.75,8.20714,32.066399};
			camera_direction[]={-0.60751998,0,0};
			camera_fov=0.2;
		};

		class Settlement_Elektrozavodsk: Scene_Base
		{
			camera_position[]={10474.4, 7.17196, 2299.57};
			camera_direction[]={0.688174, -0.0973841, 0.71898};

		};
	};
};