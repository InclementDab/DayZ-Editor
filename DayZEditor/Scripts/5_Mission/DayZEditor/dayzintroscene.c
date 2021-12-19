// themes go here
#define EDITOR_CHRISTMAS
//#define EDITOR_4TH_JULY
//#define EDITOR_ANNIVERSARY

modded class MissionMainMenu
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (m_IntroScenePC)
	    	m_IntroScenePC.OnUpdate(timeslice);
	}
	
#ifdef EDITOR_CHRISTMAS
	override void PlayMusic()
	{
		return;
	}
#endif
}

modded class DayZIntroScene
{
	// 😂
	protected Object m_FunnyMeme;
	protected ref array<Object> m_FunnyMemes = {};
	
	protected bool m_ChristmasSetup = false;
	protected float m_CameraTimer;
	
	float offset, totaltime;
	int hour, minute;	
	
	static const ref array<string> XmasGiftTypes = {
		"XmasGiftRed1",
		"XmasGiftRed2",
		"XmasGiftBlue1",
		"XmasGiftBlue2",
		"XmasGiftGreen1",
		"XmasGiftGreen2"
	};
	
	protected ref array<Object> m_ChristmasObjects = {};

	void DayZIntroScene()
	{
		delete m_Character;
		
		m_CharacterPos = m_Camera.GetPosition() + m_Camera.GetDirection() * 5;
		m_CharacterPos = m_CharacterPos + m_Camera.GetDirection() * vector.Up * -0.3;
		m_CharacterPos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 1.5;
	
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetPosition(m_CharacterPos);
		m_FunnyMeme.Update();
		
#ifdef EDITOR_CHRISTMAS
		Snow snow = new Snow();
		m_CharacterPos[1] = m_CharacterPos[1] + 10;
		SEffectManager.PlayInWorld(snow, m_CharacterPos);
#endif
	}
	
	void ~DayZIntroScene()
	{
#ifdef EDITOR_CHRISTMAS
		foreach (Object o: m_ChristmasObjects) {
			GetGame().ObjectDelete(o);
		}
#endif
		
		foreach (Object meme: m_FunnyMemes) {
			GetGame().ObjectDelete(meme);
		}
	}
	
	void OnUpdate(float timeslice)
	{
		totaltime += timeslice / 2;
		
		Input input = GetGame().GetInput();
		// Christmas time :widepeepoHappy:
#ifdef EDITOR_CHRISTMAS
		if (!m_ChristmasSetup) {
			vector tree_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetCurrentCameraDirection() * 10;
			tree_pos[0] = tree_pos[0] + Math.RandomFloat(-3, 3);
			tree_pos[2] = tree_pos[2] + Math.RandomFloat(-3, 3);
			tree_pos[1] = GetGame().SurfaceY(tree_pos[0], tree_pos[2]);
			m_ChristmasObjects.Insert(GetGame().CreateObject("ChristmasTree_Green", tree_pos));
			
			vector meme_pos = m_FunnyMeme.GetPosition();
			for (int i = 0; i < 10; i++) {
				vector gift_pos;
				gift_pos[0] = meme_pos[0] + Math.RandomFloat(-5, 5);
				gift_pos[2] = meme_pos[2] + Math.RandomFloat(-5, 5);
				gift_pos[1] = GetGame().SurfaceY(meme_pos[0], meme_pos[2]);
				
				m_ChristmasObjects.Insert(GetGame().CreateObject(XmasGiftTypes.GetRandomElement(), gift_pos));
			}
			
			Particle.Play(ParticleList.SNOW, m_FunnyMeme, Vector(0, 10, 0));
			m_ChristmasSetup = true;
		}
#endif
				
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection() * 4;
		vector lookat = vector.Direction(m_FunnyMeme.GetPosition(), mouse_pos);
		vector pos = m_FunnyMeme.GetPosition();
		
		// Makes camera 'hover' in position
		pos[1] = pos[1] + Math.Sin(totaltime * Math.PI) / 1500;
		m_FunnyMeme.SetPosition(pos);
		m_FunnyMeme.SetDirection(lookat);
		m_FunnyMeme.Update();
		
		// easter egg
		if (KeyState(KeyCode.KC_NUMPADENTER)) {
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
		}
		

		// another easter egg
		m_CameraTimer += timeslice;
		if (m_CameraTimer > 1 && (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			m_CameraTimer = 0;
			vector start = GetGame().GetCurrentCameraPosition();
			vector end = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 5000;
			vector contact_pos, contact_dir;
			int component;
			
			DayZPhysics.RaycastRV(start, end, contact_pos, contact_dir, component);
	
			vector newcam_pos = contact_pos;
			newcam_pos[1] = newcam_pos[1] + 50;
			Object new_camera = GetGame().CreateObjectEx("DSLRCamera", newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
			//Object new_camera = GetGame().CreateObjectEx(XmasGiftTypes.GetRandomElement(), newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
			
			float scale = vector.Distance(contact_pos, start) * 0.25;	        
			if (new_camera) {
				vector mins, maxs;
		        new_camera.GetBounds(mins, maxs);
		        vector center = (mins + maxs) * 0.5;
		        vector size = maxs - mins;
		        
		        new_camera.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
				new_camera.SetDynamicPhysicsLifeTime(-1);
				new_camera.EnableDynamicCCD(true);
				dBodySetMass(new_camera, 100);
				m_FunnyMemes.Insert(new_camera);
			}
		}
		
		if (KeyState(KeyCode.KC_BACK)) {
			foreach (Object obj: m_FunnyMemes) {
				GetGame().ObjectDelete(obj);
			}
		}
	}
}