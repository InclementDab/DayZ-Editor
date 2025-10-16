class PlayerCameraData
{
	float Timestamp;
	float Quat[4];
	vector Position;
}

modded class MissionGameplay
{
	protected ref EditorMainMenu m_PauseMenu;
	private const float SYNC_COMPLETE_DEBOUNCE_TIME = 1.5;
	private float m_syncDebounceTimer = -1.0;
    protected bool m_SyncCompleteAlreadySent = false; 

	ref map<int, Object> Cameras = new map<int, Object>();
	ref map<int, ref PlayerCameraData> LastCameraData = new map<int, ref PlayerCameraData>();
	ref map<int, ref PlayerCameraData> CameraData = new map<int, ref PlayerCameraData>();
	
	ref map<int, ref EditorCameraMarker> CameraMarkers = new map<int, ref EditorCameraMarker>();
	protected bool m_AutoInitializeEditor;
    private Editor m_CachedEditor;
	private bool m_IsEditorActivated = false;
	private bool m_IsEditorInitialized = false;

    // State tracking for OnUpdate logging
    private bool m_LastEditorExisted = false;
    private bool m_LastCachedEditorExisted = false;
    private bool m_LastControllingPlayerExisted = false;
    private bool m_LastEditorWasActive = false;

	// Holds the active drag session context for a drag operation initiated by another player.
	protected ref LocalDragSession m_ActiveDragSession;

	void MissionGameplay()
	{
		m_AutoInitializeEditor = CreateEditorOnStart();
    	DayZGame.Event_OnRPC.Insert(OnERPC);
	}
	
	override void OnInit()
	{
		super.OnInit();

		/*
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedIncrease").ForceDisable(true);
		GetUApi().GetInputByName("UACameraToolSpeedDecrease").ForceDisable(true);
		GetUApi().UpdateControls();*/

	}
	
	void SendSyncComplete() 
	{
        if (m_SyncCompleteAlreadySent) {
            return;
        }
        
        ScriptRPC sync_complete_rpc = new ScriptRPC();
        sync_complete_rpc.Send(null, EditorRPC.CLIENT_SYNC_COMPLETE, true);
        Print("[Editor Client] Initial synchronization appears complete. Notifying server.");
        
        m_SyncCompleteAlreadySent = true;
    }
	
	override void OnKeyPress(int key)
	{
		if (GetEditor() && GetEditor().IsActive()) {
			if (!GetEditor().OnKeyPress(key)) {
				super.OnKeyPress(key);
			}
		} else {
			super.OnKeyPress(key);
		}
	}

	override void OnKeyRelease(int key)
	{
		if (GetEditor() && GetEditor().IsActive()) {
			if (!GetEditor().OnKeyRelease(key)) {
				super.OnKeyRelease(key);
			}
		} else {
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
		// Sync debounce timer for CLIENT_SYNC_COMPLETE
		if (m_syncDebounceTimer > 0)
		{
			m_syncDebounceTimer -= timeslice;
			if (m_syncDebounceTimer <= 0)
			{
				SendSyncComplete();
			}
		}
		
		// Only log g_Editor state changes
		bool editorExists = (g_Editor != null);
		if (editorExists != m_LastEditorExisted)
		{
			if (editorExists)
			{
				PrintFormat("[EDITOR DEBUG] OnUpdate - g_Editor created");
			}
			else
			{
				PrintFormat("[EDITOR DEBUG] OnUpdate - g_Editor destroyed");
			}
			m_LastEditorExisted = editorExists;
		}
		
		// Only log m_CachedEditor state changes
		bool cachedExists = (m_CachedEditor != null);
		if (cachedExists != m_LastCachedEditorExisted)
		{
			if (cachedExists)
			{
				PrintFormat("[EDITOR DEBUG] OnUpdate - m_CachedEditor now exists");
			}
			else
			{
				PrintFormat("[EDITOR DEBUG] OnUpdate - m_CachedEditor is NULL, calling super.OnUpdate");
			}
			m_LastCachedEditorExisted = cachedExists;
		}
		
		if (m_CachedEditor)
		{
			PlayerBase controlling_player = m_CachedEditor.GetControllingPlayer();
			
			// Only log controlling player state changes
			bool playerExists = (controlling_player != null);
			if (playerExists != m_LastControllingPlayerExisted)
			{
				if (playerExists)
				{
					PrintFormat("[EDITOR DEBUG] OnUpdate - Controlling player assigned: %1", controlling_player);
				}
				else
				{
					PrintFormat("[EDITOR DEBUG] OnUpdate - No controlling player");
				}
				m_LastControllingPlayerExisted = playerExists;
			}
			
			if (controlling_player)
			{
				// Only log editor active state changes
				bool isActive = m_CachedEditor.IsActive();
				if (isActive != m_LastEditorWasActive)
				{
					if (isActive)
					{
						PrintFormat("[EDITOR DEBUG] OnUpdate - Editor is active");
					}
					else
					{
						PrintFormat("[EDITOR DEBUG] OnUpdate - Editor not active, calling super.OnUpdate");
					}
					m_LastEditorWasActive = isActive;
				}
				
				if (!isActive)
				{
					super.OnUpdate(timeslice);
				}
			}
			else
			{
				super.OnUpdate(timeslice);
			}
			
			m_CachedEditor.Update(timeslice);
		}
		else
		{
			super.OnUpdate(timeslice);
		}

		// Multiplayer camera interpolation
		if (GetGame().IsMultiplayer()) {
			foreach (int player_id, Object camera: Cameras)
			{
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
				if (CameraMarkers[player_id])
				{
					CameraMarkers[player_id].WorldPosition = mat[3];
					CameraMarkers[player_id].WorldOrientation = Math3D.MatrixToAngles(mat);
				}
			}
		}
	}
	
	/*
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
	}*/
	
	override void OnMissionFinish()
	{
	PrintFormat("[EDITOR DEBUG] OnMissionFinish called - g_Editor: exists");
		super.OnMissionFinish();

		Editor editor_ref = g_Editor;
		g_Editor = null;  
		delete editor_ref; 
	}
	
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		// Server can handle itself
		if (GetGame().IsMultiplayer()) {
			return;
		}
		
		// In the event we handle the creation on our own
		if (m_AutoInitializeEditor) {
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
			m_IsEditorInitialized = true;
			m_IsEditorActivated = true;
			m_CachedEditor = g_Editor;
		}
	}
	
	bool CreateEditorOnStart()
	{
		return true;
	}
	
	override bool IsPaused()
	{
		if (!m_AutoInitializeEditor) {
			return super.IsPaused();
		}
		
		return m_PauseMenu != null;
	}
	
	override void Pause()
	{
		if (!m_AutoInitializeEditor) {
			super.Pause();
			return;
		}
		
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
		
		if (GetEditor() && GetEditor().GetEditorHud()) {
			GetEditor().GetEditorHud().Show(false);
		}
		// open ingame menu
		m_PauseMenu = new EditorMainMenu();

		AddActiveInputExcludes({"menu"});
		AddActiveInputRestriction(EInputRestrictors.INVENTORY);

		m_PauseQueued = false;
	}
	
	override void Continue()
	{
		if (!m_AutoInitializeEditor) {
			super.Continue();
			return;
		}
		
		//GetGame().GetUIManager().Back();

		RemoveActiveInputExcludes({"menu"},true);
		RemoveActiveInputRestriction(EInputRestrictors.INVENTORY);
		delete m_PauseMenu;
		/*
				if (GetEditor() && GetEditor().IsActive())	{
			GetEditor().GetEditorHud().Show(true);
		*/ 
	}

	void ActivateEditor()
	{
		if (g_Editor) {
			g_Editor.SetActive(true);
			m_IsEditorActivated = true;
            m_CachedEditor = g_Editor; 
            	PrintFormat("[EDITOR DEBUG] Editor activated and cached");
		}
	}

	void OnERPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
	PrintFormat("[EDITOR DEBUG] ========================================");
	PrintFormat("[EDITOR DEBUG] OnERPC CALLED!");
	PrintFormat("[EDITOR DEBUG] RPC Type: %1", rpc_type);
	PrintFormat("[EDITOR DEBUG] IsMultiplayer: %1", GetGame().IsMultiplayer());
	PrintFormat("[EDITOR DEBUG] Sender: %1", sender);
	PrintFormat("[EDITOR DEBUG] Target: %1", target);
	PrintFormat("[EDITOR DEBUG] ========================================");
		
		int count, i;
		string uuid;
		
		if (!GetGame().IsMultiplayer()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED: Not multiplayer");
			return;
		}

		bool isInitialized = m_IsEditorInitialized;
		bool isActivated = m_IsEditorActivated;

		if (rpc_type != EditorRPC.EDITOR_CREATE)
		{
			if (!isInitialized)
			{
	PrintFormat("[EDITOR DEBUG] >>> RPC %1 BLOCKED - Editor not initialized", rpc_type);
				return;
			}

			// Defer UI-related RPCs until the HUD is fully active
			switch (rpc_type)
			{
				case EditorRPC.PLAYER_JOINED:
				case EditorRPC.EDITOR_DESTROY:
				case EditorRPC.SERVER_CHAT:
					if (!isActivated)
					{
		PrintFormat("[EDITOR DEBUG] >>> UI RPC %1 BLOCKED - Editor not activated", rpc_type);
						return;
					}
					break;
			}
		}

	PrintFormat("[EDITOR DEBUG] >>> Processing RPC %1...", rpc_type);

		switch (rpc_type) {
			case EditorRPC.EDITOR_CREATE: {
	PrintFormat("[EDITOR DEBUG] ========== EDITOR_CREATE RPC ==========");
				
				PlayerBase player;
				if (!ctx.Read(player)) {
	PrintFormat("[EDITOR DEBUG] ERROR: Failed to read player from context");
					return;
				}
	PrintFormat("[EDITOR DEBUG] Player read: %1", player);
				
				PlayerIdentity identity;
				if (!ctx.Read(identity)) {
	PrintFormat("[EDITOR DEBUG] ERROR: Failed to read identity from context");
					return;
				}
	PrintFormat("[EDITOR DEBUG] Identity read: %1", identity);
				
	PrintFormat("[EDITOR DEBUG] Creating new Editor instance...");
				g_Editor = new Editor(player);
				
				if (g_Editor) {
	PrintFormat("[EDITOR DEBUG] ✓ Editor created successfully: %1", g_Editor);
					m_IsEditorInitialized = true;
					m_CachedEditor = g_Editor;
	PrintFormat("[EDITOR DEBUG] Scheduling ActivateEditor in 100ms...");
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ActivateEditor, 100, false);
	PrintFormat("[EDITOR DEBUG] ========== EDITOR_CREATE COMPLETE ==========");
				} else {
	PrintFormat("[EDITOR DEBUG]  FATAL: Failed to create Editor instance! ");
				}
				break;
			}

			case EditorRPC.PLAYER_JOINED: {
	PrintFormat("[EDITOR DEBUG] === PLAYER_JOINED RPC ===");
				
				if (!GetEditor() || !GetEditor().GetEditorHud()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor/EditorHud not ready");
					return;
				}
				
				int player_id3;
				ctx.Read(player_id3);

				string name;
				ctx.Read(name);
	PrintFormat("[EDITOR DEBUG] Player joined: ID=%1, Name=%2", player_id3, name);
				
				Cameras[player_id3] = GetGame().CreateObjectEx("DSLRCamera", vector.Zero, ECE_LOCAL);
				CameraMarkers[player_id3] = new EditorCameraMarker(name);
				
				EditorHud hud = GetEditor().GetEditorHud();
				if (hud && hud.GetTemplateController()) {
					hud.GetTemplateController().InsertMapMarker(CameraMarkers[player_id3]);
					hud.GetTemplateController().RightbarPlayerData.Insert(new EditorPlayerListItem(player_id3, name));
				}
				
	PrintFormat("[EDITOR DEBUG] Camera created for player %1", player_id3);
				break;
			}

			case EditorRPC.OBJECT_CREATE: {
				if (sender && sender.GetId() == GetGame().GetPlayer().GetIdentity().GetId())
				{
					return; 
				}
	PrintFormat("[EDITOR DEBUG] === OBJECT_CREATE RPC (from SERVER) ===");
				ctx.Read(count);
	PrintFormat("[EDITOR DEBUG] Creating %1 objects", count);
				
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

					dta.WorldObject = GetGame().GetObjectByNetworkId(low, high);

					data_map[uuid] = dta;
				}
				
				if (!GetEditor()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor not ready");
					return;
				}

				
				array<EditorObject> created_objects = GetEditor().CreateObjectsByUuid(data_map, false);

				foreach(EditorObject created_obj : created_objects)
				{
					if (created_obj && !created_obj.GetWorldObject())
					{
						GetEditor().GetObjectManager().RegisterUnresolvedObject(created_obj);
						Print("[Editor Client] RC detected for UUID " + created_obj.Uuid + ". Handed off to unresolved manager.");
					}
				}
				
				if (m_IsEditorInitialized && !m_SyncCompleteAlreadySent) {
					m_syncDebounceTimer = SYNC_COMPLETE_DEBOUNCE_TIME;
				}
				
	PrintFormat("[EDITOR DEBUG] Objects created successfully");
				break;
			}

			case EditorRPC.OBJECT_DELETE: {
	PrintFormat("[EDITOR DEBUG] === OBJECT_DELETE RPC ===");
				ctx.Read(count);
	PrintFormat("[EDITOR DEBUG] Deleting %1 objects", count);
				
				array<string> deleted_data = {};
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					deleted_data.Insert(uuid);
				}
				
				if (!GetEditor()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor not ready");
					return;
				}
				
				GetEditor().DeleteObjectsByUuid(deleted_data);
	PrintFormat("[EDITOR DEBUG] Objects deleted successfully");
				break;
			}

			case EditorRPC.OBJECT_UPDATE: {
	PrintFormat("[EDITOR DEBUG] === OBJECT_UPDATE RPC ===");
				ctx.Read(count);
	PrintFormat("[EDITOR DEBUG] Updating %1 objects", count);
				
				for (i = 0; i < count; i++) {
					EditorObjectData dta2 = new EditorObjectData();
					ctx.Read(uuid);
					dta2.Read(ctx, int.MAX);
					
					if (!GetEditor()) {
		PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor not ready (iteration %1)", i);
						continue;
					}
					
					GetEditor().UpdateObjectByUuid(uuid, dta2);
				}
				
	PrintFormat("[EDITOR DEBUG] Objects updated successfully");
				break;
			}
			
