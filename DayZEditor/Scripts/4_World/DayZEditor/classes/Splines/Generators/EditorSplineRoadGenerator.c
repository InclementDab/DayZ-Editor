class EditorSplineRoadPiece
{
	EditorSplineAssetInfo AssetInfo;
	bool Reversed;
	bool IsCurve;
	float StartX;
	float StartY;
	float StartZ;
	float StartYaw;
	float EndX;
	float EndY;
	float EndZ;
	float EndYaw;
	float Chord;
	float Width;
}

// Road parts are joined end to end on their lb pb le pe memory points. Every step tries every part both ways
// and keeps the longest one that ends within MaxDeviation of the spline, or the closest one when none does
class EditorSplineRoadGenerator: EditorSplineGenerator
{
	// metres of position error one radian of heading error is worth
	protected static const float HEADING_WEIGHT = 8.0;
	protected static const int MAX_PIECES = 5000;
	protected static const int LOOKAHEAD_CANDIDATES = 5;

	protected ref array<ref EditorSplineRoadPiece> m_Pieces = {};

	override void Generate()
	{
		CollectPieces();
		if (m_Pieces.Count() == 0) {
			m_Result.Message = "No usable road parts for '" + m_Settings.RoadKit + "'.";
			return;
		}

		float shortest = float.MAX;
		foreach (EditorSplineRoadPiece measured_piece: m_Pieces) {
			shortest = Math.Min(shortest, measured_piece.Chord);
		}

		float length = m_Path.GetLength();
		float tolerance = Math.Max(0.05, m_Settings.MaxDeviation);

		vector start = m_Path.Evaluate(0);
		float x = start[0];
		float z = start[2];
		float yaw = EditorSplineMath.YawOf(m_Path.GetTangent(0));
		float station;

		for (int i = 0; i < MAX_PIECES; i++) {
			if (length - station < shortest * 0.5) {
				break;
			}

			float chosen_station;
			EditorSplineRoadPiece chosen_piece = Choose(x, z, yaw, station, tolerance, chosen_station);
			if (!chosen_piece) {
				break;
			}

			float next_x, next_z, next_yaw;
			Emit(chosen_piece, x, z, yaw, station, chosen_station, next_x, next_z, next_yaw);
			x = next_x;
			z = next_z;
			yaw = next_yaw;

			// continue from where the part really ends so parts always join
			float gap;
			station = m_Path.FindClosest(Vector(x, 0, z), station, chosen_station + 5.0, gap);
			if (station < chosen_station - 2.0) {
				station = chosen_station;
			}

			if (IsCapped()) {
				return;
			}
		}
	}

	protected EditorSplineRoadPiece Choose(float x, float z, float yaw, float station, float tolerance, out float chosen_station)
	{
		EditorSplineRoadPiece best_fit;
		float best_fit_advance = -1;
		float best_fit_cost = float.MAX;
		float best_fit_station;

		array<EditorSplineRoadPiece> candidates = {};
		array<float> candidate_costs = {};
		array<float> candidate_stations = {};

		foreach (EditorSplineRoadPiece piece: m_Pieces) {
			float new_station, cost, end_x, end_z, end_yaw;
			if (!Score(piece, x, z, yaw, station, new_station, cost, end_x, end_z, end_yaw)) {
				continue;
			}

			candidates.Insert(piece);
			candidate_costs.Insert(cost);
			candidate_stations.Insert(new_station);

			if (cost <= tolerance) {
				float advance = new_station - station;
				if (advance > best_fit_advance + 0.01 || (Math.AbsFloat(advance - best_fit_advance) <= 0.01 && cost < best_fit_cost)) {
					best_fit = piece;
					best_fit_advance = advance;
					best_fit_cost = cost;
					best_fit_station = new_station;
				}
			}
		}

		if (best_fit) {
			chosen_station = best_fit_station;
			return best_fit;
		}

		if (candidates.Count() == 0) {
			return null;
		}

		// nothing fits, of the closest few take the one that also leaves the best next step
		EditorSplineRoadPiece best_any;
		float best_any_total = float.MAX;
		for (int round = 0; round < LOOKAHEAD_CANDIDATES && round < candidates.Count(); round++) {
			int lowest_index = -1;
			float lowest_cost = float.MAX;
			for (int i = 0; i < candidates.Count(); i++) {
				if (candidate_costs[i] < lowest_cost) {
					lowest_cost = candidate_costs[i];
					lowest_index = i;
				}
			}

			if (lowest_index == -1) {
				break;
			}

			EditorSplineRoadPiece candidate = candidates[lowest_index];
			float candidate_station = candidate_stations[lowest_index];
			candidate_costs[lowest_index] = float.MAX;

			float follow_x, follow_z, follow_yaw, unused_station, unused_cost;
			Score(candidate, x, z, yaw, station, unused_station, unused_cost, follow_x, follow_z, follow_yaw);
			float total = lowest_cost + 0.5 * GetBestCost(follow_x, follow_z, follow_yaw, candidate_station);
			if (total < best_any_total) {
				best_any_total = total;
				best_any = candidate;
				chosen_station = candidate_station;
			}
		}

		return best_any;
	}

