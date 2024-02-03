class CfgPatches
{
	class Editor
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;

	class StaticCamera: HouseNoDestruct
	{
		model = "Editor\\Editor\\Camera\\Camera.p3d";
	};

	class BrushDecal: HouseNoDestruct
	{
		model = "Editor\\Editor\\Brush\\Brush.p3d";
	};

	class Cube: HouseNoDestruct
	{
		model = "Editor\\Editor\\Cube.p3d";
		hiddenSelections[] = {"BoundingBoxSelection"};
	};
};

class CfgWorlds
{
	class DefaultWorld;
	class Weather;
	class CAWorld: DefaultWorld
	{
		class Weather: Weather
		{
			class VolFog
			{
				CameraFog = 0.0;
				Item1[] = {0,0,0,0,0};
				Item2[] = {0,0,0,0,0};
			};
		};
	};
};