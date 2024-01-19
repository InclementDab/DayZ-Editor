class CfgPatches
{
	class greenScreen
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

class CfgVehicles
{
	class HouseNoDestruct;
	
	class GreenScreen_Pink: HouseNoDestruct
	{
		scope=2;
		displayName="greenScreen";
		descriptionShort="Hype train is a hoax";
		model="\Editor\Editor\GreenScreen\data\greenScreen_Pink.p3d";
	};
	class GreenScreen_Green: HouseNoDestruct
	{
		scope=2;
		displayName="greenScreen";
		descriptionShort="Hype train is a hoax";
		model="\Editor\Editor\GreenScreen\data\greenScreen_Green.p3d";
	};
	class GreenScreen_Blue: HouseNoDestruct
	{
		scope=2;
		displayName="greenScreen";
		descriptionShort="Hype train is a hoax";
		model="\Editor\Editor\GreenScreen\data\greenScreen_Blue.p3d";
	};
};
