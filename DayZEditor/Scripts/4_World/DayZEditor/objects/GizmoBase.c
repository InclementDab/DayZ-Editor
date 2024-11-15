class EditorGizmo: Managed
{
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	protected EditorObjectMap m_AllObjects;
	protected int m_CurrentGizmoAxis;
	protected Object m_CurrentInteractingGizmo;
	protected ref map<eGizmoAxis, Object> m_Gizmos = new map<eGizmoAxis, Object>();
	
	void Initialize(notnull Editor editor, EditorObject editor_object, EditorObjectMap all_objects)
	{		
		m_Editor = editor;
		m_EditorObject = editor_object;
		m_AllObjects = all_objects;

		vector gizmo_center = vector.Zero;

		vector gizmo_x_mat[4] = {
			-vector.Forward,
			vector.Aside,
			-vector.Up,
			gizmo_center
		};
		
		Object gizmo_x = GetGame().CreateStaticObjectUsingP3D("DayZEditor\\Editor\\TranslationWidgetX.p3d", gizmo_center, vector.Zero, 3.0, true);
		gizmo_x.SetTransform(gizmo_x_mat);
		gizmo_x.Update();

		Object gizmo_y = GetGame().CreateStaticObjectUsingP3D("DayZEditor\\Editor\\TranslationWidgetY.p3d", gizmo_center, vector.Zero, 3.0, true);
		vector gizmo_y_mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			gizmo_center
		};
		gizmo_y.SetTransform(gizmo_y_mat);
		gizmo_y.Update();
	
		Object gizmo_z = GetGame().CreateStaticObjectUsingP3D("DayZEditor\\Editor\\TranslationWidgetZ.p3d", gizmo_center, vector.Zero, 3.0, true);
		vector gizmo_z_mat[4] = {
			vector.Aside,
			vector.Forward,
			-vector.Up,
			gizmo_center
		};
		
		gizmo_y.SetTransform(gizmo_z_mat);
		gizmo_z.Update();

		m_Gizmos[eGizmoAxis.X_AXIS] = gizmo_x;
		m_Gizmos[eGizmoAxis.Y_AXIS] = gizmo_y;
		m_Gizmos[eGizmoAxis.Z_AXIS] = gizmo_z;
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorGizmo()
	{
		foreach (Object gizmo: m_Gizmos) {
			if (gizmo) {
				gizmo.Delete();
			}
		}
	}
	
	// Starts listening on specific gizmo_object, stops when mouse cursor is released
	bool Begin(Object gizmo_object)
	{
		if (!Contains(gizmo_object)) {
			return false;
		}
		
		m_CurrentInteractingGizmo = gizmo_object;
		return m_CurrentInteractingGizmo != null;
	}

	bool Contains(Object gizmo_object)
	{
		if (!gizmo_object) {
			return false;
		}
		
		return (m_Gizmos.GetValueArray().Find(gizmo_object) != -1);
	}
	
	bool IsInteracting()
	{
		return m_CurrentInteractingGizmo != null;
	}

	Object GetGizmoArm(eGizmoAxis gizmo_axis)
	{
		return m_Gizmos[gizmo_axis];
	}

	Plane GetGizmoPlane(eGizmoAxis gizmo_axis)
	{
		Object gizmo_object = GetGizmoArm(gizmo_axis);
		vector normal = gizmo_object.GetDirectionUp();
		vector aside = gizmo_object.GetDirectionAside();
		vector position = gizmo_object.GetPosition();
		return Plane.Create(normal, "10 10 10", position, aside);
	}
	
	Plane GetGizmoPlane(Object gizmo)
	{
		vector normal = gizmo.GetDirectionUp();
		vector aside = gizmo.GetDirectionAside();
		vector position = gizmo.GetPosition();
		return Plane.Create(normal, "10 10 10", position, aside);
	}
	
	void Update(float dt)
	{
		if (m_CurrentInteractingGizmo) {
			if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
				m_CurrentInteractingGizmo = null;
				return;
			}
			
			int mouse_x, mouse_y;
			GetMousePos(mouse_x, mouse_y);
			UpdateGizmo(mouse_x, mouse_y, m_CurrentInteractingGizmo);
		}
	}
	
	void UpdateGizmo(int mouse_x, int mouse_y, GizmoBase current_gizmo);

	void OnIntersectMouse(Ray ray)
	{
		Raycast ray_cast = ray.PerformRaycast();
		if (ray_cast && ray_cast.Hit) {
			m_CurrentGizmoAxis = m_Gizmos.GetKeyByValue(ray_cast.Hit);
		}
	}
	
	void OnUnintersectMouse(Ray ray)
	{
		m_CurrentGizmoAxis = -1;
	}
}

