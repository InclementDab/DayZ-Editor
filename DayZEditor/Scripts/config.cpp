class CfgPatches
{
	class Editor_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Scripts", "JM_CF_Scripts","DF_Scripts", "DZ_Gear_Navigation", "DZ_Worlds_Enoch"};
	};
};
class CfgMods
{
	class DayZEditor
	{
		name = "DayZ Editor";
		version = "1.2";
		dir = "DayZEditor";
		credits = "InclementDab";
		creditsJson = "DayZEditor/Scripts/Data/Credits.json";
		versionPath = "DayZEditor/scripts/Data/Version.hpp";
		inputs = "DayZEditor/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class imageSets
			{
				files[] = {
					"DayZEditor/gui/imagesets/dayz_editor_gui.imageset",
					"DabsFramework/gui/imagesets/prefabs.imageset",
					"DabsFramework/gui/icons/brands.imageset",
					"DabsFramework/gui/icons/light.imageset",
					"DabsFramework/gui/icons/regular.imageset",
					"DabsFramework/gui/icons/solid.imageset",
					"DabsFramework/gui/icons/thin.imageset"
				};
			};
			class widgetStyles
			{
				files[] = {
					"DabsFramework/gui/looknfeel/prefabs.styles",
					"DayZEditor/gui/looknfeel/editorwidgets.styles",
					"DayZEditor/gui/looknfeel/prefabs.styles"
				};
			};
			class engineScriptModule
			{
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/1_core"};
			};
			class gameScriptModule
			{
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "CreateEditorMission";
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/5_Mission"};
			};
		};
	};
};

class MapDefaults;
class RscMapControl: MapDefaults
{
	scaleMin = 0.05;
	scaleMax = 0.85;
	scaleDefault = 0.85;
	ptsPerSquareSea = 8;
	ptsPerSquareTxt = 10;
	ptsPerSquareCLn = 10;
	ptsPerSquareExp = 10;
	ptsPerSquareCost = 10;
	ptsPerSquareFor = 99;
	ptsPerSquareForEdge = 99;
	ptsPerSquareRoad = 4;
	ptsPerSquareObj = 15;
	maxSatelliteAlpha = 1;
	alphaFadeStartScale = 1;
	alphaFadeEndScale = 1;
	userMapPath = "dz\gear\navigation\data\usermap";
	maxUserMapAlpha = 0.2;
	alphaUserMapFadeStartScale = 0.5;
	alphaUserMapFadeEndScale = 0.8;
	showCountourInterval = 1;
	colorLevels[] = {0.65,0.6,0.45,0.3};
	colorSea[] = {0.2,0.5,0.7,1};
	colorForest[] = {0.36,0.78,0.08,0};
	colorRocks[] = {0.5,0.5,0.5,0.2};
	colorCountlines[] = {0.85,0.8,0.65,0.1};
	colorMainCountlines[] = {0.45,0.4,0.25,0};
	colorCountlinesWater[] = {0.25,0.4,0.5,0.3};
	colorMainCountlinesWater[] = {0.25,0.4,0.5,0.9};
	colorPowerLines[] = {0.1,0.1,0.1,1};
	colorRailWay[] = {0.8,0.2,0,1};
	colorForestBorder[] = {0.4,0.8,0,0};
	colorRocksBorder[] = {0.5,0.5,0.5,0};
	colorOutside[] = {1,1,1,1};
	colorTracks[] = {0.78,0.66,0.34,1};
	colorRoads[] = {0.69,0.43,0.23,1};
	colorMainRoads[] = {0.53,0.35,0,1};
	colorTracksFill[] = {0.96,0.91,0.6,1};
	colorRoadsFill[] = {0.92,0.73,0.41,1};
	colorMainRoadsFill[] = {0.84,0.61,0.21,1};
	colorGrid[] = {0.9,0.9,0.9,0.8};
	colorGridMap[] = {0.9,0.9,0.9,0.2};
	fontNames = "gui/fonts/sdf_MetronBook24";
	sizeExNames = 0.03;
	colorNames[] = {1,1,1,1};
	fontGrid = "gui/fonts/sdf_MetronBook24";
	sizeExGrid = 0.02;
	fontLevel = "gui/fonts/sdf_MetronBook24";
	sizeExLevel = 0.01;
	colorMountPoint[] = {0.45,0.4,0.25,0};
	mapPointDensity = 0.12;
	text = "";
	fontLabel = "gui/fonts/sdf_MetronBook24";
	fontInfo = "gui/fonts/sdf_MetronBook24";
};

class CfgMissions
{
    class Cutscenes
    {
        class ChernarusPlusIntro
        {
            directory="$CurrentDir:\MainMenu.ChernarusPlus";
        };

		class EnochIntro
		{
			directory="$CurrentDir:\MainMenu.Enoch";
		};

		class SakhalIntro
		{
			directory="$CurrentDir:\MainMenu.Sakhal";
		};
    };
};

class CfgWorlds
{
    class CAWorld;
    class ChernarusPlus: CAWorld
    {
        ceFiles="DayZEditor\Scripts\Data\ce";
	};

	class Enoch: CAWorld
	{
		ceFiles="DayZEditor\Scripts\Data\ce";
	};

	class Sakhal: CAWorld
	{
		ceFiles="DayZEditor\Scripts\Data\ce";
	};
};