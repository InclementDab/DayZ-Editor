class EditorBrushObject
{
	string Name;
	float Frequency;
	float ZOffset;
	float MinScale, MaxScale;
	
	void EditorBrushObject(string name, float frequency, float zoffset, float minscale = 1, float maxscale = 1) 
	{
		Name = name; 
		Frequency = frequency; 
		ZOffset = zoffset;
		MinScale = minscale;
		MaxScale = maxscale;
	}
}