enum eGizmoAxis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

class EditorTranslationGizmo: EditorGizmo
{
	protected vector m_DragOffset;
	protected Object m_DragObject;

	override void Update(float dt)
	{
		super.Update(dt);
		
		if (GetGame().GetInput().LocalRelease("UAFire")) {
			m_DragObject = null;
		}
		
		EditorObjectMap all_editor_objects = GetEditor().GetSelectedObjects();
		EditorObject editor_object = all_editor_objects.GetElement(0);
		if (!editor_object || all_editor_objects.Count() == 0) {
			m_Gizmos[eGizmoAxis.X_AXIS].SetPosition(vector.Zero);
			m_Gizmos[eGizmoAxis.Y_AXIS].SetPosition(vector.Zero);
			m_Gizmos[eGizmoAxis.Z_AXIS].SetPosition(vector.Zero);
			return;
		}

		vector top_center = editor_object.GetTopCenter();
		Ray cursor_ray = GetEditor().GetCursorRay();
		Raycast cursor_raycast = cursor_ray.PerformRaycast();
	
		m_Gizmos[eGizmoAxis.X_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Y_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Z_AXIS].SetPosition(top_center);
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			top_center
		};

		if (GetGame().GetInput().LocalPress("UAFire") && !m_DragObject && cursor_raycast && Contains(cursor_raycast.Hit)) {	
			m_DragObject = cursor_raycast.Hit;
			vector intersect_point = GetGizmoPlane(m_DragObject).Intersect(cursor_ray, mat);
			m_DragOffset = intersect_point - editor_object.GetPosition();
		}
		
		if (m_DragObject) {
			Debug.DestroyAllShapes();

			vector normal = vector.Direction(top_center, GetGame().GetCurrentCameraPosition());						
			normal[1] = 0;
			normal.Normalize();

			Plane normal_plane = Plane.Create(normal, "10 10 10", vector.Zero, vector.Up);
			vector intersect_point2 = normal_plane.Intersect(cursor_ray, mat);
			
			//vector intersect_point2 = GetGizmoPlane(m_DragObject).Intersect(cursor_ray, mat);
			Debug.DrawSphere(intersect_point2);
			Print(intersect_point2);
			editor_object.SetPosition(intersect_point2 - m_DragOffset);
			top_center = editor_object.GetTopCenter();
			// This widget will never rotate unless we get a local space operator
			m_Gizmos[eGizmoAxis.X_AXIS].SetPosition(top_center);
			m_Gizmos[eGizmoAxis.Y_AXIS].SetPosition(top_center);
			m_Gizmos[eGizmoAxis.Z_AXIS].SetPosition(top_center);
		}
	}

	override void OnIntersectMouse(Ray ray)
	{
		super.OnIntersectMouse(ray);
	}
	
	override void OnUnintersectMouse(Ray ray)
	{
		super.OnUnintersectMouse(ray);
	}

	override bool IsInteracting()
	{
		return m_DragObject != null;
	}
}

class GizmoBase: Object
{		
	void OnIntersectMouse(Ray ray)
	{
	}
	
	void OnUnintersectMouse(Ray ray)
	{
	}
}


class GizmoArrowX: GizmoBase
{
	override void OnIntersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoArrowY: GizmoBase
{
	override void OnIntersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoArrowZ: GizmoBase
{
	override void OnIntersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		//SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoCenter: GizmoBase
{
}