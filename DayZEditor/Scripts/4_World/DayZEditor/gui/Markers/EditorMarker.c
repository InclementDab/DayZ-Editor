class EditorMarker: ScriptView
{
	protected Editor m_Editor = GetEditor();
	protected bool m_Show = true;
	
	Widget EditorMarkerColor, EditorMarkerOutline;
	
	void EditorMarker()
	{
		SetHighlighted(false);
	}
	
	void SetPos(float x, float y) 
	{
		// Offset to center of marker
		float w, h;		
		m_LayoutRoot.GetScreenSize(w, h);
		
		m_LayoutRoot.SetScreenPos(x - w / 2, y - h / 2);
	}
	
	void GetPos(out float x, out float y)
	{
		m_LayoutRoot.GetScreenPos(x, y);
	}
	
	void SetSize(float normalize_01)
	{
		float size_min = 0, size_max = 0;
		switch (GetEditor().GetSettings().MarkerSize) {
			case 0: {
				size_min = 12;
				size_max = 16;
				break;
			}
			
			case 1: {
				size_min = 16;
				size_max = 22;
				break;
			}
			
			case 2: {
				size_min = 22;
				size_max = 28;
				break;
			}
			
			case 3: {
				// woah
				size_min = 30;
				size_max = 38;
				break;
			}
		}
		
		float size = Math.Lerp(size_min, size_max, normalize_01);
		m_LayoutRoot.SetScreenSize(size, size);
	}
	
	void SetSize(float x, float y)
	{
		Error("Deprecated function");
		m_LayoutRoot.SetSize(x, y);
	}
	
	void GetSize(out float x, out float y)
	{
		m_LayoutRoot.GetSize(x, y);
	}
	
	override void Show(bool show)
	{
		m_Show = show;
		if (m_LayoutRoot && m_LayoutRoot.IsVisible() != show) {
			m_LayoutRoot.Show(m_Show);
		}
	}
	
	bool IsDisabled()
	{
		return false;		
	}
	
	void SetHighlighted(int highlighted)
	{
		int alpha = 255;
		if (!highlighted) {
			alpha = 100;
		}
		
		if (IsDisabled()) {
			alpha = 40;
		}

		LinearColor innercolor = GetEditor().GetSettings().HighlightColor;
		LinearColor outercolor = GetEditor().GetSettings().SelectionColor;


		if (highlighted) {
			if (highlighted > 1) {
				EditorMarkerColor.SetColor(outercolor.With(3, 255) + LinearColor.Create(20, 20, 20));
				EditorMarkerOutline.SetColor(LinearColor.BLACK.With(3, 255));
			} else {
				EditorMarkerOutline.SetColor(outercolor.With(3, alpha));
			}
		} else {
			WidgetAnimator.AnimateColor(EditorMarkerColor, innercolor.With(3, alpha), 100);
			WidgetAnimator.AnimateColor(EditorMarkerOutline, LinearColor.BLACK.With(3, 220), 100);
		}
	}
	
	protected bool IsMouseInside(int c_x, int c_y)
	{
		float x, y, w, h;
		m_LayoutRoot.GetScreenPos(x, y);
		m_LayoutRoot.GetScreenSize(w, h);
		return (c_x < x + h / 2 && c_x > x - h / 2) && (c_y < y + h / 2 && c_y > y - h / 2);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/EditorMarker.layout";
	}
}