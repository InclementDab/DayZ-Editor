class EditorCommandMenuItem: ScriptView
{
	Widget Panel;
	ImageWidget Icon;
	TextWidget Name, Shortcut;
	
	void EditorCommandMenuItem(typename command_type)
	{
		Command command = GetDayZGame().GetCommand(command_type);
		if (!command) {
			Error("Unregistered command " + command_type);
			return;
		}
		
		Name.SetText(command.DisplayName);
		Shortcut.SetText(command.GetShortcutString());
		
		if (command.Icon != string.Empty) {
			Icon.LoadImageFile(0, command.Icon.Solid());
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