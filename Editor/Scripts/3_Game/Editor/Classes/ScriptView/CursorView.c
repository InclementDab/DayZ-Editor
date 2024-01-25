class CursorView: ScriptView
{
	ImageWidget Foreground, Background;
	
	void CursorView(Symbols cursor)
	{
		SetCursorWidget(m_LayoutRoot);
		m_LayoutRoot.Show(true);
		
		if (Foreground) {
			Foreground.LoadImageFile(0, cursor.Thin());
			Foreground.SetImage(0);
		}

		if (Background) {
			Background.LoadImageFile(0, cursor.Solid());
			Background.SetImage(0);
		}
	}
	
	void ~CursorView()
	{
		SetCursorWidget(null);
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Cursor.layout";
	}
}