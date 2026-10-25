// A spline sampled about every metre. Distances are measured on the ground plane so spacing on a hill stays the same
class EditorSplinePath
{
	protected ref array<vector> m_Positions = {};
	protected ref array<float> m_Distances = {};
	protected ref array<int> m_Segments = {};
	protected ref array<float> m_PointDistances = {};
	protected bool m_Closed;
	protected float m_Length;

	static EditorSplinePath Create(EditorSplineData spline)
	{
		EditorSplinePath path = new EditorSplinePath();
		if (!spline) {
			return path;
		}

		int point_count = spline.Count();
		if (point_count == 0) {
			return path;
		}

		path.m_Closed = spline.Closed && point_count > 2;
		path.AppendSample(spline.Points[0], 0);
		if (point_count == 1) {
			path.m_PointDistances.Insert(0);
			return path;
		}

		int segment_count = point_count - 1;
		if (path.m_Closed) {
			segment_count = point_count;
		}

		// coarser for very long splines so a 20km road stays cheap
		float rough_length;
		for (int i = 0; i < segment_count; i++) {
			rough_length += vector.Distance(spline.Points[i], spline.Points[(i + 1) % point_count]);
		}

		float step = Math.Max(1.0, rough_length / 8000.0);
		bool curved = spline.IsSpline() && point_count > 2;
		float tangent_scale = 1.0 - Math.Clamp(spline.Tension, 0.0, 1.0);

		for (int segment = 0; segment < segment_count; segment++) {
			path.m_PointDistances.Insert(path.m_Length);

			vector start = spline.Points[segment];
			vector end = spline.Points[(segment + 1) % point_count];
			float chord = vector.Distance(start, end);
			int steps = Math.Max(1, Math.Ceil(chord / step));

			vector start_tangent, end_tangent;
			if (curved) {
				start_tangent = GetPointDirection(spline, segment, path.m_Closed) * (chord * tangent_scale);
				end_tangent = GetPointDirection(spline, (segment + 1) % point_count, path.m_Closed) * (chord * tangent_scale);
			}

			for (int j = 1; j <= steps; j++) {
				float t = j;
				t = t / steps;

				vector sample;
				if (curved) {
					sample = Hermite(start, end, start_tangent, end_tangent, t);
				} else {
					sample = start + (end - start) * t;
				}

				path.AppendSample(sample, segment);
			}
		}

		if (!path.m_Closed) {
			path.m_PointDistances.Insert(path.m_Length);
		}

		return path;
	}

	// Catmull-Rom direction, scaled by the segment length in Create so uneven spacing does not loop
	protected static vector GetPointDirection(EditorSplineData spline, int index, bool closed)
	{
		int count = spline.Count();
		vector point = spline.Points[index];
		if (!closed && index == 0) {
			return EndDirection(point, spline.Points[1], GetInnerDirection(spline, 1, closed));
		}

		if (!closed && index == count - 1) {
			return -EndDirection(point, spline.Points[count - 2], -GetInnerDirection(spline, count - 2, closed));
		}

		return GetInnerDirection(spline, index, closed);
	}

	protected static vector GetInnerDirection(EditorSplineData spline, int index, bool closed)
	{
		int count = spline.Count();
		int previous = index - 1;
		int next = index + 1;
		if (closed) {
			previous = (index - 1 + count) % count;
			next = (index + 1) % count;
		}

		previous = Math.Clamp(previous, 0, count - 1);
		next = Math.Clamp(next, 0, count - 1);

		vector direction = spline.Points[next] - spline.Points[previous];
		if (direction.LengthSq() < 0.000001) {
			return Vector(0, 0, 1);
		}

		return direction.Normalized();
	}

	// Natural end, the neighbour tangent mirrored over the end segment so the curve does not bend at the end
	protected static vector EndDirection(vector end_point, vector neighbour, vector neighbour_direction)
	{
		vector chord = neighbour - end_point;
		if (chord.LengthSq() < 0.000001) {
			return neighbour_direction;
		}

		vector chord_direction = chord.Normalized();
		vector mirrored = chord_direction * 2.0 - neighbour_direction;
		if (mirrored.LengthSq() < 0.000001) {
			return chord_direction;
		}

		return mirrored.Normalized();
	}

