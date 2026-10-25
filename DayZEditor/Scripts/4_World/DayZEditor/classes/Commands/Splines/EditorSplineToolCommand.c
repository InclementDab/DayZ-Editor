#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorSplineToolCommand)]
#endif
class EditorSplineToolCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		EditorHud editor_hud = m_Editor.GetEditorHud();
		if (editor_hud.GetSplineDialog()) {
			editor_hud.GetSplineDialog().Delete();
		} else {
			editor_hud.ShowSplineDialog();
		}

		return true;
	}

	override string GetName()
	{
		return "Spline Tool";
	}

	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_E };
	}

	override Symbols GetSymbol()
	{
		return Symbols.BEZIER_CURVE;
	}

	override bool IsToggled()
	{
		return GetEditor().GetEditorHud() && GetEditor().GetEditorHud().GetSplineDialog();
	}
}