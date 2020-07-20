


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
    class Static;
    class HouseNoDestruct;
    class EditorObject : HouseNoDestruct
    {
        scope = 1;
        displayName = "Editor Object";
    };

    
    class DSLRCamera : HouseNoDestruct
    {
        scope=1;
        model = "DayZEditor\Editor\data\Camera.p3d";
    };

    class 3DWidget : HouseNoDestruct
    {
        scope=1;
        model = "DayZEditor\Editor\data\3DWidget.p3d";
    };

    class RotationWidget : HouseNoDestruct
    {
        scope = 1;
        model = "DayZEditor\Editor\data\RotationWidget.p3d";
    };

    class BoundingBoxBase : HouseNoDestruct
    {
        scope = 1;
        model = "DayZEditor\Editor\data\BoundingBoxBase.p3d";

        hiddenSelections[] = {"MainSection1"};
        hiddenSelectionsMaterials[] = { "DayZEditor\Editor\data\BoundingBox\BoundingBoxShown.rvmat"};
        hiddenSelectionsTextures[] = { "DayZEditor\Editor\data\BoundingBox\box_shown.paa" };
    };

    class ThunderboltNorm: HouseNoDestruct
    {
        model = "\DZ\Data\data\blesk1.p3d";
        soundSetNear = "ThunderNorm_Near_SoundSet";
        soundSetFar = "ThunderNorm_Far_SoundSet";
    };
    class ThunderboltHeavy : HouseNoDestruct
    {
        model = "\DZ\Data\data\blesk2.p3d";
        soundSetNear = "ThunderHeavy_Near_SoundSet";
        soundSetFar = "ThunderHeavy_Far_SoundSet";
    };
};



class CfgMissions
{
    class Missions
    {
        class ChernarusPlusEditor
        {
            directory = "DayZEditor\Editor\mission\DayZEditor.ChernarusPlus";
        };
    };
};
