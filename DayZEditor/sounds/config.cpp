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

	class Namalsk_Environment_SoundShader
	{
		volume = 1;
		frequency = 1;
		range = 15000;
		limitation = 0;
	};

	class Namalsk_Item_SoundShader
	{
		volume = 0.7;
		frequency = 1;
		range = 10;
		limitation = 0;
	};

	class Blowout_Begin_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\blowout_begin", 1}};
	};

	class Blowout_FullWave_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\blowout_full_wave", 1}};
	};

	class Blowout_Voices_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\blowout_psy_voices", 1}};
	};

	class Blowout_Hit_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\sounds\blowout\blowout_hit_1", 1}, 
			{"DayZEditor\sounds\blowout\blowout_hit_2", 1}, 
			{"DayZEditor\sounds\blowout\blowout_hit_3", 1}
		};
	};

	class Blowout_Wave_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\sounds\blowout\blowout_wave_1", 1}, 
			{"DayZEditor\sounds\blowout\blowout_wave_2", 1}, 
			{"DayZEditor\sounds\blowout\blowout_wave_3", 1}
		};
	};


	class APSI_Enable_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\apsi_enable", 1}};
	};

	class APSI_Detect_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\apsi_detect", 1}};
	};

	class APSI_Disable_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\sounds\blowout\apsi_disable", 1}};
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

	class Namalsk_Soundset
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};

	class Blowout_Begin: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Begin_SoundShader"};
	};

	class Blowout_FullWave: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_FullWave_SoundShader"};
	};

	class Blowout_Voices: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Voices_SoundShader"};
	};

	class Blowout_Hit: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Hit_SoundShader"};
	};

	class Blowout_Wave: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Wave_SoundShader"};
	};

	class APSI_Enable: Namalsk_Soundset
	{
		soundShaders[] = {"APSI_Enable_SoundShader"};
	};

	class APSI_Detect: Namalsk_Soundset
	{
		soundShaders[] = {"APSI_Detect_SoundShader"};
	};

	class APSI_Disable: Namalsk_Soundset
	{
		soundShaders[] = {"APSI_Disable_SoundShader"};
	};
};

