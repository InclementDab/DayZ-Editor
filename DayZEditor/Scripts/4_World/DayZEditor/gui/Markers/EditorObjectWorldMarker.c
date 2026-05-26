class EditorObjectWorldMarker: EditorObjectMarker
{	
	protected int m_ScreenX, m_ScreenY;
	protected MapWidget m_MapWidget;
	protected float m_MarkerDistSq;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().Map;
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
		m_MarkerDistSq = m_Editor.GetSettings().MarkerViewDistance * m_Editor.GetSettings().MarkerViewDistance;
		GetScreenSize(m_ScreenX, m_ScreenY);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		//m_Show = !state;
		m_Show = true;
	}
	
	override void Update(float dt)
	{
		if (!m_EditorObject) {
			Delete();
			return;
		}
		
		if (!m_EditorObject.GetWorldObject()) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		// idk man
		if (!m_MapWidget) {
			m_MapWidget = m_Editor.GetEditorHud().Map;
		}
				
		if (m_EditorObject.IsLocked() || !m_Editor.GetEditorHud().IsVisible() || m_Editor.GetEditorHud().IsAssetsBrowserOpen() || !m_Show) {
			if (m_LayoutRoot.IsVisible()) {
				m_LayoutRoot.Show(false);
			}

			return;
		}
		
		// first get a crude position to see if we are off screen. this check is very very fast
		vector position = m_EditorObject.GetPosition();
		float distancesq = vector.DistanceSq(GetGame().GetCurrentCameraPosition(), position);
		bool in_distance = m_MarkerDistSq > distancesq;
		if (!in_distance && !m_MapWidget.IsVisible()) {
			if (m_LayoutRoot.IsVisible()) {
				m_LayoutRoot.Show(false);
			}

			return;
		}
		
		if (!m_MapWidget.IsVisible()) {
			EditorMarkerColor.SetAlpha(1 - (Math.Clamp(distancesq, 0, m_MarkerDistSq) / m_MarkerDistSq));
			EditorMarkerOutline.SetAlpha(1 - (Math.Clamp(distancesq, 0, m_MarkerDistSq) / m_MarkerDistSq));
		} else {
			EditorMarkerColor.SetAlpha(1.0);
			EditorMarkerOutline.SetAlpha(1.0);
		}
		
		position = m_EditorObject.GetBottomCenter();
		if (GetEditor().GroundMode) {
			Ray downward_ray = new Ray(position, -m_EditorObject.GetWorldObject().GetDirectionUp());
			Raycast downward_raycast = downward_ray.PerformRaycastRV(m_EditorObject.GetWorldObject(), null, 0, 1000, ObjIntersect.View, true);
			if (downward_raycast) {
				position = downward_raycast.Bounce.Position;
			}
		}
		
		vector screen_pos = GetGame().GetScreenPos(position);
		bool off_screen = screen_pos[0] <= 0 || screen_pos[0] >= m_ScreenX || screen_pos[1] <= 0 || screen_pos[1] >= m_ScreenY || screen_pos[2] < 0;		
		if (off_screen && !m_MapWidget.IsVisible()) {
			if (m_LayoutRoot.IsVisible()) {
				m_LayoutRoot.Show(false);
			}

			return;
		}
		
		if (m_MapWidget.IsVisible()) {
			screen_pos = m_MapWidget.MapToScreen(position);
			m_LayoutRoot.SetSort(100);
		} else {
			m_LayoutRoot.SetSort(0);
		}
		
		m_LayoutRoot.Show(true, false);
				
		//float size = Math.Min(Math.Max(1300 / distancesq, 18), 24);
		float c = 4.0;
		if (screen_pos[2] != 0 && !m_MapWidget.IsVisible()) {
			 c = m_ViewDistance / screen_pos[2];
		}
		
		float size_min = 0, size_max = 0;
		switch (m_EditorSettings.MarkerSize) {
			case 0: {
				size_min = 1;
				size_max = 12;
				break;
			}
			
			case 1: {
				size_min = 4;
				size_max = 16;
				break;
			}
			
			case 2: {
				size_min = 8;
				size_max = 24;
				break;
			}
			
			case 3: {
				// woah
				size_min = 12;
				size_max = 42;
				break;
			}
		}
		
		if (c < size_min) {
			c = size_min;
		}
		
		if (c > size_max) {
			// Perplexity came up with this and it looks kinda awesome but it wont work for this application
			/*float t = (c - 32) / (128 - 32);  // Normalize to [0, 1] range
		    t = t * t * (3 - 2 * t);  // Smoothstep function
		    c = 32 + t * (128 - 32);*/   // Interpolate between 32 and 64

			c = size_max;
		}
		
		m_LayoutRoot.SetScreenSize(size_max, size_max, false);		
		m_LayoutRoot.SetScreenPos(screen_pos[0] - size_max / 2, screen_pos[1] - size_max / 2, false);
		m_LayoutRoot.Update();
	}
	
	override typename GetDragHandlerType()
	{
		return EditorObjectDragHandler;
	}
}
