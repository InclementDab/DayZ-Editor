class EditorSplineDialogController: ViewController
{
	ref ObservableCollection<ref EditorSplineListItem> Splines = new ObservableCollection<ref EditorSplineListItem>(this);
	ref ObservableCollection<ref ScriptView> Properties = new ObservableCollection<ref ScriptView>(this);
}

// Not a modal dialog, the world stays clickable so points can be placed while it is open
class EditorSplineDialog: EditorModal
{
	protected static float s_LastPositionX = -1;
	protected static float s_LastPositionY = -1;

	protected EditorSplineDialogController m_TemplateController;
	protected Editor m_Editor;
	protected EditorSplineTool m_SplineTool;

	// iterating the observable collections directly is not reliable
	protected ref array<EditorSplineListItem> m_SplineListItems = {};
	protected int m_Revision = -1;
	protected int m_ListRevision = -1;
	protected string m_SplineListKey;
	protected string m_PropertiesKey;
	protected bool m_RebuildProperties = true;
	protected bool m_Refreshing;

	ButtonWidget TitleClose;
	ButtonWidget NewSplineButton, NewPolylineButton, DeleteButton, EditModeButton, PresetsButton;
	ButtonWidget TypeButton, ReverseButton, DropButton, FocusButton;
	ButtonWidget GeneratorPrevButton, GeneratorNextButton;
	ButtonWidget GenerateButton, ClearButton, SavePresetButton;
	CheckBoxWidget SnapCheck, ClosedCheck, PreviewCheck, ConsoleCheck;
	EditBoxWidget SplineNameBox, TensionBox;
	TextWidget SplineCountText, SplinesEmptyText, InfoText, GeneratorText, StatusText, TensionLabel;
	Widget SplinePanel, TensionBoxWrapper;

	void EditorSplineDialog()
	{
		m_TemplateController = EditorSplineDialogController.Cast(m_Controller);
		m_Editor = GetEditor();
		m_SplineTool = m_Editor.GetSplineTool();

		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		float size_x, size_y;
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		if (s_LastPositionX >= 0) {
			m_LayoutRoot.SetScreenPos(s_LastPositionX, s_LastPositionY);
		} else {
			m_LayoutRoot.SetScreenPos(Math.Max(10, screen_x - size_x - 400), Math.Max(10, (screen_y - size_y) / 2));
		}

		m_SplineTool.SetEnabled(true);
		m_Editor.GetEditorHud().ShowCursor(true);
		Refresh();
	}

	void ~EditorSplineDialog()
	{
		if (m_LayoutRoot) {
			m_LayoutRoot.GetScreenPos(s_LastPositionX, s_LastPositionY);
		}

		if (GetEditor()) {
			GetEditor().GetSplineTool().SetEnabled(false);
		}
	}

	override void Update(float dt)
	{
		if (!m_SplineTool.IsEnabled()) {
			m_SplineTool.SetEnabled(true);
		}

		if (m_SplineTool.GetListRevision() != m_ListRevision) {
			m_ListRevision = m_SplineTool.GetListRevision();
			m_SplineListKey = string.Empty;
			m_RebuildProperties = true;
		}

		if (m_SplineTool.GetRevision() != m_Revision || m_RebuildProperties) {
			m_Revision = m_SplineTool.GetRevision();
			Refresh();
		}
	}

	protected void Refresh()
	{
		m_Refreshing = true;
		RefreshSplines();
		RefreshSplinePanel();
		RefreshProperties();
		RefreshStatus();
		m_Refreshing = false;
	}

