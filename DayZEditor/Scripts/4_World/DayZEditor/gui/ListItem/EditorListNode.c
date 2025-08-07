class EditorListNodeController: ViewController
{
	ref ObservableCollection<ref EditorListNode> ChildrenItems = new ObservableCollection<ref EditorListNode>(this);
}

class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
	
	protected EditorListNodeController m_TemplateController;
	
	Widget Collapse, IconFrame, Hide, Panel, BoundingBox, Lock, Marker;
	ButtonWidget CollapseButton, HideButton, BoundingBoxButton, LockButton, MarkerButton;
	ImageWidget CollapseIcon, IconImage, HideIcon, BoundingBoxIcon, LockIcon, MarkerIcon;
	TextWidget Text;
	EditBoxWidget Edit;
	WrapSpacerWidget Children;
	
	void EditorListNode()
	{
		m_TemplateController = EditorListNodeController.Cast(m_Controller);
		Collapse.Show(false);
	}
		
	override void Update(float dt)
	{
		super.Update(dt);
		if (IsSelected()) {
			Panel.SetColor(g_Editor.GetSettings().SelectionColor);
		} else {
			Panel.SetColor(0);
		}
	}
	
	void InsertChild(notnull EditorListNode list_node)
	{
		m_TemplateController.ChildrenItems.Insert(list_node);
		Collapse.Show(m_TemplateController.ChildrenItems.Count());
	}
	
	void SetCollapsed(bool collapsed)
	{
		Children.Show(!collapsed);
		Symbols collapse_icon = Symbols.SQUARE_PLUS;
		if (!collapsed) {
			collapse_icon = Symbols.SQUARE_MINUS;
		}
		
		collapse_icon.Load(CollapseIcon, 0);
		
		RecalculateSize();
		
		Widget parent = m_LayoutRoot.GetParent().GetParent();
		while (parent && parent.GetName() == "NodeView") {
			EditorListNode node;
			parent.GetUserData(node);
			node.RecalculateSize();
			parent = parent.GetParent().GetParent();
		}
		
		if (!collapsed) {
			IconImage.SetImage(3);
		} else {
			IconImage.SetImage(2);
		}
	}
	
	override void Show(bool show)
	{
		super.Show(show);
		
		RecalculateSize();
		
		if (show) {
			Widget parent = m_LayoutRoot.GetParent().GetParent();
			if (parent && parent.GetName() == "NodeView") {
				EditorListNode node;
				parent.GetUserData(node);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(node.ShowCuzDayZSucksAss, 0, 0, show);
			}
		}
	}
	
	protected void ShowCuzDayZSucksAss(bool show)
	{
		Show(show);
	}
	
	protected void RecalculateSize()
	{
		float w, h, x, y;
		Children.Update();
		Children.GetScreenSize(w, h);	
		
		m_LayoutRoot.GetScreenSize(x, y);
		m_LayoutRoot.SetScreenSize(x, h * Children.IsVisible() + 30);
		m_LayoutRoot.Update();
	}
	
	bool IsCollapsed()
	{
		return !Children.IsVisible();
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case CollapseButton: {
				SetCollapsed(!IsCollapsed());					
				break;
			}
		}
				
		return true;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		PrintFormat("DoubleClick %1", w);
		//if (w == m_LayoutRoot) {
		/*	Edit.Show(!Edit.IsVisible());
			Text.Show(!Text.IsVisible());
			
			if (Edit.IsVisible()) {
				SetFocus(Edit);
			} else {
				SetFocus(null);
			}*/
	//	}
		
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		s_SelectedNode = this;				
		return true;
	}
	
	protected bool IsSelected()
	{
		return s_SelectedNode == this;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return true;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\NodeTreeView.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorListNodeController;
	}
}

class EditorFolderListNode: EditorListNode
{
	void EditorFolderListNode(string text)
	{
		Text.SetText(text);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		SetCollapsed(!IsCollapsed());
		return true;
	}
}