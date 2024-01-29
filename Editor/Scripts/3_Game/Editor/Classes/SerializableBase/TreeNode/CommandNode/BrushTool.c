class BrushTool: CommandNode
{
	static const float ENTITY_COUNT_PER_SEC = 15.0;
	
	protected Object m_Ring;
	
	protected float m_Radius = 10.0;
		
	override bool Update(float dt, Raycast raycast)
	{
		if (!super.Update(dt, raycast)) {
			return false;
		}
				
		m_Ring.SetPosition(raycast.Bounce.Position);
		m_Ring.Update();
		
		if (GetGame().GetInput().LocalPress_ID(UAFire)) {
			return false;
		}
		
		if (GetGame().GetInput().LocalValue_ID(UAFire)) {
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);
			matrix[3] = raycast.Bounce.Position;
						
			vector random = Vector(((2 * Math.RandomFloat01()) - 1) * m_Radius, 0, ((2 * Math.RandomFloat01()) - 1) * m_Radius).Multiply4(matrix);
			matrix[3] = random + Vector(0, 4, 0);
			Object object = Editor.CreateObject(GetBrushedTypes().GetRandomElement(), matrix);
			
			ObjectNode editor_object = new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), object, EFE_DEFAULT);
			
			GetDayZGame().GetEditor()["EditedObjects"]["BrushedObjects"].Add(editor_object);		
			GetDayZGame().GetEditor().PlaySound(EditorSounds.PLOP);
			//m_History.InsertAction(footprint);
			return false;
		}
		
		if (GetGame().GetInput().LocalRelease_ID(UAFire)) {
			
			// Synchronize to this id
			GetDayZGame().GetEditor()["EditedObjects"]["BrushedObjects"].Synchronize();
			
			return false;
		}
		
		return true;
	}
	
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		if (state) {
			m_Ring = GetGame().CreateObjectEx("BrushBase", vector.Zero, ECE_LOCAL);
		} else {
			GetGame().ObjectDelete(m_Ring);
		}
	}
	
	array<string> GetBrushedTypes()
	{
		return {};
	}
}