	protected void RefreshSplines()
	{
		array<ref EditorSplineData> splines = m_SplineTool.GetSplines();
		string list_key;
		foreach (EditorSplineData key_spline: splines) {
			list_key += string.Format("%1|%2|%3|%4|%5|%6;", key_spline.Uuid, key_spline.Name, key_spline.Type, key_spline.Count(), key_spline.Generator.Type, key_spline.Generated.Count());
		}

		if (list_key != m_SplineListKey) {
			m_SplineListKey = list_key;
			m_TemplateController.Splines.Clear();
			m_SplineListItems.Clear();
			foreach (EditorSplineData spline: splines) {
				EditorSplineListItem list_item = new EditorSplineListItem(spline, ScriptCaller.Create(OnSplineClicked));
				m_TemplateController.Splines.Insert(list_item);
				m_SplineListItems.Insert(list_item);
			}
		}

		EditorSplineData active_spline = m_SplineTool.GetActiveSpline();
		foreach (EditorSplineListItem spline_list_item: m_SplineListItems) {
			spline_list_item.SetSelected(active_spline && spline_list_item.GetUuid() == active_spline.Uuid);
		}

		SplinesEmptyText.Show(splines.Count() == 0);
		SplineCountText.SetText(splines.Count().ToString());

		if (m_SplineTool.IsEditMode()) {
			EditModeButton.SetText("Edit Points: ON");
		} else {
			EditModeButton.SetText("Edit Points: OFF");
		}

		SnapCheck.SetChecked(m_SplineTool.GetSnapToTerrain());
		PreviewCheck.SetChecked(m_SplineTool.GetShowPreview());
		ConsoleCheck.SetChecked(m_SplineTool.GetConsoleOnly());
	}

	protected void RefreshSplinePanel()
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		SplinePanel.Show(spline != null);
		if (!spline) {
			InfoText.SetText("Select a spline in the list or click its line, or press New Spline.");
			return;
		}

		if (GetFocus() != SplineNameBox) {
			SplineNameBox.SetText(spline.Name);
		}

		ClosedCheck.SetChecked(spline.Closed);
		if (spline.IsSpline()) {
			TypeButton.SetText("Make Polyline");
		} else {
			TypeButton.SetText("Make Spline");
		}

		TensionLabel.Show(spline.IsSpline());
		TensionBoxWrapper.Show(spline.IsSpline());
		if (GetFocus() != TensionBox) {
			TensionBox.SetText(EditorSplineMath.FormatNumber(spline.Tension));
		}

		string info = string.Format("%1 points  |  %2", spline.Count(), EditorSplineMath.FormatLength(m_SplineTool.GetPath(spline).GetLength()));
		int selected_point = m_SplineTool.GetSelectedPoint();
		if (selected_point >= 0 && selected_point < spline.Count()) {
			vector point = spline.Points[selected_point];
			info += string.Format("  |  point %1 selected, %2 m above ground", selected_point + 1, EditorSplineMath.FormatNumber(point[1] - EditorSplineMath.TerrainY(point), 1));
		} else if (m_SplineTool.IsEditMode()) {
			info += "  |  Ctrl + Click add, Alt + Click add before, Shift + Drag height";
		}

