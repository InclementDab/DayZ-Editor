class EditorAnimationManagerTable
{
	int CMD_Action;
	int CMD_ActionID;
	
	void EditorAnimationManagerTable(Human human, string anim_name)
	{
		HumanAnimInterface anim_interface = human.GetAnimInterface();
		
		array<string> anim_split = {};
		
		anim_name.Split(",", anim_split);
		CMD_Action = anim_interface.BindCommand(anim_split[0]);
		
		if (anim_split.Count() > 1) {
			CMD_ActionID = anim_split[1].ToInt();
		}
	}
}