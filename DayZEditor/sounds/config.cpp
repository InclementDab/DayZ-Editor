////////////////////////////////////////////////////////////////////
//DeRap: editor\config.bin
//Produced from mikero's Dos Tools Dll version 6.44
//'now' is Tue Jul 21 08:06:58 2020 : 'file' last modified on Tue Jul 21 07:35:07 2020
//http://dev-heaven.net/projects/list_files/mikero-pbodll
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class Editor_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgSoundShaders
{
	class Editor_Soundshader
	{
		volume = 0.1;
		frequency = 1;
		range = 10;
		rangeCurve[] = {{0,1},{5,0.7},{10,0}};
		limitation = 0;
	};

	class Notification_Soundshader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\notification", 1}};
	};
};


class CfgSoundSets
{
	class Editor_Soundset
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};
	class Notification_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Notification_Soundshader"};
	};
};

