class EditorListNodeController: ViewController
{
	ref ObservableCollection<ref EditorListNode> ChildrenItems = new ObservableCollection<ref EditorListNode>(this);
}

class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
	
	protected EditorListNodeController m_TemplateController;
	
	protected bool m_QueueRecalculateSize;
	
	Widget Collapse, IconFrame, Hide, Panel, BoundingBox, Lock, Marker, ChildrenHeight;
	ButtonWidget CollapseButton, HideButton, BoundingBoxButton, LockButton, MarkerButton;
	ImageWidget CollapseIcon, IconImage, HideIcon, BoundingBoxIcon, LockIcon, MarkerIcon;
	TextWidget Text;
	EditBoxWidget Edit;
	WrapSpacerWidget Children;
	
	void EditorListNode()
	{
		m_TemplateController = EditorListNodeController.Cast(m_Controller);
		Collapse.Show(false);
		m_LayoutRoot.SetSort(1);
		CollapseIcon.LoadImageFile(0, "set:solid image:square_minus");
		CollapseIcon.LoadImageFile(1, "set:regular image:square_plus");
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
		m_TemplateController.ChildrenItems.Insert(list_node);
		Collapse.Show(m_TemplateController.ChildrenItems.Count());
	}
	
	void SetCollapsed(bool collapsed)
	{	
		Children.Show(!collapsed);
		CollapseIcon.SetImage(collapsed);
				
		RecalculateSize();
		
		EditorListNode node_parent = GetParentNode();
		if (!collapsed) {
			// Recursive
			if (node_parent) {
				node_parent.SetCollapsed(false);
			}
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
				
		if (show) {
			EditorListNode parent_node = GetParentNode();
			if (parent_node) {
				parent_node.Show(show);
			}
		}
		
		RecalculateSize();
	}
		
	protected void RecalculateSize()
	{
		//PrintFormat("RecalculateSize: %1", m_TemplateController.ChildrenItems.Count());
		m_QueueRecalculateSize = true;
		
		EditorListNode node_parent = GetParentNode();
		if (node_parent) {
			//node_parent.RecalculateSize();
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
	
	override typename GetControllerType()
	{
		return EditorListNodeController;
	}
	
	EditorListNode GetParentNode()
	{
		Widget parent = m_LayoutRoot.GetParent().GetParent();
		if (parent && parent.GetName() == "NodeView") {
			EditorListNode node;
			parent.GetUserData(node);
			return node;
		}
		
		return null;
	}
	
	protected override bool UseUpdateLoop()
	{
		return false;
	}
	
	bool FilterType(string filter)
	{
		return false;
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
	
	override bool FilterType(string filter)
	{
		return m_Text.Contains(filter);
	}
}