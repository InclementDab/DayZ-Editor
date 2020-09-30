modded class ModStructure
{
	protected ref array< ref ModInput > m_ModInputs;

	protected ref JsonDataCredits m_Credits;
	
	protected string m_Version;
	
	override void LoadData()
	{
		super.LoadData();

		m_ModInputs = new ref array< ref ModInput >;

		if ( GetGame().ConfigIsExisting( m_ModPath ) )
		{
			//GetLogger().Log( "Checking mod: " + m_ModName, "JM_CF_Mods" );
			
			if ( GetGame().ConfigIsExisting( m_ModPath + " creditsJson" ) )
			{
				//GetLogger().Log( "	Found JSON Credits", "JM_CF_Mods" );
				
				string creditsPath;
				GetGame().ConfigGetText( m_ModPath + " creditsJson", creditsPath );

				JsonFileLoader<ref JsonDataCredits>.JsonLoadFile( creditsPath, m_Credits );
			} else if ( GetGame().ConfigIsExisting( m_ModPath + " credits" ) )
			{
				//GetLogger().Log( "	Using Raw Credits", "JM_CF_Mods" );
				
				string credits = "";

				m_Credits = new ref JsonDataCredits;
				m_Credits.Departments = new array< ref JsonDataCreditsDepartment >;

				ref JsonDataCreditsDepartment mod_department_header = new JsonDataCreditsDepartment;
				mod_department_header.Sections = new array<ref JsonDataCreditsSection>;
				mod_department_header.DepartmentName = "				" + m_ModName;

				string author = "";
				bool hasAuthor = GetGame().ConfigIsExisting( m_ModPath + " author" );
				GetGame().ConfigGetText( m_ModPath + " author", author );

				//GetLogger().Log( "	Has author: " + hasAuthor, "JM_CF_Mods" );
				//GetLogger().Log( "	Author: " + author, "JM_CF_Mods" );
				if ( hasAuthor && author != "" )
				{
					ref JsonDataCreditsSection mod_section_modheader_author = new JsonDataCreditsSection;
					mod_section_modheader_author.SectionLines = new array<string>;
					mod_section_modheader_author.SectionName = ( "Author" );
					
					mod_section_modheader_author.SectionLines.Insert( author );
					
					mod_department_header.Sections.Insert( mod_section_modheader_author );
				}
				
				GetGame().ConfigGetText( m_ModPath + " credits", credits );
				//GetLogger().Log( "	Credits: " + credits, "JM_CF_Mods" );
				if ( credits != "" )
				{
					ref JsonDataCreditsSection mod_section_modheader = new JsonDataCreditsSection;
					mod_section_modheader.SectionLines = new array<string>;
					mod_section_modheader.SectionName = ( "Credits" );
					
					array<string> creditsArray = new array<string>;
					credits.Split(", ", creditsArray);
					foreach ( string credit: creditsArray )
					{
						mod_section_modheader.SectionLines.Insert( credit );
					}
					
					mod_department_header.Sections.Insert( mod_section_modheader );
				}

				m_Credits.Departments.Insert( mod_department_header );
			} else
			{
				m_Credits = new ref JsonDataCredits;
			}
			
			if ( GetGame().ConfigIsExisting( m_ModPath + " versionPath" ) )
			{
				string versionPath;
				GetGame().ConfigGetText( m_ModPath + " versionPath", versionPath );
				
				FileHandle file_handle = OpenFile( versionPath, FileMode.READ );
		
				while ( FGets( file_handle, m_Version ) > 0 )
					break;
		
				CloseFile( file_handle );
			} else if ( GetGame().ConfigIsExisting( m_ModPath + " version" ) )
			{
				GetGame().ConfigGetText( m_ModPath + " version", m_Version );
			}
			
			//GetLogger().Log( "	Mod version is " + m_Version, "JM_CF_Mods" );

			if ( GetGame().ConfigIsExisting( m_ModPath + " inputs" ) )
			{
				string inputPath;
				GetGame().ConfigGetText( m_ModPath + " inputs", inputPath );
				
				string C_START = "<input";
				string C_END = "/>";

				string ACTION_OPEN = "<actions>";
				string ACTION_CLOSE = "</actions>";

				array<string> m_XMLData = new array<string>;

				FileHandle currentFile = OpenFile( inputPath, FileMode.READ);
				string line_content = "";
				if ( currentFile != 0 )
				{
					while ( FGets( currentFile, line_content ) >= 0 )
					{
						m_XMLData.Insert( line_content );
					}
					CloseFile( currentFile );
				}

				bool isInRightPlace = false;

				//GetLogger().Log( "Formatting XML file!", "JM_CF_Mods" );
				ref array<string> rawInputs = new array<string>;
				for ( int i = 0; i < m_XMLData.Count(); ++i )
				{
					string xmlLine = m_XMLData[i];

					if ( xmlLine.Contains( ACTION_CLOSE ) && isInRightPlace )
					{
						break;
					}

					if ( xmlLine.Contains( ACTION_OPEN ) )
					{
						isInRightPlace = true;
						continue;
					}

					if ( isInRightPlace )
					{
						//xmlLine.TrimInPlace();
						rawInputs.Insert( xmlLine );
						//GetLogger().Log( "	" + xmlLine, "JM_CF_Mods" );
					}
				}
				//GetLogger().Log( "Finished XML file!", "JM_CF_Mods" );

				for (int x = 0; x < rawInputs.Count(); ++x)
				{
					ref ModInput modInput = new ref ModInput;

					string rawInput = rawInputs[x];
					
					int nameIndex = rawInput.IndexOfFrom( 0, "name=" );
					int localisationIndex = rawInput.IndexOfFrom( 0, "loc=" );
					int visibleIndex = rawInput.IndexOfFrom( 0, "visible=" );

					//GetLogger().Log( "nameIndex: " + nameIndex, "JM_CF_Mods" );
					//GetLogger().Log( "localisationIndex: " + localisationIndex, "JM_CF_Mods" );
					//GetLogger().Log( "visibleIndex: " + visibleIndex, "JM_CF_Mods" );

					int startQuotation = -1;
					int endQuotation = -1;

					if ( nameIndex >= 0 )
					{
						startQuotation = rawInput.IndexOfFrom( nameIndex, "\"" );
						endQuotation = rawInput.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;

						//GetLogger().Log( "startQuotation: " + startQuotation, "JM_CF_Mods" );
						//GetLogger().Log( "endQuotation: " + endQuotation, "JM_CF_Mods" );

						modInput.Name = rawInput.Substring( startQuotation + 1, endQuotation - 1 );

						//GetLogger().Log( "modInput.Name: " + modInput.Name, "JM_CF_Mods" );

						startQuotation = -1;
						endQuotation = -1;
					}

					if ( localisationIndex >= 0 )
					{
						startQuotation = rawInput.IndexOfFrom( localisationIndex, "\"" );
						endQuotation = rawInput.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;

						//GetLogger().Log( "startQuotation: " + startQuotation, "JM_CF_Mods" );
						//GetLogger().Log( "endQuotation: " + endQuotation, "JM_CF_Mods" );

						modInput.Localization = rawInput.Substring( startQuotation + 1, endQuotation - 1 );

						//GetLogger().Log( "modInput.Localization: " + modInput.Localization, "JM_CF_Mods" );

						startQuotation = -1;
						endQuotation = -1;
					} else
					{
						modInput.Localization = modInput.Name;
					}

					if ( visibleIndex >= 0 )
					{
						startQuotation = rawInput.IndexOfFrom( visibleIndex, "\"" );
						endQuotation = rawInput.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;

						//GetLogger().Log( "startQuotation: " + startQuotation, "JM_CF_Mods" );
						//GetLogger().Log( "endQuotation: " + endQuotation, "JM_CF_Mods" );

						string visBool = rawInput.Substring( startQuotation + 1, endQuotation - 1 );
						visBool.ToLower();

						//GetLogger().Log( "visBool: " + visBool, "JM_CF_Mods" );

						if ( visBool.Contains( "true" ) )
							modInput.Visible = true;
						else if ( visBool.Contains( "false" ) )
							modInput.Visible = false;

						//GetLogger().Log( "modInput.Visible: " + modInput.Visible, "JM_CF_Mods" );

						startQuotation = -1;
						endQuotation = -1;
					} else 
					{
						modInput.Visible = true;
					}

					m_ModInputs.Insert( modInput );
				}
			}
		}
	}
	
	ref array< ref ModInput > GetModInputs()
	{
		return m_ModInputs;
	}

	ref JsonDataCredits GetCredits()
	{
		return m_Credits;
	}

	string GetModPath()
	{
		return m_ModPath;
	}
	
	override string GetModName()
	{
		return m_ModName;
	}
	
	override string GetModLogo()
	{
		return m_ModLogo;
	}
	
	override string GetModLogoSmall()
	{
		return m_ModLogoSmall;
	}
	
	override string GetModLogoOver()
	{
		return m_ModLogoOver;
	}
	
	override string GetModActionURL()
	{
		return m_ModActionURL;
	}
	
	override string GetModToltip()
	{
		return m_ModTooltip;
	}
	
	override string GetModOverview()
	{
		return m_ModOverview;
	}

	string GetModVersion()
	{
		return m_Version;
	}
}