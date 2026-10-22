// Vanilla assets used by the generator defaults and presets, all of them work on console
class EditorSplineAssetLists
{
	static const ref array<string> ROAD_KITS = { "asf1", "asf2", "asf3", "city", "grav", "mud", "taxiway", "path_dirt", "path_rock", "custom" };

	// longest first, curves are named <degrees>_<radius>
	static const ref array<string> ROAD_STRAIGHTS = { "25", "12", "6", "30", "10" };
	static const ref array<string> ROAD_CURVES = { "0_2000", "1_1000", "7_100", "10_100", "10_75", "10_50", "10_25", "15_75", "22_50", "30_25", "60_10", "curve" };

	static const ref array<string> SPRUCE_TREES = {
		"DZ/plants/tree/t_PiceaAbies_1f.p3d", "DZ/plants/tree/t_PiceaAbies_1s.p3d", "DZ/plants/tree/t_PiceaAbies_1sb.p3d",
		"DZ/plants/tree/t_PiceaAbies_2f.p3d", "DZ/plants/tree/t_PiceaAbies_2fb.p3d", "DZ/plants/tree/t_PiceaAbies_2s.p3d",
		"DZ/plants/tree/t_PiceaAbies_2sb.p3d", "DZ/plants/tree/t_PiceaAbies_3f.p3d", "DZ/plants/tree/t_PiceaAbies_3s.p3d"
	};

	static const ref array<string> PINE_TREES = {
		"DZ/plants/tree/t_PinusSylvestris_1f.p3d", "DZ/plants/tree/t_PinusSylvestris_1s.p3d", "DZ/plants/tree/t_PinusSylvestris_2f.p3d",
		"DZ/plants/tree/t_PinusSylvestris_2fb.p3d", "DZ/plants/tree/t_PinusSylvestris_2s.p3d", "DZ/plants/tree/t_PinusSylvestris_2sb.p3d",
		"DZ/plants/tree/t_PinusSylvestris_3f.p3d", "DZ/plants/tree/t_PinusSylvestris_3fb.p3d", "DZ/plants/tree/t_PinusSylvestris_3fc.p3d",
		"DZ/plants/tree/t_PinusSylvestris_3s.p3d"
	};

	static const ref array<string> BIRCH_TREES = {
		"DZ/plants/tree/t_BetulaPendula_1f.p3d", "DZ/plants/tree/t_BetulaPendula_1fb.p3d", "DZ/plants/tree/t_BetulaPendula_1s.p3d",
		"DZ/plants/tree/t_BetulaPendula_2f.p3d", "DZ/plants/tree/t_BetulaPendula_2fb.p3d", "DZ/plants/tree/t_BetulaPendula_2fc.p3d",
		"DZ/plants/tree/t_BetulaPendula_2s.p3d", "DZ/plants/tree/t_BetulaPendula_3f.p3d", "DZ/plants/tree/t_BetulaPendula_3fb.p3d",
		"DZ/plants/tree/t_BetulaPendula_3fc.p3d", "DZ/plants/tree/t_BetulaPendula_3s.p3d"
	};

	static const ref array<string> DECIDUOUS_TREES = {
		"DZ/plants/tree/t_FagusSylvatica_1f.p3d", "DZ/plants/tree/t_FagusSylvatica_1fb.p3d", "DZ/plants/tree/t_FagusSylvatica_1s.p3d",
		"DZ/plants/tree/t_FagusSylvatica_2f.p3d", "DZ/plants/tree/t_FagusSylvatica_2fb.p3d", "DZ/plants/tree/t_FagusSylvatica_2s.p3d",
		"DZ/plants/tree/t_FagusSylvatica_3f.p3d", "DZ/plants/tree/t_FagusSylvatica_3fb.p3d", "DZ/plants/tree/t_FagusSylvatica_3s.p3d",
		"DZ/plants/tree/t_quercusRobur_1f.p3d", "DZ/plants/tree/t_quercusRobur_2f.p3d", "DZ/plants/tree/t_quercusRobur_2s.p3d",
		"DZ/plants/tree/t_quercusRobur_3f.p3d", "DZ/plants/tree/t_quercusRobur_3s.p3d", "DZ/plants/tree/t_carpinus_2s.p3d",
		"DZ/plants/tree/t_FraxinusExcelsior_2f.p3d", "DZ/plants/tree/t_FraxinusExcelsior_3s.p3d"
	};

