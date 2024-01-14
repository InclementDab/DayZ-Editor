
class CfgPatches
{
	class Editor_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts", "JM_CF_Scripts", "DF_Scripts"};
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
		dependencies[] =
			{
				"Game", "World", "Mission"};
		class defs
		{
			class imageSets
			{
				files[] =
					{
						"DayZEditor/gui/imagesets/dayz_editor_gui.imageset",
						"DabsFramework/gui/imagesets/prefabs.imageset",
						"DayZEditor/gui/icons/tree.imageset",
						"DayZEditor/gui/icons/icons.imageset"
				};
			};

			class widgetStyles
			{
				files[] =
					{
						"DayZEditor/gui/looknfeel/editorwidgets.styles",
						"DayZEditor/gui/looknfeel/prefabs.styles",
						"DabsFramework/gui/looknfeel/prefabs.imageset",
				};
			};

			class engineScriptModule
			{
				value = "";
				files[] =
					{
						"DayZEditor/scripts/common",
						"DayZEditor/scripts/1_core"};
			};

			class gameScriptModule
			{
				value = "";
				files[] =
					{
						"DayZEditor/scripts/common",
						"DayZEditor/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
					{
						"DayZEditor/scripts/common",
						"DayZEditor/scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] =
					{
						"DayZEditor/scripts/common",
						"DayZEditor/scripts/5_Mission"};
			};
		};
	};
};
