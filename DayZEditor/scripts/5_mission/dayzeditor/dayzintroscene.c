


modded class MissionMainMenu
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (m_IntroScenePC)
	    	m_IntroScenePC.OnUpdate(timeslice);
	}
}

modded class DayZIntroScene
{
	Object m_FunnyMeme;
	
	void DayZIntroScene()
	{
		
		delete m_Character;
		m_CharacterPos = Vector(0.685547, 50, 5.68823).Multiply4(m_CameraTrans);
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetOrientation(m_CharacterRot);
		m_FunnyMeme.SetPosition(m_FunnyMeme.GetPosition() + Vector(0, 1, 0));
	}


	
	float offset;
	int hour, minute;
	
	ref array<Object> m_FunnyMemes = new array<Object>();
	
	float totaltime;
	
	void OnUpdate(float timeslice)
	{
		totaltime += timeslice / 2;
		
		Input input = GetGame().GetInput();
		
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection() * 4;
		vector lookat = vector.Direction(m_FunnyMeme.GetPosition(), mouse_pos);
		
		
		
		vector pos = m_FunnyMeme.GetPosition();
		
		pos[1] = pos[1] + Math.Sin(totaltime * Math.PI) / 1500;
		m_FunnyMeme.SetPosition(pos);
		m_FunnyMeme.SetDirection(lookat);
		m_FunnyMeme.Update();
		
		
		if (input.LocalValue("UAPersonView")) {
			vector ori = m_FunnyMeme.GetOrientation();
			offset += 10;
			if (offset > 360) offset = 0;
			ori[2] = ori[2] + offset;
			
			m_FunnyMeme.SetOrientation(ori);
			m_FunnyMeme.Update();
			
			foreach (Object cam: m_FunnyMemes) {
				vector trans[4];
				cam.GetTransform(trans);
				dBodyApplyTorqueImpulse(cam, trans[2].Normalized() * timeslice * 100 * 50);
			}
			
			minute = (ori[2] / 360) * 60;
			if (minute >= 60) {
				minute = 0;
				hour++;
			}
			
			if (hour > 24) {
				hour = 0;
			}
			
			GetGame().GetWorld().SetDate(2018, 10, 1, hour, minute);
			return;
		}
		
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

	        vector mins, maxs;
	        new_camera.GetBounds(mins, maxs);
	        vector center = (mins + maxs) * 0.5;
	        vector size = maxs - mins;
	        
			
	        new_camera.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			new_camera.SetDynamicPhysicsLifeTime(-1);
			dBodySetMass(new_camera, 100);
			//new_camera.SetDynamicPhysics();
			m_FunnyMemes.Insert(new_camera);
		}
	}
}


//#define LOAD_MISSION 1

modded class MainMenu 
{
	protected ImageWidget m_Logo;
	
	void MainMenu()
	{
#ifdef LOAD_MISSION		
		Print("Loading straight into mission");
		GetGame().PlayMission(CreateEditorMission("ChernarusPlus"));
#endif
		
	}
	
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
		
		m_Logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		m_Logo.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_Logo.SetImage(0);
		m_Logo.SetFlags(m_Logo.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		
		return layoutRoot;
	}

    override void Play()
    {
		MapSelectDialog select_window = new MapSelectDialog();
		select_window.ShowDialog();
		//GetGame().GetUIManager().ShowScriptedMenu(select_window, this);
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
		if(IsFocusable(w)) {
			ColorHighlight(w);
			return true;
		}
		return false;
	}
	

}