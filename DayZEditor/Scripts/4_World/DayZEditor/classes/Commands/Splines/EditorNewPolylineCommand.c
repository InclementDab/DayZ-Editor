#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorNewPolylineCommand)]
#endif
class EditorNewPolylineCommand: EditorNewSplineCommand
{
	override EditorSplineType GetSplineType()
	{
		return EditorSplineType.POLYLINE;
	}

	override string GetName()
	{
		return "New Polyline";
	}

	override Symbols GetSymbol()
	{
		return Symbols.VECTOR_POLYGON;
	}
}