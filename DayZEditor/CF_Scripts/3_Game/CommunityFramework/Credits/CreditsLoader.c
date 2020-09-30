/*
	Epoch Mod for DayZ Standalone
	
	FileName: CreditsLoader.c

	Authors: DayZ SA Epoch Dev Team and Community Contributors
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/README.md	
	
	Licensing:
	https://github.com/EpochModTeam/DayZ_SA_Epoch/blob/experimental/LICENSE.md

	Changelog:

	[1/16/2019] AWOL / DirtySanchez
		AWOL1 - DZSA Epoch Dev Team and Community Credits
	[1/20/2019] AWOL
		AWOL2 - Made credits dynamically support other mods via 'creditsJson' var in CfgMods. Also appended DayZ Game header and credits
	[14/03/2019] Jacob_Mango
		JM3 - Changed the formatting of the credits
		
	[NewDate] Author(s)
		Initials(Change#)-Description
*/
modded class CreditsLoader
{
	override static JsonDataCredits GetData()
	{
		JsonDataCredits data = new JsonDataCredits;
		data.Departments = new array<ref JsonDataCreditsDepartment>;

		// get all mods
		array<ref ModStructure> mods = ModLoader.GetMods();
		foreach ( ref ModStructure mod: mods )
		{
			if ( mod.GetCredits() == NULL ) 
			{
				//GetLogger().Log( "Mod Credits is null for " + mod.GetModName(), "JM_CF_Credits" );
				continue;
			}

			foreach ( auto department: mod.GetCredits().Departments ) {
				data.Departments.Insert( department );
			}
		}

		// Append DayZ Game Credits Header
		ref JsonDataCreditsDepartment data_department_header = new JsonDataCreditsDepartment;
		data_department_header.Sections = new array<ref JsonDataCreditsSection>;
		data_department_header.DepartmentName = ("				DayZ Standalone");
		data.Departments.Insert(data_department_header);

		// Append DayZ Game Credits
		ref JsonDataCredits dayzCreditsData;
		JsonFileLoader<ref JsonDataCredits>.JsonLoadFile(JSON_FILE_PATH, dayzCreditsData);
		foreach(auto b: dayzCreditsData.Departments) {
			data.Departments.Insert(b);
		};
		
		return data;
	}
}