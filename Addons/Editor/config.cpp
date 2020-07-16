


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

    class 3DWidget : HouseNoDestruct
    {
        scope=1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\3DWidget.p3d";
    };

    class RotationWidget : HouseNoDestruct
    {
        scope = 1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\RotationWidget.p3d";
    };

    class BoundingBoxBase : HouseNoDestruct
    {
        scope = 1;
        model = "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBoxBase.p3d";

        hiddenSelections[] = {"MainSection1"};
        hiddenSelectionsMaterials[] = { "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBox\BoundingBoxShown.rvmat"};
        hiddenSelectionsTextures[] = { "P:\DayZ_Server\dev\DayZEditor\Addons\Editor\data\BoundingBox\box_shown.paa" };
    };
};

