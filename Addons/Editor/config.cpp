

class CfgPatches
{
    class Editor
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data"};
    };  
};

class CfgVehicles
{
    class HouseNoDestruct;
    class DSLRCamera : HouseNoDestruct
    {
        scope=1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\Camera.p3d";
    };

    class WorldCartesian : HouseNoDestruct
    {
        scope=1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\CartesianWidget.p3d";
    };


    class BoundingBox : HouseNoDestruct
    {
        scope = 1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBox.p3d";
        
    };
};

class CfgEditor
{

    class Draw
    {
        class 3D
        {
            size = "24 * pixelW";
            sizeBackground = "24 * pixelW";
            textureBackground = "";
            color[] = {"side"};
            colorSimple[] = {0.5, 0.5, 0.5, 1};
            colorSimplePreview[] = {0.5, 0.5, 0.5, 1};
            colorBackground[] = {0, 0, 0, 0};
            colorPreview[] = {"side"};
            colorPreviewBackground[] = {1, 1, 1, 0.5};
            alphaNormal = 0.7;
            alphaNormalBackground = 0;
            alphaSelected = 1;
            alphaSelectedBackground = 0;
            alphaHover = 1;
            alphaHoverBackground = 0;
            shadow = 1;
            fadeDistance = 750;
            fadeDistanceCrew = 20;
            boundingBoxBase = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBoxBase.p3d";
            sizeSimulation = "64 * pixelW";
            textureSimulationEnabled = "";
            textureSimulationDisabled = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\Simulation\simulation5_ca.paa";
            textureSimulationDynamic = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\Simulation\simulation5_ca.paa";
            colorSimulationEnabled[] = {0, 0, 0, 0};
            colorSimulationDisabled[] = {1, 0.2, 0.2, 1};
            colorSimulationDynamic[] = {0, 0.7, 0.8, 1};
            colorPreviewSimulationEnabled[] = {0, 0, 0, 0};
            colorPreviewSimulationDisabled[] = {0.5, 0.5, 0.5, 1};
            colorPreviewSimulationDynamic[] = {0, 0.7, 0.8, 1};
            alphaNormalSimulation = 0.7;
            alphaSelectedSimulation = 1;
            alphaHoverSimulation = 1;
        };
        class 2D
        {
            size = "24 * pixelW";
            sizeBackground = "24 * pixelW";
            presenceBorder = 2;
            textureBackground = "";
            color[] = {"side"};
            colorSimple[] = {0.5, 0.5, 0.5, 1};
            colorSimplePreview[] = {0.5, 0.5, 0.5, 1};
            colorBackground[] = {0, 0, 0, 0};
            colorPreview[] = {"side"};
            colorPreviewBackground[] = {0, 0, 0, 0.5};
            alphaNormal = 0.7;
            alphaNormalBackground = 0;
            alphaSelected = 1;
            alphaSelectedBackground = 0;
            alphaHover = 1;
            alphaHoverBackground = 0;
            shadow = 0;
            boundingBoxAlpha = 0.4;
            boundingBoxForceShow = 50;
            sizeSimulation = "64 * pixelW";
            textureSimulationEnabled = "";
            textureSimulationDisabled = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\Simulation\simulation5_ca.paa";
            textureSimulationDynamic = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\Simulation\simulation5_ca.paa";
            colorSimulationEnabled[] = {0, 0, 0, 0};
            colorSimulationDisabled[] = {1, 0.2, 0.2, 1};
            colorSimulationDynamic[] = {0, 0.7, 0.8, 1};
            colorPreviewSimulationEnabled[] = {0, 0, 0, 0};
            colorPreviewSimulationDisabled[] = {0.5, 0.5, 0.5, 1};
            colorPreviewSimulationDynamic[] = {0, 0.7, 0.8, 1};
            alphaNormalSimulation = 0.7;
            alphaSelectedSimulation = 1;
            alphaHoverSimulation = 1;
        };
    };
};
