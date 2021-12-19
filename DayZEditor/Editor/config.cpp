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

	class DSLRCamera: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\Camera_xmas_Normal.p3d";
	};

	class XmasGiftRed1: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_red1.p3d";
	};
	class XmasGiftRed2: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_red2.p3d";
	};
	class XmasGiftGreen1: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_green1.p3d";
	};
	class XmasGiftGreen2: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_green2.p3d";
	};
	class XmasGiftBlue1: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_blue1.p3d";
	};
	class XmasGiftBlue2: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\xmas_gift_blue2.p3d";
	};

	class Inventory_Base;
	class GizmoBase: Inventory_Base
	{
		scope = 1;
		hiddenSelections[] = {"main"};
		shadow = 0;
	};
	class GizmoArrow: GizmoBase
	{
		scope = 2;
		model = "\DayZEditor\Editor\TranslationWidget.p3d";
		shadow = 0;
	};

	class GizmoCenter: GizmoBase
	{
		scope = 2;
		model = "\DayZEditor\Editor\TranslationCenter.p3d";
		shadow = 0;
	};

	class RotationWidget: Inventory_Base
	{
		scope = 1;
		itemSize[]={16,16};
		model = "\DayZEditor\Editor\RotationWidget.p3d";
		hiddenSelections[] = {"RotationX", "RotationY", "RotationZ"};
		shadow = 0;
	};

	class BrushBase: Inventory_Base
	{
		scope = 1;
		model = "\DayZEditor\Editor\BrushBase.p3d";
		hiddenSelections[] = {"BrushBase"};
		hiddenSelectionTextures[] = { "DayZEditor\\Editor\\data\\BrushDelete.paa" };
		shadow = 0;
	};

	class BoundingBoxBase: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BoundingBoxBase.p3d";
		hiddenSelections[] = {"BoundingBoxSelection"};
		shadow = 0;
	};

	class EditorLootPoint: HouseNoDestruct
	{
		scope=2;
		model="DayZEditor\editor\DebugCylinder.p3d";
	};

	class EGG: HouseNoDestruct
	{
		scope=2;
		model="DZ\gear\food\eeg.p3d";
	};

	class DZE_Logo_Graffiti: HouseNoDestruct
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