class EditorTreeItemController: ViewController
{
	ref ObservableCollection<ref EditorTreeItem> Children = new ObservableCollection<ref EditorTreeItem>(this);
	
	Widget Icon;
	Widget CollapseWrapper;
	
	override void CollectionChanged(string collection_name, CollectionChangedEventArgs args)
	{
		Icon.Show(Children.Count() > 0);
		CollapseWrapper.Show(Children.Count() > 0);
		
		Param1<EditorTreeItem> item = Param1<EditorTreeItem>.Cast(args.ChangedValue);
		EditorTreeItem script_view = EditorTreeItem.Cast(GetParent());
		switch (args.ChangedAction) {
			case NotifyCollectionChangedAction.Insert:
			case NotifyCollectionChangedAction.InsertAt: {
				item.param1.SetParentTree(script_view);
				item.param1.ParentDisplay.Show(true);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				item.param1.SetParentTree(null);
				break;
			}
		}
	}
}

class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	protected EditorTreeItem m_Parent;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon, ParentDisplay, TreeDisplay;
	WrapSpacerWidget Children;
	
	Widget Panel;
	
	void EditorTreeItem(string name)
	{
		m_TemplateController = EditorTreeItemController.Cast(m_Controller);
		
		Text.SetText(name);
		
		SizeToChild size_to_child;
		Panel.GetScript(size_to_child);
		size_to_child.ResizeParentToChild();
	}
			
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		SetOpened(!IsOpen());
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case Panel: {
				
				break;
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Panel: {
				SetFocus(w);
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {
				WidgetAnimator.AnimateColor(Panel, ARGB(255, 75, 119, 190), 50);
				return true;
			}
		}
		
		return super.OnFocus(w, x, y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		switch (w) {
			case Panel: {
				WidgetAnimator.AnimateColor(Panel, ARGB(0, 0, 0, 0), 50);
				return true;
			}
		}
	
		
		return super.OnFocusLost(w, x, y);
	}
	
	void SetOpened(bool state)
	{
		Children.Show(state);
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(!state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
		
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		
		m_LayoutRoot.SetScreenSize(w, 18 + (state * 18 * m_TemplateController.Children.Count()));
	}
	
	bool HasChildren()
	{
		return m_TemplateController.Children.Count() > 0;
	}
	
	bool IsOpen()
	{
		return Children.IsVisible();
	}
	
	void SetParentTree(EditorTreeItem parent)
	{
		m_Parent = parent;
	}
	
	EditorTreeItem GetParentTree()
	{
		return m_Parent;
	}
	
	EditorTreeItemController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	override typename GetControllerType()
	{
		return EditorTreeItemController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\items\\EditorTreeItem.layout";
	}
}