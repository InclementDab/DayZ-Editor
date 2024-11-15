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
	protected ImageWidget Icon;

	void OnWidgetScriptInit(Widget w)
	{
		w.SetHandler(this);
		
#ifndef COMPONENT_SYSTEM
		typename command_type = CommandType.ToType();		
		m_Command = GetEditor().CommandManager[command_type];
		if (!m_Command) {
			return;
		}

		Icon = ImageWidget.Cast(w.GetChildren());
		if (Icon) {
			Icon.SetImage(1);
		}
		
		w.SetColor(GetEditor().GetSettings().HighlightColor);
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
#endif
	}

	protected void Update(float dt)
	{
		if (m_Command && Icon) {
	
			if (m_Command.IsToggled()) {
				Icon.SetColor(m_Command.GetColor());
			} else {
				Icon.SetColor(LinearColor.WHITE);
			}

			if (!m_Command.CanExecute()) {
				Icon.SetAlpha(0.3);
			} else {
				Icon.SetAlpha(1.0);
			}
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
			
			if (m_Command.IsToggled()) {
				Icon.SetImage(3);
			} else {
				Icon.SetImage(2);
			}
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
			
			if (!m_Command.IsToggled()) {
				Icon.SetImage(2);
			}
						
			GetEditor().GetEditorHud().SetCurrentTooltip(tooltip);
		}

		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_Command) {
			GetEditor().GetEditorHud().SetCurrentTooltip(null);
			
			if (!m_Command.IsToggled()) {
				Icon.SetImage(1);
			}
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

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetObjectSelect();
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
	
	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.BOX;
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

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.ELLIPSE;
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

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud().GetSelectionMode() == SelectionMode.LASSO;
	}
}

[RegisterEditorCommand(EditorTogglePlayerCommand)]
class EditorTogglePlayerCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		GetEditor().SetPlayerControlled(!GetEditor().IsPlayerControlled());

		return true;
	}

	override string GetName()
	{
		return "#STR_EDITOR_CMD_CONTROL_PLAYER";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.PERSON_WALKING;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LSHIFT, KeyCode.KC_P };
	}
	
	override bool IsToggled()
	{
		return GetEditor().IsPlayerControlled();
	}
}