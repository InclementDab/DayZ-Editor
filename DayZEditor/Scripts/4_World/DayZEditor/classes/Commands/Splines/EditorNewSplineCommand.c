#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorNewSplineCommand)]
#endif
class EditorNewSplineCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);

		m_Editor.GetEditorHud().ShowSplineDialog();

		// an empty spline of the same type is reused, clicking twice does not leave empty splines behind
		EditorSplineTool spline_tool = m_Editor.GetSplineTool();
		EditorSplineData spline = spline_tool.GetActiveSpline();
		if (spline && spline.Type == GetSplineType() && spline.Count() == 0) {
			spline_tool.SetEditMode(true);
			return true;
		}

		spline_tool.CreateSpline(GetSplineType());
		return true;
	}

	EditorSplineType GetSplineType()
	{
		return EditorSplineType.SPLINE;
	}

	override string GetName()
	{
		return "New Spline";
	}

	override Symbols GetSymbol()
	{
		return Symbols.BEZIER_CURVE;
	}

	// lit while a spline of this type is being edited
	override bool IsToggled()
	{
		if (!GetEditor().GetEditorHud() || !GetEditor().GetEditorHud().GetSplineDialog()) {
			return false;
		}

		EditorSplineTool spline_tool = GetEditor().GetSplineTool();
		EditorSplineData spline = spline_tool.GetActiveSpline();
		return spline_tool.IsEditMode() && spline && spline.Type == GetSplineType();
	}
}