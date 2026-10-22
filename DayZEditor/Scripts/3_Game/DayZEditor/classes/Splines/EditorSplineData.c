class EditorSplineData
{
	string Uuid;
	string Name;
	EditorSplineType Type;
	bool Closed;
	float Tension; // 0 = natural curve, 1 = almost straight

	ref array<vector> Points = {};
	ref EditorSplineGeneratorSettings Generator = new EditorSplineGeneratorSettings();

	// remembered so the objects can still be cleared after the file was reopened
	ref array<ref EditorSplineGeneratedData> Generated = {};

	static EditorSplineData Create(EditorSplineType type, string name)
	{
		EditorSplineData data = new EditorSplineData();
		data.Uuid = UUID.Generate();
		data.Type = type;
		data.Name = name;
		return data;
	}

	bool IsSpline()
	{
		return Type == EditorSplineType.SPLINE;
	}

	int Count()
	{
		return Points.Count();
	}

	void InsertPoint(int index, vector position)
	{
		if (index < 0 || index >= Points.Count()) {
			Points.Insert(position);
			return;
		}

		Points.InsertAt(position, index);
	}

	void RemovePoint(int index)
	{
		if (index >= 0 && index < Points.Count()) {
			Points.RemoveOrdered(index);
		}
	}

	void Reverse()
	{
		array<vector> reversed = {};
		for (int i = Points.Count() - 1; i >= 0; i--) {
			reversed.Insert(Points[i]);
		}

		Points.Copy(reversed);
	}

	vector GetCenter()
	{
		if (Points.Count() == 0) {
			return vector.Zero;
		}

		vector sum;
		foreach (vector point: Points) {
			sum = sum + point;
		}

		return sum * (1.0 / Points.Count());
	}

	// json can be missing parts, everything expects them to exist
	void Repair()
	{
		if (!Points) {
			Points = {};
		}

		if (!Generator) {
			Generator = new EditorSplineGeneratorSettings();
		}

		if (!Generated) {
			Generated = {};
		}

		if (Uuid == string.Empty) {
			Uuid = UUID.Generate();
		}

		Generator.Repair();
		Tension = Math.Clamp(Tension, 0.0, 1.0);
	}

	void Write(Serializer serializer, int version)
	{
		serializer.Write(Uuid);
		serializer.Write(Name);
		serializer.Write(Type);
		serializer.Write(Closed);
		serializer.Write(Tension);

		serializer.Write(Points.Count());
		foreach (vector point: Points) {
			serializer.Write(point);
		}

		Generator.Write(serializer, version);

		serializer.Write(Generated.Count());
		foreach (EditorSplineGeneratedData generated: Generated) {
			generated.Write(serializer, version);
		}
	}

	bool Read(Serializer serializer, int version)
	{
		serializer.Read(Uuid);
		serializer.Read(Name);
		serializer.Read(Type);
		serializer.Read(Closed);
		serializer.Read(Tension);

		int point_count;
		serializer.Read(point_count);
		for (int i = 0; i < point_count; i++) {
			vector point;
			serializer.Read(point);
			Points.Insert(point);
		}

		if (!Generator.Read(serializer, version)) {
			return false;
		}

		int generated_count;
		if (!serializer.Read(generated_count)) {
			return false;
		}

		for (int j = 0; j < generated_count; j++) {
			EditorSplineGeneratedData generated = new EditorSplineGeneratedData();
			generated.Read(serializer, version);
			Generated.Insert(generated);
		}

		Repair();
		return true;
	}
}