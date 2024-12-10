class MissionMainMenuDummy: MissionMainMenu
{
}

Mission CreateCustomMission(string path)
{
	return new MissionMainMenuDummy();
}

void main()
{
    Hive ce = CreateHive();
    if (ce) {
        ce.InitSandbox();
    }
}