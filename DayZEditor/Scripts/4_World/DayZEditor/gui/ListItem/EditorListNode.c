class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
		
	ref array<ref EditorListNode> ChildrenItems = {};
	
	protected bool m_QueueRecalculateSize;
	protected EditorListNode m_Parent;
	
	Widget Collapse, IconFrame, Hide, Panel, BoundingBox, Lock, Marker, ChildrenHeight, Favorite;
	ButtonWidget CollapseButton, HideButton, BoundingBoxButton, LockButton, MarkerButton, FavoriteButton;
	ImageWidget CollapseIcon, IconImage, HideIcon, BoundingBoxIcon, LockIcon, MarkerIcon, FavoriteIcon;
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
		
	override void Update(float dt)
	{
		super.Update(dt);
		
		if (m_QueueRecalculateSize) {
			float w, h, x, y;
			Children.Update();
			Children.GetScreenSize(w, h);	
			
			m_LayoutRoot.GetScreenSize(x, y);
			m_LayoutRoot.SetScreenSize(x, h * Children.IsVisible() + 30);
					
			ChildrenHeight.SetSize(2, h * Children.IsVisible());
						
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
		if (IsCollapsed() == collapsed) {
			m_QueueRecalculateSize = true;
			return;
		}
		
		Children.Show(!collapsed);
		CollapseIcon.SetImage(!collapsed);
						
		if (!collapsed) {
			// Recursive
			if (m_Parent) {
				m_Parent.SetCollapsed(false);
			}
			
			bool favorite_toggle = GetEditor().GetSettings().ShowFavoriteObjects;
			string search_string = GetEditor().GetEditorHud().LeftSearchBar.GetText();
			
			foreach (EditorListNode child: ChildrenItems) {
				child.GetLayoutRoot().Show(child.FilterType(search_string, favorite_toggle));
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
		super.Show(show);
				
		if (show) {
			if (m_Parent) {
				m_Parent.Show(show);
			}
			
			m_QueueRecalculateSize = true;
			
		} else if (!m_QueueRecalculateSize) {
			RecalculateSize();
		}
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
	
	override bool OnUpdate(Widget w)
	{
		return true;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
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
	
	override bool FilterType(string filter, bool favorites)
	{
		return m_Text.Contains(filter);
	}
}