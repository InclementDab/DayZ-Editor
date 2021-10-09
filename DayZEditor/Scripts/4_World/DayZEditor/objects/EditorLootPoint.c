class EditorLootPoint: House 
{
	float Range = 0.5;
	float Height = 1.5;
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Range":
			case "Height": {
				vector transform[4];
				GetTransform(transform);
				transform[0][0] = Range;
				transform[1][1] = Height;
				transform[2][2] = Range;
				SetTransform(transform);
				Update();
				break;
			}
		}
	}
}