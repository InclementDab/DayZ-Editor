class CfgPatches
{
	class Editor_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Sounds_Effects"};
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
	class MainMenu_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\when_im_gone_ltr", 1}};
	};
	class MainMenu2_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\what_dreams_may_come_ltr", 1}};
	};
	class MainMenu3_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\subconscious_ltr", 1}};
	};
	class MainMenu4_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\fast_lee_haris_ltr", 1}};
	};
	class Click0_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Click0", 1}};
	};
	class Click1_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Click1", 1}};
	};
	class Click2_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Click2", 1}};
	};
	class Click3_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Click3", 1}};
	};
	class Click4_Editor_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Click4", 1}};
	};
	class THX_Soundshader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\thx", 1}};
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

	class Click_Editor_Soundset: Editor_Soundset
	{
		soundShaders[]={"Click0_Editor_SoundShader", "Click1_Editor_SoundShader", "Click2_Editor_SoundShader", "Click3_Editor_SoundShader", "Click4_Editor_SoundShader"};
	};

    class Music_Menu_SoundSet
    {
        soundShaders[]={"MainMenu_Editor_SoundShader"};
        volumeFactor=1;
        frequencyFactor=1;
        spatial=0;
    };
    class Music_Menu_2_SoundSet
    {
        soundShaders[]={"MainMenu2_Editor_SoundShader"};
        volumeFactor=1;
        frequencyFactor=1;
        spatial=0;
    };
    class Music_Menu_3_SoundSet
    {
        soundShaders[]={"MainMenu3_Editor_SoundShader"};
        volumeFactor=1;
        frequencyFactor=1;
        spatial=0;
    };
    class Music_Menu_4_SoundSet
    {
        soundShaders[]={"MainMenu4_Editor_SoundShader"};
        volumeFactor=1;
        frequencyFactor=1;
        spatial=0;
    };

	class Notification_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Notification_Soundshader"};
	};
	class THX_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"THX_Soundshader"};
	};
};

