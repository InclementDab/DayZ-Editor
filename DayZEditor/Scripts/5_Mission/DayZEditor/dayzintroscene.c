modded class MissionMainMenu
{
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		if (m_IntroScenePC)
	    	m_IntroScenePC.OnUpdate(timeslice);
	}
	
	override void PlayMusic()
	{
		if (Editor.GetCurrentHoliday() == EditorHoliday.CHRISTMAS || Editor.GetCurrentHoliday() == EditorHoliday.NEWYEARS) {
			return;
		}
		
		super.PlayMusic();
	}
}

modded class DayZIntroScene
{
	static const float CAMERA_DISTANCE_FROM_SCREEN = 4.5;
	static const float CHRISTMAS_TREE_DISTANCE_FROM_SCREEN = 10.0;
	static const float FIREWORK_DISTANCE_FROM_SCREEN = 15.0;
	
	
	// 😂
	protected Object m_DSLRCamera;
	protected ref array<Object> m_FallenObjects = {};
	protected ref array<Object> m_ChristmasObjects = {};
	
	protected float m_Offset, m_TotalTime, m_CameraTimer;
	protected int m_Hour, m_Minute;	
	
	protected bool m_ParticleSetup;
	protected EditorHoliday m_CurrentHoliday = Editor.GetCurrentHoliday();
		
	static const ref array<string> XmasGiftTypes = {
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
		
		m_CharacterPos = m_Camera.GetPosition() + m_Camera.GetDirection() * CAMERA_DISTANCE_FROM_SCREEN;
		m_CharacterPos = m_CharacterPos + m_Camera.GetDirection() * vector.Up * -0.5;
		m_CharacterPos[1] = GetGame().SurfaceY(m_CharacterPos[0], m_CharacterPos[2]) + 1.25;
	
		// determine camera model based on holiday
		switch (m_CurrentHoliday) {
			case EditorHoliday.CHRISTMAS:
			case EditorHoliday.NEWYEARS: {
				m_DSLRCamera = GetGame().CreateObject("DSLRCameraChristmas", m_CharacterPos, true);
				break;
			}
			
			case EditorHoliday.ANNIVERSARY: {
				m_DSLRCamera = GetGame().CreateObject("DSLRCameraAnniversary", m_CharacterPos, true);
				break;
			}
			
			default: {
				m_DSLRCamera = GetGame().CreateObject("DSLRCamera", m_CharacterPos, true);
				break;
			}	
		}
		
		m_DSLRCamera.SetPosition(m_CharacterPos);
		m_DSLRCamera.Update();
	}
	
	void ~DayZIntroScene()
	{
		foreach (Object christmas_object: m_ChristmasObjects) {
			GetGame().ObjectDelete(christmas_object);
		}	
		
		foreach (Object fallen_object: m_FallenObjects) {
			GetGame().ObjectDelete(fallen_object);
		}
	}
	
	void OnUpdate(float timeslice)
	{
		m_TotalTime += timeslice / 2;
				
		if (!m_ParticleSetup) {
			if (m_CurrentHoliday == EditorHoliday.NEWYEARS || m_CurrentHoliday == EditorHoliday.CHRISTMAS) {
				
				// Christmas time :widepeepoHappy:
				vector tree_pos = m_Camera.GetPosition() + m_Camera.GetDirection() * CHRISTMAS_TREE_DISTANCE_FROM_SCREEN;
				tree_pos[0] = tree_pos[0] + Math.RandomFloat(-1, 1);
				tree_pos[2] = tree_pos[2] + Math.RandomFloat(-1, 1);
				tree_pos[1] = GetGame().SurfaceY(tree_pos[0], tree_pos[2]);
				m_ChristmasObjects.Insert(GetGame().CreateObject("ChristmasTree_Green", tree_pos));
				GetGame().GetWorld().SetDate(1, 1, 1, 9, 0);			
				
				// Set up presents
				vector present_position = m_CharacterPos;
				for (int i = 0; i < 10; i++) {
					vector gift_pos;
					gift_pos[0] = present_position[0] + Math.RandomFloat(-5, 5);
					gift_pos[2] = present_position[2] + Math.RandomFloat(-5, 5);
					gift_pos[1] = GetGame().SurfaceY(present_position[0], present_position[2]) + 0.5;
					
					m_ChristmasObjects.Insert(GetGame().CreateObject(XmasGiftTypes.GetRandomElement(), gift_pos));
				}
				
				if (m_CurrentHoliday == EditorHoliday.NEWYEARS) {
					Particle.PlayInWorld(ParticleList.FIREWORK, m_Camera.GetPosition() + m_Camera.GetDirection() * FIREWORK_DISTANCE_FROM_SCREEN);
					GetGame().GetWorld().SetDate(1, 1, 1, 0, 0);
				}
				
				Particle.PlayOnObject(ParticleList.SNOW, m_DSLRCamera, Vector(0, 0, 0));
				Particle.PlayOnObject(ParticleList.SNOW, m_DSLRCamera, Vector(0, 0, 0));
				Particle.PlayOnObject(ParticleList.SNOWFOG, m_DSLRCamera, Vector(0, 0, 0));
				Particle.PlayOnObject(ParticleList.SNOWFOG, m_DSLRCamera, Vector(0, 0, 0));
			}
		}
		
		vector lookat = vector.Direction(m_DSLRCamera.GetPosition(), m_Camera.GetPosition() + GetGame().GetPointerDirection() * (CAMERA_DISTANCE_FROM_SCREEN / 2));
		vector pos = m_DSLRCamera.GetPosition();
		
		// Makes camera 'hover' in position
		pos[1] = pos[1] + Math.Sin(m_TotalTime * Math.PI) / 1500;
		m_DSLRCamera.SetPosition(pos);
		m_DSLRCamera.SetDirection(lookat);
		m_DSLRCamera.Update();
		
		// easter egg
		if (KeyState(KeyCode.KC_NUMPADENTER)) {
			vector ori = m_DSLRCamera.GetOrientation();
			m_Offset += 10;
			if (m_Offset > 360) m_Offset = 0;
			ori[2] = ori[2] + m_Offset;
			
			m_DSLRCamera.SetOrientation(ori);
			m_DSLRCamera.Update();
			
			foreach (Object cam: m_FallenObjects) {
				vector trans[4];
				cam.GetTransform(trans);
				dBodyApplyTorqueImpulse(cam, trans[2].Normalized() * timeslice * 100 * 50);
			}
			
			m_Minute = (ori[2] / 360) * 60;
			if (m_Minute >= 60) {
				m_Minute = 0;
				m_Hour++;
			}
			
			if (m_Hour > 24) {
				m_Hour = 0;
			}
			
			GetGame().GetWorld().SetDate(2018, 10, 1, m_Hour, m_Minute);
		}
		
		// another easter egg
		m_CameraTimer += timeslice;
		if (m_CameraTimer > 1 && (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && GetGame().GetInput().HasGameFocus()) {
			m_CameraTimer = 0;
			vector start = GetGame().GetCurrentCameraPosition();
			vector end = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * 5000;
			vector contact_pos, contact_dir;
			int component;
			
			DayZPhysics.RaycastRV(start, end, contact_pos, contact_dir, component);
	
			vector newcam_pos = contact_pos;
			newcam_pos[1] = newcam_pos[1] + 50;
			
			// determine object to drop from sky depending on holiday
			Object object_to_drop;
			switch (m_CurrentHoliday) {
				case EditorHoliday.NEWYEARS:
				case EditorHoliday.CHRISTMAS: {
					object_to_drop = GetGame().CreateObjectEx(XmasGiftTypes.GetRandomElement(), newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
					break;
				}
				
				default: {
					object_to_drop = GetGame().CreateObjectEx("DSLRCamera", newcam_pos, ECE_CREATEPHYSICS | ECE_SETUP);
					break;
				}
			}
						
			float scale = vector.Distance(contact_pos, start) * 0.25;	        
			if (object_to_drop) {
				vector mins, maxs;
		        object_to_drop.GetBounds(mins, maxs);
		        vector center = (mins + maxs) * 0.5;
		        vector size = maxs - mins;
		        
		        object_to_drop.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
				object_to_drop.SetDynamicPhysicsLifeTime(-1);
				object_to_drop.EnableDynamicCCD(true);
				dBodySetMass(object_to_drop, 100);
				m_FallenObjects.Insert(object_to_drop);
			}
		}
		
		if (KeyState(KeyCode.KC_BACK)) {
			foreach (Object fallen_object: m_FallenObjects) {
				GetGame().ObjectDelete(fallen_object);
			}
		}
	}
}