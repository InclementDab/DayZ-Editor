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
    class ForestDay_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\MeadowDay_-30db", 1}};
        volume="0.3 * ( (( 1.6 * trees ) / ( trees + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (((1 - (rain factor [0.25, 1]))) max (0.8))";
    };
    class ForestNight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\ForestNight_-30db", 1}};
        volume="0.3 * ( (( 1.6 * trees ) / ( trees + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (( 1.6 * night ) / ( night + 0.6 )) * (rain factor[0.3,0.1]) * (windy factor[0.5,0.2])";
    };
    class ForestDayBirds_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\ForestDayBirds", 1}};
        volume="0.3 * 0.25 * (( 1.6 * forest ) / ( forest + 0.6 )) * ( 1 - (( 1.6 * night ) / ( night + 0.6 ))) * (rain factor[0.3,0.1]) *  (windy factor[0.7,0.2]) * ((overcast factor[0.8,0.6]) min (fog factor[0.9,0.4])) * (0.25 + 0.75*(shooting factor [0.6,1])) * (altitudeGround factor[15,10])";
    };
    class HousesDay_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\HousesDay_-30db", 1}};
        volume="0.3 * ( (( 1.6 * houses ) / ( houses + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (((1 - (rain factor [0.25, 1]))) max (0.5))";
    };
    class HousesNight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\HousesNight_-30db", 1}};
        volume="0.3 * ( (( 1.6 * houses ) / ( houses + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (( 1.6 * night ) / ( night + 0.6 )) * (rain factor[0.3,0.1]) * (windy factor[0.4,0.1])";
    };
    class MeadowDay_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\ForestDay_-30db", 1}};
        volume="0.3 * ( (( 1.6 * meadow ) / ( meadow + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (((1 - (rain factor [0.25, 1]))) max (0.5))";
    };
    class MeadowDayCrickets_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\MeadowDayCrickets", 1}};
        volume="0.3 * (( 1.6 * meadow ) / ( meadow + 0.6 )) * ( 1 - (( 1.6 * night ) / ( night + 0.6 ))) * (rain factor[0.3,0.1]) * (windy factor[0.7,0.2]) * ((overcast factor[0.8,0.6]) min (fog factor[0.9,0.4])) * (daytime factor[0.6,0.58]) * (0.25 + 0.75*(shooting factor [0.6,1]))";
    };
    class MeadowEveningCrickets_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\MeadowEveningCrickets", 1}};
        volume="0.3 * (( 1.6 * meadow ) / ( meadow + 0.6 )) * ( 1 - (( 1.6 * night ) / ( night + 0.6 ))) * (rain factor[0.3,0.1]) * (windy factor[0.7,0.2]) * ((overcast factor[0.8,0.6]) min (fog factor[0.9,0.4])) * (daytime factor[0.58,0.6]) * 0.7 * (0.25 + 0.75*(shooting factor [0.6,1]))";
    };
    class MeadowNight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\MeadowNight_-30db", 1}};
        volume="0.3 * ( (( 1.6 * meadow ) / ( meadow + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (( 1.6 * night ) / ( night + 0.6 )) * (rain factor[0.3,0.1]) * (windy factor[0.5,0.2]) * ((daytime factor[0.5,0.6]) + (daytime factor[0.1,0]))";
    };
    class MeadowNight2_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\MeadowNight2_-30db", 1}};
        volume="0.3 * ( (( 1.6 * meadow ) / ( meadow + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (( 1.6 * night ) / ( night + 0.6 )) * (rain factor[0.3,0.1]) * (windy factor[0.5,0.2]) * ((daytime factor[0,0.1]) - (daytime factor[0.5,0.6]))";
    };
    class Coast_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\Coast", 1}};
        volume="0.3 * (((altitudeSea - altitudeGround) factor[4.5,1]) max ((coast factor [0.955,1]) / 1.5)) * (sea factor[0.1,0.2]) * (altitudeSea factor[15,10]) * (houses factor[1,0.9]) * (waterDepth factor[15,10])";
    };
    class Sea_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\Sea", 1}};
        volume="0.3 * (waterDepth factor [10,15]) * (waterDepth factor [25,18]) * (sea factor[0.5,0.9])";
    };
    class FlyAmbience_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\FlyAmbience", 1}};
        volume="0.3 * (windy factor[0.4,0]) * meadow * (1 - night) * (0.25 + 0.75*(shooting factor [0.6,1])) * (1 - (fog factor[0.1,0.4])) * (1 - (rain factor[0.15,0.2]))";
    };
    class RainForestLight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainForestLight_-3db", 1}};
        volume="0.3 * (((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * 0.5 * (rain factor[0, 0.1]) * (rain factor[0.55, 0.3]) * (0.25 + 0.75*rain)";
    };
    class RainForestMedium_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainForestMedium_-3db", 1}};
        volume="0.3 * (((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (rain factor[0.2, 0.45]) * (rain factor[0.9, 0.6]) * (0.25 + 0.75*rain)";
    };
    class RainForestHeavy_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainForestHeavy_-3db", 1}};
        volume="0.3 * (((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 )))) * (rain factor[0.55, 0.85]) * (0.25 + 0.75*rain)";
    };
    class RainHousesLight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainHousesLight_-3db", 1}};
        volume="0.3 * (0.25 + 0.75 * ( (( 1.6 * houses ) / ( houses + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))))) * 0.5 * (rain factor[0, 0.1]) * (rain factor[0.55, 0.3]) * (0.25 + 0.75*rain)";
    };
    class RainHousesMedium_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainHousesMedium_-3db", 1}};
        volume="0.3 * (0.25 + 0.75 * ( (( 1.6 * houses ) / ( houses + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))))) * (rain factor[0.2, 0.45]) * (rain factor[0.9, 0.6]) * (0.25 + 0.75*rain)";
    };
    class RainHousesHeavy_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\RainHousesHeavy_-3db", 1}};
        volume="0.3 * (0.25 + 0.75 * ( (( 1.6 * houses ) / ( houses + 0.6 )) / ((( 1.6 * trees ) / ( trees + 0.6 )) + (( 1.6 * houses ) / ( houses + 0.6 )) + (( 1.6 * meadow ) / ( meadow + 0.6 )) + (( 1.6 * sea ) / ( sea + 0.6 ))))) * (rain factor[0.55, 0.85]) * (0.25 + 0.75*rain)";
    };
    class WindForestLight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\WindForestLight_-9db", 1}};
        volume="0.3 * (0.65 * (windy factor[0.2, 0.6])) * (( 2 * trees ) / ( trees + 1 )) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * (1 - 0.3*night)";
    };
    class WindForestHeavy_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\WindForestHeavy_-9db", 1}};
        volume="0.3 * (windy factor[0.55, 1]) * (( 2 * trees ) / ( trees + 1 )) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * (1 - 0.3*night)";
    };
    class WindHousesLight_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\WindHousesLight_-9db", 1}};
        volume="0.3 * (0.65 * (windy factor[0.2, 0.6])) * ( 1 - (( 2 * trees ) / ( trees + 1 ))) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * 0.7 * (1-0.75*(waterDepth factor[10,20])) * (1 - 0.3*night)";
    };
    class WindHousesHeavy_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\WindHousesHeavy_-9db", 1}};
        volume="0.3 * (windy factor[0.55, 1]) * ( 1 - (( 2 * trees ) / ( trees + 1 ))) * (((1 - (rain factor [0.25, 1]))) max (0.8)) * 0.8 * (1-0.75*(waterDepth factor[10,20])) * (1 - 0.3*night)";
    };
    class WindHills_SoundShader
    {
        samples[]={{"DZ\sounds\environment\ambients\WindHills_-9db", 1}};
        volume="0.3 * (windy factor[0.6,1]) * (hills factor[0.5,1]) * 0.4 * (1 - 0.3*night)";
    };

	class Editor_SoundShaderBase
	{
		volume = 0.1;
		frequency = 1;
		range = 10;
		rangeCurve[] = {{0,1},{5,0.7},{10,0}};
		limitation = 0;
	};


};

class CfgSoundsets
{
	class Editor_SoundSetBase
	{
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
	};



};

