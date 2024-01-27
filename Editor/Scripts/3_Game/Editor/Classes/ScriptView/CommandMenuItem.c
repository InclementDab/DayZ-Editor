class CommandMenuItemController: ViewController
{
	ref ObservableCollection<TreeView> Children = new ObservableCollection<TreeView>(this);
}

class CommandMenuItem: ScriptViewTemplate<CommandMenuItemController>
{
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
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.AnimateColor(Panel, EditorColors.SELECT, 1);
		WidgetAnimator.AnimateColor(ShortcutText, COLOR_WHITE, 1);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.AnimateColor(Panel, EditorColors.BACKGROUND_1, 100);
		
		WidgetAnimator.AnimateColor(ShortcutText, ARGB(255, 131, 131, 149), 100);
		return true;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Menu\\Item.layout";
	}
}