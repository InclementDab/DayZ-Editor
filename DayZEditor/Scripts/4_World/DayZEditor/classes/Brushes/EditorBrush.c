class EditorBrush
{
	protected EditorSettings m_EditorSettings;
	protected EntityAI m_BrushDecal;
	protected ref EditorBrushData m_BrushData;

	static float BrushRadius = 10;
	static float BrushDensity = 0.5;
	static float BrushWidth = 6;

	// Private members
	private vector m_LastMousePosition;

	private void EditorBrush(EditorBrushData settings = null)
	{
		m_BrushData = settings;
		m_BrushDecal = EntityAI.Cast(GetGame().CreateObjectEx("BrushBase", vector.Zero, ECE_NONE));
		m_EditorSettings = GetEditor().Settings;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateBrush);
	}

	void ~EditorBrush()
	{
		GetGame().ObjectDelete(m_BrushDecal);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateBrush);
	}

	static EditorBrush Create(EditorBrushData settings)
	{
		EditorLog.Trace("EditorBrush::Create " + settings.Name);

		if (settings.BrushClassName)
		{
			return EditorBrush.Cast(settings.BrushClassName.Spawn());
		}

		return new EditorBrush(settings);
	}

	void SetBrushTexture(string texture)
	{
		EditorLog.Trace("EditorBrush::SetBrushTexture " + texture);
		m_BrushDecal.SetObjectTexture(0, texture);
		m_BrushDecal.Update();
	}

	void UpdateBrush()
	{
		if (GetEditor().IsPlacing()) {
			return;
		}
		
		Input input = GetGame().GetInput();
		Ray ray = new Ray(GetGame().GetCurrentCameraPosition(), GetGame().GetPointerDirection());
		
		Raycast ray_cast = ray.PerformRaycastRVEX(BrushRadius / 100, GetEditor().GetCamera().GetSettings().ViewDistance, ObjIntersectView, null, true);
		if (ray_cast && ray_cast.Bounce) {
			vector transform[4] = {
				Vector(BrushRadius / 10, 0, 0),
				Vector(0, BrushRadius / 10, 0),
				Vector(0, 0, BrushRadius / 10),
				ray_cast.Bounce.Position
			};

			m_BrushDecal.SetTransform(transform);
			
			if (GetWidgetUnderCursor()) {
				return;
			}
	
			if (input.LocalPress("UAFire"))
			{
				OnMouseDown(ray_cast.Bounce.Position);
			}
	
			if (input.LocalValue("UAFire"))
			{
				DuringMouseDown(ray_cast.Bounce.Position);
			}
	
			if (input.LocalRelease("UAFire"))
			{
				OnMouseUp(ray_cast.Bounce.Position);
			}
		}
	}

	void DuringMouseDown(vector position)
	{
		if (!m_BrushData) {
			return;
		}
		
		if (vector.Distance(m_LastMousePosition, position) < (BrushRadius * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;

		array<ref EditorObjectData> created_data = { };

		int flags;
		if (m_EditorSettings.BrushedObjectMarkers)
		{
			flags |= EditorObjectFlags.OBJECTMARKER;
		}

		if (m_EditorSettings.BrushedListItems)
		{
			flags |= EditorObjectFlags.LISTITEM;
		}

		map<int, ref Param2<vector, EditorBrushObject>> brushes_data = new map<int, ref Param2<vector, EditorBrushObject>>();
		for (int i = 0; i < Math.Sqrt(BrushDensity) * 24; i++) {
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-BrushRadius / Math.PI, BrushRadius / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-BrushRadius / Math.PI, BrushRadius / Math.PI);
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

			EditorBrushObject object_name = m_BrushData.GetRandomObject();
			if (!object_name) {
				continue;
			}

			//TODO config objects can have magnet. P3D need to stay zeroed 
			vector ori = "0 0 0";
			ori[0] = Math.RandomFloatInclusive(0, 360);
			ori[1] = Math.RandomFloatInclusive(-4, 4);
			ori[2] = Math.RandomFloatInclusive(-4, 4);

			array<Object> objects = {};
			GetGame().GetObjectsAtPosition3D(pos, BrushWidth, objects, null);
			if (objects.Count() > 0) {
				continue;
			}

			EditorObjectData brushed_object_data = EditorObjectData.Create(object_name.Name, pos, ori, Math.RandomFloatInclusive(object_name.MinScale, object_name.MaxScale), EditorObjectFlags.OBJECTMARKER);
			
			// pass onto second pass
			brushes_data[brushed_object_data.GetID()] = new Param2<vector, EditorBrushObject>(pos, object_name);

			// just for u boba
			created_data.Insert(brushed_object_data);
		}

		EditorObjectMap object_map = GetEditor().CreateObjects(created_data, true);
		foreach (int id, EditorObject editor_object_brushed: object_map) {
			if (editor_object_brushed) {
				vector new_pos = brushes_data[id].param1;
				vector size = ObjectGetSize(editor_object_brushed.GetWorldObject());
				new_pos[1] = GetGame().SurfaceY(new_pos[0], new_pos[2]) + size[1] - 0.5 + brushes_data[id].param2.ZOffset;
				editor_object_brushed.SetScale(Math.RandomFloat(brushes_data[id].param2.MinScale, brushes_data[id].param2.MaxScale));
				editor_object_brushed.SetPosition(new_pos);
			}
			
		}
	}

	void OnMouseUp(vector position)
	{
		// Reset mouse position when releasing mouse. 
		m_LastMousePosition = vector.Zero;
	}

	void OnMouseDown(vector position)
	{

	}

	string GetName()
	{
		return m_BrushData.Name;
	}
}