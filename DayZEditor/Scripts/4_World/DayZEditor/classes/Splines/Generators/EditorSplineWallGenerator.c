// Wall pieces end to end along every straight segment, on a spline chord by chord along the curve
class EditorSplineWallGenerator: EditorSplineGenerator
{
	protected EditorSplineAssetInfo m_Wall;
	protected EditorSplineAssetInfo m_Pole;
	protected float m_PieceLength;
	protected float m_StepLength;

	override void Generate()
	{
		if (m_Settings.Wall == string.Empty) {
			m_Result.Message = "Choose a wall: select it in the world or in the placeables list and press Use Selected.";
			return;
		}

		m_Wall = ResolveAsset(m_Settings.Wall);
		if (!m_Wall) {
			m_Result.Message = "The wall '" + m_Settings.Wall + "' was not found or is not console friendly.";
			return;
		}

		if (m_Settings.PoleMode != EditorSplinePoleMode.NONE) {
			m_Pole = ResolveAsset(m_Settings.Pole);
		}

		m_PieceLength = m_Wall.GetLongestSide();
		if (m_Settings.LengthOverride > 0.05) {
			m_PieceLength = m_Settings.LengthOverride;
		}

		m_StepLength = m_PieceLength - m_Settings.Overlap;
		if (m_StepLength < 0.1) {
			m_Result.Message = string.Format("Piece length (%1 m) minus overlap (%2 m) is too small.", EditorSplineMath.FormatNumber(m_PieceLength), EditorSplineMath.FormatNumber(m_Settings.Overlap));
			return;
		}

		if (m_Spline.IsSpline()) {
			GenerateAlongCurve();
		} else {
			GenerateAlongSegments();
		}
	}

	protected void GenerateAlongSegments()
	{
		int point_count = m_Spline.Count();
		int segment_count = point_count - 1;
		if (m_Spline.Closed && point_count > 2) {
			segment_count = point_count;
		}

		for (int segment = 0; segment < segment_count; segment++) {
			vector segment_start = m_Spline.Points[segment];
			vector segment_end = m_Spline.Points[(segment + 1) % point_count];
			float segment_length = EditorSplineMath.DistanceXZ(segment_start, segment_end);
			if (segment_length < 0.05) {
				continue;
			}

			vector direction = EditorSplineMath.FlatDirection(segment_start, segment_end);
			int count;
			float spacing = m_StepLength;
			switch (m_Settings.FitMode) {
				case EditorSplineFitMode.INSIDE: {
					count = Math.Floor(segment_length / m_StepLength + 0.01);
					break;
				}

				case EditorSplineFitMode.SPREAD: {
					count = Math.Max(1, Math.Round(segment_length / m_StepLength));
					spacing = segment_length / count;
					break;
				}

				default: {
					count = Math.Ceil(segment_length / m_StepLength - 0.01);
					break;
				}
			}

			for (int i = 0; i < count; i++) {
				float along = i * spacing;
				vector piece_start = segment_start + direction * along;
				piece_start[1] = segment_start[1] + (segment_end[1] - segment_start[1]) * (along / segment_length);
				PlaceWall(piece_start, direction);

				if (m_Pole && m_Settings.PoleMode == EditorSplinePoleMode.EVERY_PIECE && i > 0) {
					PlacePole(piece_start, direction);
				}
			}

			// corner posts
			if (m_Pole) {
				PlacePole(segment_start, direction);
				if (segment == segment_count - 1 && !m_Spline.Closed) {
					PlacePole(segment_end, direction);
				}
			}

			if (IsCapped()) {
				return;
			}
		}
	}

	protected void GenerateAlongCurve()
	{
		float length = m_Path.GetLength();
		float station;
		for (int i = 0; i < EditorSplineGeneratorResult.MAX_PLACEMENTS; i++) {
			if (length - station < 0.05) {
				break;
			}

			vector piece_start = m_Path.Evaluate(station);

			// where a chord of one piece meets the curve again
			float next_station = station + m_StepLength;
			for (int refine = 0; refine < 4; refine++) {
				float reached = EditorSplineMath.DistanceXZ(piece_start, m_Path.Evaluate(next_station));
				next_station += m_StepLength - reached;
			}

			vector direction;
			if (next_station > length) {
				if (m_Settings.FitMode == EditorSplineFitMode.INSIDE) {
					break;
				}

				if (EditorSplineMath.DistanceXZ(piece_start, m_Path.Evaluate(length)) < 0.05) {
					break;
				}

				direction = EditorSplineMath.FlatDirection(piece_start, m_Path.Evaluate(length));
			} else {
				direction = EditorSplineMath.FlatDirection(piece_start, m_Path.Evaluate(next_station));
			}

			PlaceWall(piece_start, direction);
			if (m_Pole && (m_Settings.PoleMode == EditorSplinePoleMode.EVERY_PIECE || i == 0)) {
				PlacePole(piece_start, direction);
			}

			station = next_station;
			if (IsCapped()) {
				return;
			}
		}

		if (m_Pole && !m_Path.IsClosed()) {
			PlacePole(m_Path.Evaluate(length), m_Path.GetTangent(length));
		}
	}