	protected float GetBestCost(float x, float z, float yaw, float station)
	{
		if (m_Path.GetLength() - station < 3.0) {
			return 0;
		}

		float best = float.MAX;
		foreach (EditorSplineRoadPiece piece: m_Pieces) {
			float new_station, cost, end_x, end_z, end_yaw;
			if (Score(piece, x, z, yaw, station, new_station, cost, end_x, end_z, end_yaw)) {
				best = Math.Min(best, cost);
			}
		}

		if (best == float.MAX) {
			return 0;
		}

		return best;
	}

	// Where this part would end and how far that is from the spline, position plus heading
	protected bool Score(EditorSplineRoadPiece piece, float x, float z, float yaw, float station, out float new_station, out float cost, out float end_x, out float end_z, out float end_yaw)
	{
		float model_x, model_z, model_yaw;
		Place(piece, x, z, yaw, model_x, model_z, model_yaw, end_x, end_z, end_yaw);

		float gap;
		new_station = m_Path.FindClosest(Vector(end_x, 0, end_z), station, station + piece.Chord * 1.6 + 3.0, gap);
		if (new_station <= station + piece.Chord * 0.25) {
			return false;
		}

		float path_yaw = EditorSplineMath.YawOf(m_Path.GetTangent(new_station));
		float heading_error = Math.AbsFloat(EditorSplineMath.WrapAngle(end_yaw - path_yaw));
		cost = gap + heading_error * HEADING_WEIGHT;
		return true;
	}

	// 2D placement of a part whose begin goes on x z looking along yaw (radians)
	protected void Place(EditorSplineRoadPiece piece, float x, float z, float yaw, out float model_x, out float model_z, out float model_yaw, out float end_x, out float end_z, out float end_yaw)
	{
		model_yaw = yaw - piece.StartYaw;
		float cos_yaw = Math.Cos(model_yaw);
		float sin_yaw = Math.Sin(model_yaw);

		// model x axis = (cos, -sin), model z axis = (sin, cos)
		model_x = x - (cos_yaw * piece.StartX + sin_yaw * piece.StartZ);
		model_z = z - (cos_yaw * piece.StartZ - sin_yaw * piece.StartX);
		end_x = model_x + cos_yaw * piece.EndX + sin_yaw * piece.EndZ;
		end_z = model_z + cos_yaw * piece.EndZ - sin_yaw * piece.EndX;
		end_yaw = model_yaw + piece.EndYaw;
	}

	protected float GetRoadHeight(float station, vector point)
	{
		if (m_Settings.SnapToTerrain) {
			return EditorSplineMath.TerrainY(point) + m_Settings.HeightOffset;
		}

		vector on_path = m_Path.Evaluate(station);
		return on_path[1] + m_Settings.HeightOffset;
	}

	protected void Emit(EditorSplineRoadPiece piece, float x, float z, float yaw, float station, float new_station, out float next_x, out float next_z, out float next_yaw)
	{
		float model_x, model_z, model_yaw, end_x, end_z, end_yaw;
		Place(piece, x, z, yaw, model_x, model_z, model_yaw, end_x, end_z, end_yaw);

		vector start_point = Vector(x, 0, z);
		vector end_point = Vector(end_x, 0, end_z);
		float start_height = GetRoadHeight(station, start_point);
		float end_height = GetRoadHeight(new_station, end_point);
		start_point[1] = start_height;
		end_point[1] = end_height;

		vector level[4];
		level[0] = Vector(Math.Cos(model_yaw), 0, -Math.Sin(model_yaw));
		level[1] = vector.Up;
		level[2] = Vector(Math.Sin(model_yaw), 0, Math.Cos(model_yaw));
		level[3] = Vector(model_x, start_point[1] - piece.StartY, model_z);

		float run = EditorSplineMath.DistanceXZ(start_point, end_point);
		float rise = (end_point[1] - start_point[1]) - (piece.EndY - piece.StartY);
		vector tilted[4];
		EditorSplineMath.TiltAlong(level, start_point, EditorSplineMath.FlatDirection(start_point, end_point), run, rise, tilted);

		vector heading = EditorSplineMath.DirectionOfYaw(yaw);
		m_Result.Add(piece.AssetInfo.SpawnType, tilted, start_point, heading, EditorSplineColors.ROAD, 1.5);

		// the joint across the road and the centre line of the part
		vector across = EditorSplineMath.RightOf(heading) * (piece.Width * 0.5);
		vector lift = Vector(0, 0.2, 0);
		m_Result.AddLine(start_point - across + lift, start_point + across + lift, EditorSplineColors.ROAD_JOINT);
		m_Result.AddLine(start_point + lift, end_point + lift, EditorSplineColors.ROAD);

		// a sloped part covers a little less ground
		vector model_end = Vector(piece.EndX, piece.EndY, piece.EndZ);
		vector world_end = model_end.Multiply4(tilted);
		next_x = world_end[0];
		next_z = world_end[2];
		next_yaw = end_yaw;
	}

