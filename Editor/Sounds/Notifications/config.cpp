class CfgPatches
{
	class Editor_EnvironmentSounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"Editor_Sounds"};
	};
};

class CfgSoundShaders
{
    class Editor_NotificationSoundShaderBase
	{
		volume = 1;
		frequency = 1;
		range = 1;
		rangeCurve[] = {{0,1},{1,1}};
		limitation = 0;
	};

	class Plop_Notification_SoundShader: Editor_NotificationSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Notifications\notification-plop", 1}};
	};
	class PlopLow_Notification_SoundShader: Editor_NotificationSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Notifications\notification-plop-low", 1}};
	};

	class Lock_Notification_SoundShader: Editor_NotificationSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Notifications\notification-lock", 1}};
	};
	class Flute_SoundShader: Editor_NotificationSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Notifications\notification-lock", 1}};
	};
	class Highlight_SoundShader: Editor_NotificationSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Notifications\highlight", 1}};
	};
};

class CfgSoundSets
{
    class Editor_NotificationSoundSetBase
	{
		sound3DProcessingType = "Music_Event_3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 0;
		doppler = 0;
		loop = 0;
	};
	class Plop: Editor_NotificationSoundSetBase
	{
		SoundShaders[] = {"Plop_Notification_SoundShader"};
	};
	class PlopLow: Editor_NotificationSoundSetBase
	{
		SoundShaders[] = {"PlopLow_Notification_SoundShader"};
	};
	
	class Lock: Editor_NotificationSoundSetBase
	{
		SoundShaders[] = {"Lock_Notification_SoundShader"};
	};

	class Flute: Editor_NotificationSoundSetBase
	{
		SoundShaders[] = {"Flute_SoundShader"};
	};
	class Highlight: Editor_NotificationSoundSetBase
	{
		SoundShaders[] = {"Highlight_SoundShader"};
	};
};