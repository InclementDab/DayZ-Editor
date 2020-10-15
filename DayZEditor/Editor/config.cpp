////////////////////////////////////////////////////////////////////
//DeRap: editor\config.bin
//Produced from mikero's Dos Tools Dll version 6.44
//'now' is Tue Jul 21 08:06:58 2020 : 'file' last modified on Tue Jul 21 07:35:07 2020
//http://dev-heaven.net/projects/list_files/mikero-pbodll
////////////////////////////////////////////////////////////////////

#define _ARMA_

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
		model = "\DayZEditor\Editor\Camera.p3d";
	};

	class Inventory_Base;
	class TranslationWidget: Inventory_Base
	{
		scope = 2;
		itemSize[]={16,16};
		model = "\DayZEditor\Editor\TranslationWidget.p3d";
		hiddenSelections[] = {"TranslateX", "TranslateY", "TranslateZ"};
		hiddenSelectionsMaterials[] = {"\DayZEditor\Editor\data\TranslationWidget.rvmat"};
		hiddenSelectionsTextures[] = {"\DayZEditor\Editor\data\TranslationWidget.paa"};
	};

	class RotationWidget: Inventory_Base
	{
		scope = 1;
		itemSize[]={16,16};
		model = "\DayZEditor\Editor\RotationWidget.p3d";
		hiddenSelections[] = {"RotationX", "RotationY", "RotationZ"};
	};

	class BrushBase: Inventory_Base
	{
		scope = 1;
		model = "\DayZEditor\Editor\BrushBase.p3d";
		hiddenSelections[] = {"BrushBase"};
		hiddenSelectionTextures[] = { "DayZEditor\\Editor\\data\\BrushDelete.paa" };
	};

	class BoundingBoxBase: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BoundingBoxBase.p3d";
		hiddenSelections[] = {"BoundingBoxSelection"};
	};
	class ThunderboltNorm: HouseNoDestruct
	{
		model = "\DZ\Data\data\blesk1.p3d";
		soundSetNear = "ThunderNorm_Near_SoundSet";
		soundSetFar = "ThunderNorm_Far_SoundSet";
	};
	class ThunderboltHeavy: HouseNoDestruct
	{
		model = "\DZ\Data\data\blesk2.p3d";
		soundSetNear = "ThunderHeavy_Near_SoundSet";
		soundSetFar = "ThunderHeavy_Far_SoundSet";
	};
	
	class DebugCylinder: HouseNoDestruct
	{
		scope=2;
		model="DayZEditor\editor\DebugCylinder.p3d";
	};
};