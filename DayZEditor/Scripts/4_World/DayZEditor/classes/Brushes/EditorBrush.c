class EditorBrush: Managed
{
	protected EditorSettings m_EditorSettings;
	protected EntityAI m_BrushDecal;
	protected ref EditorBrushData m_BrushData;
	
	static float BrushRadius = 10;
	static float BrushDensity = 0.5;

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
		EditorLog.Trace("EditorBrush::Create " + settings.Type);

		if (settings.BrushClassName) {
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
		
		set<Object> o;
		vector CurrentMousePosition = MousePosToRay(o, null, GetEditor().Settings.ObjectViewDistance, 0, true);
		
		Input input = GetGame().GetInput();

		vector transform[4] = {
			Vector(BrushRadius / 10, 0, 0),
			Vector(0, BrushRadius / 10, 0),
			Vector(0, 0, BrushRadius / 10),
			CurrentMousePosition
		};
		
		m_BrushDecal.SetTransform(transform);
		
		//if (GetEditor().GetUIManager().IsCursorOverUI()) return;
		/*
		if (input.LocalPress("UAFire")) {
			
		}*/
		
		if (GetWidgetUnderCursor()) {
			return;
		}
		
		if (input.LocalPress("UAFire")) {
			OnMouseDown(CurrentMousePosition);
		}
		
		if (input.LocalValue("UAFire")) {
			DuringMouseDown(CurrentMousePosition);
		}
		
		if (input.LocalRelease("UAFire")) {
			OnMouseUp(CurrentMousePosition);
		}
	}

	void DuringMouseDown(vector position) 
	{ 
		if (vector.Distance(m_LastMousePosition, position) < (BrushRadius * Math.RandomFloat(0.5, 1))) return;
		m_LastMousePosition = position;
		
		array<ref EditorObjectData> created_data = {};
		
		int flags;
		if (m_EditorSettings.BrushedObjectMarkers) {
			flags |= EditorObjectFlags.OBJECTMARKER;
		}
		
		if (m_EditorSettings.BrushedListItems) {
			flags |= EditorObjectFlags.LISTITEM;
		}
		
		for (int i = 0; i < BrushDensity * 10; i++) {
						
			vector pos = position;
			pos[0] = pos[0] + Math.RandomFloat(-BrushRadius / Math.PI, BrushRadius / Math.PI);
			pos[2] = pos[2] + Math.RandomFloat(-BrushRadius / Math.PI, BrushRadius / Math.PI);
			
			if (!m_BrushData) {
				continue;
			}
			
			EditorBrushObject object_name = m_BrushData.GetRandomObject();
			if (!object_name) {
				continue;
			}
			
			vector ori = Math3D.GetRandomDir().VectorToAngles() + Vector(0, Math.RandomFloat(-0.02, 0.02), 0);
			EditorStaticPlaceableItem static_placeable = EditorStaticPlaceableItem(object_name.Type);
			Object brushed_object = static_placeable.CreateObject(pos, vector.Zero, Math.RandomFloatInclusive(object_name.MinScale, object_name.MaxScale));
						
			vector size = ObjectGetSize(brushed_object);			
			
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + size[1] / 2 + object_name.ZOffset;
			
			// just for u boba
			brushed_object.SetPosition(pos);
			brushed_object.SetDirection(ori);
			brushed_object.Update();
			//brushed_object.SetDirection(direction);
			created_data.Insert(EditorObjectData.Create(brushed_object, flags));
		}
	
		GetEditor().CreateObjects(created_data, true);
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
		return m_BrushData.Type; 
	}
}