	protected void CollectPieces()
	{
		string kit = m_Settings.RoadKit;
		kit.ToLower();
		bool custom = kit == "custom";

		array<string> names = {};
		if (custom) {
			names.Copy(m_Settings.RoadPieces);
		} else {
			string prefix = EditorSplineGeneratorSettings.GetRoadKitPrefix(m_Settings.RoadKit);
			foreach (string straight: EditorSplineAssetLists.ROAD_STRAIGHTS) {
				names.Insert(prefix + straight);
			}

			if (m_Settings.RoadCurves) {
				foreach (string curve: EditorSplineAssetLists.ROAD_CURVES) {
					names.Insert(prefix + curve);
				}
			}
		}

		foreach (string name: names) {
			EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(name);
			if (!asset_info || !asset_info.Valid) {
				// kits are tried with every suffix, a family not having one is fine
				if (custom) {
					m_Result.Missing++;
				}

				continue;
			}

			if (m_ConsoleOnly && !asset_info.ConsoleFriendly) {
				m_Result.NotConsoleFriendly++;
				continue;
			}

			AddPieces(asset_info);
		}
	}

	protected void AddPieces(EditorSplineAssetInfo asset_info)
	{
		vector begin_point, end_point, begin_direction, end_direction;
		float width;
		if (asset_info.HasRoadPoints) {
			begin_point = asset_info.RoadBegin;
			end_point = asset_info.RoadEnd;
			begin_direction = asset_info.RoadBeginDirection;
			end_direction = asset_info.RoadEndDirection;
			width = asset_info.RoadWidth;
		} else {
			// no memory points, only straight parts along the model z axis
			if (IsCurveName(asset_info.Name)) {
				m_Result.Skipped++;
				return;
			}

			float centre_x = (asset_info.ClipMin[0] + asset_info.ClipMax[0]) * 0.5;
			begin_point = Vector(centre_x, 0, asset_info.ClipMin[2]);
			end_point = Vector(centre_x, 0, asset_info.ClipMax[2]);
			begin_direction = Vector(0, 0, 1);
			end_direction = Vector(0, 0, 1);
			width = asset_info.ClipMax[0] - asset_info.ClipMin[0];
		}

		EditorSplineRoadPiece forward_piece = new EditorSplineRoadPiece();
		forward_piece.AssetInfo = asset_info;
		forward_piece.StartX = begin_point[0];
		forward_piece.StartY = begin_point[1];
		forward_piece.StartZ = begin_point[2];
		forward_piece.StartYaw = EditorSplineMath.YawOf(begin_direction);
		forward_piece.EndX = end_point[0];
		forward_piece.EndY = end_point[1];
		forward_piece.EndZ = end_point[2];
		forward_piece.EndYaw = EditorSplineMath.YawOf(end_direction);
		forward_piece.Chord = EditorSplineMath.DistanceXZ(begin_point, end_point);
		forward_piece.Width = width;
		forward_piece.IsCurve = Math.AbsFloat(EditorSplineMath.WrapAngle(forward_piece.EndYaw - forward_piece.StartYaw)) > 0.0035;
		if (forward_piece.Chord < 0.5) {
			m_Result.Skipped++;
			return;
		}

		m_Pieces.Insert(forward_piece);
		if (!forward_piece.IsCurve) {
			return;
		}

		// laid backwards a curve turns the other way
		EditorSplineRoadPiece reversed_piece = new EditorSplineRoadPiece();
		reversed_piece.AssetInfo = asset_info;
		reversed_piece.Reversed = true;
		reversed_piece.IsCurve = true;
		reversed_piece.StartX = end_point[0];
		reversed_piece.StartY = end_point[1];
		reversed_piece.StartZ = end_point[2];
		reversed_piece.StartYaw = EditorSplineMath.YawOf(-end_direction);
		reversed_piece.EndX = begin_point[0];
		reversed_piece.EndY = begin_point[1];
		reversed_piece.EndZ = begin_point[2];
		reversed_piece.EndYaw = EditorSplineMath.YawOf(-begin_direction);
		reversed_piece.Chord = forward_piece.Chord;
		reversed_piece.Width = width;
		m_Pieces.Insert(reversed_piece);
	}

	// curve parts are named <degrees>_<radius> (asf1_10_25) or end in curve
	protected static bool IsCurveName(string name)
	{
		string lower = name;
		lower.ToLower();
		if (lower.Contains("curve")) {
			return true;
		}

		int last = lower.LastIndexOf("_");
		if (last <= 0) {
			return false;
		}

		string before_last = lower.Substring(0, last);
		int second_last = before_last.LastIndexOf("_");
		if (second_last == -1) {
			return false;
		}

		string middle = before_last.Substring(second_last + 1, before_last.Length() - second_last - 1);
		if (middle.Length() == 0) {
			return false;
		}

		string digits = "0123456789";
		for (int i = 0; i < middle.Length(); i++) {
			if (digits.IndexOf(middle.Substring(i, 1)) == -1) {
				return false;
			}
		}

		return true;
	}
}