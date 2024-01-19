class EditorObjectAnimationSource: Managed
{	
	protected EntityAI m_WorldObject;
	protected string m_AnimationName;
	protected string m_PathRoot;
	
	// 0..1 value of the current animation state
	float AnimationPhase;
	
	string Source;
	int InitPhase;
	float AnimPeriod;
	
	void EditorObjectAnimationSource(EntityAI world_object, string animation_name, string path_root)
	{
		m_WorldObject = world_object;
		m_AnimationName = animation_name;
		m_PathRoot = path_root;
		
		string config_path = m_PathRoot + " " + m_WorldObject.GetType() + " AnimationSources " + m_AnimationName;
		Source = GetGame().ConfigGetTextOut(config_path + " source");
		InitPhase = GetGame().ConfigGetInt(config_path + " initPhase");
		AnimPeriod = GetGame().ConfigGetFloat(config_path + " animPeriod");
	}
	
	void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "AnimationPhase": {
				m_WorldObject.SetAnimationPhase(m_AnimationName, AnimationPhase);
				break;
			}
		}
	}
}