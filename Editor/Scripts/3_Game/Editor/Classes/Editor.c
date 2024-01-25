/*
                    Lloyd sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/



class EditorColors
{
	private void EditorColors();
	private void ~EditorColors();
	
	static const int SELECT = ARGB(255, 7, 111, 146);
}

class Editor: EditorServer
{
	protected PlayerIdentity m_Identity;
	protected DayZPlayer m_Player;
	
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;
	ref EditorCursorTool Tool;

	ref array<ref EditorObject> Placing = {};
	
	protected vector m_CursorNormal = vector.Aside;
	
	// Stack of Undo / Redo Actions
	protected ref EditorHistory m_History = new EditorHistory();
	
	void Editor(string uuid, string display_name, Symbols icon, PlayerIdentity identity, DayZPlayer player) 
	{
		m_Identity = identity;
		m_Player = player;
			
		m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + "0 10 0", ECE_LOCAL));	
		m_Camera.SetActive(true);
	
		m_Hud = new EditorHud();		
		m_Hud.GetTemplateController().LeftListItems.Insert(this["PlaceableObjects"].GetNodeView());
		m_Hud.GetTemplateController().LeftListItems.Insert(this["Brushes"].GetNodeView());
		m_Hud.GetTemplateController().RightListItems.Insert(this["NetworkedObjects"].GetNodeView());
		m_Hud.GetTemplateController().RightListItems.Insert(this["EditedObjects"].GetNodeView());
	}

	void ~Editor() 
	{
		GetGame().ObjectDelete(m_Camera);
	}
		
	void Update(bool doSim, float timeslice)
	{
		Input input = GetGame().GetInput();
		Raycast raycast = m_Camera.PerformCursorRaycast();		
		
		if (Tool && !Tool.Update(timeslice, raycast)) {
			return; // MAYBE DDONT DO THIS HERE
		}
		
		vector camera_orthogonal[4] = { raycast.Source.Direction * raycast.Bounce.Direction, raycast.Bounce.Direction, raycast.Source.Direction, raycast.Source.Position };
		Math3D.MatrixOrthogonalize4(camera_orthogonal);	
		
		vector rotation_mat[3];
		Math3D.MatrixIdentity3(rotation_mat);
		if (input.LocalPress_ID(UAZoomInOptics)) {
			Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
		}
		
		if (input.LocalPress_ID(UAZoomOutOptics)) {
			Math3D.YawPitchRollMatrix(Vector(15, 0, 0), rotation_mat);
		}
		
		Math3D.MatrixMultiply3(camera_orthogonal, rotation_mat, camera_orthogonal);
		
		//Shape.CreateMatrix(camera_orthogonal);
		
		m_CursorNormal = m_CursorNormal.Multiply3(rotation_mat);
				
		//Print(m_CursorNormal);
		//ray1.Debug();
		
		//Print(Placing.Count());
		foreach (EditorObject editor_object_placing: Placing) {
			vector transform[4] = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
			editor_object_placing.SetBaseTransform(transform);
		}
		
		if (input.LocalPress_ID(UATempRaiseWeapon)) {
			EditorNode.ClearSelections();
			return;
		}
		
		if (input.LocalPress_ID(UAFire)) {
			//Print(EditorObject.ByObject[raycast.Hit]);
			// The magic copy-paste code that handles all your interactive dreams. hasnt changed
			if (!KeyState(KeyCode.KC_LSHIFT) && !GetWidgetUnderCursor() && KeyState(KeyCode.KC_LMENU)) {
				EditorNode.ClearSelections();
			}
			
			if (raycast.Hit && EditorObject.ByObject[raycast.Hit]) {
				EditorObject editor_object = EditorObject.ByObject[raycast.Hit];
				//Print(editor_object);
				
				if (KeyState(KeyCode.KC_LCONTROL)) {
					editor_object.SetSelected(!editor_object.IsSelected());
				} else {
					editor_object.SetSelected(true);
				}
			}
			
			
			// Cursed but we ship it
			if (!GetWidgetUnderCursor() || !GetWidgetUnderCursor().GetName().Contains("Panel")) {
				foreach (EditorObject editor_object_to_place: Placing) {
					EditorFootprint footprint = new EditorFootprint();
					int tree_depth = editor_object_to_place.GetParentDepth();
					footprint.Data.GetWriteContext().Write(tree_depth);
					for (int i = tree_depth - 1; i >= 0; i--) {
						EditorNode parent = editor_object_to_place.GetParentAtDepth(i);
						footprint.Data.GetWriteContext().Write(parent.GetUUID());
						footprint.Data.GetWriteContext().Write(parent.Type().ToString());
					}

					editor_object_to_place.Write(footprint.Data.GetWriteContext(), 0);
					
					this["EditedObjects"]["PlacedObjects"].Add(editor_object_to_place);
		
					// Synchronize to this id
					this["EditedObjects"]["PlacedObjects"].Synchronize();
					
					// remove it from placing
					Placing.RemoveItem(editor_object_to_place);
					PlaySound(EditorSounds.PLOP);
					m_History.InsertAction(footprint);
				}
			}
		}
		
		if (input.LocalHold_ID(UAFire)) {
			foreach (EditorNode selected_node: EditorNode.SelectedObjects) {
				EditorObject editor_object_cast = EditorObject.Cast(selected_node);
				if (!editor_object_cast) {
					continue;
				}
								
				vector current_transform[4];
				editor_object_cast.GetBaseTransform(current_transform);
				
				// Held distance placing
				if (KeyState(KeyCode.KC_LMENU)) {
					Debug.DrawSphere(raycast.Source.Position, vector.Distance(raycast.Source.Position, current_transform[3]), COLOR_RED, ShapeFlags.ADDITIVE | ShapeFlags.WIREFRAME | ShapeFlags.ONCE);
					
					vector v3 = current_transform[1] * raycast.Source.Direction;
					if (KeyState(KeyCode.KC_LSHIFT)) {
						v3 = vector.Aside * raycast.Source.Direction;
					}
					
					float dist_z = vector.Dot(((raycast.Source.Position - current_transform[3]) * current_transform[1]), v3) / v3.LengthSq();
					if (KeyState(KeyCode.KC_LSHIFT)) {
						dist_z = vector.Dot(((raycast.Source.Position - current_transform[3]) * vector.Aside), v3) / v3.LengthSq();
					}
					
					float d1 = vector.Dot(vector.Up, raycast.Source.Direction);
					vector x = Math.Cos(d1) * (raycast.Source.Position - current_transform[3]);
					Print(x);
					
					vector pos = raycast.Source.Position + raycast.Source.Direction * dist_z;
					
					current_transform = { current_transform[0], current_transform[1], current_transform[2], pos };
					editor_object_cast.SetBaseTransform(current_transform);
				} 
				

				if (KeyState(KeyCode.KC_LSHIFT)) {
					
					vector new_forward = vector.Direction(current_transform[3], raycast.Bounce.Position).Normalized();
					
					current_transform = { (current_transform[1] * new_forward).Normalized(), current_transform[1], new_forward, current_transform[3] };
					
					editor_object_cast.SetBaseTransform(current_transform);
					
					/*
					vector p1 = Vector(2, 0, 2).Multiply4(current_transform);
					vector p2 = Vector(-2, 0, -2).Multiply4(current_transform);
					
					Shape.Create(ShapeType.BBOX, COLOR_GREEN, ShapeFlags.WIREFRAME | ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, p1, p2);
					
					float d = 1;
					float t = -(vector.Dot(vector.Up, raycast.Source.Position) + current_transform[3][1]) / vector.Dot(vector.Up, raycast.Source.Direction);
					
					vector result = raycast.Source.Position + raycast.Source.Direction * t;
					
					*/

					
					
				}
				
				
				// Any distance placing
				else {
					transform = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
					editor_object_cast.SetBaseTransform(transform);
				}
			}
		}
		
		if (input.LocalPress_ID(UAZoomIn)) {
			m_Camera.FieldOfView = GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM;
		}
		
		if (input.LocalRelease_ID(UAZoomIn)) { 
			m_Camera.FieldOfView = 1.0;
		}

		if (GetGame().GetInput().LocalPress("EditorDeleteObject")) {
			foreach (EditorNode editor_node_to_delete: EditorNode.SelectedObjects) {
				if (!editor_node_to_delete.CanDelete()) {
					continue;
				}
				
				EditorFootprint footprint_delete = new EditorFootprint();
				int delete_tree_depth = editor_node_to_delete.GetParentDepth();
				footprint_delete.Data.GetWriteContext().Write(delete_tree_depth);
				for (int i_d = delete_tree_depth - 1; i_d >= 0; i_d--) {
					EditorNode delete_parent = editor_node_to_delete.GetParentAtDepth(i_d);
					footprint_delete.Data.GetWriteContext().Write(delete_parent.GetUUID());
					footprint_delete.Data.GetWriteContext().Write(delete_parent.Type().ToString());
				}

				editor_node_to_delete.Write(footprint_delete.Data.GetWriteContext(), 0);
				
				this["EditedObjects"]["PlacedObjects"].Remove(editor_node_to_delete.GetUUID());
	
				// Synchronize to this id
				this["EditedObjects"]["PlacedObjects"].Synchronize();
				
				// remove it from placing
				EditorNode.SelectedObjects.RemoveItem(editor_node_to_delete);
				
				EditorObject editor_object_to_delete = EditorObject.Cast(editor_node_to_delete);
				if (editor_object_to_delete) {
					GetGame().ObjectDelete(editor_object_to_delete.GetObject());
				}
				
				
				delete editor_node_to_delete;
				PlaySound(EditorSounds.HIGHLIGHT); // must co-exist
				
				
				m_History.InsertAction(footprint_delete);
			}
		}
		
		if (input.LocalPress("EditorToggleCursor")) {
			GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
		}
		
		if (input.LocalPress("EditorToggleHudCommand")) {
			if (m_Camera.IsActive()) {
				m_Hud.Show(!m_Hud.IsVisible());
			}
		}
		
		if (input.LocalPress("EditorToggleActive")) {
			if (!m_Camera.IsActive()) {
				m_Camera.SetActive(true);
				m_Hud.Show(true);
			} else {
				m_Hud.Show(false);
				
				GetDayZGame().SelectPlayer(m_Identity, m_Player);
				Hud hud = GetDayZGame().GetMission().GetHud();
				hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
				hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
			}
		}
	}
			
	override void Write(Serializer serializer, int version)
	{		
		serializer.Write(m_Identity);
		
		vector camera_transform[4];
		m_Camera.GetTransform(camera_transform);	
		serializer.Write(camera_transform);
		
		super.Write(serializer, version);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		serializer.Read(m_Identity);
		
		vector camera_transform[4];
		serializer.Read(camera_transform);
		
		return super.Read(serializer, version);
	}
						
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
	}
				
	void Undo()
	{
		foreach (EditorFootprint footprint: m_History) {
			if (!footprint.IsUndone()) {
				footprint.Undo();
				return;
			}
		}
	}
	
	void Redo()
	{
		for (int i = m_History.Count() - 1; i >= 0; i--) {
			if (m_History[i] && m_History[i].IsUndone()) {
				m_History[i].Redo();
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (EditorFootprint footprint: m_History) {
			if (footprint && !footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = m_History.Count() - 1; i >= 0; i--) {
			if (m_History[i] && m_History[i].IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
			
	static Man CreateDefaultCharacter(string type, vector position)
	{
		if (GetGame().GetPlayer()) {
			return GetGame().GetPlayer();
		} 

		Man player = Man.Cast(GetGame().CreatePlayer(null, type, position, 0, string.Empty));
		if (!player) {
			Error(string.Format("Failed to create new player, type %1", type));
			return null;
		}

		player.SetPosition(position);
		player.Update();
		if (GetCurrentHoliday() == EditorHoliday.CHRISTMAS) {
			player.GetInventory().CreateAttachment("SantasHat");
			player.GetInventory().CreateAttachment("SantasBeard");
		} else {
			player.GetInventory().CreateInInventory("ConstructionHelmet_Yellow");
		}
		
    	player.GetInventory().CreateInInventory("Shirt_RedCheck");
    	player.GetInventory().CreateInInventory("Jeans_Blue");
    	player.GetInventory().CreateInInventory("WorkingBoots_Brown");
    	player.GetInventory().CreateInInventory("CivilianBelt");
    	player.GetInventory().CreateInInventory("TaloonBag_Blue");
    	player.GetInventory().CreateInInventory("SledgeHammer");
	    return player;
	}
			
	// Just annoying
	static string GetWorldName()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		return world_name;
	}
		
	static vector GetMapCenterPosition()
	{
		TIntArray values();
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", GetWorldName()), values);
				
		// they were playing the wrong game when they thought of this one
		return Vector(values[0], values[2], values[1]);
	}
	
	static vector GenerateSafeStartPosition(float radius = 2000.0)
	{
		vector position = GetMapCenterPosition();
		position[0] = Math.RandomFloat(position[0] - radius, position[0] + radius);
		position[2] = Math.RandomFloat(position[2] - radius, position[2] + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 25.0;		
		return position;
	}
		
	static Object GetObjectUnderCursor(float raycast_distance = 3000)
	{
		vector ray_start = GetGame().GetCurrentCameraPosition();
		vector ray_end = ray_start + GetGame().GetPointerDirection() * raycast_distance;
		
		vector hitPos, hitNormal;
		int hitComponentIndex;		
		set<Object> collisions = new set<Object>;
		
	
		DayZPhysics.RaycastRV(ray_start, ray_end, hitPos, hitNormal, hitComponentIndex, collisions);
		
		return collisions.Get(0);
	}
	
	static EditorHoliday GetCurrentHoliday()
	{		
		int year, month, day;
		GetYearMonthDayUTC(year, month, day);
		if (month == 12 && day > 12) {
			return EditorHoliday.CHRISTMAS;
		}
		
		if (month == 1 && day < 14) {
			return EditorHoliday.NEWYEARS;
		}
		
		if (month == 10 && day > 0 && day < 14) {
			return EditorHoliday.ANNIVERSARY;
		}
		
		if (month == 10 && day > 14) {
			return EditorHoliday.HALLOWEEN;
		}
		
		return EditorHoliday.NONE;
	}
	
	static Object CreateObject(string type, vector transform[4])
	{
		Math3D.MatrixOrthogonalize4(transform);
		
		Object object;
		if (type.Contains("\\") || type.Contains("/")) {
			object = GetGame().CreateStaticObjectUsingP3D(type, transform[3], transform[2].VectorToAngles(), 1.0, true);
		} else {
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL);
		}
		
		object.SetTransform(transform);
		object.Update();
		
		return object;
	}
	
	static int GetAutoSaveValue(float x)
	{
		return (5 * Math.Pow(x, 4) / 8) - (5 * Math.Pow(x, 3) / 12) - (45 * Math.Pow(x, 2) / 8) + (545 * x / 12) - 25;
	}
			
	/*
	void LoadSaveData(notnull EditorSaveData save_data, bool clear_before = false)
	{		
		int created_objects, deleted_objects;
		if (save_data.MapName != string.Empty && save_data.MapName != GetGame().GetWorldName()) {
			EditorLog.Warning("Different map detected");
			/*if (MessageBox.Show("Different Map Detected", string.Format("Switch map to %1?"), MessageBoxButtons.OKCancel) != DialogResult.OK) {
				return null;
			}
			
			EditorLog.Info("Loading Map %1", save_data.MapName);
			g_Game.ReportProgress(string.Format("Loading Map %1", save_data.MapName));
			
			GetGame().PlayMission(CreateEditorMission(save_data.MapName));
			
			m_Editor = GetDayZGame().GetEditor();
			
		}
		
		if (clear_before) {
			GetDayZGame().GetEditor().Clear();
		}
				
		EditorLog.Debug("Deleting %1 Objects", save_data.EditorHiddenObjects.Count().ToString());		
		foreach (EditorHiddenObjectData id: save_data.EditorHiddenObjects) {
			if (HideMapObject(id.FindObject(), false)) {
				deleted_objects++;
			}
		}
		
		EditorLog.Debug("Creating %1 Objects", save_data.EditorObjects.Count().ToString());
		foreach (EditorObjectData data: save_data.EditorObjects) {
			//if (CreateObject(data.CreateObject(), false)) {
			//	created_objects++;
			//}			
		}
		
		if (save_data.CameraPosition != vector.Zero) {
			GetCamera().SetPosition(save_data.CameraPosition);
		}
		
		string error_message;
		if (created_objects < save_data.EditorObjects.Count()) {
			error_message += string.Format("Failed to load %1 objects", save_data.EditorObjects.Count() - created_objects);
		}
		
		if (deleted_objects < save_data.EditorHiddenObjects.Count()) {
			if (error_message != string.Empty) {
				error_message += "	";
			}
			
			error_message += string.Format("Failed to delete %1 objects", save_data.EditorHiddenObjects.Count() - deleted_objects);
		}
		
		if (error_message != string.Empty) {
			EditorLog.Warning(error_message);
			m_Hud.ShowNotification(error_message, COLOR_YELLOW);
			
			// Disable auto save since we loaded a shit file
			//Settings.AutoSaveTimer = -1;
		} else {
			m_Hud.ShowNotification(string.Format("Loaded %1 objects! (%2 deletions)", save_data.EditorObjects.Count(), save_data.EditorHiddenObjects.Count()), COLOR_GREEN);
		}
	}
	
	EditorSaveData CreateSaveData(bool selected_only = false)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		save_data.CameraPosition = GetCamera().GetPosition();
		
		// Save Objects
		array<EditorObject> placed_objects = GetPlacedObjects();
		if (selected_only) {
			placed_objects.Clear();
			array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
			foreach (auto selected_object: selected_objects) {
				placed_objects.Insert(selected_object);
			}
		}
		
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				//save_data.EditorObjects.Insert(editor_object.CreateSerializedData());
			}
		}
		
		foreach (EditorHiddenObject hidden_object: m_DeletedObjects) {
			save_data.EditorHiddenObjects.Insert(hidden_object.CreateSerializedData());
		}
		
		return save_data;
	}*/
	

/*
	static EditorPlaceableObjectData GetReplaceableItem(notnull Object object)
	{		
		while (object.GetParent()) {
			object = Object.Cast(object.GetParent());
		}
		
		if (object.GetType() != string.Empty && !object.IsTree() && !object.IsBush() && !object.IsRock()) {			
			return GetDayZGame().GetPlaceableObject(object.GetType());
		}
		
		// 1346854: tank_small_white.p3d
		string debug_name = object.GetDebugNameNative();
		if (debug_name == string.Empty) {
			// lost cause, unlikely
			return null;
		}
		
		array<string> split_string = {};
		debug_name.Split(":", split_string);
		
		// also unlikely
		if (split_string.Count() == 1) {
			return null;
		}
		
		array<EditorPlaceableObjectData> placeable_items = GetDayZGame().GetReplaceableObjects(split_string[1].Trim());
		// not ideal since we dont want to feed them the p3d, but doable
		if (!placeable_items || placeable_items.Count() == 0) {			
			return null;
		}
		
		return placeable_items[0]; // better way to do other than index 0?
	}*/
				
	EditorHud GetHud() 
	{
		return m_Hud;
	}
		
	EditorCamera GetCamera() 
	{
		return m_Camera;
	}
					
	bool IsPlacing()
	{
		return Placing.Count() > 0; 
	}
	
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
		
	void SetPlayer(DayZPlayer player)
	{
		m_Player = player;
	}
	
	bool IsActive()
	{
		return m_Camera && m_Camera.IsActive();
	}
}