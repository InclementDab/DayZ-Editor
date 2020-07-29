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
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Static;
	class HouseNoDestruct;
	class EditorObject: HouseNoDestruct
	{
		scope = 1;
		displayName = "Editor Object";
	};
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
		hiddenSelectionsMaterials[] = {"\DayZEditor\Editor\data\TranslationWidget\TranslationWidget.rvmat"};
		hiddenSelectionsTextures[] = {"\DayZEditor\Editor\data\TranslationWidget\TranslationWidget.paa"};
	};
	class RotationWidget: Inventory_Base
	{
		scope = 1;
		itemSize[]={16,16};
		model = "\DayZEditor\Editor\RotationWidget.p3d";
		hiddenSelections[] = {"RotationX", "RotationY", "RotationZ"};
	};

	class BrushBase: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BrushBase.p3d";
		hiddenSelectionsTextures[] = { "\DayZEditor\Editor\data\BrushBase.paa" };
	};

	class BoundingBoxBase: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZEditor\Editor\BoundingBoxBase.p3d";
		hiddenSelections[] = {"BoundingBoxBase"};
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
};