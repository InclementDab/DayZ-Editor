enum EditorSplineForestLayer
{
	TREES,
	BUSHES,
	ROCKS
};

// Trees, bushes and rocks scattered on a jittered grid inside the spline, thinner along the edge with more bushes
class EditorSplineForestGenerator: EditorSplineGenerator
{
	protected static const int OUTLINE_POINTS = 400;

	protected ref array<vector> m_Outline;
	protected ref EditorSplineForestGrid m_TreeGrid;
	protected ref EditorRandom m_Random;
	protected float m_MinX, m_MinZ, m_MaxX, m_MaxZ;

	override void Generate()
	{
		if (m_Spline.Count() < 3) {
			m_Result.Message = "A forest needs an area: add at least three points.";
			return;
		}

		m_Path.GetOutline(OUTLINE_POINTS, m_Outline);

		// an open spline is closed with a straight line back to its first point
		if (!m_Path.IsClosed()) {
			m_Outline.Insert(m_Spline.Points[0]);
		}

		if (m_Outline.Count() < 3) {
			m_Result.Message = "A forest needs an area: add at least three points.";
			return;
		}

		array<EditorSplineAssetInfo> trees = {};
		array<EditorSplineAssetInfo> bushes = {};
		array<EditorSplineAssetInfo> rocks = {};
		ResolveAssets(m_Settings.Trees, trees);
		ResolveAssets(m_Settings.Bushes, bushes);
		ResolveAssets(m_Settings.Rocks, rocks);
		if (trees.Count() == 0 && bushes.Count() == 0 && rocks.Count() == 0) {
			m_Result.Message = "Add trees, bushes or rocks to the lists.";
			return;
		}

		m_MinX = float.MAX;
		m_MinZ = float.MAX;
		m_MaxX = -float.MAX;
		m_MaxZ = -float.MAX;
		for (int i = 0; i < m_Outline.Count(); i++) {
			vector outline_from = m_Outline[i];
			vector outline_to = m_Outline[(i + 1) % m_Outline.Count()];
			m_MinX = Math.Min(m_MinX, outline_from[0]);
			m_MinZ = Math.Min(m_MinZ, outline_from[2]);
			m_MaxX = Math.Max(m_MaxX, outline_from[0]);
			m_MaxZ = Math.Max(m_MaxZ, outline_from[2]);

			float from_height = EditorSplineMath.TerrainY(outline_from) + 0.3;
			float to_height = EditorSplineMath.TerrainY(outline_to) + 0.3;
			outline_from[1] = from_height;
			outline_to[1] = to_height;
			m_Result.AddLine(outline_from, outline_to, EditorSplineColors.OUTLINE);
		}

		m_Random = new EditorRandom(m_Settings.Seed);
		m_TreeGrid = new EditorSplineForestGrid(Math.Max(1.0, m_Settings.MinDistance));

		Scatter(trees, m_Settings.TreeDensity, EditorSplineForestLayer.TREES);
		Scatter(rocks, m_Settings.RockDensity, EditorSplineForestLayer.ROCKS);
		Scatter(bushes, m_Settings.BushDensity, EditorSplineForestLayer.BUSHES);
	}

