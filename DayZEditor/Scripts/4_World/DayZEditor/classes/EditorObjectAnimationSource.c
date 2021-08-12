class EditorObjectAnimationSource
{	
	protected Object m_WorldObject;
	protected string m_AnimationName;
	
	string Source;
	int InitPhase;
	float AnimPeriod;
	
	void EditorObjectAnimationSource(Object world_object, string animation_name)
	{
		m_WorldObject = world_object;
		m_AnimationName = animation_name;
		
		string config_path = "CfgVehicles " + m_WorldObject.GetType() + " AnimationSources " + m_AnimationName;
		Source = GetGame().ConfigGetTextOut(config_path + " source");
		InitPhase = GetGame().ConfigGetInt(config_path + " initPhase");
		AnimPeriod = GetGame().ConfigGetFloat(config_path + " animPeriod");
	}	
}