	// https://www.cubic.org/docs/hermite.htm
	protected static vector Hermite(vector p0, vector p1, vector m0, vector m1, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;
		return p0 * (2 * t3 - 3 * t2 + 1) + m0 * (t3 - 2 * t2 + t) + p1 * (-2 * t3 + 3 * t2) + m1 * (t3 - t2);
	}

	protected void AppendSample(vector position, int segment)
	{
		if (m_Positions.Count() > 0) {
			m_Length += EditorSplineMath.DistanceXZ(m_Positions[m_Positions.Count() - 1], position);
		}

		m_Positions.Insert(position);
		m_Distances.Insert(m_Length);
		m_Segments.Insert(segment);
	}

	int Count()
	{
		return m_Positions.Count();
	}

	vector GetSample(int index)
	{
		return m_Positions[index];
	}

	float GetSampleDistance(int index)
	{
		return m_Distances[index];
	}

	// Index of the point the segment of this sample starts at
	int GetSampleSegment(int index)
	{
		return m_Segments[index];
	}

	float GetLength()
	{
		return m_Length;
	}

	bool IsClosed()
	{
		return m_Closed;
	}

	int GetPointCount()
	{
		return m_PointDistances.Count();
	}

	float GetPointDistance(int index)
	{
		if (index < 0 || index >= m_PointDistances.Count()) {
			return 0;
		}

		return m_PointDistances[index];
	}

	// Last sample at or before the distance
	int FindSample(float distance)
	{
		int low = 0;
		int high = m_Distances.Count() - 1;
		if (high <= 0) {
			return 0;
		}

		if (distance >= m_Distances[high]) {
			return high - 1;
		}

		while (high - low > 1) {
			int middle = (low + high) / 2;
			if (m_Distances[middle] <= distance) {
				low = middle;
			} else {
				high = middle;
			}
		}

		return low;
	}

	protected float WrapDistance(float distance)
	{
		if (m_Length <= 0) {
			return 0;
		}

		if (!m_Closed) {
			return Math.Clamp(distance, 0, m_Length);
		}

		float wrapped = distance;
		while (wrapped < 0) {
			wrapped += m_Length;
		}

		while (wrapped > m_Length) {
			wrapped -= m_Length;
		}

		return wrapped;
	}

	vector Evaluate(float distance)
	{
		int count = m_Positions.Count();
		if (count == 0) {
			return vector.Zero;
		}

		if (count == 1) {
			return m_Positions[0];
		}

		float clamped = WrapDistance(distance);
		int index = FindSample(clamped);
		float span = m_Distances[index + 1] - m_Distances[index];
		if (span < 0.00001) {
			return m_Positions[index + 1];
		}

		float t = (clamped - m_Distances[index]) / span;
		return m_Positions[index] + (m_Positions[index + 1] - m_Positions[index]) * t;
	}

	// Horizontal direction of travel, smoothed over a metre so polyline corners blend
	vector GetTangent(float distance)
	{
		if (m_Positions.Count() < 2) {
			return Vector(0, 0, 1);
		}

		float before = distance - 0.5;
		float after = distance + 0.5;
		if (!m_Closed) {
			before = Math.Max(0, before);
			after = Math.Min(m_Length, after);
		}

		vector from = Evaluate(before);
		vector to = Evaluate(after);
		if (EditorSplineMath.DistanceXZ(from, to) > 0.001) {
			return EditorSplineMath.FlatDirection(from, to);
		}

		int index = FindSample(WrapDistance(distance));
		return EditorSplineMath.FlatDirection(m_Positions[index], m_Positions[index + 1]);
	}

