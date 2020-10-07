#!/bin/bash

if [[ $# -eq "0" ]]; then
	echo "Invalid Arguments - Please provide a pbo to rebuild!";
	echo "Example:";
	echo "$0 Scripts Particles Vehicles_Air_An2 Vehicles_Air_Merlin";
	echo "For sub-directoried files, change '/' to '_'";
	exit;
fi

dir="P:\\DayZExpansion\\"

### ||| Define Top Level File Structure ||| ###
declare -a toplevelarr=("AI" "GUI" "languagecore" "Particles" "Scripts" "Skins" "Test" "Zombies")


declare -a secondlevelarr=(
	### ||| Animals ||| ###
	"Animals_lepus_europaeus"

	### ||| Animations ||| ###
	"Animations_Animals"
	"Animations_Player"
	"Animations_Sources"

	### ||| Data ||| ###
	"Data_Characters_Armbands"
	"Data_Characters_Backpacks"
	"Data_Characters_Ghilies"
	"Data_Characters_Gloves"
	"Data_Characters_Headgear"
	"Data_Characters_Tops"

	### ||| Dta ||| ###
	"Dta_Core"
	"Dta_Game"
	"Dta_World"

	### ||| Objects ||| ###
	"Objects_Airdrop"

	### ||| Objects_Basebuilding ||| ###
	"Objects_Basebuilding_Data"
	"Objects_Basebuilding_Flags"
	"Objects_Basebuilding_Floors"
	"Objects_Basebuilding_Items"
	"Objects_Basebuilding_Misc"
	"Objects_Basebuilding_Pillars"
	"Objects_Basebuilding_Ramps"
	"Objects_Basebuilding_Safes"
	"Objects_Basebuilding_Stairs"
	"Objects_Basebuilding_Tents"
	"Objects_Basebuilding_Walls"
	"Objects_Basebuilding_Gates"

	### ||| Objects_Gear ||| ###
	"Objects_Gear_Box"
	"Objects_Gear_Chickens"
	"Objects_Gear_Consumables"
	"Objects_Gear_Electricity"
	"Objects_Gear_Military"
	"Objects_Gear_Parachute"
	"Objects_Gear_Quest"
	"Objects_Gear_Traders"
	"Objects_Gear_Vehicles"

	### ||| Objects_Structures ||| ###
	"Objects_Structures_BuilderItems"
	"Objects_Structures_Debug"
	"Objects_Structures_Editor"
	"Objects_Structures_Flags"
	"Objects_Structures_Grass"
	"Objects_Structures_Grass_Sample"
	"Objects_Structures_IvyAutomn"
	"Objects_Structures_Props"
	"Objects_Structures_Signs"

	### ||| Objects_Weapons ||| ###
	"Objects_Weapons_Attachments_Optics_3xMagnifier"
	"Objects_Weapons_Attachments_Optics_Bravo4"
	"Objects_Weapons_Attachments_Optics_Deltapoint"
	"Objects_Weapons_Attachments_Optics_exps3"
	"Objects_Weapons_Attachments_Optics_Hamr"
	"Objects_Weapons_Attachments_Optics_PMII25x"
	"Objects_Weapons_Attachments_Optics_Reflex"

	"Objects_Weapons_Explosives"

	"Objects_Weapons_Firearms_Archery"
	"Objects_Weapons_Firearms_Flaregun"
	"Objects_Weapons_Firearms_LAW"
	"Objects_Weapons_Firearms_LMG"
	"Objects_Weapons_Firearms_M97"
	"Objects_Weapons_Firearms_MGL"
	"Objects_Weapons_Firearms_RPG7"

	"Objects_Weapons_Firearms_Pistols_DE"
	"Objects_Weapons_Firearms_Pistols_M9"
	"Objects_Weapons_Firearms_Pistols_NFP"
	"Objects_Weapons_Firearms_Pistols_PR9"
	"Objects_Weapons_Firearms_Rifles_G36"
	"Objects_Weapons_Firearms_Rifles_M16"
	"Objects_Weapons_Firearms_Rifles_SCAR"
	"Objects_Weapons_Firearms_Rifles_W1873"

	"Objects_Weapons_Firearms_Shotguns_DT11"
	"Objects_Weapons_Firearms_Shotguns_M890"
	"Objects_Weapons_Firearms_Shotguns_Remore"
	"Objects_Weapons_Firearms_Shotguns_SDASS"

	"Objects_Weapons_Firearms_SMG_GTM"
	"Objects_Weapons_Firearms_SMG_MP5"
	"Objects_Weapons_Firearms_SMG_mp7"
	"Objects_Weapons_Firearms_SMG_P99"
	"Objects_Weapons_Firearms_SMG_Vector"

	"Objects_Weapons_Firearms_Sniper_AWM"

	"Objects_Weapons_Misc_RadiationHit"

	### ||| Sounds ||| ###
	"Sounds_Airdrop"
	"Sounds_BaseBuilding"
	"Sounds_Book"
	"Sounds_Church"
	"Sounds_Explosions"
	"Sounds_UI"

	"Sounds_Vehicles_Air_AN2"
	"Sounds_Vehicles_Air_Gyro"
	"Sounds_Vehicles_Air_Merlin"
	"Sounds_Vehicles_Air_Mh6"
	"Sounds_Vehicles_Air_Uh1h"

	"Sounds_Vehicles_Common"

	"Sounds_Vehicles_Ground_N250"
	"Sounds_Vehicles_Ground_TT650"
	"Sounds_Vehicles_Ground_Vodnik"

	"Sounds_Vehicles_Water_Amphibia"
	"Sounds_Vehicles_Water_LHD"
	"Sounds_Vehicles_Water_Utility"
	"Sounds_Vehicles_Water_Zodiac"

	"Sounds_Weapons_Crossbow"
	"Sounds_Weapons_M79"
	"Sounds_Weapons_RPG7"

	### ||| Vehicles ||| ###
	"Vehicles_Air_An2"
	"Vehicles_Air_C130J"
	"Vehicles_Air_Gyro"
	"Vehicles_Air_Merlin"
	"Vehicles_Air_Mh6"
	"Vehicles_Air_Uh1h"

	"Vehicles_Ground_250N"
	"Vehicles_Ground_Bus"
	"Vehicles_Ground_Forklift"
	"Vehicles_Ground_QuadAtv"
	"Vehicles_Ground_Tank"
	"Vehicles_Ground_Tractor"
	"Vehicles_Ground_TT650"
	"Vehicles_Ground_Vanilla"
	"Vehicles_Ground_Vodnik"
	"Vehicles_Ground_UAZ"

	"Vehicles_Water_Amphibia"
	"Vehicles_Water_Carrier"
	"Vehicles_Water_Utility"
	"Vehicles_Water_Zodiac"

	### ||| Worlds ||| ###
	"Worlds_ChernarusPlus_Gloom"
	"Worlds_ChernarusPlus_Vanilla"
	"Worlds_ChernarusPlus_VanillaRays"

	"Worlds_enoch_Gloom"
	)


alreadyProcessed=false;

failedArray=();

for args in "$@"; do
	for i in "${toplevelarr[@]}"; do
		if [[ $alreadyProcessed == false ]]; then
			if [[ $args == $i ]]; then
				alreadyProcessed=true
				loc=$(tr '_' "/" <<< $i)
				directory=$dir$loc
				if [[ $args == "Scripts" ]]; then
					cmd.exe /C "P:\\DayZExpansion\\Workbench\\Batchfiles\\BinarizeObfuscatePBO.bat" "$directory\\config.cpp"
				else
					cmd.exe /C "P:\\DayZExpansion\\Workbench\\Batchfiles\\BinarizePBO.bat" "$directory\\config.cpp"
				fi
				
				break;
			fi
		fi
	done
	
	for i in "${secondlevelarr[@]}"; do
		if [[ $alreadyProcessed == false ]]; then
			if [[ $args == $i ]]; then
				alreadyProcessed=true
				loc=$(tr '_' "/" <<< $i)
				directory=$dir$loc
				cmd.exe /C "P:\\DayZExpansion\\Workbench\\Batchfiles\\BinarizePBO.bat" "$directory\\config.cpp"
				break;
			fi
		fi
	done

	if [[ $alreadyProcessed == false ]]; then
		failedArray+=($args)
	fi

alreadyProcessed=false;
done

clear

echo "								"
echo "								"
echo -e "		\e[91mNNNNNNNN        NNNNNNNN"
echo -e "		\e[91mN\e[0m:::::::\e[91mN       N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::::\e[91mN      N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m:::::::::\e[91mN     N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::::::\e[91mN    N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m:::::::::::\e[91mN   N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m:::::::\e[91mN\e[0m::::\e[91mN  N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN N\e[0m::::\e[91mN N\e[0m::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN  N\e[0m::::\e[91mN\e[0m:::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN   N\e[0m:::::::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN    N\e[0m::::::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN     N\e[0m:::::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN      N\e[0m::::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN       N\e[0m:::::::\e[91mN"
echo -e "		\e[91mN\e[0m::::::\e[91mN        N\e[0m::::::\e[91mN"
echo -e "		\e[91mNNNNNNNN         NNNNNNN"
echo "								"
echo -e "								\e[0m"

if [[ ${#failedArray[@]} == 0 ]]; then
	echo -e "\e[32mSuccessfully complete!\e[0m";
else
	for a in "${failedArray[@]}"; do
		echo -e "Argument \e[32;4m$a\e[0m is \e[31mnot\e[0m valid!"
	done
	echo "All other valid arguments have successfully been completed!"
fi

