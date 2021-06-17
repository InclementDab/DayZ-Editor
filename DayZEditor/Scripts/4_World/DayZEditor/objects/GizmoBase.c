class GizmoBase: Inventory_Base
{
	void GizmoBase()
	{
		EditorLog.Trace("GizmoBase");
	}
	
	void ~GizmoBase()
	{
		EditorLog.Trace("~GizmoBase");
	}
		
	void ResetColor()
	{
		SetColor(GetColor());
	}

	void SetColor(string color)
	{	
		SetObjectTexture(GetHiddenSelectionIndex("main"), color);
	}
	
	string GetColor() 
	{
		return "#(argb,8,8,3)color(0,0,0,1.0,co)";
	}
	
	void OnMouseEnter();
	
	void OnMouseLeave();
	
	void OnDragStart();
	
	void OnDragFinish();
	
}


class GizmoArrow: GizmoBase
{
	
}

class GizmoCenter: GizmoBase
{
	override string GetColor()
	{
		return "#(argb,8,8,3)color(0.5,0.5,0.5,1.0,CO)";
	}
}

class GizmoX: GizmoArrow
{
	override string GetColor()
	{
		return "#(argb,8,8,3)color(1,0,0,1.0,co)";
	}
}

class GizmoY: GizmoArrow
{
	override string GetColor()
	{
		return "#(argb,8,8,3)color(0,1,0,1.0,co)";
	}
}

class GizmoZ: GizmoArrow
{
	override string GetColor()
	{
		return "#(argb,8,8,3)color(0,0,1,1.0,co)";
	}
}



ref EditorGizmo m_EditorGizmoTemp;

class EditorGizmo
{
	protected ref Timer m_Timer = new Timer(CALL_CATEGORY_SYSTEM);
	
	ref array<GizmoBase> m_GizmoBaseParts = {};
	
	// trade position for object
	void EditorGizmo(vector position)
	{	
		Print("EditorGizmo");
		m_GizmoBaseParts.Insert(GizmoBase.Cast(GetGame().CreateObjectEx("GizmoCenter", position, ECE_LOCAL)));
		m_GizmoBaseParts.Insert(GizmoBase.Cast(GetGame().CreateObjectEx("GizmoX", position, ECE_LOCAL)));
		m_GizmoBaseParts.Insert(GizmoBase.Cast(GetGame().CreateObjectEx("GizmoY", position, ECE_LOCAL)));
		m_GizmoBaseParts.Insert(GizmoBase.Cast(GetGame().CreateObjectEx("GizmoZ", position, ECE_LOCAL)));
		
		foreach (GizmoBase gb: m_GizmoBaseParts) {
			gb.ResetColor();
		}
		
		// X
		m_GizmoBaseParts[2].SetOrientation("0 0 90");
		m_GizmoBaseParts[2].Update();
		
		// Y
		m_GizmoBaseParts[3].SetOrientation("-90 0 90");
		m_GizmoBaseParts[3].Update();
		
		m_Timer.Run(0.01, this, "UpdateFrame", null, true);
	}
	
	void ~EditorGizmo()
	{
		Print("~EditorGizmo");
		m_Timer.Stop();
		delete m_Timer;
		
		foreach (GizmoBase gb: m_GizmoBaseParts) {
			gb.Delete();
		}
	}
	
	private void UpdateFrame()
	{
		Object obj = GetObjectUnderCursor();
		GizmoBase gizmo_base;
		
		//Print(obj);
		
		foreach (GizmoBase gb: m_GizmoBaseParts) {
			gb.ResetColor();
		}
		
		// Set yellow
		if (Class.CastTo(gizmo_base, obj)) {
			//gizmo_base.SetColor("#(argb,8,8,3)color(1,1,0,1.0,co)");
		}
	}
}