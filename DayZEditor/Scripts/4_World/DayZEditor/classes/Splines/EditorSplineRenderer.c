// Everything is projected onto the hud canvases every frame, retail does not draw debug shapes
class EditorSplineRenderer
{
	protected static const int MARKER_BUDGET = 500;
	protected static const int LINE_BUDGET = 600;
	protected static const int LINE_SEGMENTS = 300;
	protected static const float PREVIEW_DISTANCE = 500.0;
	protected static const float LINE_DISTANCE = 4000.0;
	protected static const float NEAR_PLANE = 0.3;

	protected EditorSplineTool m_SplineTool;
	protected CanvasWidget m_Canvas;

	// Only the preview markers close to the camera are drawn, a 20000 tree forest would cost too much
	protected EditorSplineGeneratorResult m_Preview;
	protected ref array<int> m_MarkerIndices = {};
	protected ref array<int> m_LineIndices = {};
	protected vector m_PreviewCameraPosition;
	protected bool m_PreviewTrimmed;

	protected vector m_CameraPosition;
	protected vector m_CameraDirection;
	protected float m_CanvasX, m_CanvasY;

	void EditorSplineRenderer(EditorSplineTool spline_tool)
	{
		m_SplineTool = spline_tool;
	}

	void Clear()
	{
		EditorHud editor_hud = GetEditor().GetEditorHud();
		if (!editor_hud || !editor_hud.SplineCanvas || !editor_hud.SplineMapCanvas) {
			return;
		}

		editor_hud.SplineCanvas.Clear();
		editor_hud.SplineMapCanvas.Clear();
	}

	void ClearPreview()
	{
		m_Preview = null;
		m_MarkerIndices.Clear();
		m_LineIndices.Clear();
		m_PreviewTrimmed = false;
	}

	// The camera moved far enough that other markers are closer now
	bool IsPreviewOutdated(vector camera_position)
	{
		return m_PreviewTrimmed && vector.DistanceSq(camera_position, m_PreviewCameraPosition) > 3600;
	}

	void SetPreview(EditorSplineGeneratorResult preview, vector camera_position)
	{
		ClearPreview();
		if (!preview) {
			return;
		}

		m_Preview = preview;
		m_PreviewCameraPosition = camera_position;
		float max_distance_sq = PREVIEW_DISTANCE * PREVIEW_DISTANCE;

		array<float> marker_distances = {};
		for (int i = 0; i < preview.Placements.Count(); i++) {
			float marker_distance_sq = vector.DistanceSq(camera_position, preview.Placements[i].MarkerPosition);
			if (marker_distance_sq > max_distance_sq) {
				m_PreviewTrimmed = true;
				continue;
			}

			m_MarkerIndices.Insert(i);
			marker_distances.Insert(marker_distance_sq);
		}

		KeepClosest(m_MarkerIndices, marker_distances, MARKER_BUDGET);

		array<float> line_distances = {};
		for (int j = 0; j < preview.Lines.Count(); j++) {
			EditorSplinePreviewLine line = preview.Lines[j];
			float line_distance_sq = Math.Min(vector.DistanceSq(camera_position, line.From), vector.DistanceSq(camera_position, line.To));
			if (line_distance_sq > max_distance_sq) {
				m_PreviewTrimmed = true;
				continue;
			}

			m_LineIndices.Insert(j);
			line_distances.Insert(line_distance_sq);
		}

		KeepClosest(m_LineIndices, line_distances, LINE_BUDGET);
	}

	protected void KeepClosest(array<int> indices, array<float> distances, int budget)
	{
		if (indices.Count() <= budget) {
			return;
		}

		m_PreviewTrimmed = true;
		array<float> sorted_distances = {};
		sorted_distances.Copy(distances);
		sorted_distances.Sort();
		float limit = sorted_distances[budget - 1];

		array<int> kept_indices = {};
		for (int i = 0; i < indices.Count() && kept_indices.Count() < budget; i++) {
			if (distances[i] <= limit) {
				kept_indices.Insert(indices[i]);
			}
		}

		indices.Copy(kept_indices);
	}

	void Draw(bool show_points, bool show_preview)
	{
		m_Canvas = GetEditor().GetEditorHud().SplineCanvas;
		if (!m_Canvas) {
			return;
		}

		m_Canvas.Clear();
		if (GetEditor().IsMapActive()) {
			return;
		}

		m_CameraPosition = GetGame().GetCurrentCameraPosition();
		m_CameraDirection = GetGame().GetCurrentCameraDirection();
		m_Canvas.GetScreenPos(m_CanvasX, m_CanvasY);

		EditorSplineData active_spline = m_SplineTool.GetActiveSpline();
		array<ref EditorSplineData> splines = m_SplineTool.GetSplines();
		foreach (EditorSplineData spline: splines) {
			if (spline == active_spline) {
				DrawPath(m_SplineTool.GetPath(spline), EditorSplineColors.LINE, 3);
			} else {
				DrawPath(m_SplineTool.GetPath(spline), EditorSplineColors.LINE_INACTIVE, 2);
			}
		}

		if (show_preview && m_Preview) {
			DrawPreview();
		}

		if (show_points) {
			DrawPoints(splines, active_spline);
		}
	}

