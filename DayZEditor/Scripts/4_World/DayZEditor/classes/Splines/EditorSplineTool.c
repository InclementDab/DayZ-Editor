// Every spline as json, small enough to keep one per undo step
class EditorSplineSnapshot
{
	ref array<ref EditorSplineData> Splines = {};
}

class EditorSplineTool
{
	protected static const float POINT_PICK_DISTANCE = 14.0;
	protected static const float LINE_PICK_DISTANCE = 10.0;
	protected static const float PREVIEW_DELAY = 0.3;

	protected Editor m_Editor;
	protected ref array<ref EditorSplineData> m_Splines = {};
	protected ref map<string, ref EditorSplinePath> m_Paths = new map<string, ref EditorSplinePath>();
	protected ref map<string, ref array<EditorObject>> m_GeneratedObjects = new map<string, ref array<EditorObject>>();
	protected ref EditorSplineRenderer m_Renderer;
	protected ref EditorSplineGeneratorResult m_Preview;
	protected string m_PreviewUuid;

	protected string m_ActiveUuid;
	protected int m_SelectedPoint = -1;
	protected int m_HoveredPoint = -1;
	protected EditorSplineType m_LastSplineType;

	protected bool m_Enabled;
	protected bool m_EditMode;
	protected bool m_SnapToTerrain = true;
	protected bool m_ShowPreview = true;
	protected bool m_ConsoleOnly = true;

	protected bool m_Dragging;
	protected bool m_DragMoved;
	protected string m_DragState;
	protected int m_DragMouseY;
	protected float m_DragHeight;

	protected bool m_WasVisible;
	protected bool m_PreviewDirty = true;
	protected float m_PreviewTimer;

	// windows refresh when these change, the list revision when the splines themselves were replaced
	protected int m_Revision;
	protected int m_ListRevision;

	void EditorSplineTool(Editor editor)
	{
		m_Editor = editor;
		m_Renderer = new EditorSplineRenderer(this);
		EditorSplineGeneratorSettings.OnPropertyChanged.Insert(OnGeneratorPropertyChanged);
	}

	void ~EditorSplineTool()
	{
		EditorSplineGeneratorSettings.OnPropertyChanged.Remove(OnGeneratorPropertyChanged);
	}

	array<ref EditorSplineData> GetSplines()
	{
		return m_Splines;
	}

	EditorSplineData GetSpline(string uuid)
	{
		foreach (EditorSplineData spline: m_Splines) {
			if (spline.Uuid == uuid) {
				return spline;
			}
		}

		return null;
	}

	EditorSplineData GetActiveSpline()
	{
		if (m_ActiveUuid == string.Empty) {
			return null;
		}

		return GetSpline(m_ActiveUuid);
	}

	void SetActiveSpline(string uuid)
	{
		if (uuid == m_ActiveUuid) {
			return;
		}

		m_ActiveUuid = uuid;
		m_SelectedPoint = -1;
		m_HoveredPoint = -1;
		m_PreviewDirty = true;
		m_PreviewTimer = 0;
		m_Revision++;
	}

	int GetSelectedPoint()
	{
		return m_SelectedPoint;
	}

	int GetHoveredPoint()
	{
		return m_HoveredPoint;
	}

	int GetRevision()
	{
		return m_Revision;
	}

	int GetListRevision()
	{
		return m_ListRevision;
	}

	EditorSplineGeneratorResult GetPreview()
	{
		EditorSplineData spline = GetActiveSpline();
		if (!spline || spline.Uuid != m_PreviewUuid) {
			return null;
		}

		return m_Preview;
	}

	bool IsPreviewPending()
	{
		return m_PreviewDirty;
	}

	EditorSplinePath GetPath(EditorSplineData spline)
	{
		if (!spline) {
			return null;
		}

		EditorSplinePath path;
		if (!m_Paths.Find(spline.Uuid, path)) {
			path = EditorSplinePath.Create(spline);
			m_Paths.Insert(spline.Uuid, path);
		}

		return path;
	}