	// Distance along the path closest to position between two distances, gap is how far away the path is there
	float FindClosest(vector position, float from_distance, float to_distance, out float gap)
	{
		gap = float.MAX;
		int count = m_Positions.Count();
		if (count < 2) {
			gap = 0;
			return 0;
		}

		float search_from = Math.Max(0, from_distance);
		float search_to = Math.Min(m_Length, to_distance);
		float best_distance = search_from;
		float best_gap_sq = float.MAX;

		for (int i = FindSample(search_from); i < count - 1; i++) {
			if (m_Distances[i] > search_to) {
				break;
			}

			vector a = m_Positions[i];
			vector b = m_Positions[i + 1];
			float ab_x = b[0] - a[0];
			float ab_z = b[2] - a[2];
			float length_sq = ab_x * ab_x + ab_z * ab_z;
			float t = 0;
			if (length_sq > 0.0000001) {
				t = Math.Clamp(((position[0] - a[0]) * ab_x + (position[2] - a[2]) * ab_z) / length_sq, 0, 1);
			}

			float gap_x = position[0] - (a[0] + ab_x * t);
			float gap_z = position[2] - (a[2] + ab_z * t);
			float gap_sq = gap_x * gap_x + gap_z * gap_z;
			if (gap_sq < best_gap_sq) {
				best_gap_sq = gap_sq;
				best_distance = m_Distances[i] + (m_Distances[i + 1] - m_Distances[i]) * t;
			}
		}

		gap = Math.Sqrt(best_gap_sq);
		return Math.Clamp(best_distance, search_from, search_to);
	}

	// Outline with at most max_points points, used by area generators
	void GetOutline(int max_points, out array<vector> outline)
	{
		outline = new array<vector>();
		int count = m_Positions.Count();
		if (count == 0) {
			return;
		}

		int stride = 1;
		if (max_points > 0 && count > max_points) {
			float ratio = count;
			stride = Math.Max(1, Math.Ceil(ratio / max_points));
		}

		for (int i = 0; i < count; i += stride) {
			outline.Insert(m_Positions[i]);
		}

		// last sample of a closed path is the first point again
		if (m_Closed && outline.Count() > 1 && EditorSplineMath.DistanceXZ(outline[0], outline[outline.Count() - 1]) < 0.01) {
			outline.Remove(outline.Count() - 1);
		}
	}

	static bool IsInside(array<vector> polygon, float x, float z)
	{
		bool inside;
		int count = polygon.Count();
		int previous_index = count - 1;
		for (int i = 0; i < count; i++) {
			vector current = polygon[i];
			vector previous = polygon[previous_index];
			if ((current[2] > z) != (previous[2] > z)) {
				float crossing = (previous[0] - current[0]) * (z - current[2]) / (previous[2] - current[2]) + current[0];
				if (x < crossing) {
					inside = !inside;
				}
			}

			previous_index = i;
		}

		return inside;
	}

	static float DistanceToEdge(array<vector> polygon, float x, float z)
	{
		float best_sq = float.MAX;
		int count = polygon.Count();
		for (int i = 0; i < count; i++) {
			vector a = polygon[i];
			vector b = polygon[(i + 1) % count];
			float ab_x = b[0] - a[0];
			float ab_z = b[2] - a[2];
			float length_sq = ab_x * ab_x + ab_z * ab_z;
			float t = 0;
			if (length_sq > 0.0000001) {
				t = Math.Clamp(((x - a[0]) * ab_x + (z - a[2]) * ab_z) / length_sq, 0, 1);
			}

			float gap_x = x - (a[0] + ab_x * t);
			float gap_z = z - (a[2] + ab_z * t);
			best_sq = Math.Min(best_sq, gap_x * gap_x + gap_z * gap_z);
		}

		return Math.Sqrt(best_sq);
	}

	// shoelace formula
	static float Area(array<vector> polygon)
	{
		float twice_area;
		int count = polygon.Count();
		for (int i = 0; i < count; i++) {
			vector a = polygon[i];
			vector b = polygon[(i + 1) % count];
			twice_area += a[0] * b[2] - b[0] * a[2];
		}

		return Math.AbsFloat(twice_area) * 0.5;
	}
}