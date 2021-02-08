/*
* needbinds.c -- need binds watching part of getops.mod
*/
/*
* Copyright (C) 2003  Brain <brain@fbi.hu>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

static int gnop_need(char *chname, char *type)
{
  int i;
  char needthis[11], data[256];
  struct chanset_t *chan = NULL;
  struct userrec *u;
  tand_t *bot;
  
  if (!egg_strcasecmp(type, "op")) {
     putlog(LOG_MISC, "*", "GetOps.mod: Need op on chan %s", chname);
     for (bot = tandbot; bot; bot = bot->next) {
         // putlog(LOG_MISC, "*", "getops.mod: opkeres %s bottol", bot->bot);
	 u = get_user_by_handle(userlist, botnetnick);
	 chan = findchan_by_dname(chname);
	 if (matchattr(u, "b|-", chan->dname)) {
	    i = nextbot(bot->bot);
	    if (i >= 0) {
	       // putlog(LOG_MISC, "*", "getops.mod: opkeres megy %s botnak", bot->bot);
	       egg_snprintf(data, sizeof data, "gop op %s %s", chan->dname, botname);
	       botnet_send_zapf(i, botnetnick, bot->bot, data);
            }
	 }
     }
  } else {
     egg_snprintf(needthis, sizeof needthis, "%s", type);
     putlog(LOG_MISC, "*", "GetOps.mod: Need %s on chan %s", type, chname);
     gaing_entrance(needthis, chname);
  }
  return 0;
}

static cmd_t getops_need[] =
{
  {"*", "", (Function) gnop_need, "gop_need"},
  {0, 0, 0, 0}
};
