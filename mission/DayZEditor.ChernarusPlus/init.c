
Mission CreateCustomMission(string path)
{
	Print("DayZEditor::CreateCustomMission " + path);
	return new EditorMissionGameplay();
}

void main()
{
	// :)
}