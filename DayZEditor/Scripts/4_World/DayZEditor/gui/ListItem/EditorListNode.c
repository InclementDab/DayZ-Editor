class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
	static ref EditorListNodeContextMenu s_ContextMenu;
		
	ref array<ref EditorListNode> ChildrenItems = {};
	
	protected bool m_QueueRecalculateSize, m_IsCollapsed = true;
	protected EditorListNode m_Parent;
	
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
		
	override void Update(float dt)
	{
		super.Update(dt);
		
		if (m_QueueRecalculateSize) {
			float w, h, x, y;
			Children.Update();
			Children.GetScreenSize(w, h);	
			
			m_LayoutRoot.GetScreenSize(x, y);
			float h_children = h * !m_IsCollapsed;
			m_LayoutRoot.SetScreenSize(x, h_children + 24);
					
			ChildrenHeight.SetSize(2, h_children);
			
			if (IsInherited(EditorFolderListNode)) {
				bool are_children_visible = false;
				foreach (EditorListNode child_node: ChildrenItems) {
					if (child_node && child_node.IsVisible()) {
						are_children_visible = true;
						break;
					}
				}
				
				Show(are_children_visible);
			}
			
			m_QueueRecalculateSize = false;
		}
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
		if (m_IsCollapsed == collapsed) {
			RecalculateSize();
			return;
		}
		
		m_IsCollapsed = collapsed;
		Children.Show(!collapsed);
		CollapseIcon.SetImage(!collapsed);
		
		if (!collapsed) {
			// Recursive
			if (m_Parent) {
				m_Parent.SetCollapsed(false);
			}
			
			bool favorite_toggle = GetEditor().GetSettings().ShowFavoriteObjects;
			string search_string = GetEditor().GetEditorHud().LeftSearchBar.GetText();
			search_string.ToLower();
			
			foreach (EditorListNode child: ChildrenItems) {
				if (!child) {
					continue;
				}
				
				bool filter = true;
				if (search_string.Length() > 3 || favorite_toggle) {
					filter = child.FilterType(search_string, favorite_toggle);
				}
				
				child.Show(filter);
			}
			
			m_QueueRecalculateSize = true;
		} else if (!m_QueueRecalculateSize) {
			RecalculateSize();
		}
		
		/*						
		if (collapsed) {
			IconImage.SetImage(3);
		} else {
			IconImage.SetImage(2);
		}*/
	}
		
	override void Show(bool show)
	{
		// Will be updated next frame due to RecalculateSize()
		m_LayoutRoot.Show(show, false);							
		if (show && m_Parent) {
			if (!m_Parent.IsVisible()) {
				m_Parent.Show(true);
			}
			
			if (m_Parent.m_IsCollapsed) {
				m_Parent.SetCollapsed(false);
			}
		}
		
		RecalculateSize();
	}
		
	protected void RecalculateSize()
	{
		m_QueueRecalculateSize = true;

		if (m_Parent && !m_Parent.m_QueueRecalculateSize) {
			m_Parent.RecalculateSize();
		}
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
				ChildrenHeight.SetColor(LinearColor.LIGHT_BLUE);
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
		return true;
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
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		SetCollapsed(!IsCollapsed());		
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.RIGHT) {
			s_ContextMenu = new EditorListNodeContextMenu(x, y, this);
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool FilterType(string filter, bool favorites)
	{
		return m_Text.Contains(filter);
	}
}