case EditorRPC.BATCH_UPDATE_TRANSFORM_PACKED: {
    PrintFormat("[CLIENT RECEIVE] BATCH_UPDATE_TRANSFORM_PACKED received");
    
    int objectCount;
    if (!ctx.Read(objectCount)) return;
    
    if (!GetEditor()) return;
    
    PrintFormat("[CLIENT RECEIVE] Processing %1 objects", objectCount);

    vector pos, ori;
    float scale;
    
    for (i = 0; i < objectCount; i++) {
        if (!ctx.Read(uuid)) break;
        if (uuid == string.Empty) continue;

        // Read each integer individually to avoid the array element bug
        int pack0, pack1, pack2, pack3;
        
        if (!ctx.Read(pack0)) break;
        if (!ctx.Read(pack1)) break;
        if (!ctx.Read(pack2)) break;
        if (!ctx.Read(pack3)) break;

        PrintFormat("[CLIENT RECEIVE | PRE-UNPACK] Received Packed Data for UUID %1: [%2, %3, %4, %5]", uuid, pack0, pack1, pack2, pack3);

        // Now create the array for UnpackTransform
        int packedData[4];
        packedData[0] = pack0;
        packedData[1] = pack1;
        packedData[2] = pack2;
        packedData[3] = pack3;

        EditorObject obj = GetEditor().GetEditorObjectByUuid(uuid); 
        if (obj) {
            EditorNetUtils.UnpackTransform(packedData, pos, ori, scale);
            obj.SetPosition(pos);
            obj.SetOrientation(ori);
            obj.SetScale(scale);
            
            PrintFormat("    [CLIENT RECEIVE] SUCCESS: Updated UUID %1 to Pos %2 | Ori %3 | Scale %4", uuid, pos.ToString(), ori.ToString(), scale);
        } else {
            PrintFormat("    [CLIENT RECEIVE] WARNING: Object with UUID %1 not found locally.", uuid);
        }
    }
    break;
}
case EditorRPC.DRAG_SESSION:
{
	HandleDragSession(sender, ctx);
	break;
}

			case EditorRPC.OBJECT_HIDE: {
	PrintFormat("[EDITOR DEBUG] === OBJECT_HIDE RPC ===");
				ctx.Read(count);
	PrintFormat("[EDITOR DEBUG] Hiding %1 objects", count);
				
				map<string, Object> hidden_objects = new map<string, Object>();
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);

					Object entity;
					ctx.Read(entity);
					
					hidden_objects[uuid] = entity;
				}
				
				if (!GetEditor()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor not ready");
					return;
				}
				
				GetEditor().HideMapObjectsByUuid(hidden_objects, true);
				
				if (m_IsEditorInitialized && !m_SyncCompleteAlreadySent) {
					m_syncDebounceTimer = SYNC_COMPLETE_DEBOUNCE_TIME;
				}
				
	PrintFormat("[EDITOR DEBUG] Objects hidden successfully");
				break;
			}

			case EditorRPC.OBJECT_UNHIDE: {
	PrintFormat("[EDITOR DEBUG] === OBJECT_UNHIDE RPC ===");
				ctx.Read(count);
	PrintFormat("[EDITOR DEBUG] Unhiding %1 objects", count);
				
				array<string> unhide_objects = {};
				for (i = 0; i < count; i++) {
					ctx.Read(uuid);
					unhide_objects.Insert(uuid);
				}
				
				if (!GetEditor()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor not ready");
					return;
				}
				
				GetEditor().UnhideMapObjectsByUuid(unhide_objects, true);
	PrintFormat("[EDITOR DEBUG] Objects unhidden successfully");
				break;
			}

			case EditorRPC.CAMERA_UPDATE: {
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

			case EditorRPC.EDITOR_DESTROY: {
	PrintFormat("[EDITOR DEBUG] === EDITOR_DESTROY RPC ===");
				
				if (!GetEditor() || !GetEditor().GetEditorHud()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor/EditorHud not ready");
					return;
				}
				
				int player_id2;
				ctx.Read(player_id2);
	PrintFormat("[EDITOR DEBUG] Destroying editor for player %1", player_id2);
				
				if (Cameras[player_id2]) {
					GetGame().ObjectDelete(Cameras[player_id2]);
					Cameras.Remove(player_id2);
				}
				
				if (CameraMarkers[player_id2]) {
					CameraMarkers.Remove(player_id2);
				}
				
				EditorHud hud2 = GetEditor().GetEditorHud();
				if (hud2 && hud2.GetTemplateController()) {
					for (i = hud2.GetTemplateController().RightbarPlayerData.Count() - 1; i >= 0; i--) {
						if (hud2.GetTemplateController().RightbarPlayerData[i].Id == player_id2) {
							hud2.GetTemplateController().RightbarPlayerData.Remove(i);
							break;
						}
					}
				}
				
	PrintFormat("[EDITOR DEBUG] Editor destroyed for player %1", player_id2);
				break;
			}

			case EditorRPC.SERVER_CHAT: {
	PrintFormat("[EDITOR DEBUG] === SERVER_CHAT RPC ===");
				
				if (!GetEditor() || !GetEditor().GetEditorHud()) {
	PrintFormat("[EDITOR DEBUG] >>> BLOCKED - Editor/EditorHud not ready");
					return;
				}
				
				string chat_text;
				ctx.Read(chat_text);
				string chat_sender;
				ctx.Read(chat_sender);
	PrintFormat("[EDITOR DEBUG] Chat from %1: %2", chat_sender, chat_text);
				
				ChatMessageEventParams chat_params = new ChatMessageEventParams(CCDirect, chat_sender, chat_text, "");
				
				EditorHud hud3 = GetEditor().GetEditorHud();
				if (hud3 && hud3.GetChat()) {
					hud3.GetChat().Add(chat_params);
				}
				
				break;
			}
			
			default: {
	PrintFormat("[EDITOR DEBUG] >>> UNKNOWN RPC TYPE: %1", rpc_type);
				break;
			}
		}	
	PrintFormat("[EDITOR DEBUG] OnERPC complete for RPC %1", rpc_type);
	}

	// DRAG SESSION RECEIVER HANDLERS

	/**
	 * @brief Client-side RPC router for all drag session operations broadcast from the server.
	 */
	private void HandleDragSession(PlayerIdentity sender, ParamsReadContext ctx)
	{
		eDragPhase phase;
		if (!ctx.Read(phase)) return;

		switch (phase)
		{
			case eDragPhase.START:
				HandleDragSession_Start(ctx);
				break;
			case eDragPhase.UPDATE:
				HandleDragSession_Update(ctx);
				break;
			case eDragPhase.END:
				HandleDragSession_End(ctx);
				break;
		}
	}

	/**
	 * @brief Creates a local session context to prepare for receiving high-frequency updates.
	 */
	private void HandleDragSession_Start(ParamsReadContext ctx)
	{
		Print("[CLIENT-DRAGSTART] HANDLING START RPC");
		
		if (m_ActiveDragSession)
		{
			Print("[CLIENT-DRAGSTART] WARNING: Active session exists, cleaning up");
			delete m_ActiveDragSession;
		}

		m_ActiveDragSession = new LocalDragSession();

		string parentUUID;
		if (!ctx.Read(parentUUID))
		{
			Print("[CLIENT-DRAGSTART] ERROR: Failed to read parentUUID");
			return;
		}

		m_ActiveDragSession.m_ParentObject = GetEditor().GetEditorObjectByUuid(parentUUID);
		if (!m_ActiveDragSession.m_ParentObject)
		{
			Print(string.Format("[CLIENT-DRAGSTART] ERROR: Parent object not found for UUID: %1", parentUUID));
			return;
		}

		int childCount;
		if (!ctx.Read(childCount))
		{
			Print("[CLIENT-DRAGSTART] ERROR: Failed to read childCount");
			return;
		}

		for (int i = 0; i < childCount; i++)
		{
			
			string childUUID;
			if (!ctx.Read(childUUID))
			{
				Print(string.Format("[CLIENT-DRAGSTART] ERROR: Failed to read childUUID at index %1", i));
				continue;
			}

			vector relativePos;
			if (!ctx.Read(relativePos))
			{
				Print(string.Format("[CLIENT-DRAGSTART] ERROR: Failed to read relativePos for child %1", childUUID));
				continue;
			}

			vector relativeOri;
			if (!ctx.Read(relativeOri))
			{
				Print(string.Format("[CLIENT-DRAGSTART] ERROR: Failed to read relativeOri for child %1", childUUID));
				continue;
			}

			EditorObject childObject = GetEditor().GetEditorObjectByUuid(childUUID);
			if (childObject)
			{
				auto childData = new LocalDragChildData(childObject, relativePos, relativeOri);
				m_ActiveDragSession.m_ChildData.Insert(childData);
			}
			else
			{
				Print(string.Format("[CLIENT-DRAGSTART] WARNING: Child object not found for UUID: %1", childUUID));
			}
		}
	}


	/**
	 * @brief Applies high-frequency transform updates to the object group for smooth visual movement.
	 */
	private void HandleDragSession_Update(ParamsReadContext ctx)
	{
		if (!m_ActiveDragSession || !m_ActiveDragSession.m_ParentObject) return;

		string parentUUID;
		if (!ctx.Read(parentUUID)) return;

		// Ensure the update is for the object we are tracking
		if (m_ActiveDragSession.m_ParentObject.Uuid != parentUUID) return;

		int pack0, pack1, pack2, pack3;
		if (!ctx.Read(pack0) || !ctx.Read(pack1) || !ctx.Read(pack2) || !ctx.Read(pack3)) return;

		int packedData[4] = {pack0, pack1, pack2, pack3};
		vector newParentPos, newParentOri;
		float newParentScale;
		EditorNetUtils.UnpackTransform(packedData, newParentPos, newParentOri, newParentScale);

		// Apply transform to parent
		m_ActiveDragSession.m_ParentObject.SetPosition(newParentPos);
		m_ActiveDragSession.m_ParentObject.SetOrientation(newParentOri);
		m_ActiveDragSession.m_ParentObject.SetScale(newParentScale);
		m_ActiveDragSession.m_ParentObject.Update(false); // Visual update only

		Print("[CLIENT-RECEIVE-UPDATE] RECEIVED UPDATE");

		// Calculate and apply transforms for all children
		vector newParentTransform[4];
		Math3D.YawPitchRollMatrix(newParentOri, newParentTransform);
		newParentTransform[3] = newParentPos;

	foreach (LocalDragChildData childData : m_ActiveDragSession.m_ChildData)
		{
			EditorObject childObject = childData.m_ChildObject;
			if (!childObject) continue;

			// 1. Reconstruct the relative transform matrix from stored offsets
			vector relativeMatrix[4];
			Math3D.YawPitchRollMatrix(childData.m_RelativeOri, relativeMatrix);
			relativeMatrix[3] = childData.m_RelativePos;

			// 2. [FIX] Create a scale matrix and apply it to the relative matrix to preserve scale
			vector scaleMatrix[3];
			Math3D.ScaleMatrix(childObject.GetScale(), scaleMatrix);
			Math3D.MatrixMultiply3(scaleMatrix, relativeMatrix, relativeMatrix);

			// 3. Calculate the final absolute transform by multiplying with the new parent transform
			vector finalChildTransform[4];
			Math3D.MatrixMultiply4(newParentTransform, relativeMatrix, finalChildTransform);

			// 4. Apply the final, complete transform to the object for visual update
			childObject.SetTransform(finalChildTransform);
			childObject.Update(false); // Visual update only
		}
	}

	/**
	 * @brief Applies the final transform and cleans up the local session context.
	 */
	private void HandleDragSession_End(ParamsReadContext ctx)
	{
		if (!m_ActiveDragSession) return;
		HandleDragSession_Update(ctx);

		// Clean up and destroy the session object.
		delete m_ActiveDragSession;
	}
}