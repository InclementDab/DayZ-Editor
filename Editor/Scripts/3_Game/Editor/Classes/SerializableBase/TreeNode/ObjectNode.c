class ObjectNode: TreeNode
{
	static const int VERSION = 1;
	static ref map<Object, ObjectNode> All = new map<Object, ObjectNode>();
		
	protected Object m_Object;		
	protected ref array<vector> m_Corners = {};
	protected ref map<ETransformationAxis, ref Plane> m_BoundingBoxSurfaces = new map<ETransformationAxis, ref Plane>();
	protected ref map<ETransformationAxis, EditorSnapPoint> m_SnapFaces = new map<ETransformationAxis, EditorSnapPoint>();
	
	protected ObjectViewMap m_ObjectViewMap;
	protected ref ObjectViewWorld m_ObjectViewWorld;
	
	protected Object m_BBoxLines[12], m_BBoxBase, m_CenterLine;		

	protected ref array<EditorSnapPoint> m_EditorSnapPoints = {};
	
	protected ref GizmoXYZ m_GizmoXYZ;
	
	void ObjectNode(string uuid, string display_name, Symbols icon, Object object)
	{
		m_Object = object;
		
		if (!GetGame().IsDedicatedServer() && m_Object) {
			InitObject(m_Object);
		}
	}
		
	void ~ObjectNode()
	{	
		if (m_Object) {
			All.Remove(m_Object);
		} else {
			// Does this work if the object gets deleted? :)
			All.RemoveElement(All.GetValueArray().Find(this));
		}
		
		GetGame().ObjectDelete(m_Object);
		EditorBoundingBox.Destroy(m_Object);
		GetGame().ObjectDelete(m_BBoxBase);
		GetGame().ObjectDelete(m_CenterLine);
						
		foreach (auto snap_point: m_EditorSnapPoints) {
			snap_point.Delete();
		}
	}
	
#ifdef DIAG_DEVELOPER	
	void DiagOnFrameUpdate(float dt)
	{		
		vector transform[4];
		m_Object.GetTransform(transform);
				
		return;
		if (HasState(TreeNodeState.ACTIVE | TreeNodeState.HOVER)) {
			for (int i = 0; i < 6; i++) {
				// Debug
				m_BoundingBoxSurfaces[i].Debug(typename.EnumToString(ETransformationAxis, i) + i.ToString(), transform);	
			}
			
			for (int j = 0; j < 6; j++) {
				// Generates each direction vector
				vector direction = vector.Zero;
				direction[j % 3] = 1 * ((j > 2) * -2 + 1);
				
			}
			
			vector min_max2[2];
			m_Object.GetCollisionBox(min_max2);
			if (min_max2[0].Length() > 0) {
				Shape.CreateSphere(COLOR_YELLOW, ShapeFlags.ONCE, min_max2[0].Multiply4(transform), 0.1);
				Shape.CreateSphere(COLOR_YELLOW, ShapeFlags.ONCE, min_max2[1].Multiply4(transform), 0.1);
			}
			
			if (m_Object.GetCollisionRadius() > 0) {
				Shape.CreateSphere(COLOR_GREEN_A, ShapeFlags.TRANSP | ShapeFlags.ONCE, transform[3], m_Object.GetCollisionRadius());
			}
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
	
	protected void InitObject(notnull Object object)
	{
		m_Object = object;
		All[m_Object] = this;
		
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
									
#ifdef DIAG_DEVELOPER
#ifndef SERVER
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DiagOnFrameUpdate);
#endif
#endif
	}
		
	override TreeView CreateView()
	{
		if (Parent.GetUUID() != EditorNode.PLACING) {
			if (!m_ObjectViewWorld) {
				m_ObjectViewWorld = new ObjectViewWorld(this);
			}
			
			if (!m_ObjectViewMap) {
				auto object_view_map = new ObjectViewMap(this);
				GetEditor().GetHud().GetTemplateController().MapMarkers.Insert(object_view_map);
				m_ObjectViewMap = object_view_map;
			}
		}
		
		return super.CreateView();
	}
	
	override bool CanDelete()
	{
		return true;
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
			m_Object = EditorNode.CreateObject(type, transform);
			if (!m_Object)  {
				Print("Failed to create object on type " + type);
				return false;
			}
			
			if (!GetGame().IsDedicatedServer()) {
				InitObject(m_Object);
			}
		}
				
		m_Object.SetTransform(transform);
		return true;
	}
			
	protected ref SuppressedObject m_SuppressedObject;
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsHover() || state.IsActive()) {
			if (total_state.IsHover() || total_state.IsActive()) {
				EditorBoundingBox.Create(m_Object);
				
			}
			
			if (!total_state.IsHover() && !total_state.IsActive()) {
				EditorBoundingBox.Destroy(m_Object);
			}
		}
			
		if (state.IsActive()) {	
			if (total_state.IsActive()) {			
				m_GizmoXYZ = new GizmoXYZ(this);
			} else {
				delete m_GizmoXYZ;
			}
		}
		
		if (state.IsSuppressed()) {
			RemoveState(TreeNodeState.ACTIVE);
			
			if (total_state.IsSuppressed()) {
				m_SuppressedObject = new SuppressedObject(m_Object);
			} else {
				delete m_SuppressedObject;
			}
			
			m_Object.Update();
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
		
		if (EditorBoundingBox.HasBoundingBox(m_Object)) {
			EditorBoundingBox.Destroy(m_Object);
			EditorBoundingBox.Create(m_Object);
		}
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
	
	Plane GetBoundingFace(ETransformationAxis axis)
	{
		return m_BoundingBoxSurfaces[axis];
	}
				
	void Hide(bool state) 
	{		
		if (!state) {
			m_Object.SetFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
		} else {
			m_Object.ClearFlags(EntityFlags.VISIBLE | EntityFlags.TOUCHTRIGGERS, true);
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
		
	override TreeNodeInteract GetInteractType()
	{		
		// This is pretty cool. inheritence with KEYBINDS??
		if (GetDayZGame().IsLeftCtrlDown()) {
			return TreeNodeInteract.TOGGLE;
		}
		
		return TreeNodeInteract.PRESS;
	}
	
	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.HOVER | TreeNodeState.ACTIVE | TreeNodeState.CONTEXT | TreeNodeState.DRAGGING | TreeNodeState.SUPPRESSED;
	}
							
	Object GetObject() 
	{		
		return m_Object;
	}
	
	ObjectViewWorld GetObjectViewWorld()
	{
		return m_ObjectViewWorld;
	}
	
	ObjectViewMap GetObjectViewMap()
	{
		return m_ObjectViewMap;
	}
		
	array<EditorSnapPoint> GetEditorSnapPoints()
	{
		return m_EditorSnapPoints;
	}
}