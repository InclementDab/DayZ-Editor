class EditorObjectPropertiesDialog: EditorDialogBase
{
	protected bool ExecuteCode;
	protected ref array<EditorObject> m_EditorObjects = {};
	protected ref EditorMultiObjectCommandController m_EditorMultiObjectCommandController;
	protected EditorObject m_EditorObject;
			
	void EditorObjectPropertiesDialog(string title, notnull array<EditorObject> editor_objects)
	{
		m_EditorObjects.InsertArray(editor_objects);
		
		UpdateViewContext();
		
		AddButton(DialogResult.OK);
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		// Ensures full persistence update when closing the properties dialog
		//TODO 1: Per tick update.
		//TODO 2: Only update changed data.
		if (GetGame().IsMultiplayer() && GetEditor() && GetEditor().GetNetActionManager() && m_EditorObjects.Count() > 0) 
		{
			foreach (EditorObject editor_object: m_EditorObjects) {
				if (editor_object) 
				{
					GetEditor().GetNetActionManager().SendObjectUpdate(editor_object);
				}
			}
		}
	}
			
	protected void OnObjectSelected(Class context, EditorObject editor_object)
	{		
		m_EditorObjects.Insert(editor_object);
		UpdateViewContext();
	}
	
	protected void OnObjectDeselected(Class context, EditorObject editor_object)
	{
		m_EditorObjects.RemoveItem(editor_object);
		UpdateViewContext();
	}
	
	void UpdateViewContext()
	{
		// cleanup content
		ClearContent();
		delete m_EditorMultiObjectCommandController;
		
		if (m_EditorObjects.Count() == 0) {
			return;
		}
		
		if (m_EditorObjects.Count() == 1) {
			SetEditorObject(m_EditorObjects[0]);
			return;
		}
		
		SetMultipleEditorObjects(m_EditorObjects);
	}
			
	void SetMultipleEditorObjects(array<EditorObject> editor_objects)
	{
		m_EditorMultiObjectCommandController = new EditorMultiObjectCommandController(editor_objects);
		
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", m_EditorMultiObjectCommandController, string.Empty);
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", m_EditorMultiObjectCommandController, "Show"));
		general_group.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", m_EditorMultiObjectCommandController, "Name"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_POSITION", m_EditorMultiObjectCommandController, "Position"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", m_EditorMultiObjectCommandController, "Orientation"));
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", m_EditorMultiObjectCommandController, "Scale", 0.01));
		
		GroupPrefab object_group = new GroupPrefab("#STR_EDITOR_OBJECT", m_EditorMultiObjectCommandController, string.Empty);
		object_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", m_EditorMultiObjectCommandController, "Health"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", m_EditorMultiObjectCommandController, "EditorOnly"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", m_EditorMultiObjectCommandController, "Simulate"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", m_EditorMultiObjectCommandController, "Locked"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", m_EditorMultiObjectCommandController, "UsePhysics"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", m_EditorMultiObjectCommandController, "AllowDamage"));
		
		AddContent(general_group);
		AddContent(object_group);
	}
	
	// This function is a mess
	void SetEditorObject(EditorObject editor_object)
	{		
		m_EditorObject = editor_object;
		
		EditorObjectController controller = m_EditorObject.GetController();
						
		// If network light
		//if (NetworkLightBase.Cast(m_EditorObject.GetWorldObject())) {
		//	NetworkLightBase.Cast(m_EditorObject.GetWorldObject()).Read(m_Data.Parameters);
		//}
		
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", this, string.Empty);
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", controller, "Show"));
		general_group.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", controller, "Name"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_POSITION", controller, "Position"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", controller, "Orientation"));
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", controller, "Scale", 0.01));

		/*
		if (editor_object.GetWorldObject().IsInherited(EditorLootPoint)) {
			general_group.Insert(new EditBoxNumberPrefab("Height", editor_object.GetWorldObject(), "Height"));
			general_group.Insert(new EditBoxNumberPrefab("Range", editor_object.GetWorldObject(), "Range"));
		}*/
		
		//general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", editor_object, "EditorOnly", editor_object.EditorOnly));
		AddContent(general_group);
		
		// All of these bindings are directly on the world object
		if (editor_object.GetWorldObject().IsInherited(NetworkLightBase)) {			
			GroupPrefab light_group = new GroupPrefab("Light Controls", editor_object.GetWorldObject(), string.Empty);
			light_group.Insert(new SliderPrefab("Brightness", editor_object.GetWorldObject(), "Brightness", 0, 40));
			light_group.Insert(new SliderPrefab("Radius", editor_object.GetWorldObject(), "Radius", 0, 1000));
			light_group.Insert(new CheckBoxPrefab("Cast Shadow", editor_object.GetWorldObject(), "CastShadow"));
			light_group.Insert(new CheckBoxPrefab("Enable Specular", editor_object.GetWorldObject(), "EnableSpecular"));
			light_group.Insert(new CheckBoxPrefab("Enable Linear", editor_object.GetWorldObject(), "EnableLinear"));
			light_group.Insert(new SliderPrefab("Pulse Coef", editor_object.GetWorldObject(), "PulseCoef"));
			light_group.Insert(new CheckBoxPrefab("Visible During Day", editor_object.GetWorldObject(), "VisibleDuringDay"));
			
			light_group.Insert(new CheckBoxPrefab("Enable Heat Haze", editor_object.GetWorldObject(), "HeatHaze"));
			light_group.Insert(new SliderPrefab("Heat Haze Radius", editor_object.GetWorldObject(), "HeatHazeRadius"));
			light_group.Insert(new SliderPrefab("Heat Haze Power", editor_object.GetWorldObject(), "HeatHazePower", 0, 1));
			
			light_group.Insert(new ColorPickerPrefab("Diffuse Color", editor_object.GetWorldObject(), "DiffuseColor"));
			light_group.Insert(new ColorPickerPrefab("Ambient Color", editor_object.GetWorldObject(), "AmbientColor"));
			
			light_group.Insert(new CheckBoxPrefab("Enable Flare", editor_object.GetWorldObject(), "FlareVisible"));
			light_group.Insert(new VectorPrefab("Flare Position", editor_object.GetWorldObject(), "FlareRelativePosition"));

			light_group.Insert(new SliderPrefab("Spot Light Angle", editor_object.GetWorldObject(), "SpotLightAngle", 0, 180));
			AddContent(light_group);
		}
		
		if (editor_object.GetWorldObject().IsInherited(NetworkParticleBase)) {
			GroupPrefab particle_group = new GroupPrefab("Particle Controls", editor_object.GetWorldObject(), string.Empty);
			DropdownListPrefab<int> particle_types = new DropdownListPrefab<int>("Particle Type", editor_object.GetWorldObject(), "ParticleType");
			
			typename particle_list = ParticleList;
			for (int i = 0; i < particle_list.GetVariableCount(); i++) {
				if (particle_list.GetVariableType(i) != int) {
					continue;
				}
				
				int particle_value;
				particle_list.GetVariableValue(null, i, particle_value);		
				if (!ParticleList.IsValidId(particle_value)) {
					continue;
				}
				
				particle_types[particle_list.GetVariableName(i)] = particle_value;
			}
			
			particle_group.Insert(particle_types);
			AddContent(particle_group);
		}
		
		if (editor_object.GetWorldObject().IsMan()) {
			GroupPrefab human_group = new GroupPrefab("#STR_EDITOR_HUMAN", this, string.Empty);
			//human_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SELECTED_PLAYER", editor_object, "Control"));
			DropdownListPrefab<int> animations = new DropdownListPrefab<int>("#STR_EDITOR_ANIMATION", this, "CurrentAnimation");
			map<string, int> emote_list = PlayerBase.GetEmoteList();
			foreach (string emote_name, int emote_id: emote_list) {
				animations[emote_name] = emote_id;
			}
			
			human_group.Insert(animations);
			human_group.Insert(new ButtonPrefab("#STR_EDITOR_CINEMATIC_CAMERA_RUN", this, "Animate"));
			
			AddContent(human_group);
		}
		
		if (editor_object.HasAnimations()) {
			GroupPrefab animations_group = new GroupPrefab("Object Animations", this, string.Empty);
			
			map<string, ref EditorObjectAnimationSource> object_animations = editor_object.GetObjectAnimations();
			foreach (string name, EditorObjectAnimationSource anim: object_animations) {
				animations_group.Insert(new SliderPrefab(name, anim, "AnimationPhase", 0, 1));
			}
			
			AddContent(animations_group);
		}
		
		GroupPrefab object_group = new GroupPrefab("#STR_EDITOR_OBJECT", editor_object, string.Empty);
		if (editor_object.GetWorldObject().HasDamageSystem()) {
			object_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", controller, "Health", 1, 0, editor_object.GetWorldObject().GetMaxHealth()));
		}
		
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", controller, "EditorOnly"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", controller, "Simulate"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", controller, "Locked"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", controller, "UsePhysics"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", controller, "AllowDamage"));
		//object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_COLLISION", editor_object, "Collision"));
		string expansion_check = "ExpansionMarketModule";
		if (expansion_check.ToType() && editor_object.GetWorldObject().IsInherited(EntityAI)) {
			object_group.Insert(new EditBoxPrefab("Trader Type", this, "ExpansionTraderType"));
		}
		
		AddContent(object_group);
		
		//AddContent(new MultilineEditBoxPrefab("Execute Code", editor_object, "TestingScript"));
		//AddContent(new ButtonPrefab("Execute", this, "ExecuteCode"));
		
		if (GetEditor().GetSettings().DebugMode) {
			/*
			GroupPrefab debug_group = new GroupPrefab("Debug", this, string.Empty);
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_TYPE", this, "Type"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_ID", editor_object, "ObjectID"));
			debug_group.Insert(new TextBoxPrefab("Flags", this, "Flags"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_MODEL", this, "Model"));
			AddContent(debug_group);*/
		}
		
		//SetupDialog();
	}
				
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w.IsInherited(TextWidget)) {
			string text = TextBoxPrefabGetString(w);
			
			if (text != string.Empty) {
				GetEditor().GetEditorHud().CreateNotification("Copied to clipboard!");
				GetGame().CopyToClipboard(text);
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	string TextBoxPrefabGetString(Widget text_widget)
	{
		TextBoxPrefab prefab;
		while (!prefab) {
			text_widget.GetUserData(prefab);
			text_widget = text_widget.GetParent();
			if (!text_widget) {
				return string.Empty;
			}
		}
		
		return prefab.GetPrefabController().Value;
	}
}