	// The long side of the model along the line, starting exactly at start with its thickness centred on the line
	protected void PlaceWall(vector start, vector direction)
	{
		bool along_x = (m_Wall.ClipMax[0] - m_Wall.ClipMin[0]) >= (m_Wall.ClipMax[2] - m_Wall.ClipMin[2]);
		vector line_direction = direction;
		if (m_Settings.WallFlip) {
			line_direction = -direction;
		}

		vector aside, forward, across_axis;
		float along_min, along_max, across_centre;
		if (along_x) {
			aside = line_direction;
			forward = aside * vector.Up;
			across_axis = forward;
			along_min = m_Wall.ClipMin[0];
			along_max = m_Wall.ClipMax[0];
			across_centre = (m_Wall.ClipMin[2] + m_Wall.ClipMax[2]) * 0.5;
		} else {
			forward = line_direction;
			aside = vector.Up * forward;
			across_axis = aside;
			along_min = m_Wall.ClipMin[2];
			along_max = m_Wall.ClipMax[2];
			across_centre = (m_Wall.ClipMin[0] + m_Wall.ClipMax[0]) * 0.5;
		}

		// the end of the model that sits on start
		float origin_along = -along_min;
		if (m_Settings.WallFlip) {
			origin_along = along_max;
		}

		vector end = start + direction * m_PieceLength;
		float start_height = GetGroundHeight(start);
		float end_height = start_height;
		if (m_Settings.SnapToTerrain) {
			end_height = GetGroundHeight(end);
		}

		float base_height = start_height;
		if (!m_Settings.FollowSlope) {
			// level piece rests on the lowest ground under it so it never floats
			float middle_height = GetGroundHeight(start + direction * (m_PieceLength * 0.5));
			base_height = Math.Min(start_height, Math.Min(middle_height, end_height));
		}

		base_height = base_height - m_Settings.Sink + m_Settings.HeightOffset;

		vector level[4];
		level[0] = aside;
		level[1] = vector.Up;
		level[2] = forward;
		level[3] = Vector(start[0], base_height - m_Wall.ClipMin[1], start[2]) + direction * origin_along - across_axis * across_centre;

		vector transform[4];
		if (m_Settings.FollowSlope) {
			EditorSplineMath.TiltAlong(level, Vector(start[0], base_height, start[2]), direction, m_PieceLength, end_height - start_height, transform);
		} else {
			for (int i = 0; i < 4; i++) {
				transform[i] = level[i];
			}
		}

		vector marker_position = Vector(start[0], start_height, start[2]);
		m_Result.Add(m_Wall.SpawnType, transform, marker_position, direction, EditorSplineColors.WALL, 2.0);
		m_Result.AddLine(marker_position + Vector(0, 1.0, 0), Vector(end[0], end_height + 1.0, end[2]), EditorSplineColors.WALL);
	}

	// A post centred on point
	protected void PlacePole(vector point, vector direction)
	{
		vector transform[4];
		EditorSplineMath.FrameFromForwardUp(direction, vector.Up, vector.Zero, transform);

		float ground = GetGroundHeight(point);
		float centre_x = (m_Pole.ClipMin[0] + m_Pole.ClipMax[0]) * 0.5;
		float centre_z = (m_Pole.ClipMin[2] + m_Pole.ClipMax[2]) * 0.5;
		vector origin = Vector(point[0], ground - m_Settings.Sink + m_Settings.HeightOffset - m_Pole.ClipMin[1], point[2]);
		origin = origin - transform[0] * centre_x - transform[2] * centre_z;
		transform[3] = origin;

		m_Result.Add(m_Pole.SpawnType, transform, Vector(point[0], ground, point[2]), direction, EditorSplineColors.POLE, 3.0);
	}
}