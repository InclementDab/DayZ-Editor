class CfgPatches
{
	class Editor_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts","JM_CF_Scripts","DF_Scripts", "DZ_Gear_Navigation"};
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
				value = "";
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/1_core"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"DayZEditor/scripts/common","DayZEditor/scripts/5_Mission"};
			};
		};
	};
};

class CfgLocationTypes
{
    class Name
    {
        name="keypoint";
        drawStyle="name";
        texture="#(argb,1,1,1)color(1,1,1,1)";
        color[]={1, 1, 1, 1};
        size=0;
        font="gui/fonts/metron22";
        textSize=0.03;
        shadow=0;
        importance=1;
    };
    class NameIcon
    {
        name="keypoint";
        drawStyle="icon";
        texture="#(argb,1,1,1)color(1,1,1,1)";
        color[]={1, 1, 1, 1};
        size=0;
        font="gui/fonts/metron22";
        textSize=0.03;
        shadow=0;
        importance=1;
    };
};
class MapDefaults;
class RscMapControl: MapDefaults
{
	font = "gui\fonts\metron22";
	scaleMin = 0.125;
	scaleMax = 4.0;
	scaleDefault = 0.32;
	ptsPerSquareSea=8;
	ptsPerSquareTxt=10;
	ptsPerSquareCLn=10;
	ptsPerSquareExp=10;
	ptsPerSquareCost=10;
	ptsPerSquareFor=9;
	ptsPerSquareForEdge=4;
	ptsPerSquareRoad=2;
	ptsPerSquareObj=13;
	maxSatelliteAlpha=1;
	alphaFadeStartScale=10;
	alphaFadeEndScale=10;

	fontLabel = "gui\fonts\metron22";
	fontGrid = "gui\fonts\metron22";
	fontUnits = "gui\fonts\metron22";
	fontNames = "gui\fonts\metron22";
	fontInfo = "gui\fonts\metron22";
	fontLevel = "gui\fonts\metron22";
};