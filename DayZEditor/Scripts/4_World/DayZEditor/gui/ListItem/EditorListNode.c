class EditorListNodeController: ViewController
{
	ref ObservableCollection<ref EditorListNode> ChildrenItems = new ObservableCollection<ref EditorListNode>(this);
}

class EditorListNode: ScriptView
{	
	static EditorListNode s_SelectedNode;
	
	protected EditorListNodeController m_TemplateController;
	
	Widget Collapse, IconFrame, Hide, Panel;
	ButtonWidget CollapseButton, HideButton;
	ImageWidget CollapseIcon, IconImage, HideIcon;
	TextWidget Text;
	EditBoxWidget Edit;
	WrapSpacerWidget Children;
	
	void EditorListNode()
	{
		m_TemplateController = EditorListNodeController.Cast(m_Controller);
		Collapse.Show(false);
	}
		
	void InsertChild(notnull EditorListNode list_node)
	{
		m_TemplateController.ChildrenItems.Insert(list_node);
		Collapse.Show(m_TemplateController.ChildrenItems.Count());
		
		//RecalculateSize();
	}
	
	void SetCollapsed(bool collapsed)
	{
		Children.Show(!collapsed);
		Symbols collapse_icon = Symbols.SQUARE_PLUS;
		if (!collapsed) {
			collapse_icon = Symbols.SQUARE_MINUS;
		}
		
		collapse_icon.Load(CollapseIcon, 0);
		
		Widget parent = m_LayoutRoot.GetParent().GetParent();
		while (parent && parent.GetName() == "NodeView") {
			EditorListNode node;
			parent.GetUserData(node);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(node.RecalculateSize);			
			parent = parent.GetParent().GetParent();
		}
		
		if (!collapsed) {
			IconImage.SetImage(3);
		} else {
			IconImage.SetImage(2);
		}
		
		RecalculateSize();
	}
	
	void RecalculateSize()
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
		if (s_SelectedNode) {
			s_SelectedNode.Panel.SetColor(0);
		}
		
		s_SelectedNode = this;
		Panel.SetColor(g_Editor.GetSettings().SelectionColor);
				
		return true;
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

class EditorPlaceableListNode: EditorListNode
{
	protected EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListNode(notnull EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		Text.SetText(m_PlaceableItem.Name);
		if (placeable_item.Type.Contains(".p3d")) {
			Symbols.TREE_DECIDUOUS.Load(IconImage, 3);
		} else {
			Symbols.HOUSE_BLANK.Load(IconImage, 3);
		}
	}
				
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorPlaceableTooltip tooltip = new EditorPlaceableTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetPosition(pos_x + size_x + 7, pos_y);
		
		//! bugfix
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		
		bool preview_disabled = GetGame().IsKindOf(m_PlaceableItem.Type, "Man") || GetGame().IsKindOf(m_PlaceableItem.Type, "DZ_LightAI");
		if (m_PlaceableItem && !preview_disabled) {
			Object preview = GetGame().CreateObjectEx(m_PlaceableItem.Type, Vector(0, -1000, 0), ECE_LOCAL);
			if (!preview) {
				// DOESNT WORK @JACOB
				string new_type = GetEditor().GetObjectManager().ConvertP3dFileToPotentialObjectType(m_PlaceableItem.Type);
				if (new_type) {
					preview = GetGame().CreateObjectEx(new_type, Vector(0, -1000, 0), ECE_LOCAL);
				}
			}

			string display_name = string.Empty;
			if (preview) {
				display_name = preview.GetDisplayName();
			}
				
			if (!display_name) {
				display_name = m_PlaceableItem.Type;
			}

			tooltip.SetTitle(display_name);	

			if (preview) {
				tooltip.SetContent(preview);
			}
		}		
		
		GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		GetEditor().GetEditorHud().ClearCurrentTooltip();
		return true;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		GetEditor().GetObjectManager().CurrentSelectedItem = m_PlaceableItem;
		switch (button) {
			case 0: {
				GetEditor().ClearHand();
				GetEditor().AddInHand(m_PlaceableItem);
				break;
			}
			
			case 1: {
				EditorHud.CurrentMenu = new EditorPlaceableContextMenu(x, y, m_PlaceableItem);
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		if (!GetEditor().IsPlacing()) {
			GetEditor().AddInHand(m_PlaceableItem);
			w.Show(false);
		}
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager[EditorPlaceObjectCommand].Execute(this, null);
			w.Show(true);
		}
		
		return super.OnDrop(w, x, y, reciever);
	}
}