class EditorMarker: ScriptView
{
	protected Editor m_Editor = GetEditor();
	protected bool m_Show = true;
	
	Widget EditorMarkerColor;
	Widget EditorMarkerOutlineColor;
	
	void EditorMarker()
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
		SetColor(m_Editor.Settings.MarkerPrimaryColor);
		SetOutlineColor(m_Editor.Settings.MarkerPrimaryColor);
	}
	
	void SetPos(float x, float y) 
	{
		float h, w;
		GetSize(h, w);
		// Offset to center of marker		
		m_LayoutRoot.SetPos(x - h / 2, y - w / 2);
	}
	
	void GetPos(out float x, out float y)
	{
		m_LayoutRoot.GetPos(x, y);
	}
	
	void SetSize(float x, float y)
	{
		m_LayoutRoot.SetSize(x, y);
	}
	
	void GetSize(out float x, out float y)
	{
		m_LayoutRoot.GetSize(x, y);
	}
	
	void Show(bool show)
	{
		m_Show = show;
		if (m_LayoutRoot && m_LayoutRoot.IsVisible() != show) {
			m_LayoutRoot.Show(m_Show);
		}
	}
	
	void SetColor(int color)
	{
		if (EditorMarkerColor) {
			EditorMarkerColor.SetColor(color);
		}
	}
	
	void SetOutlineColor(int color)
	{
		if (EditorMarkerOutlineColor) {
			EditorMarkerOutlineColor.SetColor(color);
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