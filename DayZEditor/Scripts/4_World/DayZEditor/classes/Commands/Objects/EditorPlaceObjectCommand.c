class EditorPlaceObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.PlaceObject();
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_PLACE_OBJECT";
	}
}

class EditorButtonScript: ScriptedWidgetEventHandler
{
	reference string CommandType;

	protected EditorCommand m_Command;

	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		
#ifndef COMPONENT_SYSTEM
		typename command_type = CommandType.ToType();		
		m_Command = GetEditor().CommandManager[command_type];
		if (!m_Command) {
			return;
		}

		ImageWidget child_image = ImageWidget.Cast(w.GetChildren());
		if (child_image) {
			string symbol_icon = m_Command.GetIcon();
			if (m_Command.GetSymbol()) {
				symbol_icon = m_Command.GetSymbol().Regular();
			}
			
			child_image.LoadImageFile(0, symbol_icon);
			child_image.SetImage(0);
		}
#endif
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
		}

		return true;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Command && !EditorHud.CurrentMenu) {
			float pos_x, pos_y, size_x, size_y;
			w.GetScreenPos(pos_x, pos_y);
			w.GetScreenSize(size_x, size_y);
							
			EditorTooltip tooltip = EditorTooltip.CreateOnButton(m_Command, w, TooltipPositions.BOTTOM_LEFT);
			if (!m_Command.CanExecute()) {
				tooltip.GetLayoutRoot().SetAlpha(100);
			}
			
			GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
		}

		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Command) {
			GetEditor().GetEditorHud().SetCurrentTooltip(null);
		}

		return true;
	}
}

[RegisterEditorCommand(EditorObjectSelectionCommand)]
class EditorObjectSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().ToggleObjectSelect();
		return true;
	}
	
	override string GetName()
	{
		return "Objects Are Clickable";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.HOUSE_BLANK;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_U };
	}
}
[RegisterEditorCommand(EditorBoxSelectionCommand)]
class EditorBoxSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.BOX);
		return true;
	}
	
	override string GetName()
	{
		return "Box Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.SQUARE_DASHED;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_I };
	}
}

[RegisterEditorCommand(EditorEllipseSelectionCommand)]
class EditorEllipseSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.ELLIPSE);
		return true;
	}
	
	override string GetName()
	{
		return "Circle Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.CIRCLE_DASHED;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_O };
	}
}

[RegisterEditorCommand(EditorLassoSelectionCommand)]
class EditorLassoSelectionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		GetEditor().GetEditorHud().SetSelectionMode(SelectionMode.LASSO);
		return true;
	}
	
	override string GetName()
	{
		return "Lasso Selection";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.LASSO;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_P };
	}
}