
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
		Input input = GetGame().GetInput();
		
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection() * 4;
		vector lookat = vector.Direction(m_FunnyMeme.GetPosition(), mouse_pos);
		
		m_FunnyMeme.SetDirection(lookat);
		m_FunnyMeme.Update();
		
		if (input.LocalPress("UAFire")) {
			
			vector start = GetGame().GetCurrentCameraPosition();
			vector end = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 5000;
			vector contact_pos, contact_dir;
			int component;
			
			DayZPhysics.RaycastRV(start, end, contact_pos, contact_dir, component);
	
			vector newcam_pos = contact_pos;
			newcam_pos[1] = newcam_pos[1] + 50;
			Object new_camera = GetGame().CreateObjectEx("DSLRCamera", newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
			
			float scale = vector.Distance(contact_pos, start) * 0.25;
			vector mat[4] = {
				Vector(scale, 0, 0),
				Vector(0, scale, 0),
				Vector(0, 0, scale),
				newcam_pos
			};

			
			new_camera.SetTransform(mat);
			
			
			
			

 

			
	        vector mins, maxs;
	        new_camera.GetBounds(mins, maxs);
	        vector center = (mins + maxs) * 0.5;
	        vector size = maxs - mins;
	
	        PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateBox(size), "material/default", 0xffffffff)};
	        dBodyDynamic(new_camera, true);
	       	new_camera.SetDynamicPhysicsLifeTime(15.0);
			dBodySetMass(new_camera, 100);
			dBodySetDamping(new_camera, 100, 500);
	        dBodyActive(new_camera, ActiveState.ACTIVE);
	        new_camera.CreateDynamicPhysics(PhxInteractionLayers.RAGDOLL);
		}
		
		
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
	

}