class EditorSplineAssetGenerator: EditorSplineGenerator
{
	override void Generate()
	{
		array<EditorSplineAssetInfo> asset_infos = {};
		ResolveAssets(m_Settings.Assets, asset_infos);
		if (asset_infos.Count() == 0) {
			m_Result.Message = "Add an asset: select it in the world or in the placeables list and press + Selected.";
			return;
		}

		float length = m_Path.GetLength();
		array<float> stations = {};
		if (m_Settings.PlaceMode == EditorSplinePlaceMode.ON_POINTS) {
			for (int i = 0; i < m_Path.GetPointCount(); i++) {
				stations.Insert(m_Path.GetPointDistance(i));
			}
		} else {
			float spacing = Math.Max(0.25, m_Settings.Spacing);
			float station = Math.Clamp(m_Settings.StartOffset, 0, length);
			float last_station = length + 0.001;

			// a closed loop ends where it starts
			if (m_Path.IsClosed()) {
				last_station = length - spacing * 0.5;
			}

			while (station <= last_station && stations.Count() < EditorSplineGeneratorResult.MAX_PLACEMENTS) {
				stations.Insert(station);
				station += spacing;
			}
		}

		EditorRandom random = new EditorRandom(m_Settings.Seed);
		int side_count = 1;
		if (m_Settings.BothSides) {
			side_count = 2;
		}

		int placed_count;
		foreach (float distance: stations) {
			vector on_line = m_Path.Evaluate(distance);
			vector tangent = m_Path.GetTangent(distance);
			vector right = EditorSplineMath.RightOf(tangent);

			for (int side_index = 0; side_index < side_count; side_index++) {
				float side = 1.0;
				if (side_index == 1) {
					side = -1.0;
				}

				EditorSplineAssetInfo asset_info = asset_infos[placed_count % asset_infos.Count()];
				if (m_Settings.PickMode == EditorSplinePickMode.RANDOM) {
					asset_info = asset_infos[random.Index(asset_infos.Count())];
				}

				placed_count++;

				vector ground = on_line + right * (m_Settings.SideOffset * side);
				if (m_Settings.PositionJitter > 0) {
					ground[0] = ground[0] + random.Range(-m_Settings.PositionJitter, m_Settings.PositionJitter);
					ground[2] = ground[2] + random.Range(-m_Settings.PositionJitter, m_Settings.PositionJitter);
				}

				float ground_height = GetGroundHeight(ground);
				ground[1] = ground_height;

				vector up = vector.Up;
				if (m_Settings.AlignToTerrain) {
					up = GetGame().SurfaceGetNormal(ground[0], ground[2]);
				}

				float scale = random.Range(m_Settings.ScaleMin, m_Settings.ScaleMax);
				PlaceAsset(asset_info, ground, GetForward(tangent, right, side, random), up, scale, m_Settings.HeightOffset, EditorSplineColors.ASSET, 2.5);
				if (IsCapped()) {
					return;
				}
			}
		}
	}

	protected vector GetForward(vector tangent, vector right, float side, EditorRandom random)
	{
		vector forward = tangent;
		switch (m_Settings.DirectionMode) {
			case EditorSplineDirectionMode.REVERSE: {
				forward = -tangent;
				break;
			}

			// both sides face away from the line, lamps and signs want a 180 yaw offset to face it
			case EditorSplineDirectionMode.SIDEWAYS: {
				forward = right * side;
				break;
			}

			case EditorSplineDirectionMode.RANDOM: {
				forward = EditorSplineMath.DirectionOfYaw(random.Range(0, Math.PI2));
				break;
			}

			case EditorSplineDirectionMode.FIXED: {
				forward = Vector(0, 0, 1);
				break;
			}
		}

		float yaw_offset = m_Settings.YawOffset;
		if (m_Settings.YawJitter > 0) {
			yaw_offset += random.Range(-m_Settings.YawJitter, m_Settings.YawJitter);
		}

		if (yaw_offset != 0) {
			forward = EditorSplineMath.RotateYaw(forward, yaw_offset);
		}

		return forward;
	}
}