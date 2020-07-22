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
		model = "DayZEditor\Editor\data\Camera.p3d";
	};
	class TranslationWidget: HouseNoDestruct
	{
		scope = 1;
		model = "DayZEditor\Editor\data\TranslationWidget.p3d";
		hiddenSelections[] = {"TranslateX", "TranslateY", "TranslateZ", "BoundingBox"};
		hiddenSelectionsMaterials[] = {"DayZEditor\Editor\data\TranslationWidget\TranslationWidget.rvmat"};
		hiddenSelectionsTextures[] = {"DayZEditor\Editor\data\TranslationWidget\TranslationWidget.paa"};
	};
	class RotationWidget: HouseNoDestruct
	{
		scope = 1;
		model = "DayZEditor\Editor\data\RotationWidget.p3d";
	};


	class BoundingBoxBase: HouseNoDestruct
	{
		scope = 1;
		model = "DayZEditor\Editor\data\BoundingBoxBase.p3d";
		hiddenSelections[] = {"MainSection1"};
		hiddenSelectionsMaterials[] = {"DayZEditor\Editor\data\BoundingBox\BoundingBoxShown.rvmat"};
		hiddenSelectionsTextures[] = {"DayZEditor\Editor\data\BoundingBox\box_shown.paa"};
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
class CfgMissions
{
	class Missions
	{
		class ChernarusPlusEditor
		{
			directory = "DayZEditor\Editor\mission\DayZEditor.ChernarusPlus";
		};
	};
};
