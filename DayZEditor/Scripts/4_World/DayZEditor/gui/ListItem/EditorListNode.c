class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
	static ref EditorListNodeContextMenu s_ContextMenu;
		
	ref array<ref EditorListNode> ChildrenItems = {};
	
	protected bool m_IsCollapsed = true;
	protected EditorListNode m_Parent;
	
	bool m_TemporaryReveal;
	
	Widget Collapse, IconFrame, Hide, Panel, ChildrenHeight, Favorite;
	ButtonWidget CollapseButton, HideButton, FavoriteButton;
	ImageWidget CollapseIcon, IconImage, HideIcon, FavoriteIcon;
	TextWidget Text;
	EditBoxWidget Edit;
	WrapSpacerWidget Children;
	
	void EditorListNode()
	{
		Collapse.Show(false);
		m_LayoutRoot.SetSort(1);
		CollapseIcon.LoadImageFile(1, "set:solid image:square_minus");
		CollapseIcon.LoadImageFile(0, "set:regular image:square_plus");
	}
	
	void ~EditorListNode()
	{
		ChildrenItems.Clear();
	}
	
	void InsertChild(notnull EditorListNode list_node)
	{
		ChildrenItems.Insert(list_node);
		Collapse.Show(ChildrenItems.Count());
		Children.AddChild(list_node.GetLayoutRoot());
		
		list_node.m_Parent = this;
	}
	
	void SetCollapsed(bool collapsed)
	{			
		if (!ChildrenItems.Count()) {
			return;
		}
		
		m_IsCollapsed = collapsed;
		Children.Show(!collapsed);
		CollapseIcon.SetImage(!collapsed);
		
		float w, h, x, y;
		Children.Update();
		Children.GetScreenSize(w, h);
		
		h *= !collapsed;
		
		ChildrenHeight.SetScreenSize(2, h);
		
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);		
		m_LayoutRoot.SetScreenSize(screen_x, h + 24 * screen_y / 1080.0);
		
		// If you are setting this as collapsed, the parents must be uncollapsed because you can access it. we are using this to update the collapse state
		if (GetListParent()) {
			GetListParent().SetCollapsed(false);
		}
	}
			
	void CollapseAll()
	{		
		for (int i = 0; i < ChildrenItems.Count(); i++) {
			ChildrenItems[i].SetCollapsed(true);
		}
		
		SetCollapsed(true);
	}
	
	void ExpandAll()
	{		
		for (int i = 0; i < ChildrenItems.Count(); i++) {
			ChildrenItems[i].SetCollapsed(false);
		}
		
		SetCollapsed(false);
	}
					
	bool IsCollapsed()
	{
		return m_IsCollapsed;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case CollapseButton: {
				if (button == MouseState.LEFT) {
					SetCollapsed(!IsCollapsed());	
				}	
							
				break;
			}
		}
						
		return true;
	}		
	
	override bool OnDrag(Widget w, int x, int y)
	{
		w.SetPos(0, 0);
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		w.SetPos(0, 0);
		
		return super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		w.SetPos(0, 0);
		
		return super.OnDrop(w, x, y, reciever);
	}	
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{				
		if (s_SelectedNode) {
			s_SelectedNode.Panel.SetColor(0);
		}
		
		s_SelectedNode = this;		
		Panel.SetColor(g_Editor.GetSettings().SelectionColor);
				
		SetFocus(null);

		return true;
	}
	
	protected bool IsSelected()
	{
		return s_SelectedNode == this;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case ChildrenHeight: {
				WidgetAnimator.CancelAnimate(ChildrenHeight);
				ChildrenHeight.SetColor(GetEditor().GetSettings().HighlightColor);
				break;
			}
			
			case m_LayoutRoot: {
				if (!IsSelected()) {
					WidgetAnimator.CancelAnimate(Panel);
					Panel.SetColor(GetEditor().GetSettings().HighlightColor);
				}
				
				break;
			}
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case ChildrenHeight: {
				WidgetAnimator.AnimateColor(ChildrenHeight, 0xffc8c8c8, 100);
				break;
			}
			
			case m_LayoutRoot: {
				if (!IsSelected()) {
					WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 0, 100);
				}
				break;
			}
		}
		
		return true;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\NodeTreeView.layout";
	}
			
	protected override bool UseUpdateLoop()
	{
		return false;
	}
	
	EditorListNode GetListParent()
	{
		return m_Parent;
	}
		
	bool FilterType(string filter, bool favorites)
	{
		return false;
	}
	
	// All of these slow down the process
	override bool OnUpdate(Widget w)
	{
		return true;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		return true;
	}
	
	override bool OnResize(Widget w, int x, int y)
	{
		return true;
	}
}

class EditorFolderListNode: EditorListNode
{
	protected string m_Text;
	
	void EditorFolderListNode(string text)
	{
		m_Text = text;
		Text.SetText(text);
		m_Text.ToLower();
		m_LayoutRoot.SetSort(0);
		m_LayoutRoot.ClearFlags(WidgetFlags.DRAGGABLE);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		if (w == m_LayoutRoot) {
			SetCollapsed(!IsCollapsed());	
		}
		
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.RIGHT) {
			s_ContextMenu = new EditorListNodeContextMenu(x, y, this);
		}
		
		GetEditor().ClearHand();
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool FilterType(string filter, bool favorites)
	{
		return m_Text.Contains(filter);
	}
}