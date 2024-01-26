class EditorCommandMenuItem: ScriptView
{
	Widget Panel;
	ImageWidget Background, Foreground;
	TextWidget Name, Shortcut;
	
	void EditorCommandMenuItem(typename command_type)
	{
		Command command = GetDayZGame().GetCommand(command_type);
		if (!command) {
			Error("Unregistered command " + command_type);
			return;
		}
		
		Name.SetText(command.GetDisplayName());
		Shortcut.SetText(command.GetShortcutString());
		
		if (command.GetIcon() != string.Empty) {
			Background.LoadImageFile(0, command.GetIcon().Solid());
			Background.SetImage(0);
			Foreground.LoadImageFile(0, command.GetIcon().Thin());
			Foreground.SetImage(0);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 1.0, 100);
		WidgetAnimator.AnimateColor(Name, COLOR_BLACK, 100);
		WidgetAnimator.AnimateColor(Shortcut, COLOR_BLACK, 100);
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		WidgetAnimator.Animate(Panel, WidgetAnimatorProperty.COLOR_A, 0, 100);
		WidgetAnimator.AnimateColor(Name, COLOR_WHITE, 100);
		WidgetAnimator.AnimateColor(Shortcut, COLOR_WHITE, 100);
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
	ImageWidget IconWidget;
	TextWidget Name, Shortcut;
	
	void EditorMenuItem(string name, string desc = "", Symbols icon = "")
	{
		Name.SetText(name);
		if (desc != string.Empty) {
			Shortcut.SetText(desc);
		}
		
		if (icon != string.Empty) {
			IconWidget.LoadImageFile(0, icon.Solid());
			IconWidget.SetImage(0);
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