	// Enabled while the spline dialog is open
	void SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
		m_EditMode = enabled;
		m_HoveredPoint = -1;
		m_PreviewDirty = true;
		if (!enabled) {
			EndDrag();
			m_Renderer.ClearPreview();
		}

		m_Revision++;
	}

	bool IsEnabled()
	{
		return m_Enabled;
	}

	void SetEditMode(bool edit_mode)
	{
		m_EditMode = edit_mode;
		if (edit_mode) {
			// the gizmo of selected objects would take the clicks meant for points
			m_Editor.ClearSelection();
		} else {
			EndDrag();
			m_HoveredPoint = -1;
		}

		m_Revision++;
	}

	bool IsEditMode()
	{
		return m_EditMode;
	}

	bool IsDraggingPoint()
	{
		return m_Dragging;
	}

	void SetSnapToTerrain(bool snap_to_terrain)
	{
		m_SnapToTerrain = snap_to_terrain;
		m_Revision++;
	}

	bool GetSnapToTerrain()
	{
		return m_SnapToTerrain;
	}

	void SetShowPreview(bool show_preview)
	{
		m_ShowPreview = show_preview;
		m_PreviewDirty = true;
		m_PreviewTimer = 0;
		if (!show_preview) {
			m_Renderer.ClearPreview();
		}

		m_Revision++;
	}

	bool GetShowPreview()
	{
		return m_ShowPreview;
	}

	void SetConsoleOnly(bool console_only)
	{
		m_ConsoleOnly = console_only;
		m_PreviewDirty = true;
		m_PreviewTimer = 0;
		m_Revision++;
	}

	bool GetConsoleOnly()
	{
		return m_ConsoleOnly;
	}

	void Clear()
	{
		m_Splines.Clear();
		Reset();
	}

	// Splines of a file that was opened or imported
	void InsertSplines(array<ref EditorSplineData> splines)
	{
		foreach (EditorSplineData spline: splines) {
			spline.Repair();
			if (GetSpline(spline.Uuid)) {
				spline.Uuid = UUID.Generate();
			}

			m_Splines.Insert(spline);
		}

		m_ListRevision++;
		m_Revision++;
	}

	protected void Reset()
	{
		m_Dragging = false;
		m_DragState = string.Empty;
		m_ActiveUuid = string.Empty;
		m_SelectedPoint = -1;
		m_HoveredPoint = -1;
		m_Paths.Clear();
		m_GeneratedObjects.Clear();
		m_Preview = null;
		m_PreviewUuid = string.Empty;
		m_Renderer.ClearPreview();
		m_PreviewDirty = true;
		m_PreviewTimer = 0;
		m_ListRevision++;
		m_Revision++;
	}

	string CaptureState()
	{
		EditorSplineSnapshot snapshot = new EditorSplineSnapshot();
		foreach (EditorSplineData spline: m_Splines) {
			snapshot.Splines.Insert(spline);
		}
		
		string state;
		JsonSerializer serializer = new JsonSerializer();
		serializer.WriteToString(snapshot, false, state);
		return state;
	}

	// One step on the editors own undo stack, when something really changed
	void PushUndo(string state)
	{
		string current_state = CaptureState();
		if (state == string.Empty || current_state == state) {
			return;
		}

		EditorAction action = new EditorAction("SetSplines", "SetSplines");
		action.InsertUndoParameter(new Param1<string>(state));
		action.InsertRedoParameter(new Param1<string>(current_state));
		m_Editor.InsertAction(action);
	}

	void RestoreState(string state)
	{
		EditorSplineSnapshot snapshot = new EditorSplineSnapshot();
		JsonSerializer serializer = new JsonSerializer();
		string error;
		if (!serializer.ReadFromString(snapshot, state, error)) {
			EditorLog.Error(error);
			return;
		}

		m_Dragging = false;
		m_DragState = string.Empty;
		m_Splines.Clear();
		foreach (EditorSplineData spline: snapshot.Splines) {
			spline.Repair();
			m_Splines.Insert(spline);
		}

		EditorSplineData active_spline = GetActiveSpline();
		if (!active_spline) {
			m_ActiveUuid = string.Empty;
		}

		if (!active_spline || m_SelectedPoint >= active_spline.Count()) {
			m_SelectedPoint = -1;
		}

		m_Paths.Clear();
		m_PreviewDirty = true;
		m_PreviewTimer = 0;
		m_ListRevision++;
		m_Revision++;
	}

	// The line is rebuilt now and the preview a moment later
	void MarkChanged(EditorSplineData spline, bool geometry)
	{
		if (spline && geometry) {
			m_Paths.Remove(spline.Uuid);
		}

		m_PreviewDirty = true;
		m_PreviewTimer = PREVIEW_DELAY;
		m_Revision++;
	}

	void Update(float dt)
	{
		if (!m_Editor.IsActive() || m_Editor.IsInventoryEditorActive()) {
			if (m_WasVisible) {
				m_Renderer.Clear();
				m_WasVisible = false;
			}

			return;
		}

		if (!m_WasVisible) {
			m_WasVisible = true;
			m_PreviewDirty = true;
			m_PreviewTimer = 0;
		}

		UpdatePreview(dt);
		m_Renderer.Draw(m_Enabled, m_Enabled && m_ShowPreview);
		m_Renderer.DrawMap(m_Enabled);
	}

	protected void UpdatePreview(float dt)
	{
		EditorSplineData spline = GetActiveSpline();
		if (!m_Enabled || !spline || spline.Generator.Type == EditorSplineGeneratorType.NONE) {
			if (m_Preview) {
				m_Preview = null;
				m_PreviewUuid = string.Empty;
				m_Renderer.ClearPreview();
				m_Revision++;
			}

			return;
		}

		vector camera_position = GetGame().GetCurrentCameraPosition();
		if (m_PreviewDirty || m_PreviewUuid != spline.Uuid) {
			m_PreviewTimer -= dt;
			if (m_PreviewTimer > 0 || m_Dragging) {
				return;
			}

			m_PreviewDirty = false;
			m_Preview = EditorSplineGenerator.Run(spline, GetPath(spline), m_ConsoleOnly);
			m_PreviewUuid = spline.Uuid;
			m_Revision++;

			m_Renderer.ClearPreview();
			if (m_ShowPreview) {
				m_Renderer.SetPreview(m_Preview, camera_position);
			}

			return;
		}

		if (m_ShowPreview && m_Renderer.IsPreviewOutdated(camera_position)) {
			m_Renderer.SetPreview(m_Preview, camera_position);
		}
	}

	static bool IsCursorOverWorld()
	{
		Widget widget_under_cursor = GetWidgetUnderCursor();
		if (!widget_under_cursor) {
			return true;
		}

		return widget_under_cursor.IsInherited(MapWidget) || widget_under_cursor.GetName() == "HudPanel" || widget_under_cursor.GetName() == "CursorIcons";
	}

	// Called before the editors own click handling, true when the click was ours
	bool ProcessInput(float dt)
	{
		if (!m_Enabled || !m_EditMode || !m_Editor.IsActive() || m_Editor.IsPlacing()) {
			m_HoveredPoint = -1;
			EndDrag();
			return false;
		}

		UAInput left_click_input = GetUApi().GetInputByID(UAFire);
		if (m_Dragging) {
			if (left_click_input.LocalValue()) {
				UpdateDrag();
			} else {
				EndDrag();
			}

			return true;
		}

		if (!IsCursorOverWorld()) {
			m_HoveredPoint = -1;
			return false;
		}

		EditorSplineData point_spline;
		int point_index = FindPointUnderCursor(point_spline);
		m_HoveredPoint = -1;
		if (point_spline && point_spline.Uuid == m_ActiveUuid) {
			m_HoveredPoint = point_index;
		}

		if (!left_click_input.LocalPress() || m_Editor.IsDragging()) {
			return false;
		}

		bool ctrl_down = m_Editor.IsCtrlDown();
		bool alt_down = KeyState(KeyCode.KC_LMENU) || KeyState(KeyCode.KC_RMENU);
		if (point_spline && !ctrl_down && !alt_down) {
			SetActiveSpline(point_spline.Uuid);
			m_SelectedPoint = point_index;
			m_Revision++;
			BeginDrag();
			return true;
		}

		if (ctrl_down || alt_down) {
			InsertPointAtCursor(alt_down);
			return true;
		}

		int line_segment;
		EditorSplineData line_spline = FindSplineUnderCursor(line_segment);
		if (line_spline) {
			SetActiveSpline(line_spline.Uuid);
			m_SelectedPoint = -1;
			m_Revision++;
			return true;
		}

		// nothing of ours, the editor gets the click
		if (m_SelectedPoint != -1) {
			m_SelectedPoint = -1;
			m_Revision++;
		}

		return false;
	}

	// Ctrl + Click after the selected point, Alt + Click before it
	protected void InsertPointAtCursor(bool before_selected)
	{
		vector position;
		if (!GetCursorPosition(position)) {
			return;
		}

		string state = CaptureState();
		EditorSplineData spline = GetActiveSpline();
		if (!spline) {
			spline = InsertSpline(m_LastSplineType);
		}

		int insert_index = spline.Count();
		if (before_selected) {
			insert_index = Math.Max(0, m_SelectedPoint);
		} else if (m_SelectedPoint >= 0 && m_SelectedPoint < spline.Count()) {
			insert_index = m_SelectedPoint + 1;
		} else {
			// no point selected, into the line under the cursor
			int line_segment;
			if (FindSplineUnderCursor(line_segment) == spline && spline.Count() > 1) {
				insert_index = line_segment + 1;
			}
		}

		spline.InsertPoint(insert_index, position);
		m_SelectedPoint = Math.Min(insert_index, spline.Count() - 1);
		PushUndo(state);
		MarkChanged(spline, true);
	}

	bool GetCursorPosition(out vector position)
	{
		if (m_Editor.IsMapActive()) {
			int mouse_x, mouse_y;
			GetMousePos(mouse_x, mouse_y);
			position = m_Editor.GetEditorHud().Map.ScreenToMap(Vector(mouse_x, mouse_y, 0));
			float map_height = EditorSplineMath.TerrainY(position);
			position[1] = map_height;
			return true;
		}

		array<Object> ignore_objects = {};
		if (m_Editor.GetPlayer()) {
			ignore_objects.Insert(m_Editor.GetPlayer());
		}

		Raycast raycast = m_Editor.GetCursorRaycastModeSafeEx(ignore_objects, m_SnapToTerrain);
		if (!raycast || !raycast.Bounce) {
			return false;
		}

		position = raycast.Bounce.Position;
		if (m_SnapToTerrain) {
			float ground_height = EditorSplineMath.TerrainY(position);
			position[1] = ground_height;
		}

		return true;
	}

	protected void BeginDrag()
	{
		EditorSplineData spline = GetActiveSpline();
		if (!spline || m_SelectedPoint < 0 || m_SelectedPoint >= spline.Count()) {
			return;
		}

		m_Dragging = true;
		m_DragMoved = false;
		m_DragState = CaptureState();
		int mouse_x;
		GetMousePos(mouse_x, m_DragMouseY);
		vector point = spline.Points[m_SelectedPoint];
		m_DragHeight = point[1];
	}

	protected void UpdateDrag()
	{
		EditorSplineData spline = GetActiveSpline();
		if (!spline || m_SelectedPoint < 0 || m_SelectedPoint >= spline.Count()) {
			EndDrag();
			return;
		}

		vector point = spline.Points[m_SelectedPoint];
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		vector position;
		if (m_Editor.IsShiftDown()) {
			// straight up and down with the mouse
			float metres_per_pixel = Math.Max(0.005, vector.Distance(GetGame().GetCurrentCameraPosition(), point) * 0.0015);
			position = point;
			position[1] = m_DragHeight + (m_DragMouseY - mouse_y) * metres_per_pixel;
		} else {
			if (!GetCursorPosition(position)) {
				return;
			}

			m_DragHeight = position[1];
			m_DragMouseY = mouse_y;
		}

		if (vector.DistanceSq(position, point) > 0.000001) {
			spline.Points[m_SelectedPoint] = position;
			m_DragMoved = true;
			MarkChanged(spline, true);
		}
	}

	protected void EndDrag()
	{
		if (!m_Dragging) {
			return;
		}

		m_Dragging = false;
		if (m_DragMoved) {
			PushUndo(m_DragState);
			m_PreviewTimer = 0;
		}

		m_DragState = string.Empty;
		m_Revision++;
	}

	// Called before the editors hotkeys, true when the key was ours
	bool OnKeyPress(int key)
	{
		if (!m_Enabled) {
			return false;
		}

		Widget focus = GetFocus();
		if (focus && focus.IsInherited(EditBoxWidget)) {
			return false;
		}

		EditorSplineData spline = GetActiveSpline();
		if (!spline || m_SelectedPoint < 0 || m_SelectedPoint >= spline.Count()) {
			return false;
		}

		switch (key) {
			case KeyCode.KC_DELETE: {
				if (!m_EditMode) {
					return false;
				}

				RemoveSelectedPoint();
				return true;
			}

			case KeyCode.KC_ESCAPE: {
				m_SelectedPoint = -1;
				m_Revision++;
				return true;
			}
		}

		return false;
	}

	void RemoveSelectedPoint()
	{
		EditorSplineData spline = GetActiveSpline();
		if (!spline || m_SelectedPoint < 0 || m_SelectedPoint >= spline.Count()) {
			return;
		}

		string state = CaptureState();
		spline.RemovePoint(m_SelectedPoint);
		m_SelectedPoint = Math.Min(m_SelectedPoint, spline.Count() - 1);
		PushUndo(state);
		MarkChanged(spline, true);
	}

	// Point under the mouse, the active spline wins when points overlap
	int FindPointUnderCursor(out EditorSplineData found_spline)
	{
		found_spline = null;
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		int found_index = -1;
		float best_distance_sq = POINT_PICK_DISTANCE * POINT_PICK_DISTANCE;
		foreach (EditorSplineData spline: m_Splines) {
			for (int i = 0; i < spline.Count(); i++) {
				vector screen_position = GetScreenPosition(spline.Points[i]);
				if (screen_position[2] <= 0) {
					continue;
				}

				float dx = screen_position[0] - mouse_x;
				float dy = screen_position[1] - mouse_y;
				float distance_sq = dx * dx + dy * dy;
				if (spline.Uuid == m_ActiveUuid) {
					distance_sq *= 0.5;
				}

				if (distance_sq < best_distance_sq) {
					best_distance_sq = distance_sq;
					found_spline = spline;
					found_index = i;
				}
			}
		}

		return found_index;
	}

	// Spline whose line passes close to the mouse, segment is the point segment under the mouse
	EditorSplineData FindSplineUnderCursor(out int segment)
	{
		segment = -1;
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);

		EditorSplineData found_spline;
		float best_distance = LINE_PICK_DISTANCE;
		foreach (EditorSplineData spline: m_Splines) {
			EditorSplinePath path = GetPath(spline);
			if (!path || path.Count() < 2) {
				continue;
			}

			int stride = Math.Max(1, path.Count() / 600);
			vector previous_position = GetSampleScreenPosition(path, 0);
			for (int i = stride; i < path.Count() + stride; i += stride) {
				int sample_index = Math.Min(i, path.Count() - 1);
				vector screen_position = GetSampleScreenPosition(path, sample_index);
				if (previous_position[2] > 0 && screen_position[2] > 0) {
					float distance = GetScreenDistanceToLine(mouse_x, mouse_y, previous_position, screen_position);
					if (distance < best_distance) {
						best_distance = distance;
						found_spline = spline;
						segment = path.GetSampleSegment(Math.Max(0, sample_index - 1));
					}
				}

				previous_position = screen_position;
				if (sample_index == path.Count() - 1) {
					break;
				}
			}
		}

		return found_spline;
	}

	protected vector GetSampleScreenPosition(EditorSplinePath path, int index)
	{
		vector sample = path.GetSample(index);
		float height = Math.Max(sample[1], EditorSplineMath.TerrainY(sample)) + 0.15;
		sample[1] = height;
		return GetScreenPosition(sample);
	}

	// On the map while it is open, z <= 0 is behind the camera
	protected vector GetScreenPosition(vector position)
	{
		if (m_Editor.IsMapActive()) {
			vector map_position = m_Editor.GetEditorHud().Map.MapToScreen(position);
			map_position[2] = 1;
			return map_position;
		}

		return GetGame().GetScreenPos(position);
	}

	protected float GetScreenDistanceToLine(float x, float y, vector from, vector to)
	{
		float line_x = to[0] - from[0];
		float line_y = to[1] - from[1];
		float length_sq = line_x * line_x + line_y * line_y;
		float t = 0;
		if (length_sq > 0.0001) {
			t = Math.Clamp(((x - from[0]) * line_x + (y - from[1]) * line_y) / length_sq, 0, 1);
		}

		float dx = x - (from[0] + line_x * t);
		float dy = y - (from[1] + line_y * t);
		return Math.Sqrt(dx * dx + dy * dy);
	}

	protected EditorSplineData InsertSpline(EditorSplineType type)
	{
		EditorSplineData spline = EditorSplineData.Create(type, GetFreeName(type));
		m_Splines.Insert(spline);
		m_LastSplineType = type;
		SetActiveSpline(spline.Uuid);
		return spline;
	}

	protected string GetFreeName(EditorSplineType type)
	{
		string type_name = "Spline";
		if (type == EditorSplineType.POLYLINE) {
			type_name = "Polyline";
		}

		for (int i = 1; i < 10000; i++) {
			string name = string.Format("%1 %2", type_name, i);
			bool taken = false;
			foreach (EditorSplineData spline: m_Splines) {
				if (spline.Name == name) {
					taken = true;
					break;
				}
			}

			if (!taken) {
				return name;
			}
		}

		return type_name;
	}

	EditorSplineData CreateSpline(EditorSplineType type)
	{
		string state = CaptureState();
		EditorSplineData spline = InsertSpline(type);
		SetEditMode(true);
		PushUndo(state);
		Notify(spline.Name + " created: Ctrl + Click in the world to add points.", 6.0);
		return spline;
	}

	void DeleteSpline(EditorSplineData spline)
	{
		if (!spline) {
			return;
		}

		string state = CaptureState();
		m_Splines.RemoveItem(spline);
		m_Paths.Remove(spline.Uuid);
		if (m_ActiveUuid == spline.Uuid) {
			m_ActiveUuid = string.Empty;
			m_SelectedPoint = -1;
		}

		PushUndo(state);
		MarkChanged(null, false);
		Notify("Spline deleted, the objects it generated are kept. Ctrl + Z brings it back.", 6.0);
	}

	void RenameSpline(EditorSplineData spline, string name)
	{
		string trimmed_name = name.Trim();
		if (!spline || trimmed_name == string.Empty || trimmed_name == spline.Name) {
			return;
		}

		spline.Name = trimmed_name;
		MarkChanged(spline, false);
	}

	void SetClosed(EditorSplineData spline, bool closed)
	{
		if (!spline || spline.Closed == closed) {
			return;
		}

		string state = CaptureState();
		spline.Closed = closed;
		PushUndo(state);
		MarkChanged(spline, true);
	}

	void ToggleType(EditorSplineData spline)
	{
		if (!spline) {
			return;
		}

		string state = CaptureState();
		if (spline.IsSpline()) {
			spline.Type = EditorSplineType.POLYLINE;
		} else {
			spline.Type = EditorSplineType.SPLINE;
		}

		m_LastSplineType = spline.Type;
		PushUndo(state);
		MarkChanged(spline, true);
	}

	void SetTension(EditorSplineData spline, float tension)
	{
		if (!spline) {
			return;
		}

		spline.Tension = Math.Clamp(tension, 0.0, 1.0);
		MarkChanged(spline, true);
	}

	void Reverse(EditorSplineData spline)
	{
		if (!spline || spline.Count() < 2) {
			return;
		}

		string state = CaptureState();
		spline.Reverse();
		if (m_SelectedPoint >= 0) {
			m_SelectedPoint = spline.Count() - 1 - m_SelectedPoint;
		}

		PushUndo(state);
		MarkChanged(spline, true);
	}

	void DropToGround(EditorSplineData spline)
	{
		if (!spline || spline.Count() == 0) {
			return;
		}

		string state = CaptureState();
		for (int i = 0; i < spline.Count(); i++) {
			vector point = spline.Points[i];
			float ground_height = EditorSplineMath.TerrainY(point);
			point[1] = ground_height;
			spline.Points[i] = point;
		}

		PushUndo(state);
		MarkChanged(spline, true);
	}

	void Focus(EditorSplineData spline)
	{
		if (!spline || spline.Count() == 0) {
			return;
		}

		vector center = spline.GetCenter();
		float radius = 10;
		foreach (vector point: spline.Points) {
			radius = Math.Max(radius, EditorSplineMath.DistanceXZ(center, point));
		}

		m_Editor.GetCamera().SetPosition(center + Vector(0, radius * 0.9 + 15, -radius * 0.9 - 15));
		m_Editor.GetCamera().LookAt(center);
	}

	// A prefab in the spline dialog changed a generator setting, no undo step per key press
	protected void OnGeneratorPropertyChanged(EditorSplineGeneratorSettings settings, string property_name)
	{
		EditorSplineData spline = GetActiveSpline();
		if (spline && spline.Generator == settings) {
			MarkChanged(spline, false);
		}
	}

	void SetGeneratorType(EditorSplineData spline, EditorSplineGeneratorType type)
	{
		if (!spline || spline.Generator.Type == type) {
			return;
		}

		string state = CaptureState();
		spline.Generator.ApplyTypeDefaults(type);
		if (type == EditorSplineGeneratorType.FOREST && spline.Count() > 2) {
			spline.Closed = true;
		}

		PushUndo(state);
		MarkChanged(spline, true);
	}

	void ApplyPreset(EditorSplineData spline, EditorSplineGeneratorSettings settings)
	{
		if (!spline || !settings) {
			return;
		}

		string state = CaptureState();
		spline.Generator = settings;
		if (settings.Type == EditorSplineGeneratorType.FOREST && spline.Count() > 2) {
			spline.Closed = true;
		}

		PushUndo(state);
		MarkChanged(spline, true);
		Notify(string.Format("%1 applied to %2, check the preview then press Generate.", settings.PresetName, spline.Name), 6.0);
	}

	// Types of the objects selected in the world, or the placeable selected in the list
	void GetSelectedTypes(array<string> types)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		if (selected_objects) {
			foreach (int id, EditorObject selected_object: selected_objects) {
				if (selected_object && selected_object.GetType() != string.Empty && types.Find(selected_object.GetType()) == -1) {
					types.Insert(selected_object.GetType());
				}
			}
		}

		if (types.Count() > 0) {
			return;
		}

		EditorPlaceableItem placeable_item = m_Editor.GetObjectManager().CurrentSelectedItem;
		if (placeable_item && placeable_item.GetSpawnType() != string.Empty) {
			types.Insert(placeable_item.GetSpawnType());
		}
	}

	int Generate(EditorSplineData spline)
	{
		if (!spline) {
			return 0;
		}

		if (spline.Generator.Type == EditorSplineGeneratorType.NONE) {
			Notify("Choose a generator first.");
			return 0;
		}

		EditorSplineGeneratorResult result = EditorSplineGenerator.Run(spline, GetPath(spline), m_ConsoleOnly);
		EditorLog.Info("EditorSplineTool::Generate %1 (%2): %3", spline.Name, EditorSplineGeneratorSettings.GetTypeName(spline.Generator.Type), result.GetSummary());
		if (result.Placements.Count() == 0) {
			Notify("Nothing to generate. " + result.GetSummary(), 8.0);
			return 0;
		}

		string state = CaptureState();
		int removed_count = ClearGeneratedObjects(spline);

		array<ref EditorObjectData> object_data = {};
		foreach (EditorSplinePlacement placement: result.Placements) {
			object_data.Insert(EditorSplineGenerator.CreateObjectData(placement));
		}

		array<EditorObject> generated_objects = {};
		EditorObjectMap created_objects = m_Editor.CreateObjects(object_data);
		if (created_objects) {
			foreach (int id, EditorObject created_object: created_objects) {
				if (created_object) {
					generated_objects.Insert(created_object);
				}
			}
		}

		m_GeneratedObjects.Set(spline.Uuid, generated_objects);

		// select what was placed, not for big forests
		if (generated_objects.Count() > 0 && generated_objects.Count() <= 300) {
			m_Editor.ClearSelection();
			foreach (EditorObject generated_object: generated_objects) {
				m_Editor.SelectObject(generated_object);
			}
		}

		spline.Generated.Clear();
		foreach (EditorObjectData data: object_data) {
			spline.Generated.Insert(EditorSplineGeneratedData.Create(data.Type, data.Position));
		}

		PushUndo(state);
		m_Revision++;

		string message = string.Format("%1: generated %2 objects", spline.Name, object_data.Count());
		if (removed_count > 0) {
			message += string.Format(" (replaced %1)", removed_count);
		}

		if (result.NotConsoleFriendly > 0) {
			message += string.Format(", %1 assets skipped because they are not console friendly", result.NotConsoleFriendly);
		}

		Notify(message, 6.0);
		return object_data.Count();
	}

	int ClearGenerated(EditorSplineData spline)
	{
		if (!spline) {
			return 0;
		}

		string state = CaptureState();
		int removed_count = ClearGeneratedObjects(spline);
		PushUndo(state);
		m_Revision++;
		Notify(string.Format("%1: removed %2 generated objects.", spline.Name, removed_count));
		return removed_count;
	}

	// Deletes what this spline generated before, also objects from before the file was reopened
	protected int ClearGeneratedObjects(EditorSplineData spline)
	{
		map<int, EditorObject> delete_objects = new map<int, EditorObject>();
		array<EditorObject> generated_objects = m_GeneratedObjects.Get(spline.Uuid);
		if (generated_objects) {
			foreach (EditorObject generated_object: generated_objects) {
				if (generated_object) {
					delete_objects.Set(generated_object.GetID(), generated_object);
				}
			}
		}

		if (spline.Generated.Count() > delete_objects.Count()) {
			map<string, EditorObject> placed_by_key = new map<string, EditorObject>();
			EditorObjectMap placed_objects = m_Editor.GetPlacedObjects();
			if (placed_objects) {
				foreach (int id, EditorObject placed_object: placed_objects) {
					if (placed_object) {
						placed_by_key.Set(GetGeneratedKey(placed_object.GetType(), placed_object.GetPosition()), placed_object);
					}
				}
			}

			foreach (EditorSplineGeneratedData generated: spline.Generated) {
				EditorObject found_object = placed_by_key.Get(GetGeneratedKey(generated.Type, generated.Position));
				if (found_object) {
					delete_objects.Set(found_object.GetID(), found_object);
				}
			}
		}

		m_GeneratedObjects.Remove(spline.Uuid);
		spline.Generated.Clear();
		if (delete_objects.Count() == 0) {
			return 0;
		}

		return m_Editor.DeleteObjects(delete_objects.GetValueArray());
	}

	protected static string GetGeneratedKey(string type, vector position)
	{
		string type_lower = type;
		type_lower.ToLower();
		int x = Math.Round(position[0] * 10);
		int z = Math.Round(position[2] * 10);
		return string.Format("%1|%2|%3", type_lower, x, z);
	}

	void Notify(string text, float duration = 4.0)
	{
		if (m_Editor.GetEditorHud()) {
			m_Editor.GetEditorHud().CreateNotification(text, duration);
		}
	}
}