		InfoText.SetText(info);
	}

	protected void RefreshStatus()
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			GeneratorText.SetText("-");
			StatusText.SetText(string.Empty);
			return;
		}

		GeneratorText.SetText(EditorSplineGeneratorSettings.GetTypeName(spline.Generator.Type));
		if (spline.Generator.Type == EditorSplineGeneratorType.NONE) {
			StatusText.SetText("Choose a generator with < > or open Presets...");
			return;
		}

		string status = "Updating preview...";
		EditorSplineGeneratorResult preview = m_SplineTool.GetPreview();
		if (!m_SplineTool.IsPreviewPending() && preview) {
			status = "Preview: " + preview.GetSummary();
		}

		if (spline.Generated.Count() > 0) {
			status += string.Format("  |  %1 placed", spline.Generated.Count());
		}

		StatusText.SetText(status);
	}

	protected void RefreshProperties()
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		string properties_key;
		if (spline) {
			EditorSplineGeneratorSettings settings = spline.Generator;
			properties_key = string.Format("%1|%2|%3|%4|%5|%6", spline.Uuid, settings.Type, settings.RoadKit, settings.Wall, settings.Pole, settings);
		}

		if (properties_key == m_PropertiesKey && !m_RebuildProperties) {
			return;
		}

		m_PropertiesKey = properties_key;
		m_RebuildProperties = false;

		m_TemplateController.Properties.Clear();
		if (spline) {
			CreateProperties(spline.Generator);
		}
	}

	protected void AddContent(ScriptView content)
	{
		content.SetParent(this);
		m_TemplateController.Properties.Insert(content);
	}

	protected void CreateProperties(EditorSplineGeneratorSettings settings)
	{
		if (settings.Type == EditorSplineGeneratorType.NONE) {
			return;
		}

		GroupPrefab general_group = new GroupPrefab("General", settings, string.Empty);
		general_group.Insert(new EditBoxPrefab("Preset Name", settings, "PresetName"));
		general_group.Insert(new EditBoxNumberPrefab("Random Seed", settings, "Seed"));
		AddContent(general_group);

		switch (settings.Type) {
			case EditorSplineGeneratorType.ASSET: {
				AddContent(CreateAssetGroup("Assets", "Assets", settings.Assets));

				GroupPrefab asset_group = new GroupPrefab("Placement", settings, string.Empty);
				asset_group.Insert(CreatePickModes(settings));
				DropdownListPrefab<EditorSplinePlaceMode> place_mode = new DropdownListPrefab<EditorSplinePlaceMode>("Placement", settings, "PlaceMode");
				place_mode["Along The Line"] = EditorSplinePlaceMode.ALONG;
				place_mode["On Every Point"] = EditorSplinePlaceMode.ON_POINTS;
				asset_group.Insert(place_mode);
				asset_group.Insert(new EditBoxNumberPrefab("Spacing", settings, "Spacing", 0.5));
				asset_group.Insert(new EditBoxNumberPrefab("Start Offset", settings, "StartOffset", 0.5));

				DropdownListPrefab<EditorSplineDirectionMode> direction_mode = new DropdownListPrefab<EditorSplineDirectionMode>("Facing", settings, "DirectionMode");
				direction_mode["Follow The Line"] = EditorSplineDirectionMode.FOLLOW;
				direction_mode["Reverse"] = EditorSplineDirectionMode.REVERSE;
				direction_mode["Sideways"] = EditorSplineDirectionMode.SIDEWAYS;
				direction_mode["Random"] = EditorSplineDirectionMode.RANDOM;
				direction_mode["Fixed"] = EditorSplineDirectionMode.FIXED;
				asset_group.Insert(direction_mode);
				asset_group.Insert(new EditBoxNumberPrefab("Yaw Offset", settings, "YawOffset", 5));
				asset_group.Insert(new EditBoxNumberPrefab("Random Yaw", settings, "YawJitter", 5));
				asset_group.Insert(new EditBoxNumberPrefab("Side Offset", settings, "SideOffset", 0.5));
				asset_group.Insert(new CheckBoxPrefab("Both Sides", settings, "BothSides"));
				asset_group.Insert(new EditBoxNumberPrefab("Random Offset", settings, "PositionJitter", 0.1));
				asset_group.Insert(new EditBoxNumberPrefab("Height Offset", settings, "HeightOffset", 0.1));
				asset_group.Insert(new CheckBoxPrefab("Snap To Terrain", settings, "SnapToTerrain"));
				asset_group.Insert(new CheckBoxPrefab("Tilt With Terrain", settings, "AlignToTerrain"));
				asset_group.Insert(new EditBoxNumberPrefab("Scale Min", settings, "ScaleMin", 0.05));
				asset_group.Insert(new EditBoxNumberPrefab("Scale Max", settings, "ScaleMax", 0.05));
				AddContent(asset_group);
				break;
			}

			case EditorSplineGeneratorType.ROAD: {
				GroupPrefab road_group = new GroupPrefab("Road", settings, string.Empty);
				DropdownListPrefab<string> road_kit = new DropdownListPrefab<string>("Road Kit", settings, "RoadKit");
				foreach (string kit: EditorSplineAssetLists.ROAD_KITS) {
					road_kit[kit] = kit;
				}

				road_group.Insert(road_kit);
				road_group.Insert(new CheckBoxPrefab("Use Curve Parts", settings, "RoadCurves"));
				road_group.Insert(new EditBoxNumberPrefab("Max Deviation", settings, "MaxDeviation", 0.05));
				road_group.Insert(new CheckBoxPrefab("Follow Terrain", settings, "SnapToTerrain"));
				road_group.Insert(new EditBoxNumberPrefab("Height Offset", settings, "HeightOffset", 0.01));
				AddContent(road_group);

				if (settings.RoadKit == "custom") {
					AddContent(CreateAssetGroup("Road Parts", "RoadPieces", settings.RoadPieces));
				}

				break;
			}

			case EditorSplineGeneratorType.WALL: {
				GroupPrefab wall_group = new GroupPrefab("Wall", settings, string.Empty);
				wall_group.Insert(new EditorSplineAssetInputView("Wall", "Wall", true, ScriptCaller.Create(OnAssetEntered), ScriptCaller.Create(OnAssetSelected), null));
				if (settings.Wall != string.Empty) {
					wall_group.Insert(new EditorSplineAssetView("Wall", 0, settings.Wall, ScriptCaller.Create(OnAssetRemoved)));
				}

				wall_group.Insert(new EditorSplineAssetInputView("Post", "Pole", true, ScriptCaller.Create(OnAssetEntered), ScriptCaller.Create(OnAssetSelected), null));
				if (settings.Pole != string.Empty) {
					wall_group.Insert(new EditorSplineAssetView("Pole", 0, settings.Pole, ScriptCaller.Create(OnAssetRemoved)));
				}

				DropdownListPrefab<EditorSplinePoleMode> pole_mode = new DropdownListPrefab<EditorSplinePoleMode>("Posts", settings, "PoleMode");
				pole_mode["None"] = EditorSplinePoleMode.NONE;
				pole_mode["At Corners"] = EditorSplinePoleMode.CORNERS;
				pole_mode["Between Pieces"] = EditorSplinePoleMode.EVERY_PIECE;
				wall_group.Insert(pole_mode);

				DropdownListPrefab<EditorSplineFitMode> fit_mode = new DropdownListPrefab<EditorSplineFitMode>("Fit", settings, "FitMode");
				fit_mode["Fill"] = EditorSplineFitMode.FILL;
				fit_mode["Whole Pieces Only"] = EditorSplineFitMode.INSIDE;
				fit_mode["Spread Evenly"] = EditorSplineFitMode.SPREAD;
				wall_group.Insert(fit_mode);

				wall_group.Insert(new EditBoxNumberPrefab("Piece Length", settings, "LengthOverride", 0.1));
				wall_group.Insert(new EditBoxNumberPrefab("Overlap", settings, "Overlap", 0.05));
				wall_group.Insert(new CheckBoxPrefab("Flip Side", settings, "WallFlip"));
				wall_group.Insert(new CheckBoxPrefab("Tilt With Slope", settings, "FollowSlope"));
				wall_group.Insert(new CheckBoxPrefab("Snap To Terrain", settings, "SnapToTerrain"));
				wall_group.Insert(new EditBoxNumberPrefab("Sink", settings, "Sink", 0.05));
				wall_group.Insert(new EditBoxNumberPrefab("Height Offset", settings, "HeightOffset", 0.05));
				AddContent(wall_group);
				break;
			}

			case EditorSplineGeneratorType.FOOTPATH: {
				AddContent(CreateAssetGroup("Pieces", "PathPieces", settings.PathPieces));

				GroupPrefab footpath_group = new GroupPrefab("Footpath", settings, string.Empty);
				footpath_group.Insert(CreatePickModes(settings));
				footpath_group.Insert(new EditBoxNumberPrefab("Piece Length", settings, "LengthOverride", 0.1));
				footpath_group.Insert(new EditBoxNumberPrefab("Overlap", settings, "Overlap", 0.05));
				footpath_group.Insert(new CheckBoxPrefab("Lie On Slope", settings, "AlignToTerrain"));
				footpath_group.Insert(new CheckBoxPrefab("Snap To Terrain", settings, "SnapToTerrain"));
				footpath_group.Insert(new EditBoxNumberPrefab("Random Yaw", settings, "YawJitter", 1));
				footpath_group.Insert(new EditBoxNumberPrefab("Random Offset", settings, "PositionJitter", 0.05));
				footpath_group.Insert(new EditBoxNumberPrefab("Height Offset", settings, "HeightOffset", 0.01));
				footpath_group.Insert(new EditBoxNumberPrefab("Scale Min", settings, "ScaleMin", 0.05));
				footpath_group.Insert(new EditBoxNumberPrefab("Scale Max", settings, "ScaleMax", 0.05));
				AddContent(footpath_group);
				break;
			}

			case EditorSplineGeneratorType.FOREST: {
				AddContent(CreateAssetGroup("Trees", "Trees", settings.Trees));
				AddContent(CreateAssetGroup("Bushes", "Bushes", settings.Bushes));
				AddContent(CreateAssetGroup("Rocks", "Rocks", settings.Rocks));

				GroupPrefab forest_group = new GroupPrefab("Forest", settings, string.Empty);
				forest_group.Insert(new EditBoxNumberPrefab("Trees / Hectare", settings, "TreeDensity", 10));
				forest_group.Insert(new EditBoxNumberPrefab("Bushes / Hectare", settings, "BushDensity", 10));
				forest_group.Insert(new EditBoxNumberPrefab("Rocks / Hectare", settings, "RockDensity", 1));
				forest_group.Insert(new EditBoxNumberPrefab("Tree Distance", settings, "MinDistance", 0.5));
				forest_group.Insert(new EditBoxNumberPrefab("Edge Width", settings, "EdgeWidth", 1));
				forest_group.Insert(new EditBoxNumberPrefab("Edge Bushes", settings, "EdgeBushBoost", 0.5));
				forest_group.Insert(new EditBoxNumberPrefab("Max Slope", settings, "MaxSlope", 1));
				forest_group.Insert(new CheckBoxPrefab("Avoid Water", settings, "AvoidWater"));
				forest_group.Insert(new EditBoxNumberPrefab("Scale Min", settings, "ScaleMin", 0.05));
				forest_group.Insert(new EditBoxNumberPrefab("Scale Max", settings, "ScaleMax", 0.05));
				forest_group.Insert(new EditBoxNumberPrefab("Sink Trees", settings, "TreeSink", 0.05));
				forest_group.Insert(new EditBoxNumberPrefab("Sink Rocks", settings, "RockSink", 0.05));
				forest_group.Insert(new CheckBoxPrefab("Rocks Follow Slope", settings, "AlignRocks"));
				AddContent(forest_group);
				break;
			}
		}
	}

	protected DropdownListPrefab<EditorSplinePickMode> CreatePickModes(EditorSplineGeneratorSettings settings)
	{
		DropdownListPrefab<EditorSplinePickMode> pick_mode = new DropdownListPrefab<EditorSplinePickMode>("Pick", settings, "PickMode");
		pick_mode["In Order"] = EditorSplinePickMode.IN_ORDER;
		pick_mode["Random"] = EditorSplinePickMode.RANDOM;
		return pick_mode;
	}

	protected GroupPrefab CreateAssetGroup(string caption, string field, array<string> assets)
	{
		GroupPrefab asset_group = new GroupPrefab(string.Format("%1 (%2)", caption, assets.Count()), this, string.Empty);
		asset_group.Insert(new EditorSplineAssetInputView(caption, field, false, ScriptCaller.Create(OnAssetEntered), ScriptCaller.Create(OnAssetSelected), ScriptCaller.Create(OnAssetsCleared)));
		for (int i = 0; i < assets.Count(); i++) {
			asset_group.Insert(new EditorSplineAssetView(field, i, assets[i], ScriptCaller.Create(OnAssetRemoved)));
		}

		return asset_group;
	}

	protected array<string> GetAssets(EditorSplineGeneratorSettings settings, string field)
	{
		switch (field) {
			case "Assets": return settings.Assets;
			case "Trees": return settings.Trees;
			case "Bushes": return settings.Bushes;
			case "Rocks": return settings.Rocks;
			case "RoadPieces": return settings.RoadPieces;
			case "PathPieces": return settings.PathPieces;
		}

		return null;
	}

	// Lists get the asset added, single fields (wall, post) are replaced
	protected void InsertAssets(string field, array<string> assets)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline || assets.Count() == 0) {
			return;
		}

		string state = m_SplineTool.CaptureState();
		array<string> spline_assets = GetAssets(spline.Generator, field);
		if (spline_assets) {
			foreach (string asset: assets) {
				if (spline_assets.Find(asset) == -1) {
					spline_assets.Insert(asset);
				}
			}
		} else {
			EnScript.SetClassVar(spline.Generator, field, 0, assets[0]);
		}

		foreach (string checked_asset: assets) {
			EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(checked_asset);
			if (asset_info && asset_info.Valid && !asset_info.ConsoleFriendly) {
				m_SplineTool.Notify(asset_info.GetDisplayName() + " is not console friendly, it is left out while Console Friendly Assets Only is on.", 6.0);
			}
		}

		m_SplineTool.PushUndo(state);
		m_SplineTool.MarkChanged(spline, false);
		m_RebuildProperties = true;
	}

	protected void OnAssetEntered(EditorSplineAssetInputView view, string field, string asset)
	{
		array<string> assets = { asset };
		InsertAssets(field, assets);
	}

	protected void OnAssetSelected(EditorSplineAssetInputView view, string field)
	{
		array<string> selected_types = {};
		m_SplineTool.GetSelectedTypes(selected_types);
		if (selected_types.Count() == 0) {
			m_SplineTool.Notify("Select an object in the world or an item in the placeables list first.");
			return;
		}

		InsertAssets(field, selected_types);
	}

	protected void OnAssetsCleared(EditorSplineAssetInputView view, string field)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			return;
		}

		array<string> spline_assets = GetAssets(spline.Generator, field);
		if (!spline_assets || spline_assets.Count() == 0) {
			return;
		}

		string state = m_SplineTool.CaptureState();
		spline_assets.Clear();
		m_SplineTool.PushUndo(state);
		m_SplineTool.MarkChanged(spline, false);
		m_RebuildProperties = true;
	}

	protected void OnAssetRemoved(EditorSplineAssetView view, string field, int index)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			return;
		}

		string state = m_SplineTool.CaptureState();
		array<string> spline_assets = GetAssets(spline.Generator, field);
		if (spline_assets) {
			if (index >= 0 && index < spline_assets.Count()) {
				spline_assets.RemoveOrdered(index);
			}
		} else {
			EnScript.SetClassVar(spline.Generator, field, 0, string.Empty);
		}

		m_SplineTool.PushUndo(state);
		m_SplineTool.MarkChanged(spline, false);
		m_RebuildProperties = true;
	}

	protected void OnSplineClicked(EditorSplineListItem list_item, string uuid, bool double_click)
	{
		m_SplineTool.SetActiveSpline(uuid);
		if (double_click) {
			m_SplineTool.Focus(m_SplineTool.GetActiveSpline());
		}
	}

	protected void StepGeneratorType(int direction)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			return;
		}

		EditorSplineGeneratorType type = spline.Generator.Type + direction;
		if (type > EditorSplineGeneratorType.FOOTPATH) {
			type = EditorSplineGeneratorType.NONE;
		}

		if (type < EditorSplineGeneratorType.NONE) {
			type = EditorSplineGeneratorType.FOOTPATH;
		}

		m_SplineTool.SetGeneratorType(spline, type);
		m_RebuildProperties = true;
	}

	protected void OnPresetSelected(string file, eDialogExtraSetting extra_settings)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			m_SplineTool.Notify("Select a spline first.");
			return;
		}

		EditorSplineGeneratorSettings settings = EditorSplinePresets.Load(file);
		if (!settings) {
			m_SplineTool.Notify("Could not read " + File.GetName(file));
			return;
		}

		if (settings.PresetName == string.Empty) {
			settings.PresetName = File.GetNameWithoutExtension(file);
		}

		m_SplineTool.ApplyPreset(spline, settings);
		m_RebuildProperties = true;
	}

	protected void OnPresetSaved(string file, eDialogExtraSetting extra_settings)
	{
		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		if (!spline) {
			return;
		}

		if (File.GetExtension(file) != EditorSplinePresetFile.EXTENSION) {
			file += EditorSplinePresetFile.EXTENSION;
		}

		spline.Generator.PresetName = File.GetNameWithoutExtension(file);
		if (EditorSplinePresets.Save(file, spline.Generator)) {
			m_SplineTool.Notify(string.Format("Saved preset %1", File.GetName(file)));
		}

		m_RebuildProperties = true;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnClick(w, x, y, button);
		}

		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		switch (w) {
			case TitleClose: {
				Delete();
				return true;
			}

			case NewSplineButton: {
				m_SplineTool.CreateSpline(EditorSplineType.SPLINE);
				return true;
			}

			case NewPolylineButton: {
				m_SplineTool.CreateSpline(EditorSplineType.POLYLINE);
				return true;
			}

			case DeleteButton: {
				m_SplineTool.DeleteSpline(spline);
				return true;
			}

			case EditModeButton: {
				m_SplineTool.SetEditMode(!m_SplineTool.IsEditMode());
				return true;
			}

			case PresetsButton: {
				m_Editor.GetEditorHud().ShowFileDialog("Generator Presets", EditorSplinePresetFile, ScriptCaller.Create(OnPresetSelected), eDialogMode.OPEN, eDialogFlags.ALLOW_DOUBLE_CLICK);
				return true;
			}

			case TypeButton: {
				m_SplineTool.ToggleType(spline);
				return true;
			}

			case ReverseButton: {
				m_SplineTool.Reverse(spline);
				return true;
			}

			case DropButton: {
				m_SplineTool.DropToGround(spline);
				return true;
			}

			case FocusButton: {
				m_SplineTool.Focus(spline);
				return true;
			}

			case GeneratorPrevButton: {
				StepGeneratorType(-1);
				return true;
			}

			case GeneratorNextButton: {
				StepGeneratorType(1);
				return true;
			}

			case GenerateButton: {
				m_SplineTool.Generate(spline);
				return true;
			}

			case ClearButton: {
				m_SplineTool.ClearGenerated(spline);
				return true;
			}

			case SavePresetButton: {
				if (!spline || spline.Generator.Type == EditorSplineGeneratorType.NONE) {
					m_SplineTool.Notify("Choose a generator first.");
					return true;
				}

				string preset_name = spline.Generator.PresetName;
				if (preset_name == string.Empty) {
					preset_name = spline.Name;
				}

				m_Editor.GetEditorHud().ShowFileDialog("Save Generator Preset", EditorSplinePresetFile, ScriptCaller.Create(OnPresetSaved), eDialogMode.SAVE, eDialogFlags.WARN_ON_OVERWRITE, preset_name + EditorSplinePresetFile.EXTENSION);
				return true;
			}
		}

		if (w == SnapCheck || w == PreviewCheck || w == ConsoleCheck || w == ClosedCheck) {
			OnCheckBoxChanged(w);
			return false;
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (m_Refreshing) {
			return super.OnChange(w, x, y, finished);
		}

		EditorSplineData spline = m_SplineTool.GetActiveSpline();
		switch (w) {
			case SplineNameBox: {
				m_SplineTool.RenameSpline(spline, SplineNameBox.GetText());
				return true;
			}

			case TensionBox: {
				float tension;
				if (EditorSplineMath.ParseNumber(TensionBox.GetText(), tension)) {
					m_SplineTool.SetTension(spline, tension);
				}

				return true;
			}

			case SnapCheck:
			case PreviewCheck:
			case ConsoleCheck:
			case ClosedCheck: {
				OnCheckBoxChanged(w);
				return false;
			}
		}

		return super.OnChange(w, x, y, finished);
	}

	protected void OnCheckBoxChanged(Widget w)
	{
		if (m_Refreshing) {
			return;
		}

		switch (w) {
			case SnapCheck: {
				if (SnapCheck.IsChecked() != m_SplineTool.GetSnapToTerrain()) {
					m_SplineTool.SetSnapToTerrain(SnapCheck.IsChecked());
				}

				break;
			}

			case PreviewCheck: {
				if (PreviewCheck.IsChecked() != m_SplineTool.GetShowPreview()) {
					m_SplineTool.SetShowPreview(PreviewCheck.IsChecked());
				}

				break;
			}

			case ConsoleCheck: {
				if (ConsoleCheck.IsChecked() != m_SplineTool.GetConsoleOnly()) {
					m_SplineTool.SetConsoleOnly(ConsoleCheck.IsChecked());
					m_RebuildProperties = true;
				}

				break;
			}

			case ClosedCheck: {
				m_SplineTool.SetClosed(m_SplineTool.GetActiveSpline(), ClosedCheck.IsChecked());
				break;
			}
		}
	}

	override typename GetControllerType()
	{
		return EditorSplineDialogController;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/splines/SplineDialog.layout";
	}

	override string GetTitle()
	{
		return "Spline Tool";
	}
}