class EditorGizmo: Managed
{

}

enum eGizmoAxis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

class EditorTranslationGizmo: EditorGizmo
{
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	protected ref map<eGizmoAxis, GizmoBase> m_Gizmos = new map<eGizmoAxis, GizmoBase>();

	void EditorTranslationGizmo(notnull Editor editor, notnull EditorObject editor_object)
	{		
		m_Editor = editor;
		m_EditorObject = editor_object;

		vector gizmo_center = m_EditorObject.GetTopCenter();

		GizmoBase gizmo_x = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowX", gizmo_center, ECE_NONE));
		vector gizmo_x_mat[4] = {
			-vector.Forward,
			vector.Aside,
			-vector.Up,
			gizmo_center
		};
		gizmo_x.SetTransform(gizmo_x_mat);
		gizmo_x.Update();

		GizmoBase gizmo_y = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowY", gizmo_center, ECE_NONE));
		vector gizmo_y_mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			gizmo_center
		};
		gizmo_y.SetTransform(gizmo_y_mat);
		gizmo_y.Update();

		GizmoBase gizmo_z = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowZ", gizmo_center, ECE_NONE));
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

	void ~EditorTranslationGizmo()
	{
		foreach (Object gizmo: m_Gizmos) {
			if (gizmo) {
				gizmo.Delete();
			}
		}
	}
	
	void Update(float dt)
	{
		if (!m_EditorObject) {
			m_EditorObject = m_Editor.GetSelectedObjects().GetElement(0);
			if (!m_EditorObject) {
				delete this;
				return;
			}
		}

		vector top_center = m_EditorObject.GetTopCenter();

		// This widget will never rotate unless we get a local space operator
		m_Gizmos[eGizmoAxis.X_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Y_AXIS].SetPosition(top_center);
		m_Gizmos[eGizmoAxis.Z_AXIS].SetPosition(top_center);
	}
}

class GizmoBase: House
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
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoArrowY: GizmoBase
{
	override void OnIntersectMouse(Ray ray)
	{
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoArrowZ: GizmoBase
{
	override void OnIntersectMouse(Ray ray)
	{
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(0.5,0.5,0.5,1.0,co)");
	}

	override void OnUnintersectMouse(Ray ray)
	{
		SetObjectTexture(GetHiddenSelectionIndex("main"), "(argb,8,8,3)color(1,1,0,1.0,co)");
	}
}

class GizmoCenter: GizmoBase
{
}