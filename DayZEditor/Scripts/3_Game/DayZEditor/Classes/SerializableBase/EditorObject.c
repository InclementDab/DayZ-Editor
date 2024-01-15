class EditorObject: SerializableBase
{	
	static const int VERSION = 1;
	
	static const ref array<vector> LINE_CENTER_DIRECTIONS = { 
		Vector(-1, 0, 0), 
		Vector(0, 0, -1), 
		Vector(-1, 0, 0), 
		Vector(0, 0, -1),
		Vector(-1, 0, 0),
		Vector(0, 0, 1),
		Vector(-1, 0, 0),
		Vector(1, 0, 0),
		Vector(1, 0, 0),
		Vector(1, 0, 0),
		Vector(1, 0, 0),
		Vector(0, 0, 1),
	};
	
	static ref array<EditorObject> SelectedObjects = {};
	
	protected UUID m_UUID;
	protected Object m_Object;
	protected EditorObjectFlags m_Flags;
	protected string m_DisplayName;
	
	// View references
	/*protected ref EditorObjectMapMarker	m_EditorObjectMapMarker;
	protected ref EditorObjectWorldMarker m_EditorObjectWorldMarker;
	protected ref EditorPlacedListItem m_EditorPlacedListItem;*/
	
	protected ref EditorTreeItem m_TreeItem;
	
	protected Object m_BBoxLines[12], m_BBoxBase, m_CenterLine;		
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected vector m_LineCenters[12], m_LineVerticies[8], m_BasePoint; 
	protected bool m_IsSelected;
	
	protected bool m_IsDirty;
	
	protected ref array<EditorSnapPoint> m_EditorSnapPoints = {};
	
	// Human Properties
	int CurrentAnimation;
	bool Animate;
	
	// Custom stuff
	string ExpansionTraderType;
	string TestingScript;
		
	ref ScriptInvoker OnObjectSelected = new ScriptInvoker();
	ref ScriptInvoker OnObjectDeselected = new ScriptInvoker();
	
	void EditorObject(UUID uuid)
	{
		m_UUID = uuid;
				
#ifdef DIAG_DEVELOPER
#ifndef SERVER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DiagOnFrameUpdate);
#endif
#endif
	}
		
	void ~EditorObject()
	{
		EditorBoundingBox.Destroy(m_Object);
		
		GetGame().ObjectDelete(m_Object);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
				
		delete m_TreeItem;
		
		foreach (auto snap_point: m_EditorSnapPoints) {
			snap_point.Delete();
		}
	}
	
#ifdef DIAG_DEVELOPER
	void DiagOnFrameUpdate(float dt)
	{
		if (!m_Object) {
			delete this;
			return;
		}
		
		vector transform[4];
		m_Object.GetTransform(transform);
		for (int i = 0; i < 8; i++) {
			vector pos = m_LineVerticies[i].Multiply4(transform);
			Debug.DrawSphere(pos, 0.1, COLOR_GREEN, ShapeFlags.ONCE);
			
		}
		
		for (int j = 0; j < 12; j++) {
			vector pos2 = m_LineCenters[j].Multiply4(transform);
			//Debug.DrawSphere(pos2, 0.1, COLOR_APPLE, ShapeFlags.ONCE);
			
			vector mat[4];
			Math3D.DirectionAndUpMatrix(transform[1], LINE_CENTER_DIRECTIONS[j], mat);
			mat[3] = pos2;
			Shape.CreateMatrix(mat);
			//DayZPlayerUtils.DrawDebugText(j.ToString(), mat[3], 1);
		}
	}
#endif
	
	static EditorObject CreateNew(notnull Object object, EditorObjectFlags flags)
	{
		EditorObject editor_object = new EditorObject(UUID.Generate());
		editor_object.m_Object = object;
		editor_object.m_Flags = flags;
		
		editor_object.m_DisplayName = editor_object.m_Object.GetType();
						
		vector clip_info[2];
		editor_object.m_Object.ClippingInfo(clip_info);
	
		editor_object.m_LineVerticies[0] = clip_info[0];
		editor_object.m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		editor_object.m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		editor_object.m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		editor_object.m_LineVerticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		editor_object.m_LineVerticies[5] = clip_info[1];
		editor_object.m_LineVerticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		editor_object.m_LineVerticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
				
		editor_object.m_LineCenters[0] = AverageVectors(editor_object.m_LineVerticies[0], editor_object.m_LineVerticies[1]);
		editor_object.m_LineCenters[1] = AverageVectors(editor_object.m_LineVerticies[0], editor_object.m_LineVerticies[3]);
		editor_object.m_LineCenters[2] = AverageVectors(editor_object.m_LineVerticies[0], editor_object.m_LineVerticies[7]);
		editor_object.m_LineCenters[3] = AverageVectors(editor_object.m_LineVerticies[4], editor_object.m_LineVerticies[7]);
		editor_object.m_LineCenters[4] = AverageVectors(editor_object.m_LineVerticies[6], editor_object.m_LineVerticies[7]);

		editor_object.m_LineCenters[5] = AverageVectors(editor_object.m_LineVerticies[1], editor_object.m_LineVerticies[2]);
		editor_object.m_LineCenters[6] = AverageVectors(editor_object.m_LineVerticies[1], editor_object.m_LineVerticies[6]);
		editor_object.m_LineCenters[7] = AverageVectors(editor_object.m_LineVerticies[3], editor_object.m_LineVerticies[2]);
		editor_object.m_LineCenters[8] = AverageVectors(editor_object.m_LineVerticies[3], editor_object.m_LineVerticies[4]);

		editor_object.m_LineCenters[9] = AverageVectors(editor_object.m_LineVerticies[5], editor_object.m_LineVerticies[2]);
		editor_object.m_LineCenters[10] = AverageVectors(editor_object.m_LineVerticies[5], editor_object.m_LineVerticies[4]);		
		editor_object.m_LineCenters[11] = AverageVectors(editor_object.m_LineVerticies[5], editor_object.m_LineVerticies[6]);
				

		editor_object.m_BasePoint = AverageVectors(AverageVectors(editor_object.m_LineVerticies[0], editor_object.m_LineVerticies[1]), AverageVectors(editor_object.m_LineVerticies[2], editor_object.m_LineVerticies[3]));
			
		vector transform[4];
		editor_object.m_Object.GetTransform(transform);
		
		for (int j = 0; j < 12; j++) {
			vector snap_point_position = editor_object.m_LineCenters[j].Multiply4(transform);
			EditorSnapPoint snap_point = EditorSnapPoint.Cast(GetGame().CreateObjectEx("EditorSnapPoint", snap_point_position, ECE_LOCAL));
			
			vector mat[4];
			Math3D.DirectionAndUpMatrix(transform[1], LINE_CENTER_DIRECTIONS[j], mat);
			mat[3] = snap_point_position;
			Shape.CreateMatrix(mat);
			//DayZPlayerUtils.DrawDebugText(j.ToString(), mat[3], 1);
									
			snap_point.SetTransform(mat);
			editor_object.m_Object.AddChild(snap_point, -1);
			
			editor_object.m_EditorSnapPoints.Insert(snap_point);
		}
		
		// Map marker		
		/*if (((editor_object.m_Flags & EditorObjectFlags.MAPMARKER) == EditorObjectFlags.MAPMARKER)) {
			editor_object.m_EditorObjectMapMarker = new EditorObjectMapMarker(editor_object);
			GetEditor().GetEditorHud().GetTemplateController().InsertMapMarker(editor_object.m_EditorObjectMapMarker);
		}
		
		// World Marker
		if (((editor_object.m_Flags & EditorObjectFlags.OBJECTMARKER) == EditorObjectFlags.OBJECTMARKER)) {
			editor_object.m_EditorObjectWorldMarker = new EditorObjectWorldMarker(editor_object);
		}
		*/
		
		// Browser item
		if (((editor_object.m_Flags & EditorObjectFlags.LISTITEM) == EditorObjectFlags.LISTITEM)) {
			editor_object.m_TreeItem = new EditorTreeItem(editor_object.GetDisplayName(), ScriptCaller.Create(editor_object.OnTreeItemSelected));
		}
				
		// Needed for AI Placement			
		EntityAI entity_ai = EntityAI.Cast(editor_object.m_Object);
		//if (entity_ai && GetEditor().GeneralSettings.SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0)) {
		//	entity_ai.OnDebugSpawn();
		//}	
		
		// Load animations
		array<string> paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
		foreach (string path: paths) {
			string config_path = path + " " + editor_object.m_Object.GetType() + " AnimationSources";
			if (GetGame().ConfigIsExisting(config_path) && entity_ai) {
				for (int k = 0; k < GetGame().ConfigGetChildrenCount(config_path); k++) {
					string child_name;
					GetGame().ConfigGetChildName(config_path, k, child_name);
					editor_object.m_ObjectAnimations[child_name] = new EditorObjectAnimationSource(entity_ai, child_name, path);
				}
			}	
		}
		
		return editor_object;
	}
	
	static EditorObject CreateNew(string type, vector transform[4], EditorObjectFlags flags)
	{
		Object object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL);
		object.SetTransform(transform);
		return CreateNew(object, flags);
	}
	
	static EditorObject CreateFromSerializer(Serializer serializer)
	{
		UUID uuid;
		if (!serializer.Read(uuid)) {
			return null;
		}
		
		EditorObject editor_object = new EditorObject(uuid);
		editor_object.Read(serializer, VERSION);
		return editor_object;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(version);
		serializer.Write(m_UUID);
		serializer.Write(m_Flags);
		serializer.Write(m_DisplayName);
		
		// Object properties
		serializer.Write(m_Object.GetType());
		vector transform[4];
		m_Object.GetTransform(transform);
		serializer.Write(transform);
		
		//if (NetworkLightBase.Cast(m_Object)) {
		//	NetworkLightBase.Cast(m_Object).Read(m_Data.Parameters);
		//}
		
		// Update Attachments
		/*EntityAI entity = EntityAI.Cast(m_Object);
		if (entity) {
			data.Attachments.Clear();
			array<EntityAI> attachments = {};
			for (int i = 0; i < entity.GetInventory().AttachmentCount(); i++) {			
				EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
				if (!attachment) {
					continue;
				}
				
				data.Attachments.Insert(attachment.GetType());
			}
		}*/
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(version);
		serializer.Read(m_UUID);
		
		serializer.Read(m_Flags);
		serializer.Read(m_DisplayName);
		
		string type;
		serializer.Read(type);
		vector transform[4];
		serializer.Read(transform);
		m_Object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL);
		m_Object.SetTransform(transform);
		
		return true;
	}
	
	EditorTreeItem GetTreeItem()
	{
		return m_TreeItem;
	}
	
	void OnTreeItemSelected(EditorTreeItem tree_item)
	{
		SetSelected(tree_item.Button.GetState());
	}
			
	bool GetGroundUnderObject(out vector position, out vector direction)
	{
		vector transform[4];
		m_Object.GetTransform(transform);
		
		int component;
		return DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, position, direction, component, null, null, null, false, true);
	}
	
	vector GetBasePoint()
	{
		vector transform[4];
		m_Object.GetTransform(transform);

		return m_BasePoint.Multiply4(transform);
	}
	
	vector GetBasePointOffset()
	{
		return m_BasePoint * -1;
	}
		
	bool OnMouseEnter(int zx, int y)	
	{
		return true;
	}
	
	bool OnMouseLeave(int x, int y) 
	{
		return true;
	}
			
	void Hide(bool state) 
	{		
		if (!state) {
			m_Object.SetFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
		} else {
			m_Object.ClearFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
		}
		
		if (state) {
			SetFlag(EditorObjectFlags.HIDDEN);
		} else {
			ClearFlag(EditorObjectFlags.HIDDEN);
		}
	}
	
	Param2<vector, vector> GetTransformArray() 
	{
		return new Param2<vector, vector>(m_Object.GetPosition(), m_Object.GetOrientation());
	}
	
	vector GetSize()
	{
		vector result;
		vector clip_info[2];
		m_Object.ClippingInfo(clip_info);
		result[0] = Math.AbsFloat(clip_info[0][0]) + Math.AbsFloat(clip_info[1][0]);
		result[1] = Math.AbsFloat(clip_info[0][1]) + Math.AbsFloat(clip_info[1][1]);
		result[2] = Math.AbsFloat(clip_info[0][2]) + Math.AbsFloat(clip_info[1][2]);
		return result;
	}
	
	void SetDisplayName(string display_name)
	{
		m_DisplayName = display_name;
	}
	
	string GetDisplayName()
	{
		return m_DisplayName;
	}
		
	void SetFlag(EditorObjectFlags flag)
	{
		m_Flags |= flag;
	}
	
	void ClearFlag(EditorObjectFlags flag)
	{
		m_Flags &= ~flag;
	}
	
	bool IsFlagEnabled(EditorObjectFlags flag)
	{
		return ((m_Flags & flag) == flag);
	}
	
	EditorObjectFlags GetFlags()
	{
		return m_Flags;
	}
		
	void SetSelected(bool selected)
	{
		m_IsSelected = selected;
		if (selected) {
			if (((m_Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX)) {
				EditorBoundingBox.Create(m_Object);
			}
			
			SelectedObjects.Insert(this);
			OnObjectSelected.Invoke(this);
		} else {
			EditorBoundingBox.Destroy(m_Object);
			SelectedObjects.RemoveItem(this);
			OnObjectDeselected.Invoke(this);
		}
	}
	
	bool IsSelected() 
	{
		return m_IsSelected;
	}
		
	map<string, ref EditorObjectAnimationSource> GetObjectAnimations()
	{
		return m_ObjectAnimations;
	}
	
	bool HasAnimations()
	{
		return (m_ObjectAnimations.Count() != 0);
	}
			
	Object GetWorldObject() 
	{		
		return m_Object;
	}
	
	UUID GetUUID()
	{
		return m_UUID;
	}
	
	array<EditorSnapPoint> GetEditorSnapPoints()
	{
		return m_EditorSnapPoints;
	}
		
	void SetSynchDirty()
	{
		m_IsDirty = true;
	}
	
	bool IsSynchDirty()
	{
		return m_IsDirty;
	}
	
	static void ClearSelections()
	{
		foreach (EditorObject selected_object: SelectedObjects) {
			if (selected_object && selected_object.IsSelected()) {
				selected_object.SetSelected(false);
			}
		}
		
		SelectedObjects.Clear();
	}
}

	// EditorObjects can also be psuedo-controllers
	/*void PropertyChanged(string property_name)
	{
		//EditorLog.Trace("EditorObject::PropertyChanged %1", property_name);
		switch (property_name) {
			case "Name": {
				SetDisplayName(Name);
				break;
			}
			
			case "Position": {
				EditorAction position_undo = new EditorAction("SetTransform", "SetTransform");
				position_undo.InsertUndoParameter(GetTransformArray());
				SetPosition(Position);
				position_undo.InsertRedoParameter(GetTransformArray());
				GetEditor().InsertAction(position_undo);
				break;
			}
			
			case "Orientation": {
				EditorAction orientation_undo = new EditorAction("SetTransform", "SetTransform");
				orientation_undo.InsertUndoParameter(GetTransformArray());
				SetOrientation(Orientation);
				orientation_undo.InsertRedoParameter(GetTransformArray());
				GetEditor().InsertAction(orientation_undo);
				break;
			}
			
			case "Scale": {
				if (Scale < 0.000001) {
					Scale = 0.000001;
				}
								
				SetScale(Scale);
				break;
			}
			
			case "Show": {
				Show(Show);
				break;
			}
			
			case "Locked": {
				Lock(Locked);
				break;
			}
			
			case "Physics": {
				EnablePhysics(Physics);
				break;
			}
						
			case "Simulate": {
				EntityAI ai = EntityAI.Cast(m_Object);
				if (ai) {
					ai.DisableSimulation(!Simulate);
				}
				break;
			}
			
			case "Animate": {
				PlayerBase emote_player = PlayerBase.Cast(m_Object);
				if (emote_player) {
					emote_player.GetEmoteManager().PlayEmote(CurrentAnimation);
				}
				
				break;
			}
						
			case "EditorOnly": {
				//m_Data.EditorOnly = EditorOnly;
				break;
			}
			
			case "AllowDamage": {
				m_Object.SetAllowDamage(AllowDamage);
				break;
			}
			
			case "Health": {
				if (m_Object.HasDamageSystem()) {
					Health = Math.Clamp(Health, 0, 100);
					m_Object.SetHealth("", "Health", Health);
				}
				break;
			}
			
			case "Collision": {
				if (Collision) {
					m_Object.SetFlags(EntityFlags.SOLID, true);
				} else {
					m_Object.ClearFlags(EntityFlags.SOLID, true);
				}
				
				m_Object.Update();
				break;
			}
			
			case "ExpansionTraderType": {
				// storing the custom data				
				m_Data.Parameters["ExpansionTraderType"] = SerializableParam1<string>.Create(ExpansionTraderType);
				break;
			}
		}
			
		Update();
	}

	// pass in `this` in the context of the WorldObject
	void ExecuteCode(string script_content = string.Empty)
	{
		Print(TestingScript);
		if (script_content == string.Empty) {
			script_content = TestingScript;
		}
		
		if (script_content == string.Empty) {
			EditorLog.Debug("No content to execute");
			return;
		}
		
		script_content.Replace("this", "world_object");
		
		string sanitized_content;
		for (int i = 0; i < script_content.Length(); i++) {
			if (script_content[i] == "\n") {
				continue;
			}
			
			sanitized_content += script_content[i];
		}
		
		string file_name = "$saves:_.c";
		FileHandle handle = OpenFile(file_name, FileMode.WRITE);
		string file_data = "static void main(Object world_object)\n{\n" + sanitized_content + "\n}";
		FPrintln(handle, file_data);		
		
		if (handle) {
			CloseFile(handle);
		}
		
		ScriptModule script_module = ScriptModule.LoadScript(GetGame().GetMission().MissionScript, file_name, true);
		if (!script_module) {
			MessageBox.Show("Error", "Invalid Syntax in Script Editor", MessageBoxButtons.OK);
			return;
		}
		
		script_module.CallFunction(null, "main", null, m_Object);
		
		DeleteFile(file_name);	
	}
*/