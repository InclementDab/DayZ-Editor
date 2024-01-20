
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
	class Editor
	{
		name = "DayZ Editor";
		version = "1.2";
		dir = "Editor";
		credits = "InclementDab";
		creditsJson = "Editor/Scripts/Data/Credits.json";
		versionPath = "Editor/scripts/Data/Version.hpp";
		inputs = "Editor/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class imageSets
			{
				files[] = {
					"Editor/gui/imagesets/dayz_editor_gui.imageset",
					"DabsFramework/gui/imagesets/prefabs.imageset",
					"Editor/gui/imagesets/tree.imageset"
				};
			};

			class widgetStyles
			{
				files[] = {
					"Editor/gui/looknfeel/editorwidgets.styles",
					"Editor/gui/looknfeel/prefabs.styles",
					"DabsFramework/gui/looknfeel/prefabs.imageset"
				};
			};

			class engineScriptModule
			{
				value = "";
				files[] = {"Editor/scripts/1_core"};
			};

			class gameScriptModule
			{
				value = "";
				files[] = {"Editor/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"Editor/scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] = {"Editor/scripts/5_Mission"};
			};
		};
	};
};
