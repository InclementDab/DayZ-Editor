
class CfgPatches
{
	class Editor_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgMods 
{
	class Editor_Scripts_
	{
		dir = "Editor";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class coreScriptModule
			{
				value = "";
				files[] = {"DayZEditor/scripts/1_core"};
			};

			class gameScriptModule
			{
				value="";
				files[] = {"DayZEditor/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value="";
				files[] = {"DayZEditor/scripts/4_World"};
			};

			class missionScriptModule 
			{
				value="";
				files[] = {"DayZEditor/scripts/5_Mission"};
			};
		};
	};
};