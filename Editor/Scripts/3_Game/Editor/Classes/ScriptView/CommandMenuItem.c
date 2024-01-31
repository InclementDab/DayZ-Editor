class CommandMenuItemController: ViewController
{
	ref ObservableCollection<TreeView> Children = new ObservableCollection<TreeView>(this);
}

class CommandMenuItem: ScriptView
{
	protected CommandMenuItemController m_TemplateController;
	
	Widget Panel, Shortcut, Caret;
	ImageWidget Icon;
	TextWidget Name, ShortcutText;
	
	void CommandMenuItem(notnull CommandNode node)
	{		
		Name.SetText(node.GetDisplayName());
		
		if (node.GetShortcutString() != string.Empty) {
			Shortcut.Show(true);
			ShortcutText.SetText(node.GetShortcutString());
		}
		
		if (node.Children.Count() > 0) {
			foreach (string uuid, TreeNode node1: node.Children) {
				//m_TemplateController.Children.Insert(node1.GetNodeView());
			}
			
			Caret.Show(true);
			Shortcut.Show(false);
		}
		
		if (node.GetIcon() != string.Empty) {
			Icon.LoadImageFile(0, node.GetIcon().Solid());
			Icon.SetImage(0);
			Icon.Show(true);
		}
	}
	

	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Menu\\Item.layout";
	}
	
	override typename GetControllerType()
	{
		return CommandMenuItemController;
	}
}