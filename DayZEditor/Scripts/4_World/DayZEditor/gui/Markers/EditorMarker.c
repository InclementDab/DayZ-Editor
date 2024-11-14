class EditorMarker: ScriptView
{
	static ref array<EditorMarker> s_AllMarkers = {};

	protected Editor m_Editor = GetEditor();
	protected bool m_Show = true;
	
	Widget EditorMarkerColor, EditorMarkerOutline;
	
	void EditorMarker()
	{
		SetHighlighted(0);

		if (!s_AllMarkers) {
			s_AllMarkers = {};
		}

		s_AllMarkers.Insert(this);
	}

	void ~EditorMarker()
	{
		if (s_AllMarkers) {
			s_AllMarkers.RemoveItem(this);
		}
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
	
	float GetSize()
	{
		float size;
		m_LayoutRoot.GetScreenSize(size, size);
		return size;
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
		WidgetAnimator.CancelAnimate(EditorMarkerColor);
		WidgetAnimator.CancelAnimate(EditorMarkerOutline);

		if (highlighted) {
			if (highlighted > 1) {
				EditorMarkerColor.SetColor(outercolor.With(3, 255) + LinearColor.Create(20, 20, 20));
				EditorMarkerOutline.SetColor(LinearColor.BLACK.With(3, 255));
			} else {
				EditorMarkerOutline.SetColor(outercolor.With(3, alpha));
			}
		} else {
			EditorMarkerColor.SetColor(LinearColor.WHITE.With(3, alpha));
			EditorMarkerOutline.SetColor(LinearColor.BLACK.With(3, 220));
			//WidgetAnimator.AnimateColor(EditorMarkerColor, LinearColor.WHITE.With(3, alpha), 20);
			//WidgetAnimator.AnimateColor(EditorMarkerOutline, LinearColor.BLACK.With(3, 220), 20);
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