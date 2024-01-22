class EditorObject: EditorNode
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
		
	protected Object m_Object;
	protected string m_Type;
	protected vector m_Transform[4];
	protected EditorObjectFlags m_Flags;
		
	protected ref array<ref EditorPointView> m_PointViews = {};
	protected ref EditorObjectView m_EditorObjectView;
	
	protected Object m_BBoxLines[12], m_BBoxBase, m_CenterLine;		
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected vector m_LineCenters[12], m_LineVerticies[8], m_BasePoint, m_TopPoint; 
		
	protected ref array<EditorSnapPoint> m_EditorSnapPoints = {};
	
	protected Object m_TranslationGizmo;
	
	void EditorObject(string uuid, string display_name, string icon, string type, int flags)
	{
		m_Type = type;
		m_Flags = flags;
		
		if (GetGame().IsDedicatedServer()) {
			return;
		}
								
		vector clip_info[2];
		m_Object.ClippingInfo(clip_info);
	
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		m_LineVerticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		m_LineVerticies[5] = clip_info[1];
		m_LineVerticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		m_LineVerticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
		
		m_LineCenters[0] = AverageVectors(m_LineVerticies[0], m_LineVerticies[1]);
		m_LineCenters[1] = AverageVectors(m_LineVerticies[0], m_LineVerticies[3]);
		m_LineCenters[2] = AverageVectors(m_LineVerticies[0], m_LineVerticies[7]);
		m_LineCenters[3] = AverageVectors(m_LineVerticies[4], m_LineVerticies[7]);
		m_LineCenters[4] = AverageVectors(m_LineVerticies[6], m_LineVerticies[7]);

		m_LineCenters[5] = AverageVectors(m_LineVerticies[1], m_LineVerticies[2]);
		m_LineCenters[6] = AverageVectors(m_LineVerticies[1], m_LineVerticies[6]);
		m_LineCenters[7] = AverageVectors(m_LineVerticies[3], m_LineVerticies[2]);
		m_LineCenters[8] = AverageVectors(m_LineVerticies[3], m_LineVerticies[4]);

		m_LineCenters[9] = AverageVectors(m_LineVerticies[5], m_LineVerticies[2]);
		m_LineCenters[10] = AverageVectors(m_LineVerticies[5], m_LineVerticies[4]);		
		m_LineCenters[11] = AverageVectors(m_LineVerticies[5], m_LineVerticies[6]);
		
		m_BasePoint = AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3]));
		m_PointViews.Insert(new EditorPointView(this, m_BasePoint, 1000));
		
		m_TopPoint = AverageVectors(m_LineVerticies[0], m_LineVerticies[2]);
		
		for (int i = 0; i < 8; i++) {
			m_PointViews.Insert(new EditorPointView(this, m_LineVerticies[i], 30));
		}
		
		vector transform[4];
		m_Object.GetTransform(transform);	

		for (int j = 0; j < 12; j++) {
			vector snap_point_position = m_LineCenters[j].Multiply4(transform);
			EditorSnapPoint snap_point = EditorSnapPoint.Cast(GetGame().CreateObjectEx("EditorSnapPoint", snap_point_position, ECE_LOCAL));
			
			vector mat[4];
			Math3D.DirectionAndUpMatrix(transform[1], LINE_CENTER_DIRECTIONS[j], mat);
			mat[3] = snap_point_position;
			Shape.CreateMatrix(mat);
			//DayZPlayerUtils.DrawDebugText(j.ToString(), mat[3], 1);
									
			snap_point.SetTransform(mat);
			m_Object.AddChild(snap_point, -1);
			
			m_EditorSnapPoints.Insert(snap_point);
		}
								
		// Needed for AI Placement			
		
		//if (entity_ai && GetDayZGame().GetEditor().GeneralSettings.SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0)) {
		//	entity_ai.OnDebugSpawn();
		//}	
		
		m_EditorObjectView = new EditorObjectView(this);
		
		// Load animations
		EntityAI entity_ai = EntityAI.Cast(m_Object);
		array<string> paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
		foreach (string path: paths) {
			string config_path = path + " " + m_Object.GetType() + " AnimationSources";
			if (GetGame().ConfigIsExisting(config_path) && entity_ai) {
				for (int k = 0; k < GetGame().ConfigGetChildrenCount(config_path); k++) {
					string child_name;
					GetGame().ConfigGetChildName(config_path, k, child_name);
					m_ObjectAnimations[child_name] = new EditorObjectAnimationSource(entity_ai, child_name, path);
				}
			}	
		}
				
