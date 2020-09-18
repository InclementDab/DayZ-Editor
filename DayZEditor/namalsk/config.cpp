class CfgPatches
{
	class EditorNamalsk
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Characters"};
	};
};

class CfgVehicles
{
    class Clothing_Base;
	class Clothing: Clothing_Base{};
	class APSI: Clothing
	{
		scope = 2;
		displayName = "$STR_APSI";
        model = "\DayZEditor\namalsk\CameraOld.p3d";
		descriptionShort = "$STR_APSI_DESC";
        inventorySlot[] = {"Headgear"};
        simulation = "clothing";
		vehicleClass = "Clothing";
		itemInfo[] = {"Clothing","Headgear"};
		weight = 1000;
		itemSize[] = {4,3};
        heatIsolation = 0.13;
        attachments[] = {"BatteryD"};

        class EnergyManager
		{
			hasIcon = 1;
			energyUsagePerSecond = 0.1;
			plugType = 1;
			attachmentAction = 1;
		};

        class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 70;
				};
			};
		};
	};
};

class CfgSoundShaders
{
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
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_begin", 1}};
	};

	class Blowout_FullWave_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_full_wave", 1}};
	};

	class Blowout_Voices_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_psy_voices", 1}};
	};

	class Blowout_Hit_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_hit_1", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_hit_2", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_hit_3", 1}
		};
	};

	class Blowout_Wave_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_wave_1", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_wave_2", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_wave_3", 1}
		};
	};


	class APSI_Enable_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\apsi_enable", 1}};
	};

	class APSI_Detect_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\apsi_detect", 1}};
	};

	class APSI_Disable_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\apsi_disable", 1}};
	};
};

class CfgSoundSets
{
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