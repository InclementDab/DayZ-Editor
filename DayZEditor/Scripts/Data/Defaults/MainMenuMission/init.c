class MissionMainMenuDummy: EditorMainMenuMission
{
}

Mission CreateCustomMission(string path)
{
	return new MissionMainMenuDummy(path);
}

void main()
{
    MissionMainMenuDummy mission = MissionMainMenuDummy.Cast(GetGame().GetMission());
    bool can_launch_cle = mission.CanLaunchCLE();
    if (can_launch_cle) {
        Hive ce = CreateHive();
        if (ce) {
            ce.InitSandbox();
        }
    }
}