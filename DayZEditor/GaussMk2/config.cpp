class CfgPatches
{
	class A2GaussMk2_Base
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Firearms","DZ_Weapons_Ammunition","DZ_Weapons_Magazines"};
		magazines[] = {"A2GaussMk2_Magazine"};
		ammo[] = {};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class cfgWeapons
{
	class M4A1;
	class A2GaussMk2_Base: M4A1
	{
		scope = 0;
		descriptionShort = "";
		model = "\Namalsk_Weapon\GaussMk2\A2GaussMk2.p3d";
		weight = 13000;
		itemSize[] = {10,4};
		dexterity = 1.8;
		absorbency = 0.1;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30,25};
		discreteDistance[] = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};
		discreteDistanceInitIndex = 0;
		modelOptics = "-";
		distanceZoomMin = 100;
		distanceZoomMax = 100;
		PPDOFProperties[] = {0,0,0,0,0,0};
		optics = 1;
		opticsFlare = 0;
		value = 0;
		chamberedRound = "";
		chamberSize = 1;
		attachments[] = {"weaponOptics","weaponWrap","weaponOpticsHunting"};
		chamberableFrom[] = {"Ammo_ER7RFW"};
		magazines[] = {"A2GaussMk2_Magazine"};
		magazineSwitchTime = 0.25;
		barrelArmor = 3000;
		ejectType = 1;
		recoilModifier[] = {1,3,2};
		reloadAction = "ReloadSVD";
		modes[] = {"SemiAuto"};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[] = {"ER7_Shot_SoundSet","FNFAL_Tail_SoundSet","FNFAL_InteriorTail_SoundSet"};
			reloadTime = 0.425;
			recoil = "recoil_AKM";
			recoilProne = "recoil_AKM_prone";
			dispersion = 0.00075;
			magazineSlot = "magazine";
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash{};
				class SmokeCloud1
				{
					overrideParticle = "weapon_shot_winded_smoke";
				};
				class ChamberFlash
				{
					overridePoint = "Usti hlavne";
					illuminateWorld = 1;
					overrideParticle = "weapon_shot_izh18_01";
				};
				class MuzzleFlash2
				{
					overrideParticle = "weapon_shot_m4a1_01";
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
				};
				class MuzzleFlashStar
				{
					overrideParticle = "weapon_shot_Flame_3D_4star";
					overridePoint = "usti hlavne";
				};
				class GasPistonBurstR
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {0,0,0};
				};
				class GasPistonBurstL
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {180,0,0};
				};
				class GasPistonSmokeRaiseR
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {0,0,0};
				};
				class GasPistonSmokeRaiseL
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "GasPiston";
					overrideDirectionVector[] = {180,0,0};
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 3;
				maxOverheatingValue = 30;
				overheatingDecayInterval = 2;
				class SmokingBarrelBase
				{
					positionOffset[] = {0.1,0,0};
					onlyWithinRainLimits[] = {0,0.2};
				};
				class SmokingBarrel1: SmokingBarrelBase
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.05,-0.02,0};
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.2};
				};
				class SmokingBarrelHot1: SmokingBarrelBase
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.12,-0.02,0};
					overrideParticle = "smoking_barrel";
					onlyWithinOverheatLimits[] = {0.2,0.6};
				};
				class SmokingBarrelHot3: SmokingBarrelBase
				{
					overridePoint = "GasPiston";
					positionOffset[] = {0.21,-0.02,0};
					overrideParticle = "smoking_barrel_heavy";
					onlyWithinOverheatLimits[] = {0.6,1};
				};
				class Steam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.4,0.02,0};
					onlyWithinOverheatLimits[] = {0,0.5};
					onlyWithinRainLimits[] = {0.2,1};
				};
				class Steam2
				{
					overrideParticle = "smoking_barrel_steam";
					positionOffset[] = {0.45,0.02,0};
					onlyWithinOverheatLimits[] = {0.5,1};
					onlyWithinRainLimits[] = {0.2,1};
				};
				class ChamberSmokeRaise
				{
					overrideParticle = "smoking_barrel_small";
					overridePoint = "Nabojnicestart";
					onlyWithinOverheatLimits[] = {0.5,1};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class walkErc_L
				{
					soundSet = "Weapon_Movement_HRifle_walkErc_L_SoundSet";
					id = 1;
				};
				class walkErc_R
				{
					soundSet = "Weapon_Movement_HRifle_walkErc_R_SoundSet";
					id = 2;
				};
				class runErc_L
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 3;
				};
				class runErc_R
				{
					soundSet = "Weapon_Movement_HRifle_runErc_R_SoundSet";
					id = 4;
				};
				class sprintErc_L
				{
					soundSet = "Weapon_Movement_HRifle_sprintErc_SoundSet";
					id = 5;
				};
				class sprintErc_R
				{
					soundSet = "Weapon_Movement_HRifle_sprintErc_SoundSet";
					id = 6;
				};
				class walkCro_L
				{
					soundSet = "Weapon_Movement_HRifle_walkCro_SoundSet";
					id = 21;
				};
				class walkCro_R
				{
					soundSet = "Weapon_Movement_HRifle_walkCro_SoundSet";
					id = 22;
				};
				class runCro_L
				{
					soundSet = "Weapon_Movement_HRifle_runCro_SoundSet";
					id = 23;
				};
				class runCro_R
				{
					soundSet = "Weapon_Movement_HRifle_runCro_SoundSet";
					id = 24;
				};
				class walkProne_L
				{
					soundSet = "Weapon_Movement_HRifle_walkCro_SoundSet";
					id = 31;
				};
				class walkProne_R
				{
					soundSet = "Weapon_Movement_HRifle_walkCro_SoundSet";
					id = 32;
				};
				class runProne_L
				{
					soundSet = "Weapon_Movement_HRifle_runCro_SoundSet";
					id = 33;
				};
				class runProne_R
				{
					soundSet = "Weapon_Movement_HRifle_runCro_SoundSet";
					id = 34;
				};
				class jumpErc_L
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 35;
				};
				class jumpErc_R
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 36;
				};
				class landFootErc_L
				{
					soundSet = "Weapon_Movement_HRifle_landFootErc_SoundSet";
					id = 37;
				};
				class landFootErc_R
				{
					soundSet = "Weapon_Movement_HRifle_landFootErc_SoundSet";
					id = 38;
				};
				class walkRasErc_L
				{
					soundSet = "Weapon_Movement_HRifle_walkRasErc_SoundSet";
					id = 51;
				};
				class walkRasErc_R
				{
					soundSet = "Weapon_Movement_HRifle_walkRasErc_SoundSet";
					id = 52;
				};
				class runRasErc_L
				{
					soundSet = "Weapon_Movement_HRifle_runRasErc_SoundSet";
					id = 53;
				};
				class runRasErc_R
				{
					soundSet = "Weapon_Movement_HRifle_runRasErc_SoundSet";
					id = 54;
				};
				class HandStep_L
				{
					soundSet = "Weapon_Movement_HRifle_walkErc_L_SoundSet";
					id = 61;
				};
				class HandStep_R
				{
					soundSet = "Weapon_Movement_HRifle_walkErc_L_SoundSet";
					id = 62;
				};
				class HandStep_Hard_L
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 63;
				};
				class HandStep_Hard_R
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 64;
				};
				class landFeetErc
				{
					soundSet = "Weapon_Movement_HRifle_landFeetErc_SoundSet";
					id = 100;
				};
				class Weapon_Movement_HRifle_Walk
				{
					soundSet = "Weapon_Movement_HRifle_walkErc_L_SoundSet";
					id = 101;
				};
				class Weapon_Movement_HRifle_Run
				{
					soundSet = "Weapon_Movement_HRifle_runErc_L_SoundSet";
					id = 102;
				};
				class Weapon_Movement_HRifle_Sprint
				{
					soundSet = "Weapon_Movement_HRifle_sprintErc_SoundSet";
					id = 103;
				};
				class Weapon_Movement_HRifle_Land
				{
					soundSet = "Weapon_Movement_HRifle_landFeetErc_SoundSet";
					id = 104;
				};
				class Char_Gestures_Hand_Grab_Rifle
				{
					soundSet = "Char_Gestures_Hand_Grab_FabricRifle_SoundSet";
					id = 892;
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					healthLevels[]=
					{
						
						{
							1.01,
							
							{
								"Namalsk_Weapon\GaussMk2\data\GaussMk2.rvmat",
							}
						},
						
						{
							0.69999999,
							
							{
								"Namalsk_Weapon\GaussMk2\data\GaussMk2.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"Namalsk_Weapon\GaussMk2\data\GaussMk2_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"Namalsk_Weapon\GaussMk2\data\GaussMk2_damage.rvmat"
							}
						},
						
						{
							0.01,
							
							{
								"Namalsk_Weapon\GaussMk2\data\GaussMk2_destruct.rvmat",
							}
						}
					};
				};
			};
		};
	};
	class A2GaussMk2_Black: A2GaussMk2_Base
	{
		scope = 2;
		displayName = "ER7 RFW MK2 (Black)";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsMaterials[] = {"Namalsk_Weapon\GaussMk2\data\GaussMk2.rvmat"};
		hiddenSelectionsTextures[] = {"Namalsk_Weapon\GaussMk2\data\dayz_co.paa"};
	};
	class A2GaussMk2_Gold: A2GaussMk2_Base
	{
		scope = 2;
		displayName = "ER7 RFW MK2 (Gold)";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsMaterials[] = {"Namalsk_Weapon\GaussMk2\data\GuassMk2_Gold.rvmat"};
		hiddenSelectionsTextures[] = {"Namalsk_Weapon\GaussMk2\data\dayz_co.paa"};
	};
};

