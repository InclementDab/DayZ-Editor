#!/bin/bash

#!!! BE SURE TO CHANGE THIS WHEN RUNNING TO YOUR DIRECTORY, YOU CANNOT USE P:\\DAYZEXPANSION\\ !!!#
dir="/mnt/e/GitHub-Projects/DayZ/DayZ-Expansion/DayZExpansion/"

name="config.cpp"

currBranch=$(git rev-parse --abbrev-ref HEAD)

if [[ $(cmd.exe /C git rev-parse HEAD) != $(cmd.exe /C git rev-parse origin/$currBranch) ]]; then
	read -p  "Your local version is `echo -e "\e[91mout of date\e[0m"` compared to the $currBranch branch. Did you forget to push/pull?
	`echo -e "\nLocal: $(cmd.exe /C git rev-parse head)\nUpstream: $(cmd.exe /C git rev-parse origin/$currBranch)"`
	Would you like to pull/push now? [Y/n] `echo $'\n'`" -n 1 -r #once again `echo $'\n' is a hacky fix, it works.

	if [[ $REPLY =~ ^[Yy]$ ]]; then
		echo -e "\nWhat would you like to do, push or pull?"
		while [[ true ]]; do
			read ANS

			lcANS=$(echo $ANS | tr '[:upper:]' '[:lower:]')

			if [[ $lcANS == "push" ]]; then
				echo -e "\nPushing to ${currBranch}..."
				cmd.exe /C git push
				break
			elif [[ $lcANS == "pull" ]]; then
				echo -e "\nPulling from ${currBranch}..."
				cmd.exe /C git pull
				break
			else
				echo -e "\nUnknown command, please re-enter"
				continue
			fi
		done
	else
		echo -e "\nContinuing..."
	fi
fi

if [[ ! -z "$(cmd.exe /C git status --porcelain)" ]]; then
	read -p "`echo "$(cmd.exe /C git status --porcelain)"`
	You have `echo -e "\e[91muncommited changes\e[0m"`, would you like to commit/push them now? [Y/n]`echo $'\n'`" -n 1 -r #`echo $'\n'` is a hacky fix for adding in a new line inside of the read function, but it works.

	if [[ $REPLY =~ ^[Yy]$ ]]; then
		echo -e "\nWhat do you want the commit name to be?"
		read commitName

		cmd.exe /C git reset --soft HEAD~1

		echo "Adding files..."
		cmd.exe /C git add .
		echo "Commiting files to ${currBranch}..."
		cmd.exe /C git commit -m "$commitName"
		echo "Performing sanity check..."
		cmd.exe /C git pull
		echo "Pushing files to ${currBranch}..."
		cmd.exe /C git push

		if [[ ! "$(cmd.exe /C git push)" ]]; then
			echo "Successfully pushed to ${currBranch} branch! Continuing..."
		else
			echo -e "\n\n\e[91mThere was an error!\e[0m Please report this to Thurston. Press any key to exit."
			while [[ true ]]; do
				read -t 3 -n 1
				if [[ $? = 0 ]]; then
					exit
				fi
			done
		fi
	else
		echo -e "\nContinuing..."
	fi
fi

if [[ ! -z "$(cmd.exe /C git cherry)" ]]; then
	read -p "`echo "$(cmd.exe /C git cherry -v)"`
	You have `echo -e "\e[91munpushed changes\e[0m"`, would you like to push them now? [Y/n] `echo $'\n'`" -n 1 -r #`echo $'\n'` is a hacky fix for adding in a new line inside of the read function, but it works.

	if [[ $REPLY =~ ^[Yy]$ ]]; then
		echo "Pushing files to ${currBranch}..."
		cmd.exe /C git push

		if [[ ! "$(cmd.exe /C git push --porcelain)" ]]; then
			echo "Successfully pushed to ${currBranch} branch! Continuing..."
		else
			echo -e "\n\n\e[91mThere was an error!\e[0m Please report this to Thurston. Press any key to exit."
			while [[ true ]]; do
				read -t 3 -n 1
				if [[ $? = 0 ]]; then
					exit
				fi
			done
		fi
	else
		echo -e "\nContinuing..."
	fi
fi


declare -a licensedArray=();
declare -a ignoredArray=();

for f in $(find "$dir" -name "$name"); do

	fullDirectory=$(dirname "$f")

	currentDirFile=${fullDirectory#"$dir"}

	pboName=$(echo $currentDirFile | tr "/" "_")

	findIgnore=$(find "$fullDirectory" -type f -name "IGNORE")

	findLicensed=$(find "$fullDirectory" -type f -name "LICENSED")

	if [[ -n $findIgnore ]]; then
		ignoredArray+=( $currentDirFile )
		continue;
	elif [[ -n $findLicensed ]]; then
		licensedData+=( $currentDirFile )
		continue;
	fi

	cmd.exe /C "P:\\DayZExpansion\\Workbench\\Batchfiles\\BinarizePBO.bat" "P:\\DayZExpansion\\$currentDirFile\\config.cpp" "null" 1
done

clear

if [[ ! ${#licensedData[@]} == 0 ]]; then
	echo "There is licensed data!"
	for a in "${licensedData[@]}"; do
		cmd.exe /C "P:\\DayZExpansion\\Workbench\\Batchfiles\\BinarizeLicensedPBO.bat" "P:\\DayZExpansion\\${a}\\config.cpp" null 1
	done
else
	echo "There is no licensed data - This is an error, please report it!"
fi

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

if [[ ! ${#ignoredArray[@]} == 0 ]]; then
	for f in "${ignoredArray[@]}"; do
		echo -e "Ignored Data \e[32;4m$f\e[0m!"
	done
fi