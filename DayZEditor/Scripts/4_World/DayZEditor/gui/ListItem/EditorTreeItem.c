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
	
	void EditorTreeItem(string name)
	{
		m_TemplateController = EditorTreeItemController.Cast(m_Controller);
		
		Text.SetText(name);
	}
		
	void OnCollapseExecute(ButtonCommandArgs args)
	{
		SetOpened(!IsOpen());
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