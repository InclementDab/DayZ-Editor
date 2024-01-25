class LightningBrush: EditorBrush
{
	
}

class BetulaPendula_Brush: EditorBrush
{
	override array<string> GetBrushedTypes() return { "DZ/plants/tree/t_BetulaPendula_1f.p3d", "DZ/plants/tree/t_BetulaPendula_1fb.p3d", "DZ/plants/tree/t_BetulaPendula_1s.p3d", "DZ/plants/tree/t_BetulaPendula_2f.p3d", "DZ/plants/tree/t_BetulaPendula_2fb.p3d", "DZ/plants/tree/t_BetulaPendula_2fc.p3d", "DZ/plants/tree/t_BetulaPendula_2s.p3d", "DZ/plants/tree/t_BetulaPendula_2w.p3d", "DZ/plants/tree/t_BetulaPendula_3f.p3d", "DZ/plants/tree/t_BetulaPendula_3fb.p3d", "DZ/plants/tree/t_BetulaPendula_3fc.p3d", "DZ/plants/tree/t_BetulaPendula_3s.p3d" };
}

class EditorBrush: EditorCursorTool
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
			
			//EditorObject editor_object = new EditorObject(UUID.Generate(), );
						
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);
			matrix[3] = raycast.Bounce.Position;
						
			vector random = Vector(((2 * Math.RandomFloat01()) - 1) * m_Radius, 0, ((2 * Math.RandomFloat01()) - 1) * m_Radius).Multiply4(matrix);
			matrix[3] = random + Vector(0, 4, 0);
			Object object = Editor.CreateObject(GetBrushedTypes().GetRandomElement(), matrix);
			
			EditorObject editor_object = new EditorObject(UUID.Generate(), m_UUID, GetIcon(), object, EFE_DEFAULT);
			
			GetDayZGame().GetEditor()["EditedObjects"]["BrushedObjects"].Add(editor_object);
		
			// remove it from placing
			//Placing.RemoveItem(editor_objects);
			//m_Editor.PlaySound(EditorSounds.PLOP);
			//m_History.InsertAction(footprint);
			
			return false;
		}
		
		if (GetGame().GetInput().LocalRelease_ID(UAFire)) {
			
			// Synchronize to this id
			//m_Editor.Synchronize(m_Editor.GetMaster()["EditedObjects"]["BrushedObjects"]);
			return false;
		}
		
		return true;
	}
	
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
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

class EditorCursorTool: EditorNode
{
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			GetDayZGame().GetEditor().Tool = this;
		} else {
			GetDayZGame().GetEditor().Tool = null;
		}
	}
	
	// Called in editor when running, return TRUE when you want editor to continue processing inputs
	bool Update(float dt, Raycast raycast)
	{		
		if (raycast.Bounce != Raycast.INVALID) {
			raycast.Debug();
			Shape.CreateSphere(COLOR_GREEN, ShapeFlags.ONCE, raycast.Bounce.Position, 0.5);
		}
		
		return true;
	}
}