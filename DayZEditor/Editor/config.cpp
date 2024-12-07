class CfgPatches
{
	class Editor
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Worlds_Chernarusplus_World"};
	};
};


class CfgVehicles
{
	class Static;
	class HouseNoDestruct;
	class BigCamera : HouseNoDestruct
	{
		scope = 2;
		model = "\DayZEditor\Editor\BigCamera.p3d";
	};

	class DSLRCamera : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\CameraNormal.p3d";
	};

	class DSLRCameraChristmas : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\Camera_xmas_Normal.p3d";
	};

	class DSLRCameraAnniversary : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\Camera.p3d";
	};

	class XmasGiftRed1 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_red1.p3d";
	};
	class XmasGiftRed2 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_red2.p3d";
	};
	class XmasGiftGreen1 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_green1.p3d";
	};
	class XmasGiftGreen2 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_green2.p3d";
	};
	class XmasGiftBlue1 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_blue1.p3d";
	};
	class XmasGiftBlue2 : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_blue2.p3d";
	};

	class GizmoBase : HouseNoDestruct
	{
		scope = 0;
	};

	class GizmoTranslation : GizmoBase
	{
		scope = 1;
		model = "\DayZEditor\Editor\MoveGizmo.p3d";
		hiddenSelections[] = {
			"origin_ball",
			"x_arrow", 
			"y_arrow", 
			"z_arrow",
			"x_square", 
			"y_square", 
			"z_square"
		};

		hiddenSelectionsTextures[] = {
			"#(argb,8,8,3)color(1.0,1.0,1.0,1.000,co)", 
			"#(argb,8,8,3)color(1.0,0.0,0.0,1.000,co)", 
			"#(argb,8,8,3)color(0.0,1.0,0.0,1.000,co)", 
			"#(argb,8,8,3)color(0.0,0.0,1.0,1.000,co)",
			"#(argb,8,8,3)color(1.0,0.0,0.0,1.000,co)", 
			"#(argb,8,8,3)color(0.0,1.0,0.0,1.000,co)", 
			"#(argb,8,8,3)color(0.0,0.0,1.0,1.000,co)"
		};

		hiddenSelectionsMaterials[] = {
			"DayZEditor\\Editor\\data\\no_sort.rvmat", 
			"DayZEditor\\Editor\\data\\no_sort.rvmat", 
			"DayZEditor\\Editor\\data\\no_sort.rvmat", 
			"DayZEditor\\Editor\\data\\no_sort.rvmat",
			"DayZEditor\\Editor\\data\\no_sort.rvmat", 
			"DayZEditor\\Editor\\data\\no_sort.rvmat", 
			"DayZEditor\\Editor\\data\\no_sort.rvmat"
		};
	};

	class GizmoRotation: GizmoBase
	{
		scope = 1;
		model = "\DayZEditor\Editor\RotateGizmo.p3d";
	};

	class GizmoScale: GizmoBase
	{
		scope = 1;
		model = "\DayZEditor\Editor\ScaleGizmo.p3d";
	};

	class BrushBase : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BrushBase.p3d";
		hiddenSelections[] = {"BrushBase"};
		hiddenSelectionTextures[] = { "DayZEditor\Editor\data\BrushDelete.paa" };
		shadow = 0;
	};

	class BoundingBoxBase : HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BoundingBoxBase.p3d";
		hiddenSelections[] = {"BoundingBoxSelection"};
		shadow = 0;
	};

	class EditorLootPoint : HouseNoDestruct
	{
		scope=2;
		model="DayZEditor\editor\DebugCylinder.p3d";
	};

	class Egg : HouseNoDestruct
	{
		scope=2;
		model="DZ\gear\food\eeg.p3d";
	};

	class DZE_Logo_Graffiti : HouseNoDestruct
	{
		scope=2;
		displayName="DZE_Logo_Graffiti";
		descriptionShort="Hype train is a hoax";
		model="\DayZEditor\Editor\Logo_Graffiti.p3d";
		shadow = 0;
	};
	/*
		class CinematicCamera: HouseNoDestruct
		{
			scope = 1;
			model = "\DayZEditor\Editor\CinematicCamera.p3d";
			hiddenSelections[] = {"CinematicCameraHighlight"};
		};*/
};