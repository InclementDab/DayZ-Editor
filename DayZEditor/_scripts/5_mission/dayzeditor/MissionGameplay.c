
string CreateEditorMission(string map_name = "ChernarusPlus")
{
	Print("CreateEditorMission");
	string mission = "$saves:DayZEditor." + map_name;
	
	if (!FileExist(mission)) {
		Print("Editor Mission not found, creating....");
		MakeDirectory(mission);
	}
	
	FileHandle init = OpenFile(mission + "/init.c", FileMode.WRITE);
	FPrint(init, "\/\/ Returns Offline Editor Mission");
	FPrint(init, "Mission CreateCustomMission(string path)");
	FPrint(init, "{");
	FPrint(init, "return new EditorMissionGameplay();");
	FPrint(init, "}");
		
	CloseFile(init);
	
	return mission;
}


class EditorMissionGameplay: MissionGameplay
{
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
	}
	
	
	override void OnInit()
	{
		super.OnInit();
	
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();
		
	}
}


modded class MissionGameplay
{

	void HandleInputs(Input input)
	{
		
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
		} 
		
		if (input.LocalPress("EditorToggleCursor")) {

		} 
		
		
	}
	
	override void OnUpdate(float timeslice)
	{
		Input input = GetGame().GetInput();
		
		HandleInputs(input);
		
		//super.OnUpdate(timeslice);
		
	}
		
	override bool IsPaused()
    {
        return GetGame().GetUIManager().IsMenuOpen(MENU_INGAME);
    }

    override void Pause()
    {
        if (IsPaused() || (GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().GetID() == MENU_INGAME))
            return;

        if (g_Game.IsClient() && g_Game.GetGameState() != DayZGameState.IN_GAME)
            return;


        CloseAllMenus();

        // open ingame menu
        GetUIManager().EnterScriptedMenu(MENU_INGAME, GetGame().GetUIManager().GetMenu());
        PlayerControlDisable(INPUT_EXCLUDE_ALL);
    }
	
	override void Continue()
    {
        int menu_id = GetGame().GetUIManager().GetMenu().GetID();
        if (!IsPaused() || (menu_id != MENU_INGAME && menu_id != MENU_LOGOUT) || (m_Logout && m_Logout.layoutRoot.IsVisible()))
            return;
        
        PlayerControlEnable(true);
        GetUIManager().CloseMenu(MENU_INGAME);
		
		EditorUI ui = new EditorUI();
		GetUIManager().ShowScriptedMenu(ui, GetGame().GetUIManager().GetMenu());
		GetEditor().GetUIManager().SetEditorUI(ui);
    }	
}