	static const ref array<string> MIXED_TREES = {
		"DZ/plants/tree/t_PiceaAbies_2f.p3d", "DZ/plants/tree/t_PiceaAbies_3f.p3d", "DZ/plants/tree/t_PiceaAbies_2s.p3d",
		"DZ/plants/tree/t_PinusSylvestris_2f.p3d", "DZ/plants/tree/t_PinusSylvestris_3f.p3d", "DZ/plants/tree/t_BetulaPendula_2f.p3d",
		"DZ/plants/tree/t_BetulaPendula_3f.p3d", "DZ/plants/tree/t_FagusSylvatica_2f.p3d", "DZ/plants/tree/t_FagusSylvatica_3f.p3d",
		"DZ/plants/tree/t_quercusRobur_2f.p3d", "DZ/plants/tree/t_carpinus_2s.p3d", "DZ/plants/tree/t_LarixDecidua_2f.p3d"
	};

	static const ref array<string> LIVONIA_TREES = {
		"DZ/plants_bliss/Tree/t_PiceaAbies_1f_summer.p3d", "DZ/plants_bliss/Tree/t_PiceaAbies_2f_summer.p3d", "DZ/plants_bliss/Tree/t_PiceaAbies_3f_summer.p3d",
		"DZ/plants_bliss/Tree/t_PinusSylvestris_2f_summer.p3d", "DZ/plants_bliss/Tree/t_PinusSylvestris_3f_summer.p3d", "DZ/plants_bliss/Tree/t_FagusSylvatica_2f_summer.p3d",
		"DZ/plants_bliss/Tree/t_FagusSylvatica_3f_summer.p3d", "DZ/plants_bliss/Tree/t_BetulaPendulaE_2f_summer.p3d", "DZ/plants_bliss/Tree/t_BetulaPendulaE_3f_summer.p3d",
		"DZ/plants_bliss/Tree/t_acer_2s_summer.p3d", "DZ/plants_bliss/Tree/t_LarixDecidua_2f_summer.p3d"
	};

	static const ref array<string> BUSHES = {
		"DZ/plants/bush/b_corylusAvellana_1f.p3d", "DZ/plants/bush/b_corylusAvellana_2s.p3d", "DZ/plants/bush/b_crataegusLaevigata_1s.p3d",
		"DZ/plants/bush/b_crataegusLaevigata_2s.p3d", "DZ/plants/bush/b_prunusSpinosa_1s.p3d", "DZ/plants/bush/b_prunusSpinosa_2s.p3d",
		"DZ/plants/bush/b_rosaCanina_2s.p3d", "DZ/plants/bush/b_sambucusNigra_1s.p3d", "DZ/plants/bush/b_sambucusNigra_2s.p3d",
		"DZ/plants/tree/b_PiceaAbies_1f.p3d", "DZ/plants/tree/b_BetulaPendula_1f.p3d", "DZ/plants/tree/b_FagusSylvatica_1f.p3d"
	};

	static const ref array<string> LIVONIA_BUSHES = {
		"DZ/plants_bliss/Bush/b_corylusHeterophylla_1s_summer.p3d", "DZ/plants_bliss/Bush/b_corylusHeterophylla_2s_summer.p3d",
		"DZ/plants_bliss/Bush/b_prunusSpinosa_1s_summer.p3d", "DZ/plants_bliss/Bush/b_rosaCanina_2s_summer.p3d",
		"DZ/plants_bliss/Bush/b_sambucusNigra_1s_summer.p3d", "DZ/plants_bliss/Bush/b_caraganaArborescens_2s_summer.p3d",
		"DZ/plants_bliss/Bush/b_betulaNana_1s_summer.p3d"
	};

	static const ref array<string> FOOTPATH_DIRT = { "StaticObj_Roads_Partpath_dirt_10" };
	static const ref array<string> FOOTPATH_ROCK = { "StaticObj_Roads_Partpath_rock_10" };
	static const ref array<string> FOOTPATH_TRAIL = { "StaticObj_Decal_dirt_damage_long1", "StaticObj_Decal_dirt_damage_long2", "StaticObj_Decal_dirt_damage_long3" };
	static const ref array<string> FOOTPATH_PANELS = { "StaticObj_Path_Panels", "StaticObj_Path_Panels", "StaticObj_Path_Panels_Damaged" };
	static const ref array<string> FOOTPATH_SIDEWALK = { "StaticObj_Sidewalk1_10m" };

	// Nasdara
	static const ref array<string> NASDARA_TRAIL = { "StaticObj_Decal_dirt_damage_long1_TK", "StaticObj_Decal_dirt_damage_long3_TK" };
	static const ref array<string> NASDARA_SIDEWALK_6 = { "StaticObj_Sidewalk6_6m" };
	static const ref array<string> NASDARA_SIDEWALK_7 = { "StaticObj_Sidewalk7_6m" };
	static const ref array<string> NASDARA_SIDEWALK_8 = { "StaticObj_Sidewalk8_6m" };

