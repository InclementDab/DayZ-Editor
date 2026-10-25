// yaw 0 looks north (+z), yaw 90 looks east (+x), vector * vector is the cross product
class EditorSplineMath
{
	static float DistanceXZ(vector a, vector b)
	{
		float dx = b[0] - a[0];
		float dz = b[2] - a[2];
		return Math.Sqrt(dx * dx + dz * dz);
	}

	// Horizontal unit direction, north when the points are on top of each other
	static vector FlatDirection(vector from, vector to)
	{
		vector direction = Vector(to[0] - from[0], 0, to[2] - from[2]);
		float length = direction.Length();
		if (length < 0.0001) {
			return Vector(0, 0, 1);
		}

		return direction * (1.0 / length);
	}

	static vector Flatten(vector direction)
	{
		vector flat = Vector(direction[0], 0, direction[2]);
		float length = flat.Length();
		if (length < 0.0001) {
			return Vector(0, 0, 1);
		}

		return flat * (1.0 / length);
	}

	static vector RightOf(vector direction)
	{
		return Vector(direction[2], 0, -direction[0]);
	}

	// Positive degrees turn clockwise seen from above
	static vector RotateYaw(vector direction, float degrees)
	{
		float radians = degrees * Math.DEG2RAD;
		float cos_yaw = Math.Cos(radians);
		float sin_yaw = Math.Sin(radians);
		return Vector(direction[0] * cos_yaw + direction[2] * sin_yaw, direction[1], direction[2] * cos_yaw - direction[0] * sin_yaw);
	}

	static float YawOf(vector direction)
	{
		return Math.Atan2(direction[0], direction[2]);
	}

	static vector DirectionOfYaw(float radians)
	{
		return Vector(Math.Sin(radians), 0, Math.Cos(radians));
	}

	// -PI .. PI
	static float WrapAngle(float radians)
	{
		float wrapped = radians;
		while (wrapped > Math.PI) {
			wrapped = wrapped - Math.PI2;
		}

		while (wrapped < -Math.PI) {
			wrapped = wrapped + Math.PI2;
		}

		return wrapped;
	}

	// Up is kept exactly, forward is made perpendicular to it
	static void FrameFromForwardUp(vector forward, vector up, vector position, out vector frame[4])
	{
		vector up_axis = up.Normalized();
		if (up_axis.LengthSq() < 0.0001) {
			up_axis = vector.Up;
		}

		vector aside_axis = up_axis * forward;
		if (aside_axis.LengthSq() < 0.000001) {
			aside_axis = up_axis * Vector(0, 0, 1);
			if (aside_axis.LengthSq() < 0.000001) {
				aside_axis = Vector(1, 0, 0);
			}
		}

		aside_axis.Normalize();
		vector forward_axis = aside_axis * up_axis;
		forward_axis.Normalize();

		frame[0] = aside_axis;
		frame[1] = up_axis;
		frame[2] = forward_axis;
		frame[3] = position;
	}

	static void ScaledFrame(vector forward, vector up, vector position, float scale, out vector frame[4])
	{
		vector unscaled[4];
		FrameFromForwardUp(forward, up, position, unscaled);
		frame[0] = unscaled[0] * scale;
		frame[1] = unscaled[1] * scale;
		frame[2] = unscaled[2] * scale;
		frame[3] = unscaled[3];
	}

	// Tilts a level transform around pivot so chord rises by rise over run metres, tilted must be a different array
	static void TiltAlong(vector level[4], vector pivot, vector chord, float run, float rise, out vector tilted[4])
	{
		if (run < 0.01 || Math.AbsFloat(rise) < 0.0005) {
			for (int i = 0; i < 4; i++) {
				tilted[i] = level[i];
			}

			return;
		}

		vector side = vector.Up * chord;
		side.Normalize();
		vector sloped = chord * run + vector.Up * rise;
		sloped.Normalize();
		vector sloped_up = sloped * side;
		sloped_up.Normalize();

		for (int j = 0; j < 3; j++) {
			tilted[j] = TiltVector(level[j], side, chord, sloped, sloped_up);
		}

		tilted[3] = pivot + TiltVector(level[3] - pivot, side, chord, sloped, sloped_up);
	}

	protected static vector TiltVector(vector value, vector side, vector chord, vector sloped, vector sloped_up)
	{
		return side * vector.Dot(value, side) + sloped_up * value[1] + sloped * vector.Dot(value, chord);
	}

	static float TerrainY(vector position)
	{
		return GetGame().SurfaceY(position[0], position[2]);
	}

	// degrees, 0 = flat
	static float TerrainSlope(vector position)
	{
		vector normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		return Math.Acos(Math.Clamp(normal[1], -1.0, 1.0)) * Math.RAD2DEG;
	}

	static bool IsWater(vector position)
	{
		return GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]);
	}

	// 10.5 without trailing zeros
	static string FormatNumber(float value, int decimals = 2)
	{
		float factor = Math.Pow(10, decimals);
		float rounded = Math.Round(Math.AbsFloat(value) * factor);
		int whole = Math.Floor(rounded / factor);
		int fraction = rounded - whole * factor;

		string text = whole.ToString();
		if (fraction > 0) {
			string fraction_text = fraction.ToString();
			while (fraction_text.Length() < decimals) {
				fraction_text = "0" + fraction_text;
			}

			while (fraction_text.Length() > 0 && fraction_text.Substring(fraction_text.Length() - 1, 1) == "0") {
				fraction_text = fraction_text.Substring(0, fraction_text.Length() - 1);
			}

			text = text + "." + fraction_text;
		}

		if (value < 0 && (whole > 0 || fraction > 0)) {
			text = "-" + text;
		}

		return text;
	}

	// false for anything that is not a number, accepts 12,5 too
	static bool ParseNumber(string text, out float value)
	{
		string clean = text.Trim();
		clean.Replace(",", ".");
		if (clean == string.Empty || clean == "-" || clean == "." || clean == "-.") {
			return false;
		}

		string digits = "0123456789";
		int dots;
		for (int i = 0; i < clean.Length(); i++) {
			string character = clean.Substring(i, 1);
			if (character == "-" && i == 0) {
				continue;
			}

			if (character == ".") {
				dots++;
				if (dots > 1) {
					return false;
				}

				continue;
			}

			if (digits.IndexOf(character) == -1) {
				return false;
			}
		}

		value = clean.ToFloat();
		return true;
	}

	static string FormatLength(float metres)
	{
		if (metres >= 1000) {
			return FormatNumber(metres / 1000.0, 2) + " km";
		}

		return FormatNumber(metres, 1) + " m";
	}
}