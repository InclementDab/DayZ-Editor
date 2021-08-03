class EditorAlignToSurfaceCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		EditorObject editor_object = data.param1;
		
		vector transform[4];
		editor_object.GetTransform(transform);
		
		// Get Ground Position
		vector ground_position, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground_position, ground_dir, component, null, null, null, false, true);
		
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		vector local_ori = editor_object.GetWorldObject().GetDirection();
		transform[0] = surface_normal * local_ori;
		transform[1] = surface_normal;
		transform[2] = surface_normal * (local_ori * vector.Up);
		
		editor_object.SetTransform(transform);
		editor_object.Update();
		
		return true;
	}
	
	override string GetName()
	{
		return "Align To Surface";
	}
}