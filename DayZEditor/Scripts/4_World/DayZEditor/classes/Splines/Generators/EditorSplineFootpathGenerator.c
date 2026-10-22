// Flat pieces end to end along the line, chord by chord so they follow every bend, each one lying on the ground
class EditorSplineFootpathGenerator: EditorSplineGenerator
{
	override void Generate()
	{
		array<EditorSplineAssetInfo> asset_infos = {};
		ResolveAssets(m_Settings.PathPieces, asset_infos);
		if (asset_infos.Count() == 0) {
			m_Result.Message = "Add path pieces: select one in the world or in the placeables list and press + Selected.";
			return;
		}

		EditorRandom random = new EditorRandom(m_Settings.Seed);
		float length = m_Path.GetLength();
		float station;
		for (int i = 0; i < EditorSplineGeneratorResult.MAX_PLACEMENTS; i++) {
			if (length - station < 0.05) {
				break;
			}

			EditorSplineAssetInfo asset_info = asset_infos[i % asset_infos.Count()];
			if (m_Settings.PickMode == EditorSplinePickMode.RANDOM) {
				asset_info = asset_infos[random.Index(asset_infos.Count())];
			}

			float scale = random.Range(m_Settings.ScaleMin, m_Settings.ScaleMax);
			float yaw_jitter = random.Range(-m_Settings.YawJitter, m_Settings.YawJitter);
			float side_jitter = random.Range(-m_Settings.PositionJitter, m_Settings.PositionJitter);

			float piece_length = asset_info.GetLongestSide();
			if (m_Settings.LengthOverride > 0.05) {
				piece_length = m_Settings.LengthOverride;
			}

			piece_length *= scale;
			float step_length = piece_length - m_Settings.Overlap;
			if (step_length < 0.1) {
				m_Result.Message = string.Format("Piece length (%1 m) minus overlap (%2 m) is too small.", EditorSplineMath.FormatNumber(piece_length), EditorSplineMath.FormatNumber(m_Settings.Overlap));
				return;
			}

			vector piece_start = m_Path.Evaluate(station);

			// where a chord of one step meets the line again
			float next_station = station + step_length;
			for (int refine = 0; refine < 4; refine++) {
				float reached = EditorSplineMath.DistanceXZ(piece_start, m_Path.Evaluate(next_station));
				next_station += step_length - reached;
			}

			vector aim = m_Path.Evaluate(Math.Min(next_station, length));
			if (EditorSplineMath.DistanceXZ(piece_start, aim) < 0.05) {
				break;
			}

			vector direction = EditorSplineMath.FlatDirection(piece_start, aim);
			if (yaw_jitter != 0) {
				direction = EditorSplineMath.RotateYaw(direction, yaw_jitter);
			}

			PlacePiece(asset_info, piece_start, direction, piece_length, scale, side_jitter);
			station = next_station;
			if (IsCapped()) {
				return;
			}
		}
	}

	protected void PlacePiece(EditorSplineAssetInfo asset_info, vector start, vector direction, float piece_length, float scale, float side_offset)
	{
		vector start_ground = start;
		vector end_ground = start + direction * piece_length;
		float start_height = GetGroundHeight(start_ground);
		float end_height = GetGroundHeight(end_ground);
		start_ground[1] = start_height;
		end_ground[1] = end_height;

		vector up = vector.Up;
		if (m_Settings.AlignToTerrain) {
			vector middle = (start_ground + end_ground) * 0.5;
			up = GetGame().SurfaceGetNormal(middle[0], middle[2]);
			up.Normalize();
		}

		// the chord with the part pointing into the ground removed
		vector chord = end_ground - start_ground;
		vector along = chord - up * vector.Dot(chord, up);
		if (along.LengthSq() < 0.0001) {
			along = direction;
		}

		along.Normalize();

		bool along_x = (asset_info.ClipMax[0] - asset_info.ClipMin[0]) >= (asset_info.ClipMax[2] - asset_info.ClipMin[2]);
		vector aside, forward, across_axis;
		float along_min, across_centre;
		if (along_x) {
			aside = along;
			forward = aside * up;
			across_axis = forward;
			along_min = asset_info.ClipMin[0];
			across_centre = (asset_info.ClipMin[2] + asset_info.ClipMax[2]) * 0.5;
		} else {
			forward = along;
			aside = up * forward;
			across_axis = aside;
			along_min = asset_info.ClipMin[2];
			across_centre = (asset_info.ClipMin[0] + asset_info.ClipMax[0]) * 0.5;
		}

		vector origin = start_ground - along * (along_min * scale) - across_axis * (across_centre * scale) + across_axis * side_offset;
		origin = origin + up * (-asset_info.ClipMin[1] * scale + m_Settings.HeightOffset);

		vector transform[4];
		transform[0] = aside * scale;
		transform[1] = up * scale;
		transform[2] = forward * scale;
		transform[3] = origin;
		m_Result.Add(asset_info.SpawnType, transform, start_ground, along, EditorSplineColors.PATH, 0.8);

		vector lift = Vector(0, 0.25, 0);
		m_Result.AddLine(start_ground + lift, end_ground + lift, EditorSplineColors.PATH);
	}
}