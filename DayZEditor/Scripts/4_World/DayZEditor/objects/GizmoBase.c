class EditorGizmo: Managed
{
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	protected EditorObjectMap m_AllObjects;
	protected int m_CurrentGizmoAxis;
	protected ref map<eGizmoAxis, GizmoBase> m_Gizmos = new map<eGizmoAxis, GizmoBase>();
	
	void Initialize(notnull Editor editor, notnull EditorObject editor_object, EditorObjectMap all_objects)
	{		
		m_Editor = editor;
		m_EditorObject = editor_object;
		m_AllObjects = all_objects;

		vector gizmo_center = m_EditorObject.GetTopCenter();

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
	
	void Update(float dt)
	{
	}

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
	override void Update(float dt)
	{
		if (!m_EditorObject) {
			m_EditorObject = m_Editor.GetSelectedObjects().GetElement(0);
			if (!m_EditorObject) {
				delete this;
				return;
			}
		}
		
		vector top_center = m_EditorObject.GetTopCenter();
		Ray cursor_ray = GetEditor().GetCursorRay();
		vector mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			top_center
		};

		vector intersect_point = GetGizmoPlane(m_CurrentGizmoAxis).Intersect(cursor_ray, mat);
		Debug.DestroyAllShapes();
		Debug.DrawSphere(intersect_point, 0.5);

		// This widget will never rotate unless we get a local space operator
		m_Gizmos[eGizmoAxis.X_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Y_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Z_AXIS].SetPosition(top_center);
	}

	override void OnIntersectMouse(Ray ray)
	{
		super.OnIntersectMouse(ray);
	}
	
	override void OnUnintersectMouse(Ray ray)
	{
		super.OnUnintersectMouse(ray);
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