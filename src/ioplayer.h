//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Base class for the Player Loader/Saver
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_IOPLAYER_H__
#define __OTSERV_IOPLAYER_H__

#include <vector>
#include <list>
#include <map>
#include <string>
#include <stdint.h>
#include <boost/algorithm/string/predicate.hpp>
#include "database_driver.h"
#include "const.h"

class Item;
class Player;
class Creature;
struct DeathEntry;

typedef std::vector<DeathEntry> DeathList;

enum UnjustKillPeriod_t{
	UNJUST_KILL_PERIOD_DAY,
	UNJUST_KILL_PERIOD_WEEK,
	UNJUST_KILL_PERIOD_MONTH
};

typedef std::pair<int32_t, Item*> itemBlock;
typedef std::list<itemBlock> ItemBlockList;

/** Class responsible for loading players from database. */
class IOPlayer {
public:
	static IOPlayer* instance();

	/** Load a player
	  * \param player Player structure to load to
	  * \param name Name of the player
	  * \param preload if set to true only group, guid and account id will be loaded, default: false
	  * \return returns true if the player was successfully loaded
	  */
	bool loadPlayer(Player* player, const std::string& name, bool preload = false);

	/** Save a player
	  * \param player the player to save
	  * \return true if the player was successfully saved
	  */
	bool savePlayer(Player* player, bool shallow = false);

	bool addPlayerDeath(Player* dying_player, const DeathList& dl);
	int32_t getPlayerUnjustKillCount(const Player* player, UnjustKillPeriod_t period);
	bool sendMail(Creature* actor, const std::string name, uint32_t depotId, Item* item);

	bool getGuidByName(uint32_t& guid, std::string& player_name);
	bool getGuidByNameEx(uint32_t& guid, bool& specialVip, const std::string& player_name);
	bool getAccountByName(uint32_t& acc, const std::string& name);
	bool getWorldByName(uint32_t& world_id, std::string& name);
	bool getAccountByName(std::string& acc, const std::string& player_name);
	bool getDefaultTown(std::string& name, uint32_t& townId);
	bool getNameByGuid(uint32_t guid, std::string& name);
	bool getGuildIdByName(uint32_t& guildId, const std::string& guildName);
	bool playerExists(std::string name);
	bool getLastIP(uint32_t& ip, uint32_t guid);
	bool hasFlag(PlayerFlags flag, uint32_t guid);
	void updateLoginInfo(Player* player);
	void updateLogoutInfo(Player* player);
	bool isPlayerOnlineByAccount(uint32_t acc);
	bool cleanOnlineInfo();

protected:
	bool storeNameByGuid(DatabaseDriver &mysql, uint32_t guid);

	struct StringCompareCase
	{
		bool operator()(const std::string& l, const std::string& r) const
		{
			return boost::algorithm::ilexicographical_compare(l, r);
		}
	};

	typedef std::map<int,std::pair<Item*,int> > ItemMap;

	void loadItems(ItemMap& itemMap, DBResult* result);
	bool saveItems(Player* player, const ItemBlockList& itemList, DBInsert& query_insert);

	typedef std::map<uint32_t, std::string> NameCacheMap;
	typedef std::map<std::string, uint32_t, StringCompareCase> GuidCacheMap;

	struct UnjustKillBlock{
		uint32_t dayUnjustCount;
		int64_t dayQueryTime;	//the time which was used to query the database
		int64_t dayExpireTime;	//the time when the cached value expires (lowest date that the query returned)

		uint32_t weekUnjustCount;
		int64_t weekQueryTime;
		int64_t weekExpireTime;

		uint32_t monthUnjustCount;
		int64_t monthQueryTime;
		int64_t monthExpireTime;

		UnjustKillBlock()
		{
			dayUnjustCount = 0;
			dayQueryTime = 0;
			dayExpireTime = 0;

			weekUnjustCount = 0;
			weekQueryTime = 0;
			weekExpireTime = 0;

			monthUnjustCount = 0;
			monthQueryTime = 0;
			monthExpireTime = 0;
		}
	};

	typedef std::map<uint32_t, UnjustKillBlock > UnjustCacheMap;

	NameCacheMap nameCacheMap;
	GuidCacheMap guidCacheMap;
	UnjustCacheMap unjustKillCacheMap;
};

#endif