#ifdef DIAG_DEVELOPER
#ifndef SERVER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DiagOnFrameUpdate);
#endif
#endif
	}
		
	void ~EditorObject()
	{
		EditorBoundingBox.Destroy(m_Object);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
						
		delete m_PointViews;
		delete m_ObjectAnimations;
		
		foreach (auto snap_point: m_EditorSnapPoints) {
			snap_point.Delete();
		}
	}
	
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
		
		serializer.Write(m_Type);
		
		vector transform[4];
		m_Object.GetTransform(transform);
		serializer.Write(transform);
		
		serializer.Write(m_Flags);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(m_Type);
		
		vector transform[4];
		serializer.Read(transform);

		if (!m_Object) {
			m_Object = Editor.CreateObject(m_Type, transform);
		}
		
		serializer.Read(m_Flags);
		return true;
	}
			
#ifdef DIAG_DEVELOPER
	void DiagOnFrameUpdate(float dt)
	{		
		vector transform[4];
		m_Object.GetTransform(transform);
		for (int i = 0; i < 8; i++) {
			vector pos = m_LineVerticies[i].Multiply4(transform);
			
			//Debug.DrawSphere(pos, 0.1, COLOR_GREEN, ShapeFlags.ONCE);
		}
		
		for (int j = 0; j < 12; j++) {
			vector pos2 = m_LineCenters[j].Multiply4(transform);
			//Debug.DrawSphere(pos2, 0.1, COLOR_APPLE, ShapeFlags.ONCE);
			
			vector mat[4];
			Math3D.DirectionAndUpMatrix(transform[1], LINE_CENTER_DIRECTIONS[j], mat);
			Math3D.MatrixMultiply3(transform, mat, mat);
			mat[3] = pos2;
			Shape.CreateMatrix(mat);
			//DayZPlayerUtils.DrawDebugText(j.ToString(), mat[3], 1);
		}
	}
#endif
		
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			if (((m_Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX)) {
				EditorBoundingBox.Create(m_Object);
			}
			
			m_TranslationGizmo = GetGame().CreateObjectEx("TranslationGizmo", GetTopPoint(), ECE_LOCAL);
		} else {
			EditorBoundingBox.Destroy(m_Object);
		}
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
	
	vector GetTopPoint()
	{
		vector transform[4];
		m_Object.GetTransform(transform);

		return m_TopPoint.Multiply4(transform);
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
			
	map<string, ref EditorObjectAnimationSource> GetObjectAnimations()
	{
		return m_ObjectAnimations;
	}
	
	bool HasAnimations()
	{
		return (m_ObjectAnimations.Count() != 0);
	}
			
	Object GetObject() 
	{		
		return m_Object;
	}
		
	array<EditorSnapPoint> GetEditorSnapPoints()
	{
		return m_EditorSnapPoints;
	}
	
	static array<EditorObject> GetSelectedEditorObjects()
	{
		array<EditorObject> editor_objects = {};
		foreach (EditorNode selectable_base: SelectedObjects) {
			EditorObject editor_object = EditorObject.Cast(selectable_base);
			if (editor_object) {
				editor_objects.Insert(editor_object);
			}
		}
		
		return editor_objects;
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
				GetDayZGame().GetEditor().InsertAction(position_undo);
				break;
			}
			
			case "Orientation": {
				EditorAction orientation_undo = new EditorAction("SetTransform", "SetTransform");
				orientation_undo.InsertUndoParameter(GetTransformArray());
				SetOrientation(Orientation);
				orientation_undo.InsertRedoParameter(GetTransformArray());
				GetDayZGame().GetEditor().InsertAction(orientation_undo);
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
				Man emote_player = Man.Cast(m_Object);
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