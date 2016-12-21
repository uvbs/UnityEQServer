/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2013 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stdio.h>

#include "../common/eqemu_logsys.h"
#include "../common/global_define.h"
#include "../common/shareddb.h"
#include "../common/eqemu_config.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "../common/rulesys.h"
#include "../common/eqemu_exception.h"
#include "../common/string_util.h"
#include "items.h"
#include "npc_faction.h"
#include "loot.h"
#include "skill_caps.h"
#include "spells.h"
#include "base_data.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

EQEmuLogSys Log;

int main(int argc, char **argv) {
	RegisterExecutablePlatform(ExePlatformSharedMemory);
	Log.LoadLogSettingsDefaults();
	set_exception_handler();

	Log.Out(Logs::General, Logs::Status, "Shared Memory Loader Program");
	if(!EQEmuConfig::LoadConfig()) {
		Log.Out(Logs::General, Logs::Error, "Unable to load configuration file.");
		return 1;
	}

	const EQEmuConfig *config = EQEmuConfig::get();

	SharedDatabase database;
	Log.Out(Logs::General, Logs::Status, "Connecting to database...");
	if(!database.Connect(config->DatabaseHost.c_str(), config->DatabaseUsername.c_str(),
		config->DatabasePassword.c_str(), config->DatabaseDB.c_str(), config->DatabasePort)) {
		Log.Out(Logs::General, Logs::Error, "Unable to connect to the database, cannot continue without a "
			"database connection");
		return 1;
	}

	/* Register Log System and Settings */
	database.LoadLogSettings(Log.log_settings);
	Log.StartFileLogs();

	std::string hotfix_name = "";
	bool load_all = true;
	bool load_items = false;
	bool load_factions = false;
	bool load_loot = false;
	bool load_skill_caps = false;
	bool load_spells = false;
	bool load_bd = false;
	if(argc > 1) {
		for(int i = 1; i < argc; ++i) {
			switch(argv[i][0]) {	
			case 'b':
				if(strcasecmp("base_data", argv[i]) == 0) {
					load_bd = true;
					load_all = false;
				}
				break;
	
			case 'i':
				if(strcasecmp("items", argv[i]) == 0) {
					load_items = true;
					load_all = false;
				}
				break;
	
			case 'f':
				if(strcasecmp("factions", argv[i]) == 0) {
					load_factions = true;
					load_all = false;
				}
				break;
	
			case 'l':
				if(strcasecmp("loot", argv[i]) == 0) {
					load_loot = true;
					load_all = false;
				}
				break;
	
			case 's':
				if(strcasecmp("skill_caps", argv[i]) == 0) {
					load_skill_caps = true;
					load_all = false;
				} else if(strcasecmp("spells", argv[i]) == 0) {
					load_spells = true;
					load_all = false;
				}
				break;
			case '-': {
				auto split = SplitString(argv[i], '=');
				if(split.size() >= 2) {
					auto command = split[0];
					auto argument = split[1];
					if(strcasecmp("-hotfix", command.c_str()) == 0) {
						hotfix_name = argument;
						load_all = true;
					}
				}
				break;
			}
			}
		}
	}

	if(hotfix_name.length() > 0) {
		Log.Out(Logs::General, Logs::Status, "Writing data for hotfix '%s'", hotfix_name.c_str());
	}
	
	if(load_all || load_items) {
		Log.Out(Logs::General, Logs::Status, "Loading items...");
		try {
			LoadItems(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	if(load_all || load_factions) {
		Log.Out(Logs::General, Logs::Status, "Loading factions...");
		try {
			LoadFactions(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	if(load_all || load_loot) {
		Log.Out(Logs::General, Logs::Status, "Loading loot...");
		try {
			LoadLoot(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	if(load_all || load_skill_caps) {
		Log.Out(Logs::General, Logs::Status, "Loading skill caps...");
		try {
			LoadSkillCaps(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	if(load_all || load_spells) {
		Log.Out(Logs::General, Logs::Status, "Loading spells...");
		try {
			LoadSpells(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	if(load_all || load_bd) {
		Log.Out(Logs::General, Logs::Status, "Loading base data...");
		try {
			LoadBaseData(&database, hotfix_name);
		} catch(std::exception &ex) {
			Log.Out(Logs::General, Logs::Error, "%s", ex.what());
			return 1;
		}
	}
	
	Log.CloseFileLogs();
	return 0;
}
