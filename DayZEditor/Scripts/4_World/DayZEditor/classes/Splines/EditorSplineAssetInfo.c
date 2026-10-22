class EditorSplineAssetInfo
{
	string Name;
	string SpawnType;
	string Model;
	bool Valid;
	bool ConsoleFriendly;
	bool IsStatic;
	vector ClipMin;
	vector ClipMax;

	// Road parts, model space
	bool HasRoadPoints;
	vector RoadBegin;
	vector RoadEnd;
	vector RoadBeginDirection;
	vector RoadEndDirection;
	float RoadWidth;

	string GetDisplayName()
	{
		string display_name = Name;
		display_name.Replace("\\", "/");
		int slash = display_name.LastIndexOf("/");
		if (slash != -1) {
			display_name = display_name.Substring(slash + 1, display_name.Length() - slash - 1);
		}

		return display_name;
	}

	// Length along the longest horizontal side
	float GetLongestSide()
	{
		return Math.Max(ClipMax[0] - ClipMin[0], ClipMax[2] - ClipMin[2]);
	}
}