	protected void DrawPath(EditorSplinePath path, LinearColor color, float width)
	{
		if (!path || path.Count() < 2) {
			return;
		}

		int last_sample = path.Count() - 1;
		int stride = Math.Max(1, path.Count() / LINE_SEGMENTS);
		float max_distance_sq = LINE_DISTANCE * LINE_DISTANCE;
		vector previous = GetAboveGround(path.GetSample(0));
		for (int i = stride; i < last_sample + stride; i += stride) {
			int sample_index = Math.Min(i, last_sample);
			vector current = GetAboveGround(path.GetSample(sample_index));
			if (vector.DistanceSq(m_CameraPosition, current) < max_distance_sq || vector.DistanceSq(m_CameraPosition, previous) < max_distance_sq) {
				DrawWorldLine(previous, current, color, width);
			}

			previous = current;
			if (sample_index == last_sample) {
				break;
			}
		}
	}

	// splines can dip under the terrain between points
	protected vector GetAboveGround(vector position)
	{
		float height = Math.Max(position[1], EditorSplineMath.TerrainY(position)) + 0.15;
		return Vector(position[0], height, position[2]);
	}

	protected void DrawPreview()
	{
		foreach (int line_index: m_LineIndices) {
			if (line_index < m_Preview.Lines.Count()) {
				EditorSplinePreviewLine line = m_Preview.Lines[line_index];
				DrawWorldLine(line.From, line.To, line.Color, 2);
			}
		}

		foreach (int marker_index: m_MarkerIndices) {
			if (marker_index >= m_Preview.Placements.Count()) {
				continue;
			}

			EditorSplinePlacement placement = m_Preview.Placements[marker_index];
			vector top = placement.MarkerPosition + Vector(0, placement.MarkerHeight, 0);
			vector tick = top + EditorSplineMath.Flatten(placement.MarkerDirection) * Math.Max(0.8, placement.MarkerHeight * 0.4);
			DrawWorldLine(placement.MarkerPosition, top, placement.Color, 2);
			DrawWorldLine(top, tick, placement.Color, 2);
		}
	}

	protected void DrawPoints(array<ref EditorSplineData> splines, EditorSplineData active_spline)
	{
		int selected_point = m_SplineTool.GetSelectedPoint();
		int hovered_point = m_SplineTool.GetHoveredPoint();
		foreach (EditorSplineData spline: splines) {
			for (int i = 0; i < spline.Count(); i++) {
				if (spline != active_spline) {
					DrawWorldSquare(spline.Points[i], 7, EditorSplineColors.POINT_INACTIVE);
					continue;
				}

				if (i == selected_point) {
					DrawWorldSquare(spline.Points[i], 14, EditorSplineColors.POINT_SELECTED);
				} else if (i == hovered_point) {
					DrawWorldSquare(spline.Points[i], 12, EditorSplineColors.POINT_HOVER);
				} else if (i == 0) {
					DrawWorldSquare(spline.Points[i], 10, EditorSplineColors.POINT_FIRST);
				} else {
					DrawWorldSquare(spline.Points[i], 10, EditorSplineColors.POINT);
				}
			}
		}

		if (!active_spline) {
			return;
		}

		// height of the selected point above the ground
		if (selected_point >= 0 && selected_point < active_spline.Count()) {
			vector point = active_spline.Points[selected_point];
			float ground = EditorSplineMath.TerrainY(point);
			if (Math.AbsFloat(point[1] - ground) > 0.05) {
				DrawWorldLine(point, Vector(point[0], ground, point[2]), EditorSplineColors.HEIGHT, 1);
			}
		}

		// arrow at the start showing which way the spline runs
		EditorSplinePath path = m_SplineTool.GetPath(active_spline);
		if (path && path.Count() > 1 && path.GetLength() > 1) {
			vector arrow_start = GetAboveGround(active_spline.Points[0]);
			float arrow_length = Math.Clamp(vector.Distance(m_CameraPosition, arrow_start) * 0.06, 1.5, 30.0);
			arrow_length = Math.Min(arrow_length, path.GetLength() * 0.5);
			DrawWorldArrow(arrow_start, GetAboveGround(path.Evaluate(arrow_length)), EditorSplineColors.DIRECTION);
		}
	}

	// Cut at the near plane so lines passing behind the camera still draw correctly
	protected void DrawWorldLine(vector from, vector to, LinearColor color, float width)
	{
		float from_depth = vector.Dot(from - m_CameraPosition, m_CameraDirection);
		float to_depth = vector.Dot(to - m_CameraPosition, m_CameraDirection);
		if (from_depth < NEAR_PLANE && to_depth < NEAR_PLANE) {
			return;
		}

		vector visible_from = from;
		vector visible_to = to;
		if (from_depth < NEAR_PLANE) {
			visible_from = from + (to - from) * ((NEAR_PLANE - from_depth) / (to_depth - from_depth));
		}

		if (to_depth < NEAR_PLANE) {
			visible_to = to + (from - to) * ((NEAR_PLANE - to_depth) / (from_depth - to_depth));
		}

		vector screen_from = GetGame().GetScreenPos(visible_from);
		vector screen_to = GetGame().GetScreenPos(visible_to);
		m_Canvas.DrawLine(screen_from[0] - m_CanvasX, screen_from[1] - m_CanvasY, screen_to[0] - m_CanvasX, screen_to[1] - m_CanvasY, width, color);
	}

