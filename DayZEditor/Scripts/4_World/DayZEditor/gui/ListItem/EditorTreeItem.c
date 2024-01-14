class EditorTreeItemController: ViewController
{
	ref ObservableCollection<ref EditorTreeItem> Children = new ObservableCollection<ref EditorTreeItem>(this);
}

class EditorTreeItem: ScriptView
{
	protected EditorTreeItemController m_TemplateController;
	
	TextWidget Text;
	ImageWidget Icon, CollapseIcon;
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
		
		CollapseIcon.LoadImageFile(0, Ternary<string>.If(state, "set:dayz_gui image:Expand", "set:dayz_gui image:Collapse"));
		CollapseIcon.SetImage(0);
	}
	
	bool HasChildren()
	{
		return m_TemplateController.Children.Count() > 0;
	}
	
	bool IsOpen()
	{
		return Children.IsVisible();
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