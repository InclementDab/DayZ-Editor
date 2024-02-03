class CfgPatches
{
	class Editor_GreenScreen
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
		displayName="Green Screen";
		model="Editor\\Editor\\GreenScreen\\Pink.p3d";
	};
	class GreenScreen_Green: HouseNoDestruct
	{
		scope=2;
		displayName="Green Screen";
		model="Editor\\Editor\\GreenScreen\\Green.p3d";
	};
	class GreenScreen_Blue: HouseNoDestruct
	{
		scope=2;
		displayName="Green Screen";
		model="Editor\\Editor\\GreenScreen\\Blue.p3d";
	};
};
