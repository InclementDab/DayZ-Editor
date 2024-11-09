class EditorGizmo: Managed
{

}

class EditorTranslationGizmo: EditorGizmo
{
	protected ref EditorObjectMap m_ObjectList;
	protected ref array<GizmoBase> m_Gizmos = {};

	void EditorTranslationGizmo(EditorObjectMap object_map)
	{
		m_ObjectList = object_map;
		if (m_ObjectList.Count() == 0) {
			return;
		}

		EditorObject first_object = m_ObjectList.GetElement(0);
		vector gizmo_center = first_object.GetTopCenter();

		GizmoBase gizmo_x = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowX", gizmo_center, ECE_NONE));
		vector gizmo_x_mat[4] = {
			-vector.Forward,
			vector.Aside,
			-vector.Up,
			gizmo_center
		};
		gizmo_x.SetTransform(gizmo_x_mat);
		gizmo_x.Update();
		m_Gizmos.Insert(gizmo_x);

		GizmoBase gizmo_y = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowY", gizmo_center, ECE_NONE));
		vector gizmo_y_mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			gizmo_center
		};
		gizmo_y.SetTransform(gizmo_y_mat);
		gizmo_y.Update();
		m_Gizmos.Insert(gizmo_y);

		GizmoBase gizmo_z = GizmoBase.Cast(GetGame().CreateObjectEx("GizmoArrowZ", gizmo_center, ECE_NONE));
		vector gizmo_z_mat[4] = {
			vector.Aside,
			vector.Forward,
			-vector.Up,
			gizmo_center
		};
		gizmo_y.SetTransform(gizmo_z_mat);
		gizmo_z.Update();
		m_Gizmos.Insert(gizmo_z);
	}

	void ~EditorTranslationGizmo()
	{
		foreach (Object gizmo: m_Gizmos) {
			if (gizmo) {
				gizmo.Delete();
			}
		}
	}
}

class GizmoBase: EntityAI
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