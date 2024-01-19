class CfgPatches
{
	class Editor_GUI
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DF_GUI"};
	};
};
class DayZMapControl
{
	scaleMin = 0.02;
	scaleMax = 3.0;
	scaleDefault = 2;
	ptsPerSquareSea = 8;
	ptsPerSquareTxt = 10;
	ptsPerSquareCLn = 10;
	ptsPerSquareExp = 10;
	ptsPerSquareCost = 10;
	ptsPerSquareFor = 9;
	ptsPerSquareForEdge = 15;
	ptsPerSquareRoad = 4;
	ptsPerSquareObj = 15;
	maxSatelliteAlpha = 1;
	alphaFadeStartScale = 0.08;
	alphaFadeEndScale = 0.16;
	userMapPath = "dz\gear\navigation\data\usermap";
	maxUserMapAlpha = 0;
	alphaUserMapFadeStartScale = 0;
	alphaUserMapFadeEndScale = 0;
	showCountourInterval = 0;
	colorLevels[] = {0.65,0.6,0.45,0.95};
	colorSea[] = {0.64,0.76,0.89,1};
	colorForest[] = {0.36,0.78,0.08,0.5};
	colorRocks[] = {0.5,0.5,0.5,0.5};
	colorCountlines[] = {0.85,0.8,0.65,1};
	colorMainCountlines[] = {0.45,0.4,0.25,1};
	colorCountlinesWater[] = {0.25,0.4,0.5,0.3};
	colorMainCountlinesWater[] = {0.25,0.4,0.5,0.9};
	colorPowerLines[] = {0.1,0.1,0.1,1};
	colorRailWay[] = {0.8,0.2,0,1};
	colorForestBorder[] = {0.4,0.8,0,1};
	colorRocksBorder[] = {0.5,0.5,0.5,1};
	colorOutside[] = {1,1,1,1};
	colorTracks[] = {0.78,0.66,0.34,1};
	colorRoads[] = {0.69,0.43,0.23,1};
	colorMainRoads[] = {0.53,0.35,0,1};
	colorTracksFill[] = {0.96,0.91,0.6,1};
	colorRoadsFill[] = {0.92,0.73,0.41,1};
	colorMainRoadsFill[] = {0.84,0.61,0.21,1};
	colorGrid[] = {0.15,0.15,0.05,0.9};
	colorGridMap[] = {0.25,0.25,0.1,0.75};
	fontNames = "gui/fonts/metron";
	sizeExNames = 0.04;
	colorNames[] = {0,0,0,1};
	fontGrid = "gui/fonts/metron";
	sizeExGrid = 0.01;
	fontLevel = "gui/fonts/metron";
	sizeExLevel = 0.01;
	colorMountPoint[] = {0.45,0.4,0.25,1};
	mapPointDensity = 0.12;
	text = "";
	fontLabel = "gui/fonts/metron";
	fontInfo = "gui/fonts/metron";
	class Legend
	{
		x = 0.05;
		y = 0.85;
		w = 0.4;
		h = 0;
		font = "gui/fonts/metron";
		sizeEx = 0;
		colorBackground[] = {1,1,1,0};
		color[] = {0,0,0,0};
	};
	class Bush
	{
		icon = "\dz\gear\navigation\data\map_bush_ca.paa";
		color[] = {0.4,0.8,0,0.8};
		size = 14;
		importance = "0.2 * 14 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class SmallTree
	{
		icon = "\dz\gear\navigation\data\map_smalltree_ca.paa";
		color[] = {0.4,0.8,0,0.8};
		size = 12;
		importance = "0.6 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Tree
	{
		icon = "\dz\gear\navigation\data\map_tree_ca.paa";
		color[] = {0.4,0.8,0,0.8};
		size = 12;
		importance = "0.9 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Rock
	{
		icon = "\dz\gear\navigation\data\map_rock_ca.paa";
		color[] = {0.1,0.1,0.1,0.8};
		size = 12;
		importance = "0.5 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fuelstation
	{
		icon = "\dz\gear\navigation\data\map_fuelstation_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.75;
		coefMax = 4;
	};
	class Lighthouse
	{
		icon = "\dz\gear\navigation\data\map_lighthouse_ca.paa";
		size = 14;
		color[] = {0,0,0,1};
		importance = "3 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Stack
	{
		icon = "\dz\gear\navigation\data\map_stack_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Transmitter
	{
		icon = "\dz\gear\navigation\data\map_transmitter_ca.paa";
		color[] = {0,0,0,1};
		size = 16;
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Watertower
	{
		icon = "\dz\gear\navigation\data\map_watertower_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 16;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Shipwreck
	{
		icon = "\dz\gear\navigation\data\map_shipwreck_ca.paa";
		color[] = {0,0,0,1};
		size = 16;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Monument
	{
		icon = "\dz\gear\navigation\data\map_monument_ca.paa";
		color[] = {0,0,0,1};
		size = 20;
		importance = "1.2 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class BusStop
	{
		icon = "\dz\gear\navigation\data\map_busstop_ca.paa";
		color[] = {0.15,0.26,0.87,1};
		size = 12;
		importance = "1 * 10 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fountain
	{
		icon = "\dz\gear\navigation\data\map_fountain_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 15;
		importance = "1 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Waterpump
	{
		icon = "\dz\gear\navigation\data\map_waterpump_ca.paa";
		color[] = {0.2,0.45,0.7,1};
		size = 16;
		importance = "1 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Hospital
	{
		icon = "\dz\gear\navigation\data\map_hospital_ca.paa";
		color[] = {0.15,0.26,0.87,1};
		size = 17;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Store
	{
		icon = "\dz\gear\navigation\data\map_store_ca.paa";
		color[] = {0,0,0,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Police
	{
		icon = "\dz\gear\navigation\data\map_police_ca.paa";
		color[] = {0.11,0.1,0.25,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class FireDep
	{
		icon = "\dz\gear\navigation\data\map_firedep_ca.paa";
		color[] = {0.764,0,0,1};
		size = 20;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class TouristSign
	{
		icon = "\dz\gear\navigation\data\map_tsign_ca.paa";
		color[] = {0,0,0,1};
		size = 18;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Tourism
	{
		icon = "\dz\gear\navigation\data\map_tourism_ca.paa";
		color[] = {0,0,0,1};
		size = 14;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Cross
	{
		icon = "\dz\gear\navigation\data\map_cross_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "0.7 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Chapel
	{
		icon = "\dz\gear\navigation\data\map_chapel_ca.paa";
		color[] = {0,0,0,1};
		size = 16;
		importance = "1 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class Church
	{
		icon = "\dz\gear\navigation\data\map_church_ca.paa";
		color[] = {0,0,0,1};
		size = 10;
		importance = "2 * 16 * 0.05";
		coefMin = 0.9;
		coefMax = 4;
	};
	class ViewTower
	{
		icon = "\dz\gear\navigation\data\map_viewtower_ca.paa";
		color[] = {0,0,0,1};
		size = 16;
		importance = "2.5 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class TouristShelter
	{
		icon = "\dz\gear\navigation\data\map_tshelter_ca.paa";
		color[] = {0,0,0,1};
		size = 16;
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Quay
	{
		icon = "\dz\gear\navigation\data\map_quay_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.5;
		coefMax = 4;
	};
	class Bunker
	{
		icon = "\dz\gear\navigation\data\map_bunker_ca.paa";
		color[] = {0,0,0,1};
		size = 14;
		importance = "1.5 * 14 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Fortress
	{
		icon = "\dz\gear\navigation\data\map_fortress_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "2 * 16 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Rocks
	{
		icon = "\dz\gear\navigation\data\map_rock_ca.paa";
		color[] = {0.1,0.1,0.1,0.8};
		size = 12;
		importance = "0.5 * 12 * 0.05";
		coefMin = 0.25;
		coefMax = 4;
	};
	class Ruin
	{
		icon = "\dz\gear\navigation\data\map_ruin_ca.paa";
		size = 16;
		color[] = {0,0,0,1};
		importance = "1.2 * 16 * 0.05";
		coefMin = 1;
		coefMax = 4;
	};
};
class RscMapControl: DayZMapControl
{
	moveOnEdges = 1;
	x = "SafeZoneXAbs";
	y = "SafeZoneY + 1.5 * ((((safezoneW / safezoneH) min 1.2) / 1.2) / 25)";
	w = "SafeZoneWAbs";
	h = "SafeZoneH - 1.5 * ((((safezoneW / safezoneH) min 1.2) / 1.2) / 25)";
	shadow = 0;
	ptsPerSquareSea = 5;
	ptsPerSquareTxt = 20;
	ptsPerSquareCLn = 10;
	ptsPerSquareExp = 10;
	ptsPerSquareCost = 10;
	ptsPerSquareFor = 9;
	ptsPerSquareForEdge = 9;
	ptsPerSquareRoad = 6;
	ptsPerSquareObj = 9;
	showCountourInterval = 0;
	scaleMin = 0.001;
	scaleMax = 1.0;
	scaleDefault = 0.16;
	maxSatelliteAlpha = 0.85;
	alphaFadeStartScale = 2;
	alphaFadeEndScale = 2;
	colorLevels[]={0.64999998,0.60000002,0.44999999,0.94999999};
	colorBackground[] = {0.969,0.957,0.949,1.0};
	colorSea[] = {0.467,0.631,0.851,0.5};
	colorForest[] = {0.624,0.78,0.388,0.5};
	colorForestBorder[] = {0.0,0.0,0.0,0.0};
	colorRocks[] = {0.0,0.0,0.0,0.3};
	colorRocksBorder[] = {0.0,0.0,0.0,0.0};
	colorMainCountlines[] = {0.572,0.354,0.188,0.5};
	colorCountlines[] = {0.572,0.354,0.188,0.25};
	colorMainCountlinesWater[] = {0.491,0.577,0.702,0.6};
	colorCountlinesWater[] = {0.491,0.577,0.702,0.3};
	colorPowerLines[] = {0.1,0.1,0.1,1.0};
	colorRailWay[] = {0.8,0.2,0.0,1.0};
	colorNames[] = {0.1,0.1,0.1,0.9};
	colorInactive[] = {1.0,1.0,1.0,0.5};
	colorOutside[] = {0.0,0.0,0.0,1.0};
	colorTracks[] = {0.84,0.76,0.65,0.15};
	colorTracksFill[] = {0.84,0.76,0.65,1.0};
	colorRoads[] = {0.7,0.7,0.7,1.0};
	colorRoadsFill[] = {1.0,1.0,1.0,1.0};
	colorMainRoads[] = {0.9,0.5,0.3,1.0};
	colorMainRoadsFill[] = {1.0,0.6,0.4,1.0};
	colorGrid[] = {0.1,0.1,0.1,0.6};
	colorGridMap[] = {0.1,0.1,0.1,0.6};
	colorTrails[] = {0.84,0.76,0.65,0.15};
	colorTrailsFill[] = {0.84,0.76,0.65,0.65};
	widthRailWay = 4.0;
	sizeExLabel = "(   (   (   ((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	sizeExGrid = 0.02;
	sizeExUnits = "(((   ((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	sizeExNames = "(   (   (   ((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8) * 2";
	sizeExInfo = "(   (   (   ((safezoneW / safezoneH) min 1.2) / 1.2) / 25) * 0.8)";
	sizeExLevel = 0.02;
	text = "#(argb,8,8,3)color(1,1,1,1)";
	idcMarkerColor = -1;
	idcMarkerIcon = -1;
	textureComboBoxColor = "#(argb,8,8,3)color(1,1,1,1)";
	showMarkers = 1;
	class LineMarker
	{
		lineWidthThin = 0.008;
		lineWidthThick = 0.014;
		lineDistanceMin = 3e-005;
		lineLengthMin = 5;
	};
	class Legend
	{
		colorBackground[] = {1,1,1,0};
		color[] = {0,0,0,0};
	};
};
