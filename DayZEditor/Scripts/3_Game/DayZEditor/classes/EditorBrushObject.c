class EditorBrushObject
{
	string Name;
	float Frequency;
	float ZOffset;
	
	void EditorBrushObject(string name, float frequency, float zoffset) 
	{
		Name = name; 
		Frequency = frequency; 
		ZOffset = zoffset;
	}
}