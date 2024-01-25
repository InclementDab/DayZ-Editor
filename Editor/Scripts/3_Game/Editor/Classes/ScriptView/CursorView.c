class CursorView: ScriptView
{
	ImageWidget Foreground, Background;
	
	void CursorView(Symbols cursor)
	{
		Foreground.LoadImageFile(0, cursor.Thin());
		Foreground.SetImage(0);

		Background.LoadImageFile(0, cursor.Solid());
		Background.SetImage(0);
		
		SetCursorWidget(m_LayoutRoot);
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