	protected void Scatter(array<EditorSplineAssetInfo> asset_infos, float density, EditorSplineForestLayer layer)
	{
		if (asset_infos.Count() == 0 || density <= 0) {
			return;
		}

		// bushes are scattered at their edge density, the inside keeps the normal share of them
		float grid_density = density;
		float edge_boost = Math.Max(1.0, m_Settings.EdgeBushBoost);
		if (layer == EditorSplineForestLayer.BUSHES) {
			grid_density = density * edge_boost;
		}

		float cell = Math.Max(0.5, Math.Sqrt(10000.0 / grid_density));
		float edge_width = Math.Max(0, m_Settings.EdgeWidth);
		float keep_distance = Math.Max(0.5, m_Settings.MinDistance);
		if (layer != EditorSplineForestLayer.TREES) {
			keep_distance *= 0.5;
		}

		int columns = Math.Ceil((m_MaxX - m_MinX) / cell);
		int rows = Math.Ceil((m_MaxZ - m_MinZ) / cell);
		if (columns * rows > 4000000) {
			m_Result.Message = "The area is too large for this density, lower the density or split the spline.";
			return;
		}

		LinearColor color = EditorSplineColors.TREE;
		float marker_height = 4.0;
		float sink = m_Settings.TreeSink;
		switch (layer) {
			case EditorSplineForestLayer.BUSHES: {
				color = EditorSplineColors.BUSH;
				marker_height = 1.5;
				sink = m_Settings.TreeSink * 0.5;
				break;
			}

			case EditorSplineForestLayer.ROCKS: {
				color = EditorSplineColors.ROCK;
				marker_height = 1.0;
				sink = m_Settings.RockSink;
				break;
			}
		}

		for (int column = 0; column < columns; column++) {
			for (int row = 0; row < rows; row++) {
				float x = m_MinX + (column + m_Random.Next01()) * cell;
				float z = m_MinZ + (row + m_Random.Next01()) * cell;

				// same amount of random numbers every cell, so changing one filter does not reshuffle the rest
				float keep_roll = m_Random.Next01();
				float yaw_roll = m_Random.Range(0, 360);
				float scale_roll = m_Random.Range(m_Settings.ScaleMin, m_Settings.ScaleMax);
				int pick_roll = m_Random.Index(asset_infos.Count());

				if (!EditorSplinePath.IsInside(m_Outline, x, z)) {
					continue;
				}

				if (edge_width > 0) {
					float edge_factor = Math.Clamp(EditorSplinePath.DistanceToEdge(m_Outline, x, z) / edge_width, 0, 1);
					if (layer == EditorSplineForestLayer.TREES && keep_roll > 0.35 + 0.65 * edge_factor) {
						continue;
					}

					if (layer == EditorSplineForestLayer.BUSHES && edge_factor >= 1 && keep_roll > 1.0 / edge_boost) {
						continue;
					}
				} else if (layer == EditorSplineForestLayer.BUSHES && keep_roll > 1.0 / edge_boost) {
					continue;
				}

				vector ground = Vector(x, 0, z);
				if (m_Settings.AvoidWater && EditorSplineMath.IsWater(ground)) {
					continue;
				}

				if (m_Settings.MaxSlope > 0 && m_Settings.MaxSlope < 90 && EditorSplineMath.TerrainSlope(ground) > m_Settings.MaxSlope) {
					continue;
				}

				if (m_TreeGrid.IsBlocked(x, z, keep_distance)) {
					continue;
				}

				if (layer == EditorSplineForestLayer.TREES) {
					m_TreeGrid.Add(x, z);
				}

				float ground_height = EditorSplineMath.TerrainY(ground);
				ground[1] = ground_height;

				vector up = vector.Up;
				if (layer == EditorSplineForestLayer.ROCKS && m_Settings.AlignRocks) {
					up = GetGame().SurfaceGetNormal(x, z);
				}

				PlaceAsset(asset_infos[pick_roll], ground, EditorSplineMath.DirectionOfYaw(yaw_roll * Math.DEG2RAD), up, scale_roll, m_Settings.HeightOffset - sink, color, marker_height);
				if (IsCapped()) {
					return;
				}
			}
		}
	}
}

// Spatial hash of placed trees for the minimum distance
class EditorSplineForestGrid
{
	protected float m_CellSize;
	protected ref map<int, ref array<vector>> m_Cells = new map<int, ref array<vector>>();

	void EditorSplineForestGrid(float cell_size)
	{
		m_CellSize = cell_size;
	}

	protected int GetKey(int cell_x, int cell_z)
	{
		return cell_x * 65536 + cell_z;
	}

	void Add(float x, float z)
	{
		int cell_x = Math.Floor(x / m_CellSize);
		int cell_z = Math.Floor(z / m_CellSize);
		int key = GetKey(cell_x, cell_z);
		array<vector> cell;
		if (!m_Cells.Find(key, cell)) {
			cell = new array<vector>();
			m_Cells.Insert(key, cell);
		}

		cell.Insert(Vector(x, 0, z));
	}

	bool IsBlocked(float x, float z, float distance)
	{
		float distance_sq = distance * distance;
		int reach = Math.Ceil(distance / m_CellSize);
		int centre_x = Math.Floor(x / m_CellSize);
		int centre_z = Math.Floor(z / m_CellSize);
		for (int offset_x = -reach; offset_x <= reach; offset_x++) {
			for (int offset_z = -reach; offset_z <= reach; offset_z++) {
				array<vector> cell;
				if (!m_Cells.Find(GetKey(centre_x + offset_x, centre_z + offset_z), cell)) {
					continue;
				}

				foreach (vector other: cell) {
					float dx = other[0] - x;
					float dz = other[2] - z;
					if (dx * dx + dz * dz < distance_sq) {
						return true;
					}
				}
			}
		}

		return false;
	}
}