	// Same size on screen at any distance, with a dark outline
	protected void DrawWorldSquare(vector position, float size, LinearColor color)
	{
		if (vector.Dot(position - m_CameraPosition, m_CameraDirection) < NEAR_PLANE) {
			return;
		}

		vector screen = GetGame().GetScreenPos(position);
		float x = screen[0] - m_CanvasX;
		float y = screen[1] - m_CanvasY;
		float outline = size + 2;
		m_Canvas.DrawLine(x - outline * 0.5, y, x + outline * 0.5, y, outline, EditorSplineColors.POINT_OUTLINE);
		m_Canvas.DrawLine(x - size * 0.5, y, x + size * 0.5, y, size, color);
	}

	protected void DrawWorldArrow(vector from, vector to, LinearColor color)
	{
		if (vector.Dot(from - m_CameraPosition, m_CameraDirection) < NEAR_PLANE || vector.Dot(to - m_CameraPosition, m_CameraDirection) < NEAR_PLANE) {
			return;
		}

		vector screen_from = GetGame().GetScreenPos(from);
		vector screen_to = GetGame().GetScreenPos(to);
		float dx = screen_to[0] - screen_from[0];
		float dy = screen_to[1] - screen_from[1];
		float length = Math.Sqrt(dx * dx + dy * dy);
		if (length < 4) {
			return;
		}

		float ux = dx / length;
		float uy = dy / length;
		float head = Math.Min(12, length * 0.4);
		float tip_x = screen_to[0] - m_CanvasX;
		float tip_y = screen_to[1] - m_CanvasY;
		m_Canvas.DrawLine(screen_from[0] - m_CanvasX, screen_from[1] - m_CanvasY, tip_x, tip_y, 3, color);
		m_Canvas.DrawLine(tip_x, tip_y, tip_x - (ux * 0.87 - uy * 0.5) * head, tip_y - (uy * 0.87 + ux * 0.5) * head, 3, color);
		m_Canvas.DrawLine(tip_x, tip_y, tip_x - (ux * 0.87 + uy * 0.5) * head, tip_y - (uy * 0.87 - ux * 0.5) * head, 3, color);
	}

	// Lines always, points while the spline tool is open
	void DrawMap(bool show_points)
	{
		EditorHud editor_hud = GetEditor().GetEditorHud();
		CanvasWidget map_canvas = editor_hud.SplineMapCanvas;
		if (!map_canvas) {
			return;
		}

		map_canvas.Clear();
		if (!editor_hud.Map.IsVisible()) {
			return;
		}

		float canvas_x, canvas_y;
		map_canvas.GetScreenPos(canvas_x, canvas_y);

		EditorSplineData active_spline = m_SplineTool.GetActiveSpline();
		int selected_point = m_SplineTool.GetSelectedPoint();
		array<ref EditorSplineData> splines = m_SplineTool.GetSplines();
		foreach (EditorSplineData spline: splines) {
			LinearColor line_color = EditorSplineColors.LINE_INACTIVE;
			float line_width = 2;
			if (spline == active_spline) {
				line_color = EditorSplineColors.LINE;
				line_width = 3;
			}

			EditorSplinePath path = m_SplineTool.GetPath(spline);
			if (path && path.Count() > 1) {
				int last_sample = path.Count() - 1;
				int stride = Math.Max(1, path.Count() / 800);
				vector previous = editor_hud.Map.MapToScreen(path.GetSample(0));
				for (int i = stride; i < last_sample + stride; i += stride) {
					int sample_index = Math.Min(i, last_sample);
					vector current = editor_hud.Map.MapToScreen(path.GetSample(sample_index));
					map_canvas.DrawLine(previous[0] - canvas_x, previous[1] - canvas_y, current[0] - canvas_x, current[1] - canvas_y, line_width, line_color);
					previous = current;
					if (sample_index == last_sample) {
						break;
					}
				}
			}

			if (!show_points) {
				continue;
			}

			for (int j = 0; j < spline.Count(); j++) {
				vector map_position = editor_hud.Map.MapToScreen(spline.Points[j]);
				float point_size = 5;
				LinearColor point_color = EditorSplineColors.POINT_INACTIVE;
				if (spline == active_spline) {
					point_size = 7;
					point_color = EditorSplineColors.POINT;
					if (j == 0) {
						point_color = EditorSplineColors.POINT_FIRST;
					}

					if (j == selected_point) {
						point_size = 10;
						point_color = EditorSplineColors.POINT_SELECTED;
					}
				}

				float x = map_position[0] - canvas_x;
				float y = map_position[1] - canvas_y;
				map_canvas.DrawLine(x - point_size * 0.5, y, x + point_size * 0.5, y, point_size, point_color);
			}
		}
	}
}