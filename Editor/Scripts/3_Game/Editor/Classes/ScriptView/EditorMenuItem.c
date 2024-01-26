class EditorCommandMenuItem: ScriptView
{
	Widget Panel, Shortcut, Caret;
	ImageWidget Icon;
	TextWidget Name, ShortcutText;
	
	void EditorCommandMenuItem(EditorNode node)
	{		
		Name.SetText(node.GetDisplayName());
		
		if (node.GetShortcutString() != string.Empty) {
			Shortcut.Show(true);
			ShortcutText.SetText(node.GetShortcutString());
		}
		
		if (node.GetChildren().Count() > 0) {
			
			map<string, ref EditorNode> nodes = node.GetChildren();
			foreach (string uuid, EditorNode node1: nodes) {
				
				
			}
			
			Caret.Show(true);
		}
		
		if (node.GetIcon() != string.Empty) {
			Icon.LoadImageFile(0, node.GetIcon().Solid());
			Icon.SetImage(0);
			Icon.Show(true);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 0, 100);
		return true;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Menu\\Item.layout";
	}
}

class EditorMenuItem: ScriptView
{
	Widget Panel;
	ImageWidget Icon;
	TextWidget Name, Shortcut;
	
	void EditorMenuItem(string name, string desc = "", Symbols icon = "")
	{
		Name.SetText(name);
		if (desc != string.Empty) {
			Shortcut.SetText(desc);
		}
		
		if (icon != string.Empty) {
			Icon.LoadImageFile(0, icon.Solid());
			Icon.SetImage(0);
			Icon.Show(true);
		}
	}	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
				
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 0, 100);
		
		return true;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Menu\\Item.layout";
	}
}