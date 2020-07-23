
string CreateEditorMission(string map_name = "ChernarusPlus")
{
	Print("CreateEditorMission");
	string mission = "$saves:DayZEditor." + map_name;
	
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
		m_CharacterPos = Vector(0.685547, 3, 5.68823).Multiply4(m_CameraTrans);
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetOrientation(m_CharacterRot);
		m_FunnyMeme.SetPosition(m_FunnyMeme.GetPosition() + Vector(0, 1, 0));
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(FunnyMeme);
	}
	
	void ~DayZIntroScene()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(FunnyMeme);
	}
	
	
	
	void FunnyMeme()
	{
		
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection() * 4;
		vector lookat = vector.Direction(m_FunnyMeme.GetPosition(), mouse_pos);
		
		m_FunnyMeme.SetDirection(lookat);
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
		
		return layoutRoot;
	}

    override void Play()
    {
		MapSelectWindow select_window = new MapSelectWindow();
		GetGame().GetUIManager().ShowScriptedMenu(select_window, this);
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
		if(IsFocusable(w)) {
			ColorHighlight( w );
			return true;
		}
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button) 
	{
		super.OnClick(w, x, y, button);
		return false;
	}
}