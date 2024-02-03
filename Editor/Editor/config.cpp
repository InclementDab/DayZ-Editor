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