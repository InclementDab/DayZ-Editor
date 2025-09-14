class PlayerCameraData
{
	float Timestamp;
	float Quat[4];
	vector Position;
}

modded class MissionGameplay
{	
	protected ref EditorMainMenu m_PauseMenu;
	
	ref map<int, Object> Cameras = new map<int, Object>();
	ref map<int, ref PlayerCameraData> LastCameraData = new map<int, ref PlayerCameraData>();
	ref map<int, ref PlayerCameraData> CameraData = new map<int, ref PlayerCameraData>();
	
	ref map<int, ref EditorCameraMarker> CameraMarkers = new map<int, ref EditorCameraMarker>();
	
	override void OnInit()
	{
		super.OnInit();
				
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceDisable(true);
		GetUApi().UpdateControls();
				
		DayZGame.Event_OnRPC.Insert(OnERPC);
	}
	
	override void OnKeyPress(int key)
	{			
		if (!GetEditor() || !GetEditor().OnKeyPress(key)) {
			super.OnKeyPress(key);
		}	
	}
	
	override void OnKeyRelease(int key)
	{
		if (!GetEditor() || !GetEditor().OnKeyRelease(key)) {
			super.OnKeyRelease(key);
		}
	}
		
	override void OnMouseButtonRelease(int button)
	{
		if (!GetEditor() || !GetEditor().OnMouseRelease(button)) {
			super.OnMouseButtonRelease(button);
		}
	}

	override void OnUpdate(float timeslice)
	{
		if (GetEditor()) {
			if (GetEditor().GetControllingPlayer() && !GetEditor().IsActive()) {
				super.OnUpdate(timeslice);
			}

			GetEditor().Update(timeslice);
		} else {
			super.OnUpdate(timeslice);
		}
		
		if (GetGame().IsMultiplayer()) {
			foreach (int player_id, Object camera: Cameras) {
				if (!camera) {
					continue;
				}
				
				PlayerCameraData last_camera_data = LastCameraData[player_id];
				PlayerCameraData camera_data = CameraData[player_id];
				if (!last_camera_data || !camera_data) {
					continue;
				}
				
				float time_passed = camera_data.Timestamp - last_camera_data.Timestamp;
				if (time_passed >= 1.0) {
					continue;
				}
				
				float qout[4];
				Math3D.QuatLerp(qout, last_camera_data.Quat, camera_data.Quat, time_passed);
				
				vector mat[4];
				Math3D.QuatToMatrix(qout, mat);
				mat[3] = vector.Lerp(last_camera_data.Position, camera_data.Position, time_passed);
								
				camera.SetTransform(mat);
				camera.Update();
				
				CameraMarkers[player_id].WorldPosition = mat[3];
				CameraMarkers[player_id].WorldOrientation = Math3D.MatrixToAngles(mat);
			}
		}
	}
	
	override void ShowInventory()
	{
		GetGame().GetPlayer().GetHumanInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		super.ShowInventory();
		m_Hud.InventoryShown = true;
	}
	
	override void HideInventory()
	{
		super.HideInventory();
		m_Hud.InventoryShown = false;
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		delete g_Editor;
	}
	
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
		
		// Server can handle itself
		if (GetGame().IsMultiplayer()) {
			return;
		}

		vector center_pos = Editor.GetMapCenterPosition();
		vector start_pos = Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 3500);
		PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), start_pos);
		if (!player) {
			Error("Player was not created, exiting");
			return;
		}

		// Make sure to select player immediately so they can be controlled
		GetGame().SelectPlayer(null, player);

		g_Editor = new Editor(player);
		g_Editor.SetActive(true);
	}
		
	/*
	override void Continue()
	{
		super.Continue();
		
		if (GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}*/
	
	override bool IsPaused()
	{
		return m_PauseMenu != null;
	}
	
	override void Pause()
	{
		if (IsPaused() || m_PauseMenu)
		{
			return;
		}

		m_PauseQueued = true;

		if (g_Game.GetGameState() != DayZGameState.IN_GAME) {
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && !player.IsPlayerLoaded() || IsPlayerRespawning()) {
			return;
		}
		
		CloseAllMenus();
		
		// open ingame menu
		m_PauseMenu = new EditorMainMenu();

		AddActiveInputExcludes({"menu"});
		AddActiveInputRestriction(EInputRestrictors.INVENTORY);

		m_PauseQueued = false;
	}
	
	override void Continue()
	{
		//GetGame().GetUIManager().Back();
		
		RemoveActiveInputExcludes({"menu"},true);
		RemoveActiveInputRestriction(EInputRestrictors.INVENTORY);
		delete m_PauseMenu;
			
		if (GetEditor() && GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		}
	}
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		
		switch (eventTypeId)
		{
			case ChatMessageEventTypeID:
				break;
		}
	}
	
	void OnERPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		int count, i;
		string uuid;
		
		if (!GetGame().IsMultiplayer()) {
			return;
		}
		
		switch (rpc_type) {
			case 39250: {
				Print("Editor Create");
				// Initialize editor
				PlayerBase player;
				ctx.Read(player);
				
				PlayerIdentity identity;
				ctx.Read(identity);
				
				if (identity.GetId() == GetGame().GetPlayer().GetIdentity().GetId()) {				
					g_Editor = new Editor(player);
					if (g_Editor) {
						g_Editor.SetActive(true);
					}
				} else {
					int player_id3 = identity.GetPlayerId();
					Cameras[player_id3] = GetGame().CreateObjectEx("DSLRCamera", vector.Zero, ECE_LOCAL);
					CameraMarkers[player_id3] = new EditorCameraMarker(identity.GetName());
					GetEditor().GetEditorHud().GetTemplateController().InsertMapMarker(CameraMarkers[player_id3]);
					GetEditor().GetEditorHud().GetTemplateController().RightbarPlayerData.Insert(new EditorPlayerListItem(player_id3, identity.GetName()));
					PrintFormat("Created camera for %1", player_id3);
				}
				
				break;
			}
			
            case 39252: {
               	ctx.Read(count);
				Print("Create objects " + count);
				map<string, ref EditorObjectData> data_map = new map<string, ref EditorObjectData>();
				
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					
					int low, high;
					ctx.Read(low);
					ctx.Read(high);
					
               		EditorObjectData dta = new EditorObjectData();
					dta.Read(ctx, int.MAX);
					
					dta.m_LowBits = low;
					dta.m_HighBits = high;
									
					data_map[uuid] = dta;
				}
				
				GetEditor().CreateObjectsByUuid(data_map, false);
                break;
            }
			
			case 39253: {
				ctx.Read(count);
				
				array<string> deleted_data = {};
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					deleted_data.Insert(uuid);
				}
				
				GetEditor().DeleteObjectsByUuid(deleted_data);				
				
                break;
            }
			
			case 39254: {
				ctx.Read(count);
				for (i = 0; i < count; i++) {
					EditorObjectData dta2 = new EditorObjectData();
					ctx.Read(uuid);
					dta2.Read(ctx, int.MAX);

					GetEditor().UpdateObjectByUuid(uuid, dta2);
				}				
				
                break;
            }
			
			case 39255: {
				ctx.Read(count);
				map<string, Object> hidden_objects = new map<string, Object>();
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					
					Object entity;
					ctx.Read(entity);
					
					hidden_objects[uuid] = entity;
				}
				
				GetEditor().HideMapObjectsByUuid(hidden_objects, true);
				break;
			}
			
			case 39256: {
				ctx.Read(count);
				array<string> unhide_objects = {};
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					unhide_objects.Insert(uuid);
				}
				
				GetEditor().UnhideMapObjectsByUuid(unhide_objects, true);
				break;
			}
			
			case 39257: {
				int player_id;
				float camera_quat[4];
				vector camera_pos;
				
				ctx.Read(player_id);
				ctx.Read(camera_pos);
				ctx.Read(camera_quat);
				
				PlayerCameraData camera_data = new PlayerCameraData();
				copyarray(camera_data.Quat, camera_quat);
				camera_data.Position = camera_pos;
				camera_data.Timestamp = GetDayZGame().GetTickTime();
				LastCameraData[player_id] = CameraData[player_id];
				CameraData[player_id] = camera_data;
				break;
			}
			
			case 39251: {
				int player_id2;
				ctx.Read(player_id2);
				
				Cameras[player_id2].Delete();
				Cameras.Remove(player_id2);
				CameraMarkers.Remove(player_id2);
				
				for (i = GetEditor().GetEditorHud().GetTemplateController().RightbarPlayerData.Count() - 1; i >= 0; i--) {
					if (GetEditor().GetEditorHud().GetTemplateController().RightbarPlayerData[i].Id == player_id2) {
						GetEditor().GetEditorHud().GetTemplateController().RightbarPlayerData.Remove(i);
						break;
					}
				}
				
				break;	
			}
			
			case 39260: {				
				string chat_text;
				ctx.Read(chat_text);
				
				string chat_sender;
				ctx.Read(chat_sender);
				
				ChatMessageEventParams chat_params = new ChatMessageEventParams(CCDirect, chat_sender, chat_text, "");
				GetEditor().GetEditorHud().GetChat().Add(chat_params);
				break;
			}
        }
    } 
}