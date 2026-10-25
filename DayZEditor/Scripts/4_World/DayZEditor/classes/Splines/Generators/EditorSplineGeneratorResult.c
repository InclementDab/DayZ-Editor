class EditorSplinePlacement
{
	string Type;
	vector Transform[4];

	// Preview marker, a line standing on the ground with a tick on top the way the object faces
	vector MarkerPosition;
	vector MarkerDirection;
	float MarkerHeight;
	LinearColor Color;
}

class EditorSplinePreviewLine
{
	vector From;
	vector To;
	LinearColor Color;
}

// The preview and Generate both use this, so what you see is what gets placed
class EditorSplineGeneratorResult
{
	static const int MAX_PLACEMENTS = 25000;

	ref array<ref EditorSplinePlacement> Placements = {};
	ref array<ref EditorSplinePreviewLine> Lines = {};
	int Missing;
	int NotConsoleFriendly;
	int Skipped;
	bool Capped;
	string Message;

	bool IsFull()
	{
		return Placements.Count() >= MAX_PLACEMENTS;
	}

	EditorSplinePlacement Add(string type, vector transform[4], vector marker_position, vector marker_direction, LinearColor color, float marker_height)
	{
		if (IsFull()) {
			Capped = true;
			return null;
		}

		EditorSplinePlacement placement = new EditorSplinePlacement();
		placement.Type = type;
		for (int i = 0; i < 4; i++) {
			placement.Transform[i] = transform[i];
		}

		placement.MarkerPosition = marker_position;
		placement.MarkerDirection = marker_direction;
		placement.MarkerHeight = marker_height;
		placement.Color = color;
		Placements.Insert(placement);
		return placement;
	}

	void AddLine(vector from, vector to, LinearColor color)
	{
		EditorSplinePreviewLine line = new EditorSplinePreviewLine();
		line.From = from;
		line.To = to;
		line.Color = color;
		Lines.Insert(line);
	}

	string GetSummary()
	{
		string summary = string.Format("%1 objects", Placements.Count());
		if (Capped) {
			summary += string.Format(" (limit of %1 reached)", MAX_PLACEMENTS);
		}

		if (NotConsoleFriendly > 0) {
			summary += string.Format(" | %1 assets skipped: not console friendly", NotConsoleFriendly);
		}

		if (Missing > 0) {
			summary += string.Format(" | %1 assets not found", Missing);
		}

		if (Skipped > 0) {
			summary += string.Format(" | %1 left out", Skipped);
		}

		if (Message != string.Empty) {
			summary = Message + "   " + summary;
		}

		return summary;
	}
}