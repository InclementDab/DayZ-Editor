class CfgPatches
{
	class Editor_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data", "DZ_Sounds_Effects"};
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
	class THX_Soundshader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\thx", 1}};
	};

	class Plop_Notification_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Notifications\notification-plop", 1}};
	};
	class PlopLow_Notification_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Notifications\notification-plop-low", 1}};
	};

	class Lock_Notification_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Notifications\notification-lock", 1}};
	};
	class Flute_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Notifications\notification-lock", 1}};
	};
	class Highlight_SoundShader: Editor_Soundshader
	{
		samples[] = {{"DayZEditor\sounds\Notifications\highlight", 1}};
	};

	class Sea_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\Sea",1}};
		volume = "(waterDepth factor [10,15]) * (waterDepth factor [25,18]) * (sea factor[0.5,0.9])";
	};
	class WindForestLight_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\WindForestLight_-9db",1}};
		volume = "(0.65 * (windy factor[0.2, 0.6])) * (( 2 * trees ) / ( trees + 1 )) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * (1 - 0.3*night)";
	};
	class WindForestHeavy_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\WindForestHeavy_-9db",1}};
		volume = "(windy factor[0.55, 1]) * (( 2 * trees ) / ( trees + 1 )) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * (1 - 0.3*night)";
	};
	class WindHousesLight_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\WindHousesLight_-9db",1}};
		volume = "(0.65 * (windy factor[0.2, 0.6])) * ( 1 - (( 2 * trees ) / ( trees + 1 ))) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * 0.7 * (1-0.75*(waterDepth factor[10,20])) * (1 - 0.3*night)";
	};
	class WindHousesHeavy_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\WindHousesHeavy_-9db",1}};
		volume = "(windy factor[0.55, 1]) * ( 1 - (( 2 * trees ) / ( trees + 1 ))) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * 0.8 * (1-0.75*(waterDepth factor[10,20])) * (1 - 0.3*night)";
	};
	class WindHills_SoundShader
	{
		samples[] = {{"DayZEditor\Sounds\Environment\WindHills_-9db",1}};
		volume = "(windy factor[0.6,1]) * (hills factor[0.5,1]) * 0.4 * (1 - 0.3*night)";
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
	class THX_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"THX_Soundshader"};
	};

	class Plop_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Plop_Notification_SoundShader"};
	};
	class PlopLow_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"PlopLow_Notification_SoundShader"};
	};
	
	class Lock_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Lock_Notification_SoundShader"};
	};

	class Flute_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Flute_SoundShader"};
	};
	class Highlight_SoundSet: Editor_Soundset
	{
		soundShaders[] = {"Highlight_SoundShader"};
	};
};

