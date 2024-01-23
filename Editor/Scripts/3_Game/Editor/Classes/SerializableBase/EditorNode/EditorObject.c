class EditorObject: EditorNode
{	
	static const int VERSION = 1;
		
	protected string m_Type;
	protected Object m_Object;
	protected EditorObjectFlags m_Flags;
		
	protected ref array<vector> m_Corners = {};
	protected ref map<Axis, ref Plane> m_BoundingBoxSurfaces = new map<Axis, ref Plane>();
	
	protected ref array<ref EditorPointView> m_PointViews = {};
	protected ref EditorObjectView m_EditorObjectView;
	
	protected Object m_BBoxLines[12], m_BBoxBase, m_CenterLine;		
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected ref array<EditorSnapPoint> m_EditorSnapPoints = {};
	
	protected Object m_TranslationGizmo;
	
	void EditorObject(string uuid, string display_name, string icon, string type, vector transform[4], int flags)
	{
		m_Type = type;
		m_Flags = flags;
		
		Math3D.MatrixOrthogonalize4(transform);		
		m_Object = Editor.CreateObject(type, transform);
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		vector clip[2];
		m_Object.ClippingInfo(clip);
		
		// Corner positions
		vector corners[8];
		corners[0] = Vector(clip[0][0], clip[0][1], clip[0][2]);
		corners[1] = Vector(clip[0][0], clip[0][1], clip[1][2]);
		corners[2] = Vector(clip[1][0], clip[0][1], clip[1][2]);
		corners[3] = Vector(clip[1][0], clip[0][1], clip[0][2]);
		
		corners[4] = Vector(clip[0][0], clip[1][1], clip[1][2]);
		corners[5] = Vector(clip[1][0], clip[1][1], clip[1][2]);
		corners[6] = Vector(clip[1][0], clip[1][1], clip[0][2]);
		corners[7] = Vector(clip[0][0], clip[1][1], clip[0][2]);		
				
		for (int i = 0; i < 6; i++) {
			bool top_or_bottom = (i == Axis.TOP || i == Axis.BOTTOM);
			int j = Ternary<int>.If(top_or_bottom, i + 2, (i + 4) % 8);
			vector plane_corners[2] = { corners[i], corners[j] };
			m_BoundingBoxSurfaces[i] = Axis.ALL[i].CreatePlane(plane_corners);

			// Idk how i feel abt this being a scripted entity
			m_EditorSnapPoints.Insert(EditorSnapPoint.Cast(GetGame().CreateObjectEx("EditorSnapPoint", m_BoundingBoxSurfaces[i].GetPosition().Multiply4(transform), ECE_LOCAL)));
		}
		
		ScriptedEntity scripted_entity = ScriptedEntity.Cast(m_Object);
		if (scripted_entity) {
			TriggerShape shape = scripted_entity.GetTriggerShape();
			switch (shape) {
				case TriggerShape.BOX: {
					scripted_entity.SetCollisionBox("2 2 2", "-2 -2 -2");
					break;
				}
			}
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
		//EditorBoundingBox.Destroy(m_Object);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
		GetGame().ObjectDelete(m_TranslationGizmo);
						
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
		Math3D.MatrixOrthogonalize4(transform);
		if (!m_Object) {	
			m_Object = Editor.CreateObject(m_Type, transform);
		}
		
		m_Object.SetTransform(transform);
		
		serializer.Read(m_Flags);
		return true;
	}
			
#ifdef DIAG_DEVELOPER	
	void DiagOnFrameUpdate(float dt)
	{		
		vector transform[4];
		m_Object.GetTransform(transform);

		for (int i = 0; i < m_BoundingBoxSurfaces.Count(); i++) {
			// Debug
			typename x = Axis;
			typename y = int;
			m_BoundingBoxSurfaces[i].Debug(x.GetVariableName(i + y.GetVariableCount()), transform);
		}
					
		/*
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
			//Shape.CreateMatrix(mat);
			//DayZPlayerUtils.DrawDebugText(j.ToString(), mat[3], 1);
		}*/
				
		ScriptedEntity scripted_entity = ScriptedEntity.Cast(m_Object);
		if (scripted_entity) {
			TriggerShape shape = scripted_entity.GetTriggerShape();
			switch (shape) {
				case TriggerShape.BOX: {
					vector min_max[2];
					scripted_entity.GetCollisionBox(min_max);
					
					vector transform_box[4];
					m_Object.GetTransform(transform_box);
					
					Shape bbox = Shape.Create(ShapeType.BBOX, COLOR_RED_A, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE | ShapeFlags.TRANSP, min_max[0], min_max[1]);
					bbox.SetMatrix(transform_box);
					break;
				}
				
				case TriggerShape.SPHERE: {
					//Debug.DrawBox(
					break;
				}
			}
		}
	}
#endif
		
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			if (((m_Flags & EditorObjectFlags.BBOX) == EditorObjectFlags.BBOX)) {
				//EditorBoundingBox.Create(m_Object);
			}
			
			m_TranslationGizmo = GetGame().CreateObjectEx("TranslationGizmo", GetTopPoint(), ECE_LOCAL);
		} else {
			//EditorBoundingBox.Destroy(m_Object);
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
		return m_BoundingBoxSurfaces[Axis.BOTTOM].GetPosition();
	}
	
	vector GetTopPoint()
	{
		return m_BoundingBoxSurfaces[Axis.TOP].GetPosition();
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