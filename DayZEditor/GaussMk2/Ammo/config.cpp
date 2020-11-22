class CfgPatches
{
	class AmmoBox_ER7RFW
	{
		units[] = {"AmmoBox_ER7RFW"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Magazines","DZ_Weapons_Ammunition"};
		magazines[] = {"Ammo_ER7RFW"};
		ammo[] = {"Bullet_ER7RFW"};
	};
};

class CfgVehicles
{
	class Box_Base;
	class AmmoBox_ER7RFW: Box_Base
	{
		scope = 2;
		displayName = "ER7 RFW (SN Rounds) AmmoBox";
		descriptionShort = "ER7 RFW Rounds shoot at 500m/s with a very needle nose bullet (Not final version)";
		model = "\dz\weapons\ammunition\00Buck_10RoundBox.p3d";
		itemSize[] = {1,2};
		rotationFlags = 17;
		weight = 300;
		hiddenSelections[] = {"camoGround","zbytek"};
		hiddenSelectionsTextures[] = {"CPBAmmo\50Cal\Data\50calbox.paa","CPBAmmo\50Cal\Data\50calbox.paa"};
		class Resources
		{
			class Ammo_ER7RFW
			{
				value = 10;
				variable = "quantity";
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class interact
				{
					soundset = "ammoboxUnpack_SoundSet";
					id = 70;
				};
			};
		};
	};
};

class CfgMagazines
{
	class Ammunition_Base;
	class Ammo_ER7RFW: Ammunition_Base
	{
		scope = 2;
		displayName = "ER7 RFW (SN Rounds)";
		descriptionShort = "ER7 RFW SN Rounds are used in the almighty ER7 RFW Gauss Rifle.";
		model = "Namalsk_Weapon\GaussMk2\Ammo\ER7RFW_Ammo.p3d";
		itemSize[] = {1,3};
		rotationFlags = 34;
		weight = 24;
		count = 20;
		ammo = "Bullet_ER7RFW";
	};
};
class cfgAmmoTypes
{
	class Bullet_ER7RFW
	{
		name = "Bullet_ER7RFW";
	};
};
class cfgAmmo
{
	class Bullet_Base;
	class Bullet_ER7RFW: Bullet_Base
	{
		scope = 2;
		lootCategory = "Crafted";
		cartridge = "FxCartridge_9mm";
		spawnPileType = "Ammo_ER7RFW";
		hit = 11;
		indirectHit = 0;
		indirectHitRange = 0;
		tracerScale = 1;
		caliber = 0.65000004;
		deflecting = 45;
		visibleFire = 5;
		audibleFire = 9;
		typicalSpeed = 2000;
		airFriction = 0;
		supersonicCrackNear[] = {};
		supersonicCrackFar[] = {};
		initSpeed = 2000;
		weight = 0.0149;
		class DamageApplied
		{
			type = "Projectile";
			dispersion = 0;
			bleedThreshold = 1;
			class Health
			{
				damage = 50000;
			};
			class Blood
			{
				damage = 50000;
			};
			class Shock
			{
				damage = 50000;
			};
		};
		class NoiseHit
		{
			strength = 15;
			type = "shot";
		};
	};
};