class CfgMagazines
{
	class Mag_AKM_30Rnd;
	class A2GaussMk2_Magazine: Mag_AKM_30Rnd
	{
		scope = 2;
		displayName = "ER7 Magazine";
		descriptionShort = "A ten round M82 magazine.";
		model = "Namalsk_Weapon\GaussMk2\A2GaussMk2_Magazine.p3d";
		weight = 230;
		itemSize[] = {2,2};
		count = 10;
		ammo = "Bullet_ER7RFW";
		ammoItems[] = {"Ammo_ER7RFW"};
		isMeleeWeapon = 0;
		tracersEvery = 1;
	};
};


class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyA2GaussMk2_Magazine: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "magazine";
		model = "Namalsk_Weapon\GaussMk2\A2GaussMk2_Magazine.p3d";
	};
};


class CfgSoundShaders
{
	class baseCharacter_SoundShader;
	class ER7_Shot_SoundShader
	{
		samples[] = {{"Namalsk_Weapon\GaussMk2\Sound\er7_fire",1}};
		volume = 0.7;
		range = 50;
		rangeCurve = "closeShotCurve";
	};
	class ER7_midShot_SoundShader
	{
		samples[] = {{"Namalsk_Weapon\GaussMk2\Sound\er7_fire",1}};
		volume = 1.2;
		range = 3000;
		rangeCurve[] = {{0,0.2},{50,1},{3000,0},{3000,0}};
	};
	class ER7_distShot_SoundShader
	{
		samples[] = {{"Namalsk_Weapon\GaussMk2\Sound\er7_fire",1}};
		volume = 2;
		range = 3000;
		rangeCurve[] = {{0,0},{50,0},{3000,1},{3000,2}};
	};
};

class CfgSoundSets
{
	class Rifle_Shot_Base_SoundSet;
	class rifle_tail_Base_soundset;
	class Rifle_InteriorTail_Base_SoundSet;
	class Rifle_silencerHomemade_Base_SoundSet;
	class Rifle_silencerHomemadeTail_Base_SoundSet;
	class Rifle_silencerInteriorHomeMadeTail_Base_SoundSet;
	class baseCharacter_SoundSet;
	class ER7_Shot_SoundSet: Rifle_Shot_Base_SoundSet
	{
		soundShaders[] = {"ER7_Shot_SoundShader","ER7_midShot_SoundShader","ER7_distShot_SoundShader"};
	};
};

class CfgMods
{
	class ER7_Mod
	{
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"Namalsk_Weapon\GaussMk2\Scripts\4_World"
				};
			};
		};
	};
};