	static const ref array<string> NASDARA_PALMS = {
		"DZ/plants_takistan/Tree/t_PhoenixRupicola_1s.p3d", "DZ/plants_takistan/Tree/t_PhoenixRupicola_3s.p3d"
	};

	static const ref array<string> NASDARA_OLIVES = {
		"DZ/plants_takistan/Tree/t_OleaEuropaea_1s.p3d", "DZ/plants_takistan/Tree/t_OleaEuropaea_2s.p3d"
	};

	static const ref array<string> NASDARA_PINES = {
		"DZ/plants_takistan/Tree/t_PinusBrutia_1s.p3d", "DZ/plants_takistan/Tree/t_PinusBrutia_2s.p3d", "DZ/plants_takistan/Tree/t_PinusBrutia_2sb.p3d"
	};

	static const ref array<string> NASDARA_OASIS_TREES = {
		"DZ/plants_takistan/Tree/t_PhoenixRupicola_1s.p3d", "DZ/plants_takistan/Tree/t_PhoenixRupicola_3s.p3d", "DZ/plants_takistan/Tree/t_FicusBenjamina_1s.p3d",
		"DZ/plants_takistan/Tree/t_FicusBenjamina_2s.p3d", "DZ/plants_takistan/Tree/t_Populus_2s.p3d", "DZ/plants_takistan/Tree/t_Populus_2sb.p3d",
		"DZ/plants_takistan/Tree/t_PrunusDomestica_1s.p3d", "DZ/plants_takistan/Tree/t_PrunusDomestica_2s.p3d", "DZ/plants_takistan/Tree/t_FraxinusExcelsior_2s_spring.p3d"
	};

	static const ref array<string> NASDARA_BUSHES = {
		"DZ/plants_takistan/Bush/b_PeganumHarmala_1s.p3d", "DZ/plants_takistan/Bush/b_PeganumHarmala_2s.p3d", "DZ/plants_takistan/Bush/b_PeganumHarmala_2sb.p3d",
		"DZ/plants_takistan/Bush/b_NeriumOleander_1s.p3d", "DZ/plants_takistan/Bush/b_NeriumOleander_1sb.p3d", "DZ/plants_takistan/Bush/b_Cycas_2s.p3d",
		"DZ/plants_takistan/Bush/b_Gardenia_1s.p3d", "DZ/plants_takistan/Bush/b_PinusBrutia_1s.p3d"
	};

	static const ref array<string> NASDARA_SCRUB = {
		"DZ/plants_takistan/Bush/b_PeganumHarmala_1s.p3d", "DZ/plants_takistan/Bush/b_PeganumHarmala_2s.p3d", "DZ/plants_takistan/Bush/b_PeganumHarmala_2sb.p3d",
		"DZ/plants_takistan/Bush/b_OpuntiaFicusIndica_1s.p3d", "DZ/plants_takistan/Bush/b_OpuntiaFicusIndica_2s.p3d", "DZ/plants_takistan/Bush/b_OpuntiaFicusIndica_3s.p3d",
		"DZ/plants_takistan/Bush/p_ViciaSativa_1s.p3d", "DZ/plants_takistan/Bush/p_ViciaSativa_1sB.p3d"
	};

	static const ref array<string> NASDARA_STONES = {
		"DZ/rocks_takistan/stone1_sand.p3d", "DZ/rocks_takistan/stone2_sand.p3d", "DZ/rocks_takistan/stone3_sand.p3d",
		"DZ/rocks_takistan/stone4_sand.p3d", "DZ/rocks_takistan/stone5_sand.p3d"
	};

	static const ref array<string> NASDARA_BOULDERS = {
		"DZ/rocks_takistan/rock_sand_apart1.p3d", "DZ/rocks_takistan/rock_sand_apart2.p3d", "DZ/rocks_takistan/stones_sand_erosion.p3d",
		"DZ/rocks_takistan/stone1_sand.p3d", "DZ/rocks_takistan/stone3_sand.p3d", "DZ/rocks_takistan/stone5_sand.p3d"
	};

	static const ref array<string> ROCKS = {
		"DZ/rocks_bliss/stone6.p3d", "DZ/rocks_bliss/stone7.p3d", "DZ/rocks_bliss/stone8.p3d", "DZ/rocks_bliss/stone9.p3d",
		"DZ/rocks_bliss/stone10.p3d", "DZ/rocks_bliss/stone6_moss.p3d", "DZ/rocks_bliss/stone8_moss.p3d", "DZ/rocks_bliss/stone10_moss.p3d"
	};
}