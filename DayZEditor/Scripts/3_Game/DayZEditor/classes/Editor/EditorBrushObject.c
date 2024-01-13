class EditorBrushObject
{
	string Type;
	float Frequency;
	float ZOffset;
	float MinScale, MaxScale;
	
	void EditorBrushObject(string type, float frequency, float zoffset, float minscale = 1, float maxscale = 1) 
	{
		Type = type; 
		Frequency = frequency; 
		ZOffset = zoffset;
		MinScale = minscale;
		MaxScale = maxscale;
	}
}