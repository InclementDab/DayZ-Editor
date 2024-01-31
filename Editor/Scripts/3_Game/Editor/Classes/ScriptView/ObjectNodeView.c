class ObjectNodeView: ScriptView
{
	protected ObjectNode m_ObjectNode;
	protected vector m_CursorAside = vector.Aside;
	
	protected ref Raycast m_StartPosition;
	
	protected bool m_IsDragging;
	
	ButtonWidget Button;
	ImageWidget Image, Outline;
	
	void ObjectNodeView(notnull ObjectNode editor_object)
	{
		m_ObjectNode = editor_object;
		m_ObjectNode.State_OnChanged.Insert(OnStateChanged);
		
		Symbols icon = m_ObjectNode.GetIcon();
		
		Image.LoadImageFile(0, icon.Solid());
		Image.SetImage(0);
		
		Outline.LoadImageFile(0, icon.Thin());
		Outline.SetImage(0);
	}
	
	void OnStateChanged(TreeNode node, TreeNodeState state)
	{
		Outline.SetAlpha(state.IsHover());
		
		if (state.IsDragging() || state.IsActive()) {			
			Image.SetColor(EditorColors.SELECT);
		} else {
			Image.SetColor(ARGB(150, 255, 255, 255));
		}
		
		if (state.IsDragging()) {
			m_ObjectNode.GetEditor().GetHud().SetCursor(Symbols.UP_DOWN_LEFT_RIGHT);
		} else {
			m_ObjectNode.GetEditor().GetHud().ClearCursor();
		}
	}
	
	override void Update(float dt)
	{
		vector base_position = m_ObjectNode.GetBasePoint();
		base_position = GetGame().GetScreenPos(base_position);
		
		if (base_position[2] < 0) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		Raycast raycast = m_ObjectNode.GetEditor().GetCamera().PerformCursorRaycast(m_ObjectNode.GetObject());
				
		// Dragging
		if (m_ObjectNode.HasState(TreeNodeState.DRAGGING) && raycast) {
			//raycast.Debug();
			Shape.CreateArrow(m_StartPosition.Bounce.Position, raycast.Bounce.Position, 1, COLOR_BLACK, ShapeFlags.ONCE);
			
			Input input = GetGame().GetInput();
			vector transform[4];
			m_ObjectNode.GetBaseTransform(transform);
						
			// Held distance placing
			if (KeyState(KeyCode.KC_LMENU)) {
				Debug.DrawSphere(raycast.Source.Position, vector.Distance(raycast.Source.Position, transform[3]), COLOR_RED, ShapeFlags.ADDITIVE | ShapeFlags.WIREFRAME | ShapeFlags.ONCE);
				
				vector v3 = vector.Up * raycast.Source.Direction;					
				float dist_z = vector.Dot(((raycast.Source.Position - transform[3]) * vector.Up), v3) / v3.LengthSq();
				transform = { transform[0], transform[1], transform[2], raycast.Source.Position + raycast.Source.Direction * dist_z };
				m_ObjectNode.SetBaseTransform(transform);
			} 
			
	
			else if (KeyState(KeyCode.KC_LSHIFT)) {
				
				Plane face = m_ObjectNode.GetBoundingFace(ETransformationAxis.BOTTOM);
				face.Debug("Cursor intersection", transform);
				
				vector point = face.Intersect(raycast.Source, transform);

				vector new_forward = vector.Direction(transform[3], point).Normalized();
				vector aside = (transform[1] * new_forward).Normalized();
				
				transform = { aside, new_forward * aside, new_forward, transform[3] };
				//Math3D.MatrixOrthogonalize3(transform);
				Print(transform);
				m_ObjectNode.SetBaseTransform(transform);
				
				//vector p1 = Vector(2, 0, 2).Multiply4(transform);
				//vector p2 = Vector(-2, 0, -2).Multiply4(transform);
				
				//Shape.Create(ShapeType.BBOX, COLOR_GREEN, ShapeFlags.WIREFRAME | ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, p1, p2);
				
				
				
				
			}
			
			
			// Any distance placing
			else {
				float delta_y = transform[3][1] - raycast.Bounce.Position[1];
				transform = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position + Vector(0, delta_y, 0) };				
				m_ObjectNode.SetBaseTransform(transform);
			}
		}
		
		base_position = GetGame().GetScreenPos(m_ObjectNode.GetBasePoint());
		m_LayoutRoot.SetPos(base_position[0] - 12.5, base_position[1] - 12.5);
		m_LayoutRoot.Show(true);
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		switch (m_ObjectNode.GetInteractType()) {
			case TreeNodeInteract.HOLD: {
				m_ObjectNode.AddState(TreeNodeState.ACTIVE);
				return true;
			}
			
			case TreeNodeInteract.TOGGLE: {
				if (m_ObjectNode.HasState(TreeNodeState.ACTIVE)) {
					m_ObjectNode.RemoveState(TreeNodeState.ACTIVE);
				} else {
					m_ObjectNode.AddState(TreeNodeState.ACTIVE);
				}
				return true;
			}
			
			case TreeNodeInteract.PRESS: {
				m_ObjectNode.AddState(TreeNodeState.ACTIVE);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		switch (m_ObjectNode.GetInteractType()) {
			case TreeNodeInteract.HOLD: {
				m_ObjectNode.RemoveState(TreeNodeState.ACTIVE);
				return true;
			}
		}
				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		
				
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return false;
		}
		
		switch (m_ObjectNode.GetInteractType()) {
			case TreeNodeInteract.DOUBLE: {
				if (m_ObjectNode.HasState(TreeNodeState.ACTIVE)) {
					m_ObjectNode.RemoveState(TreeNodeState.ACTIVE);
				} else {
					m_ObjectNode.AddState(TreeNodeState.ACTIVE);
				}
				return true;
			}
		}
		
		return false;
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_ObjectNode.AddState(TreeNodeState.HOVER);
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_ObjectNode.RemoveState(TreeNodeState.HOVER);
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
		
	override bool OnDrag(Widget w, int x, int y)
	{
		m_ObjectNode.AddState(TreeNodeState.DRAGGING);
		m_StartPosition = m_ObjectNode.GetEditor().GetCamera().PerformCursorRaycast(m_ObjectNode.GetObject());
		return true;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{	
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		m_ObjectNode.RemoveState(TreeNodeState.DRAGGING);
		m_ObjectNode.Synchronize();	
		
		return true;
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Marker.layout";
	}
}