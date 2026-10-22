// Seeded random so a generator gives the exact same result every time (Park-Miller, Schrage's method so it never overflows)
class EditorRandom
{
	protected static const int MODULUS = 2147483647;
	protected static const int MULTIPLIER = 16807;
	protected static const int QUOTIENT = 127773;
	protected static const int REMAINDER = 2836;

	protected int m_State;

	void EditorRandom(int seed)
	{
		SetSeed(seed);
	}

	void SetSeed(int seed)
	{
		int state = seed % (MODULUS - 1);
		if (state < 0) {
			state = -state;
		}

		m_State = state + 1;

		// first values of a small seed are too close together
		for (int i = 0; i < 4; i++) {
			NextInt();
		}
	}

	int NextInt()
	{
		int high = m_State / QUOTIENT;
		int low = m_State % QUOTIENT;
		m_State = MULTIPLIER * low - REMAINDER * high;
		if (m_State <= 0) {
			m_State = m_State + MODULUS;
		}

		return m_State;
	}

	float Next01()
	{
		float value = NextInt() - 1;
		return value / 2147483646.0;
	}

	float Range(float min, float max)
	{
		return min + (max - min) * Next01();
	}

	int Index(int count)
	{
		if (count <= 1) {
			return 0;
		}

		int index = Math.Floor(Next01() * count);
		if (index >= count) {
			index = count - 1;
		}

		return index;
	}

	bool Chance(float probability)
	{
		return Next01() < probability;
	}
}