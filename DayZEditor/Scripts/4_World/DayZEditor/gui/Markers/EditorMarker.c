class EditorMarker: ScriptView
{
	protected bool m_Show = true;
	void EditorMarker()
	{
		EditorLog.Trace("EditorMarker");
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorMarker()
	{
		EditorLog.Trace("~EditorMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void Update();
	
	void SetPos(float x, float y) 
	{
		// Offset to center of marker
		m_LayoutRoot.SetPos(x - 10, y - 10);
	}
	
	void GetPos(out float x, out float y)
	{
		m_LayoutRoot.GetPos(x, y);
	}
	
	void Show(bool show)
	{
		m_Show = show;
		if (m_LayoutRoot && m_LayoutRoot.IsVisible() != show) {
			m_LayoutRoot.Show(m_Show);
		}
	}
	
	protected bool IsMouseInside(int c_x, int c_y)
	{
		float x, y, w, h;
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		return (c_x < x + h && c_x > x - h) && (c_y < y + h && c_y > y - h);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/EditorMarker.layout";
	}
}