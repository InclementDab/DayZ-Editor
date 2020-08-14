class MenuBarFile: EditorWidgetEventHandler
{
	protected WrapSpacerWidget m_FileListWrapSpacer;
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("MenuBarFile::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		m_FileListWrapSpacer = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("FileListWrapSpacer"));
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Root)
			m_FileListWrapSpacer.Show(true);
		
		
		
		return true;
	}
}
