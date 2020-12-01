


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
	// 😂
	private Object m_FunnyMeme;
	private ref array<Object> m_FunnyMemes = {};
	
	float offset, totaltime;
	int hour, minute;	
	
	private static const ref array<string> XmasGiftTypes = {
		"XmasGiftRed1",
		"XmasGiftRed2",
		"XmasGiftBlue1",
		"XmasGiftBlue2",
		"XmasGiftGreen1",
		"XmasGiftGreen2"
	};
		
	void DayZIntroScene()
	{
		delete m_Character;
		
		// Christmas time :widepeepoHappy:
		GetGame().GetWeather().GetRain().Set(1, 0, 1000);
		vector tree_pos = m_CharacterPos - Vector(6, 0, 2);
		GetGame().CreateObject("ChristmasTree", tree_pos);
		
		for (int i = 0; i < 10; i++) {
			vector gift_pos;
			gift_pos[0] = tree_pos[0] + Math.RandomFloat(-3, 3);
			gift_pos[1] = tree_pos[1];
			gift_pos[2] = tree_pos[2] + Math.RandomFloat(-3, 3);
			
			GetGame().CreateObject(XmasGiftTypes.GetRandomElement(), gift_pos);
		}
		
		
		m_CharacterPos = Vector(0.685547, 50, 5.68823).Multiply4(m_CameraTrans);
		m_FunnyMeme = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
		m_FunnyMeme.SetOrientation(m_CharacterRot);
		m_FunnyMeme.SetPosition(m_FunnyMeme.GetPosition() + Vector(0, 1, 0));
	}
	

	void OnUpdate(float timeslice)
	{
		totaltime += timeslice / 2;
		
		Input input = GetGame().GetInput();
		
		vector mouse_pos = m_Camera.GetPosition() + GetGame().GetPointerDirection() * 4;
		vector lookat = vector.Direction(m_FunnyMeme.GetPosition(), mouse_pos);
		vector pos = m_FunnyMeme.GetPosition();
		
		// Makes camera 'hover' in position
		pos[1] = pos[1] + Math.Sin(totaltime * Math.PI) / 1500;
		m_FunnyMeme.SetPosition(pos);
		m_FunnyMeme.SetDirection(lookat);
		m_FunnyMeme.Update();
		
		// easter egg
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
		
		// another easter egg
		if (input.LocalPress("UAFire")) {
			
			vector start = GetGame().GetCurrentCameraPosition();
			vector end = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 5000;
			vector contact_pos, contact_dir;
			int component;
			
			DayZPhysics.RaycastRV(start, end, contact_pos, contact_dir, component);
	
			vector newcam_pos = contact_pos;
			newcam_pos[1] = newcam_pos[1] + 50;
			//Object new_camera = GetGame().CreateObjectEx("DSLRCamera", newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
			Object new_camera = GetGame().CreateObjectEx(XmasGiftTypes.GetRandomElement(), newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
			
			float scale = vector.Distance(contact_pos, start) * 0.25;

	        vector mins, maxs;
	        new_camera.GetBounds(mins, maxs);
	        vector center = (mins + maxs) * 0.5;
	        vector size = maxs - mins;
	        
			
	        new_camera.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			new_camera.SetDynamicPhysicsLifeTime(-1);
			dBodySetMass(new_camera, 100);
			m_FunnyMemes.Insert(new_camera);
		}
	}
}