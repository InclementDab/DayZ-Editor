class EditorSplinePresetFile: EditorFileType
{
	static const string EXTENSION = ".json";

	override Managed Import(string file)
	{
		return EditorSplinePresets.Load(file);
	}

	override void Export(Managed data, string file)
	{
		EditorSplineGeneratorSettings settings = EditorSplineGeneratorSettings.Cast(data);
		if (settings) {
			EditorSplinePresets.Save(file, settings);
		}
	}

	// Opens in the folder of the generator the active spline uses
	override string GetDirectory()
	{
		EditorSplineData spline = GetEditor().GetSplineTool().GetActiveSpline();
		if (!spline) {
			return EditorSplinePresets.GetDirectory();
		}

		string directory = EditorSplinePresets.GetDirectory(spline.Generator.Type);
		if (!FileExist(directory)) {
			return EditorSplinePresets.GetDirectory();
		}

		return directory;
	}

	override eDialogExtraSetting GetExportSettings()
	{
		return 0;
	}

	override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		super.GetValidExtensions(valid_extensions);
		valid_extensions.Insert(new Param2<string, string>("Generator Preset", "*" + EXTENSION));
	}

	override string GetExtension()
	{
		return EXTENSION;
	}
}