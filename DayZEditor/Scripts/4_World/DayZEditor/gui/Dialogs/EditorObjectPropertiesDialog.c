class EditorObjectPropertiesDialog: EditorDialogBase
{
	protected bool ExecuteCode;
	protected ref array<EditorObject> m_EditorObjects = {};
	protected ref EditorMultiObjectCommandController m_EditorMultiObjectCommandController;
	protected EditorObject m_EditorObject;
	
	// Object Data
	int ObjectID;
	string Name;
	vector Position;
	vector Orientation;
	float Scale = 1.0;
	
	// Object Properties
	float Health = 100;
	bool Show = true;
	bool Locked;
	bool Simulate;
	bool UsePhysics;
	bool AllowDamage = false;
	bool Collision = true;
	bool EditorOnly = false;
	
	// Human Properties
	int CurrentAnimation;
	bool Animate;
	
	// Custom stuff 
	string ExpansionTraderType;
	string TestingScript;
	
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
		foreach (EditorObject editor_object: m_EditorObjects) {
			editor_object.UpdateNet();
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
		
		if (m_EditorObject.GetData().Parameters["ExpansionTraderType"]) {
			ExpansionTraderType = SerializableParam1<string>.Cast(m_EditorObject.GetData().Parameters["ExpansionTraderType"]).param1;
		}

		m_EditorObject.Update();
		
		ObjectID = m_EditorObject.GetWorldObject().GetID();
		Name = m_EditorObject.GetDisplayName();
		Position = m_EditorObject.GetPosition();
		Orientation = m_EditorObject.GetOrientation();
		Scale = m_EditorObject.GetScale();
		
		// Version 3
		Locked = m_EditorObject.IsLocked();
		EditorOnly = m_EditorObject.IsEditorOnly();
		AllowDamage = m_EditorObject.IsAllowDamage();
		
		if (m_EditorObject.GetWorldObject().HasDamageSystem()) {
			Health = m_EditorObject.GetWorldObject().GetHealth("", "Health");
		}

		// If network light
		//if (NetworkLightBase.Cast(m_EditorObject.GetWorldObject())) {
		//	NetworkLightBase.Cast(m_EditorObject.GetWorldObject()).Read(m_Data.Parameters);
		//}
		
		GroupPrefab general_group = new GroupPrefab("#STR_EDITOR_GENERAL", this, string.Empty);
		general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_SHOW", this, "Show"));
		general_group.Insert(new EditBoxPrefab("#STR_EDITOR_NAME", this, "Name"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_POSITION", this, "Position"));
		general_group.Insert(new VectorPrefab("#STR_EDITOR_ORIENTATION", this, "Orientation"));
		general_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_SCALE", this, "Scale", 0.01));

		if (editor_object.GetWorldObject().IsInherited(EditorLootPoint)) {
			general_group.Insert(new EditBoxNumberPrefab("Height", editor_object.GetWorldObject(), "Height"));
			general_group.Insert(new EditBoxNumberPrefab("Range", editor_object.GetWorldObject(), "Range"));
		}
		
		//general_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", editor_object, "EditorOnly", editor_object.EditorOnly));
		AddContent(general_group);
		
		// All of these bindings are directly on the world object
		if (editor_object.GetWorldObject().IsInherited(NetworkLightBase)) {			
			GroupPrefab light_group = new GroupPrefab("Light Controls", editor_object.GetWorldObject(), string.Empty);
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
			light_group.Insert(new SliderPrefab("Brightness", editor_object.GetWorldObject(), "Brightness"));
			
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
			object_group.Insert(new EditBoxNumberPrefab("#STR_EDITOR_HEALTH", this, "Health", 1, 0, editor_object.GetWorldObject().GetMaxHealth()));
		}
		
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_EDITOR_ONLY", this, "EditorOnly"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_SIMULATION", this, "Simulate"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_LOCK", this, "Locked"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_PHYSICS", this, "UsePhysics"));
		object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_DAMAGE", this, "AllowDamage"));
		//object_group.Insert(new CheckBoxPrefab("#STR_EDITOR_ENABLE_COLLISION", editor_object, "Collision"));
		string expansion_check = "ExpansionMarketModule";
		if (expansion_check.ToType() && editor_object.GetWorldObject().IsInherited(EntityAI)) {
			object_group.Insert(new EditBoxPrefab("Trader Type", this, "ExpansionTraderType"));
		}
		
		AddContent(object_group);
		
		//AddContent(new MultilineEditBoxPrefab("Execute Code", editor_object, "TestingScript"));
		//AddContent(new ButtonPrefab("Execute", this, "ExecuteCode"));
		
		if (GetEditor().GetSettings().DebugMode) {
			GroupPrefab debug_group = new GroupPrefab("Debug", this, string.Empty);
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_TYPE", this, "Type"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_ID", editor_object, "ObjectID"));
			debug_group.Insert(new TextBoxPrefab("Flags", this, "Flags"));
			debug_group.Insert(new TextBoxPrefab("#STR_EDITOR_MODEL", this, "Model"));
			AddContent(debug_group);
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
	
	// EditorObjects can also be psuedo-controllers
	void PropertyChanged(string property_name)
	{
		//EditorLog.Trace("EditorObject::PropertyChanged %1", property_name);
		EntityAI entity_world_object = EntityAI.Cast(m_EditorObject.GetWorldObject());
		switch (property_name) {
			case "Name": {
				m_EditorObject.SetDisplayName(Name);
				break;
			}
			
			case "Position": {
				EditorAction position_undo = new EditorAction("SetTransform", "SetTransform");
				position_undo.InsertUndoParameter(m_EditorObject.GetTransformArray());
				m_EditorObject.SetPosition(Position);
				position_undo.InsertRedoParameter(m_EditorObject.GetTransformArray());
				GetEditor().InsertAction(position_undo);
				break;
			}
			
			case "Orientation": {
				EditorAction orientation_undo = new EditorAction("SetTransform", "SetTransform");
				orientation_undo.InsertUndoParameter(m_EditorObject.GetTransformArray());
				
				m_EditorObject.SetOrientation(Orientation);
				orientation_undo.InsertRedoParameter(m_EditorObject.GetTransformArray());
				GetEditor().InsertAction(orientation_undo);
				break;
			}
			
			case "Scale": {
				if (Scale < 0.000001) {
					Scale = 0.000001;
				}
								
				m_EditorObject.SetScale(Scale);
				break;
			}
			
			case "Show": {
				m_EditorObject.Show(Show);
				break;
			}
			
			case "Locked": {
				m_EditorObject.Lock(Locked);
				break;
			}
			
			case "UsePhysics": {
				m_EditorObject.SetPhysicsEnabled(UsePhysics);
				break;
			}
			
			case "Simulate": {
				m_EditorObject.SetSimulate(Simulate);
				break;
			}
									
			case "Animate": {
				PlayerBase emote_player = PlayerBase.Cast(m_EditorObject.GetWorldObject());
				if (emote_player) {
					emote_player.GetEmoteManager().PlayEmote(CurrentAnimation);
				}
				
				break;
			}
						
			case "EditorOnly": {
				//m_Data.EditorOnly = EditorOnly;
				break;
			}
			
			case "AllowDamage": {
				m_EditorObject.SetAllowDamage(AllowDamage);
				break;
			}
			
			case "Health": {
				if (m_EditorObject.GetWorldObject().HasDamageSystem()) {
					Health = Math.Clamp(Health, 0, m_EditorObject.GetWorldObject().GetMaxHealth("", "Health"));
					m_EditorObject.SetHealth(Health);
				}
				break;
			}
			
			case "Collision": {
				if (Collision) {
					m_EditorObject.GetWorldObject().SetFlags(EntityFlags.SOLID, true);
				} else {
					m_EditorObject.GetWorldObject().ClearFlags(EntityFlags.SOLID, true);
				}
				
				break;
			}
			
			case "ExpansionTraderType": {
				// storing the custom data				
				m_EditorObject.GetData().Parameters["ExpansionTraderType"] = SerializableParam1<string>.Create(ExpansionTraderType);
				break;
			}
			
			case "ExecuteCode": {
				ExecuteCode = false;
				if (!m_EditorObjects[0]) {
					break;
				}
				
				m_EditorObjects[0].ExecuteCode();
				break;
			}
		}
	}
}