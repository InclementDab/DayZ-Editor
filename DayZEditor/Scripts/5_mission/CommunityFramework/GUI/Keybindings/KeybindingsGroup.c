modded class KeybindingsGroup
{
	void KeybindingsGroup( int index, Input input, Widget parent, KeybindingsMenu menu )
	{
		Widget subgroup	= m_Root.FindAnyWidget( "group_content" );

		TIntArray actions = new TIntArray;
		GetUApi().GetActiveInputs( actions );

		for( int i = 0; i < ModLoader.GetMods().Count(); i++ )
		{
			ModStructure mod = ModLoader.GetMods().Get( i );

			bool display = GetGame().ConfigIsExisting( mod.GetModPath() + " inputs" );

			TIntArray tempActions = new TIntArray;

			AddCFSubgroup( subgroup, input, mod, display, actions, tempActions );

			actions.Clear();
			actions.Copy( tempActions );
		}

		AddDayZSubgroup( subgroup, input, actions );
		
		subgroup.Update();
		
		m_Root.SetHandler( this );
	}

	void AddCFAction( int index, Widget parent, Input input, string displayName )
	{
		ref KeybindingElement ele = new ref KeybindingElement( index, parent, this );
		ele.R_WasSet = true;
		ele.R_DisplayName = displayName;
		ele.Reload();
		m_KeyWidgets.Insert( index, ele );
	}

	override void AddSubgroup( /*int index, */Widget parent, Input input )
	{
		// does nothing
	}

	void AddCFSubgroup( Widget parent, Input input, ModStructure mod, bool display, TIntArray inActions, out TIntArray remainingActions )
	{
		Widget subgroup_content;

		if ( display )
		{
			Widget subgroup = GetGame().GetWorkspace().CreateWidgets( "JM/CF/GUI/layouts/keybinding_subgroup.layout", parent );
			TextWidget subgroup_name = TextWidget.Cast( subgroup.FindAnyWidget( "subgroup_text" ) );

			string modDisplayName = "";
			GetGame().ConfigGetText( mod.GetModPath() + " name", modDisplayName );

			subgroup_name.SetText( modDisplayName );
			subgroup_content = subgroup.FindAnyWidget( "subgroup_content" );
		}
		
		//GetLogger().Log( "Attempting: " + mod.GetModPath(), "JM_CF_KeyBindings" );

		for ( int i = 0; i < inActions.Count(); i++ )
		{
			string displayName = "";
			bool found = false;

			for ( int j = 0; j < mod.GetModInputs().Count(); j++ )
			{
				int id = GetUApi().GetInputByName( mod.GetModInputs()[j].Name ).ID();
				if ( id == inActions[i] )
				{ 
					displayName = Widget.TranslateString( "#" + mod.GetModInputs()[j].Localization );
					int hashSymbol = displayName.IndexOf( " " );
					if ( hashSymbol < 1 )
					{
						displayName = mod.GetModInputs()[j].Localization;
					}
					found = true;
				}
			}

			if ( found )
			{
				if ( display )
				{
					AddCFAction( inActions.Get( i ), subgroup_content, input, displayName );
				}
			} else
			{
				remainingActions.Insert( inActions.Get( i ) );
			}

			// //GetLogger().Log( "  Found: " + found, "JM_CF_KeyBindings" );
		}
		
		if ( display )
		{
			subgroup_content.Update();
		}
	}

	void AddDayZSubgroup( Widget parent, Input input, TIntArray inActions )
	{
		Widget subgroup = GetGame().GetWorkspace().CreateWidgets( "JM/CF/GUI/layouts/keybinding_subgroup.layout", parent );
		TextWidget subgroup_name = TextWidget.Cast( subgroup.FindAnyWidget( "subgroup_text" ) );
		
		subgroup_name.SetText( "DayZ Standalone" );
		Widget subgroup_content = subgroup.FindAnyWidget( "subgroup_content" );
		
		for( int i = 0; i < inActions.Count(); i++ )
		{
			AddAction( inActions.Get( i ), subgroup_content, input );
		}
		
		subgroup_content.Update();
	}
}