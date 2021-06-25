class EditorWidget
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() 
	{
		return m_LayoutRoot; 
	}
	
	void SetLayoutRoot(Widget layout_root) 
	{
		m_LayoutRoot = layout_root;
	}
}