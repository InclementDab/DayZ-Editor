class CfgPatches
{
	class ER7Scope
	{
		units[] = {"ER7Scope"};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Optics"};
		weapons[] = {};
	};
};
class cfgVehicles
{
	class ItemOptics;
	class ER7Scope: ItemOptics
	{
		scope = 2;
		displayName = "ER7 SCOPE THING DONT HAVE A NAME YET REEEEEEEEEEE";
		descriptionShort = "FUCK WHAT DO I CALL YOU";
		model = "\Namalsk_Weapon\GaussMk2\Scope\ER7Scope.p3d";
		animClass = "Binoculars";
		attachments[] = {"BatteryD"};
		//Not used yet hiddenSelections[] = {"reddot"};
		hiddenSelectionsTextures[] = {""};
		rotationFlags = 16;
		reversed = 0;
		weight = 440;
		itemSize[] = {2,1};
		inventorySlot = "WeaponOptics";
		selectionFireAnim = "zasleh";
		simulation = "itemoptics";
		dispersionModifier = -0.000125;
		dispersionCondition = "true";
		recoilModifier[] = {0.001,0.001,0.001};
		memoryPointCamera = "eyeScope";
		cameraDir = "cameraDir";
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
								"Namalsk_Weapon\GaussMk2\Scope\Data\ER7Scope.rvmat",
							}
						},
							
						{
							0.69999999,
								
							{
								"Namalsk_Weapon\GaussMk2\Scope\Data\ER7Scope.rvmat"
							}
						},
							
						{
							0.5,
								
							{
								"Namalsk_Weapon\GaussMk2\Scope\Data\ER7Scope_damage.rvmat"
							}
						},
							
						{
							0.30000001,
								
							{
								"Namalsk_Weapon\GaussMk2\Scope\Data\ER7Scope_damage.rvmat"
							}
						},
							
						{
							0.01,
							
							{
								"Namalsk_Weapon\GaussMk2\Scope\Data\ER7Scope_destruct.rvmat",
							}
						}
					};
				};
			};
		};
		class OpticsInfo
		{
			useModelOptics = 1;
			memoryPointCamera = "eyeScope";
			cameraDir = "cameraDir";
			modelOptics = "\Namalsk_Weapon\GaussMk2\Scope\ER7Ret.p3d";
			opticsDisablePeripherialVision = 0.67;
			opticsFlare = 1;
			opticsPPEffects[] = {"OpticsCHAbera3","OpticsBlur1"};
			opticsZoomMin = "0.3926/6";
			opticsZoomMax = "0.3926/30";
			opticsZoomInit = "0.3926/30";
			discretefov[] = {"0.3926/6","0.3926/12","0.3926/20","0.3926/30"};
			discreteInitIndex = 0;
			distanceZoomMin = 100;
			distanceZoomMax = 2000;
			discreteDistance[] = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};
			discreteDistanceInitIndex = 0;
			PPMaskProperties[] = {0.5,0.5,0.5000001,0.05};
			PPLensProperties[] = {0.2,0.15,0,0};
			PPBlurProperties = 0.3;
		};
		class EnergyManager
		{
			hasIcon = 1;
			energyUsagePerSecond = 0.02;
			plugType = 1;
			attachmentAction = 1;
		};
	};
};
