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
    class Editor_EnvironemntSoundShaderBase
    {
		volume = 1.0;
		frequency = 1;
		range = 25.0;
		rangeCurve[] = {{0,1},{15,0.7},{25,0}};
		limitation = 0;
	};

    class PianoDrop_SoundShader: Editor_EnvironemntSoundShaderBase
	{
		samples[] = {{"Editor\Sounds\Environment\Piano", 1}};
	};
};

class CfgSoundSets
{
    class Editor_EnvironmentSoundSetBase
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 1;
		loop = 0;
	};
    
    class PianoDrop: Editor_EnvironmentSoundSetBase
	{
		SoundShaders[] = {"PianoDrop_SoundShader"};
	};
};