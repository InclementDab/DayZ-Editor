class EditorObject: EditorNode
{	
	static ref map<Object, EditorObject> ByObject = new map<Object, EditorObject>();
	
	static const int VERSION = 1;
		
	protected Object m_Object;
	protected EditorObjectFlags m_Flags;
		
	protected ref array<vector> m_Corners = {};
	protected ref map<ETransformationAxis, ref Plane> m_BoundingBoxSurfaces = new map<ETransformationAxis, ref Plane>();
	protected ref map<ETransformationAxis, EditorSnapPoint> m_SnapFaces = new map<ETransformationAxis, EditorSnapPoint>();
	
	protected ref array<ref EditorPointView> m_PointViews = {};
	protected ref EditorObjectView m_EditorObjectView;
	
	protected Object m_BBoxLines[12], m_BBoxBase, m_CenterLine;		
	protected ref map<string, ref EditorObjectAnimationSource> m_ObjectAnimations = new map<string, ref EditorObjectAnimationSource>();
	
	protected ref array<EditorSnapPoint> m_EditorSnapPoints = {};
	
	protected Object m_TranslationGizmo;
	
	void EditorObject(string uuid, string display_name, Symbols icon, Object object, int flags = EFE_DEFAULT)
	{
		m_Flags = flags;
		m_Object = object;
				
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		vector transform[4];
		m_Object.GetTransform(transform);
		
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
				
		for (ETransformationAxis i = 0; i < 6; i++) {
			switch (i) {
				case ETransformationAxis.BOTTOM: {
					m_BoundingBoxSurfaces[i] = new Plane(corners[0], corners[2], vector.Up, vector.Aside);
					break;
				}				
				case ETransformationAxis.LEFT: { 
					m_BoundingBoxSurfaces[i] = new Plane(corners[0], corners[4], vector.Aside * -1, vector.Up);
					break;
				}
				case ETransformationAxis.BACK: { 
					m_BoundingBoxSurfaces[i] = new Plane(corners[3], corners[7], vector.Forward * -1, vector.Up);
					break;
				}
				case ETransformationAxis.TOP: {
					m_BoundingBoxSurfaces[i] = new Plane(corners[4], corners[6], vector.Up, vector.Aside);
					break;
				}
				case ETransformationAxis.RIGHT: {
					m_BoundingBoxSurfaces[i] = new Plane(corners[2], corners[6], vector.Aside, vector.Up);
					break;
				}				
				case ETransformationAxis.FRONT: { 
					m_BoundingBoxSurfaces[i] = new Plane(corners[1], corners[5], vector.Forward, vector.Up);
					break;
				}
			}		
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
									
		foreach (ETransformationAxis axis, Plane plane: m_BoundingBoxSurfaces) {
			vector plane_matrix[4];
			plane.CreateMatrix(plane_matrix);
			Math3D.MatrixMultiply4(plane_matrix, transform, plane_matrix);
			EditorSnapPoint snap_point = EditorSnapPoint.Cast(GetGame().CreateObjectEx("EditorSnapPoint", plane_matrix[3], ECE_LOCAL));
			snap_point.SetTransform(plane_matrix);
			m_Object.AddChild(snap_point, -1);
			m_SnapFaces[axis] = snap_point;
		}
							
		m_EditorObjectView = new EditorObjectView(this);
		
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
	
#ifdef DIAG_DEVELOPER	
	void DiagOnFrameUpdate(float dt)
	{		
		vector transform[4];
		m_Object.GetTransform(transform);
		
		if (m_TranslationGizmo) {
			vector mat[4];
			m_BoundingBoxSurfaces[ETransformationAxis.TOP].CreateMatrix(mat);
			Math3D.MatrixMultiply4(transform, mat, mat);
			m_TranslationGizmo.SetTransform(mat);
		}
		
				
		for (int i = 0; i < 6; i++) {
			// Debug
			//m_BoundingBoxSurfaces[i].Debug(typename.EnumToString(ETransformationAxis, i) + i.ToString(), transform);	
		}
		
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
	
	bool IsPlacing()
	{
		return GetDayZGame().GetEditor().Placing.Find(this) != -1;
	}
	
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
		
		string type = m_Object.GetType();
		if (type == string.Empty) {
			type = m_Object.GetShapeName();
		}
		
		serializer.Write(type);
				
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
		
		string type;
		serializer.Read(type);
		
		vector transform[4];
		serializer.Read(transform);
		if (!m_Object) {	
			m_Object = Editor.CreateObject(type, transform);
			if (m_Object) ByObject[m_Object] = this;
		}
				
		serializer.Read(m_Flags);
		return true;
	}
			
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
			
			GetGame().ObjectDelete(m_TranslationGizmo);
		}
	}
			
	bool GetGroundUnderObject(out vector position, out vector direction)
	{
		vector transform[4];
		m_Object.GetTransform(transform);
		
		int component;
		return DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, position, direction, component, null, null, null, false, true);
	}
		
	void SetBaseTransform(vector mat[4])
	{		
		vector matrix[4];		
		m_BoundingBoxSurfaces[ETransformationAxis.BOTTOM].CreateMatrix(matrix);
		
		Math3D.MatrixInvMultiply4(matrix, mat, matrix);
		m_Object.SetTransform(matrix);		
	}
	
	void GetBaseTransform(out vector mat[4])
	{
		vector matrix[4];		
		m_BoundingBoxSurfaces[ETransformationAxis.BOTTOM].CreateMatrix(matrix);
		
		m_Object.GetTransform(mat);	
		Math3D.MatrixMultiply4(mat, matrix, mat);
	}
	
	vector GetBasePoint(bool world_coords = true)
	{
		if (world_coords) {
			vector transform[4];
			m_Object.GetTransform(transform);
			return m_BoundingBoxSurfaces[ETransformationAxis.BOTTOM].GetPosition().Multiply4(transform);
		}
		
		return m_BoundingBoxSurfaces[ETransformationAxis.BOTTOM].GetPosition();
	}
	
	vector GetTopPoint(bool world_coords = true)
	{
		if (world_coords) {
			vector transform[4];
			m_Object.GetTransform(transform);
			return m_BoundingBoxSurfaces[ETransformationAxis.TOP].GetPosition().Multiply4(transform);
		}
		
		return m_BoundingBoxSurfaces[ETransformationAxis.TOP].GetPosition();
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