// One object a generator placed, found again by type and position
class EditorSplineGeneratedData
{
	string Type;
	vector Position;

	static EditorSplineGeneratedData Create(string type, vector position)
	{
		EditorSplineGeneratedData data = new EditorSplineGeneratedData();
		data.Type = type;
		data.Position = position;
		return data;
	}

	void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(Position);
	}

	bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		serializer.Read(Position);
		return true;
	}
}