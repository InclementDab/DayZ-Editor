modded class EditorSaveData
{
	[NonSerialized()]
	static const string SPLINE_CHECK = "EditorSplines";

	[NonSerialized()]
	static const int SPLINE_VERSION = 1;

	ref array<ref EditorSplineData> Splines = {};

	// Written after everything else, older editors stop reading before they get here
	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);

		serializer.Write(SPLINE_CHECK);
		serializer.Write(SPLINE_VERSION);
		serializer.Write(Splines.Count());
		foreach (EditorSplineData spline: Splines) {
			spline.Write(serializer, SPLINE_VERSION);
		}
	}

	override bool Read(Serializer serializer)
	{
		if (!super.Read(serializer)) {
			return false;
		}

		// files from before splines end here
		string spline_check;
		if (!serializer.Read(spline_check) || spline_check != SPLINE_CHECK) {
			return true;
		}

		int spline_version, spline_count;
		serializer.Read(spline_version);
		serializer.Read(spline_count);
		for (int i = 0; i < spline_count; i++) {
			EditorSplineData spline = new EditorSplineData();
			if (!spline.Read(serializer, spline_version)) {
				EditorLog.Warning("Could not read spline %1", i.ToString());
				break;
			}

			Splines.Insert(spline);
		}

		return true;
	}
}