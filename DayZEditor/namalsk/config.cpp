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
        model = "\DZ\characters\glasses\NVG_Headstrap_g.p3d";
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
			energyUsagePerSecond = 0.3;
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
        radius = 15000;
	};

	class Namalsk_Item_SoundShader
	{
		volume = 1;
		frequency = 1;
		range = 10;
		limitation = 0;
	};

	class Blowout_Begin_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_begin", 1},
			{"DayZEditor\namalsk\blowout\blowout_begin_02", 1}
		};
	};

	class Blowout_FullWave_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_full_wave", 1}};
        volume = 0.2;
	};

	class Blowout_Voices_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = 
		{
			{"DayZEditor\namalsk\blowout\blowout_psy_voices", 1},
			{"DayZEditor\namalsk\blowout\sabaoth_psi", 1}
		};
        volume = 0.1;
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

    class Blowout_Drone_SoundShader: Namalsk_Environment_SoundShader
    {
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_drone1", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_drone2", 1}
		};
    };

    class Blowout_DroneQ_SoundShader: Namalsk_Environment_SoundShader
    {
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_drone1r", 1}, 
			{"DayZEditor\namalsk\blowout\blowout_drone2r", 1}
		};
    };

    class Blowout_Alarm_SoundShader: Namalsk_Environment_SoundShader
    {
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_alarm", 1}};
        range = 10000;
    };

	class Blowout_Impact_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {
			{"DayZEditor\namalsk\blowout\blowout_wave_01", 1},
			{"DayZEditor\namalsk\blowout\blowout_wave_02", 1},
			{"DayZEditor\namalsk\blowout\blowout_impact", 1},
			{"DayZEditor\namalsk\blowout\blowout_impact_02", 1}};
	};

	class Blowout_Contact_SoundShader: Namalsk_Environment_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\blowout_contact", 1}};
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

	class Blowout_NearImpact_SoundShader: Namalsk_Item_SoundShader
	{
		samples[] = {{"DayZEditor\namalsk\blowout\pux_blast", 1}};
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
        distanceFilter = "none";
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

    class Blowout_Drone: Namalsk_Soundset
    {
        soundShaders[] = {"Blowout_Drone_SoundShader", "Blowout_DroneQ_SoundShader"};
    };

    class Blowout_Alarm: Namalsk_Soundset
    {
        soundShaders[] = {"Blowout_Alarm_SoundShader"};
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

	class Blowout_NearImpact: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_NearImpact_SoundShader"};
	};

	class Blowout_Impact: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Impact_SoundShader"};
	};
	
	class Blowout_Contact: Namalsk_Soundset
	{
		soundShaders[] = {"Blowout_Contact_SoundShader"};
	};
};