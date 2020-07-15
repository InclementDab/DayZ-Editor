


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


    class BoundingBoxBase : HouseNoDestruct
    {
        scope = 1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBoxBase.p3d";
    };
};

