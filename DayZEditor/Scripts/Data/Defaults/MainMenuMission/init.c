class MissionMainMenuDummy: EditorMainMenuMission
{
}

Mission CreateCustomMission(string path)
{
	return new MissionMainMenuDummy(path);
}

void main()
{
    if (!IsEditorCLEForceDisabled()) {
        Hive ce = CreateHive();
        if (ce) {
            ce.InitSandbox();
        }
    }
}