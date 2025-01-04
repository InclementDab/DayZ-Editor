class MissionMainMenuDummy: EditorMainMenuMission
{
}

Mission CreateCustomMission(string path)
{
	return new MissionMainMenuDummy(path);
}

void main()
{
    Hive ce = CreateHive();
    if (ce) {
        ce.InitSandbox();
    }
}