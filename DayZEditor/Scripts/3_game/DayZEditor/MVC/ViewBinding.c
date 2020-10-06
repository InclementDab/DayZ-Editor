class ViewBinding : ScriptedViewBase
{
	// Name of Variable to bind to
	reference string Binding_Name;

	// Name of Variable to bind "selected item" to
	reference string Selected_Item;

	// If true, Bindings go both ways. Otherwise the controller is the master
	reference bool Two_Way_Binding;

	// Type of RelayCommand class that is controlled by ViewBinding
	reference string Relay_Command;

	// Strong reference to Relay Command
	protected autoptr RelayCommand m_RelayCommand;
	void SetRelayCommand(RelayCommand relayCommand)
	{
		m_RelayCommand = relayCommand;
		m_RelayCommand.SetViewBinding(this);
	}

	RelayCommand GetRelayCommand()
	{
		return m_RelayCommand;
	}

	protected autoptr TypeConverter m_PropertyConverter;
	TypeConverter GetPropertyConverter()
	{
		return m_PropertyConverter;
	}

	protected autoptr TypeConverter m_SelectedConverter;
	TypeConverter GetSelectedConverter()
	{
		return m_SelectedConverter;
	}

	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		Log("Loaded from Widget: %1", m_LayoutRoot.GetName());

		// Check for two way binding support
		if (Two_Way_Binding && !m_WidgetController.CanTwoWayBind())
		{
			Error("Two Way Binding for %1 is not supported!", m_LayoutRoot.Type().ToString());
		}
	}

	void SetProperties(typename binding_type, typename selected_type)
	{
		Trace("SetProperties");
		// Were not trying to data bind to empty Binding_Name
		if (binding_type && Binding_Name != string.Empty)
		{
			Log("Loading TypeConverter for Variable: %1 of Type: %2", Binding_Name, binding_type.ToString());
			m_PropertyConverter = LayoutBindingManager.GetTypeConversion(binding_type);
			if (!m_PropertyConverter)
			{
				Error("Could not find TypeConverter for type %1 in %2\n\nMod LayoutBindingManager.RegisterConversionTemplates to register custom TypeConverters", binding_type.ToString(), Binding_Name);
			}
		}

		// Were not trying to data bind to empty Selected_Item
		if (selected_type && Selected_Item != string.Empty)
		{
			Log("Loading TypeConverter for Variable: %1 of Type: %2", Selected_Item, selected_type.ToString());
			m_SelectedConverter = LayoutBindingManager.GetTypeConversion(selected_type);
			if (!m_SelectedConverter)
			{
				Error("Could not find TypeConverter for type %1 in %2\n\nMod LayoutBindingManager.RegisterConversionTemplates to register custom TypeConverters", selected_type.ToString(), Selected_Item);
			}
		}
	}

	// Controller -> view
	void UpdateView(Controller controller)
	{
		Trace("UpdateView");
		if (!m_WidgetController)
			return;

		// Binding_Name handler
		if (m_PropertyConverter)
		{
			Log("Updating %1 to the value of %2", m_LayoutRoot.GetName(), Binding_Name);
			m_PropertyConverter.GetFromController(controller, Binding_Name, 0);
			m_WidgetController.Set(m_PropertyConverter);
		}

		// Selected_Item handler
		if (m_SelectedConverter)
		{
			Log("Updating %1 to the value of %2", m_LayoutRoot.GetName(), Selected_Item);
			m_SelectedConverter.GetFromController(controller, Selected_Item, 0);
			m_WidgetController.SetSelection(m_SelectedConverter);
		}
	}

	// View -> Controller
	void UpdateController(Controller controller)
	{
		Trace("UpdateController");
		if (!m_WidgetController)
			return;

		// Binding_Name handler
		if (m_PropertyConverter && Two_Way_Binding && m_WidgetController.CanTwoWayBind())
		{
			Log("Setting %1 to the value of %2", Binding_Name, m_LayoutRoot.GetName());
			m_WidgetController.Get(m_PropertyConverter);
			m_PropertyConverter.SetToController(controller, Binding_Name, 0);
			controller.NotifyPropertyChanged(Binding_Name);
		}

		// Selected_Item handler
		if (m_SelectedConverter)
		{
			Log("Setting Selection of %1 with value of %2", Selected_Item, m_LayoutRoot.GetName());
			m_WidgetController.GetSelection(m_SelectedConverter);
			m_SelectedConverter.SetToController(controller, Selected_Item, 0);
			controller.NotifyPropertyChanged(Selected_Item);
		}
	}

	// Collection -> view
	void UpdateViewFromCollection(CollectionChangedEventArgs args)
	{
		Trace("UpdateViewFromCollection");

		if (!m_WidgetController)
			return;

		Log("Updating Collection View: %1", m_LayoutRoot.Type().ToString());

		// We dont want to work with type Observable for everything
		TypeConverter collectionConverter = args.Source.GetTypeConverter();
		if (!collectionConverter)
		{
			Error("Type Converter not found for Collection %1", args.Source.ToString());
			return;
		}

		// Anonymous Data Setter
		if (args.ChangedValue && args.ChangedIndex != -1)
		{
			collectionConverter.SetParam(args.ChangedValue);
		}

		switch (args.ChangedAction)
		{
			case NotifyCollectionChangedAction.Insert:
			{
				m_WidgetController.Insert(collectionConverter);
				break;
			}

			case NotifyCollectionChangedAction.InsertAt:
			{
				m_WidgetController.InsertAt(args.ChangedIndex, collectionConverter);
				break;
			}

			case NotifyCollectionChangedAction.Remove:
			{
				m_WidgetController.Remove(args.ChangedIndex, collectionConverter);
				break;
			}

			case NotifyCollectionChangedAction.Replace:
			{
				m_WidgetController.Replace(args.ChangedIndex, collectionConverter);
				break;
			}

			case NotifyCollectionChangedAction.Move:
			{
				m_WidgetController.Move(args.ChangedIndex, collectionConverter);
				break;
			}

			case NotifyCollectionChangedAction.Swap:
			{
				CollectionSwapArgs swap_args = CollectionSwapArgs.Cast(args.ChangedValue);
				m_WidgetController.Swap(swap_args.param1, swap_args.param2);
				break;
			}

			case NotifyCollectionChangedAction.Clear:
			{
				m_WidgetController.Clear();
				break;
			}

			default:
			{
				Error("Invalid NotifyCollectionChangedAction Type %1", args.ChangedAction.ToString());
			}
		}
	}

	// View -> Collection
	void UpdateCollectionFromView(ref CollectionChangedEventArgs args)
	{
		Trace("UpdateCollectionFromView");

		if (!m_WidgetController)
			return;

		Log("Updating Collection from View: %1", m_LayoutRoot.Type().ToString());
	}

	private bool InvokeCommand(ScriptedViewBase context, CommandArgs args)
	{
		Trace("InvokeCommand");

		bool handled;
		args.Context = this;
		if (m_RelayCommand && m_RelayCommand.CanExecute())
		{
			Log("Attempting to execute RelayCommand %1", Relay_Command);
			handled = m_RelayCommand.Execute(context, args);
		} else if (!m_RelayCommand && Relay_Command != string.Empty)
		{
			Log("Attempting to call function %1 on %2", Relay_Command, context.ToString());
			g_Script.CallFunction(context, Relay_Command, handled, args);
		}

		if (!handled && context && context.GetParent())
		{
			handled = InvokeCommand(context.GetParent(), args);
		}

		if (!context)
		{
			handled = true;
		}

		return handled;
	}

	// Command interfaces
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Trace("OnClick");
		switch (w.Type())
		{
			case ButtonWidget:
			{ // only thing that isnt called in OnChange for some reason
				if (InvokeCommand(this, new ButtonCommandArgs(ButtonWidget.Cast(w), button)))
				{
					// Weird situation but I need to call UpdateController from Controller without calling OnClick
					// if (w) is just an edge case if the object is deleted inside of the Command
					if (w)
						super.OnClick(w, x, y, button);

					return true;
				}

				break;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Trace("OnChange");
		switch (w.Type())
		{
			case CheckBoxWidget:
			{
				if (InvokeCommand(this, new CheckBoxCommandArgs(CheckBoxWidget.Cast(w))))
				{
					// Weird situation but I need to call UpdateController from Controller without calling OnChange
					// if (w) is just an edge case if the object is deleted inside of the Command
					if (w)
						super.OnChange(w, x, y, finished);
					return true;
				}

				break;
			}
		}

		return super.OnChange(w, x, y, finished);
	}

	/*
	void HandleDropReceived(ScriptedViewBase drop_target, Controller controller)
	{
		EditorLog.Trace("ViewBinding::HandleDropReceived");
		Print(drop_target);
		Print(m_WidgetController);
		if (!m_WidgetController || !m_WidgetController.CanTwoWayBind() || !Two_Way_Binding) return;
		

			
		Observable collection;
		EnScript.GetClassVar(controller, Binding_Name, 0, collection);
		Print(collection);
		if (collection) {
		
			//g_Script.CallFunction(collection, "Remove", null, drop_target);
			
			if (false) {	 // reciever_view && reciever != m_LayoutRoot
								
				int find;// = m_WidgetController.Find(reciever_view.GetTypeConversion());
				
				g_Script.CallFunction(collection, "Find", find, this);
				if (find != -1) {
					Print(drop_target);
					//m_WidgetController.InsertAt(find + 1, drop_target.GetTypeConversion());
					g_Script.CallFunctionParams(collection, "InsertAtEx", null, new Param2<TypeConverter, int>(GetTypeConversion(), find + 1));
				}
			} else {
				g_Script.Call(collection, "Insert", drop_target);
			}
		}		
	}*/
};