
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
	class DZ_Editor
	{
		name = "DayZ Editor";
		picture = "DayZEditor/gui/images/dayz_editor_icon_hq.edds";
		logo = "DayZEditor/gui/images/dayz_editor_icon_hq.edds";
		logoSmall = "DayZEditor/gui/images/dayz_editor_icon_hq.edds";
		logoOver = "DayZEditor/gui/images/dayz_editor_icon_hq.edds";
		tooltip = "DayZ Editor";
		overview = "The DayZ Editor is a 3D Editing tool built to design areas, create screenshots, or test game mechanics. Please report all bugs / suggestions to our Discord at discord.io/DayZEditor. Created by InclementDab";
		action = "https://dab.dev";
		author = "InclementDab";
		authorID = "76561198247958888";
		version = "1.2";
		dir = "DayZEditor";
		credits = "InclementDab";
		
		creditsJson = "DayZEditor/Scripts/Data/Credits.json";
		versionPath = "DayZEditor/scripts/Data/Version.hpp";
		inputs = "DayZEditor/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"DayZEditor/gui/imagesets/dayz_editor_gui.imageset",
					"DabsFramework/gui/imagesets/prefabs.imageset"
				};
			};

			class widgetStyles
			{
				files[]=
				{
					"DayZEditor/gui/looknfeel/editorwidgets.styles",
					"DabsFramework/gui/looknfeel/prefabs.imageset"
				};
			};

			class engineScriptModule
			{
				value="";
				files[] =
				{
					"DayZEditor/scripts/common",
					"DayZEditor/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"DayZEditor/scripts/common",
					"DayZEditor/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"DayZEditor/scripts/common",
					"DayZEditor/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"DayZEditor/scripts/common",
					"DayZEditor/scripts/5_Mission"
				};
			};
		};
	};
};
