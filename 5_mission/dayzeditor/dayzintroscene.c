
string CreateEditorMission()
{
	Print("CreateEditorMission");
	string mission = "$saves:DayZEditor.ChernarusPlus";
	
	if (!FileExist(mission)) {
		Print("Editor Mission not found, creating....");
		MakeDirectory(mission);
	}
	
	FileHandle init = OpenFile(mission + "/init.c", FileMode.WRITE);
	CloseFile(init);
	
	return mission;
}


modded class DayZIntroScene
{
	Object m_FunnyMeme;
	
	void DayZIntroScene()
	{
		
		delete m_Character;
		m_CharacterPos = Vector(0.685547, 1, 5.68823).Multiply4(m_CameraTrans);
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetOrientation(m_CharacterRot);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(FunnyMeme);
	}
	
	void ~DayZIntroScene()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(FunnyMeme);
	}
	
	
	
	void FunnyMeme()
	{
		int x, y;
		GetMousePos(x, y);
		
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection();
		mouse_pos[0] = mouse_pos[0] - 0.8;
		mouse_pos[1] = mouse_pos[1] + 0.25;

		vector lookat = vector.Direction(mouse_pos, m_Camera.GetPosition());
		lookat[0] = (lookat[0] * -1.2);
		lookat[1] = lookat[1] * -1;
		
		vector final = lookat - GetGame().GetCurrentCameraDirection();
		m_FunnyMeme.SetDirection(final);
		m_FunnyMeme.Update();
	}
}


modded class MainMenu 
{
	
	
	override Widget Init()
	{
		Print("Editor::MainMenu");
		super.Init();
		
		m_ChooseServer.Show(false);
		m_CustomizeCharacter.Show(false);
		m_Stats.HideStats();
		
		Widget c = layoutRoot.FindAnyWidget("character");
		c.Show(false);
		
		TextWidget tw = TextWidget.Cast(layoutRoot.FindAnyWidget("play_label"));
		tw.SetText("Open Editor");
		
		//PluginFileHandler
		return layoutRoot;
	}

    override void Play()
    {
        //GetGame().SetMission(CreateMission("DayZEditor/Editor/mission/DayZEditor.ChernarusPlus"));
        GetGame().PlayMission(CreateEditorMission());
		//GetGame().PlayMission("ChernarusEditor");
		
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
		if(IsFocusable(w)) {
			ColorHighlight( w );
			return true;
		}
		return false;
	}
}