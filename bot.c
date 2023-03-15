/* 
 * bot.c -- the main functions part of getops.mod
 */
/*
 * Copyright (C) 2003 Brain <brain@fbi.hu>
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

static void gaing_entrance(char *needthis, char *chname)
{
  int i;
  int bots = 0;
  char data[256];
  tand_t *bot;
  struct chanset_t *chan = NULL;
  struct userrec *u;

  if ((!(chan = findchan_by_dname(chname))) || (ismember(chan, botname))) {
     return;
  }
  if (!strcasecmp(needthis, "key")) {
     // putlog(LOG_MISC, "*", "--- GETOPS MOD LOG --- Keyt kerek botoktol");
     for (bot = tandbot; bot; bot = bot->next) {
         u = get_user_by_handle(userlist, bot->bot);
	 if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname))) {
	     i = nextbot(bot->bot);
	     if (i >= 0) {
	        snprintf(data, sizeof data, "gop key %s %s", chan->dname, botname);
		botnet_send_zapf(i, botnetnick, bot->bot, data);
	        bots++;
	     }
	 }
     }
     putlog(LOG_MISC, "*", "GetOps.mod: Requested key on chan %s", chan->dname);
  }
  if (!strcasecmp(needthis, "invite")) {
     for (bot = tandbot; bot; bot = bot->next) {
         u = get_user_by_handle(userlist, bot->bot);
	 if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname))) {
	     i = nextbot(bot->bot);
	     if (i >= 0) {
	        snprintf(data, sizeof data, "gop invite %s %s", chan->dname, botname);
		botnet_send_zapf(i, botnetnick, bot->bot, data);
	        bots++;
	     }
	 }
     }
     putlog(LOG_MISC, "*", "GetOps.mod: Requested invite on chan %s", chan->dname);
  }
  if (!strcasecmp(needthis, "unban")) {
     for (bot = tandbot; bot; bot = bot->next) {
         u = get_user_by_handle(userlist, bot->bot);
	 if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname))) {
	     i = nextbot(bot->bot);
	     if (i >= 0) {
	        snprintf(data, sizeof data, "gop unban %s %s %s!%s", chan->dname, botname, botname, botuserhost);
		botnet_send_zapf(i, botnetnick, bot->bot, data);
	        bots++;
	     }
	 }
     }
     putlog(LOG_MISC, "*", "GetOps.mod: Requested unban on chan %s", chan->dname);
  }
  if (!strcasecmp(needthis, "op")) {
     for (bot = tandbot; bot; bot = bot->next) {
         u = get_user_by_handle(userlist, bot->bot);
	 if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname))) {
	     i = nextbot(bot->bot);
	     if (i >= 0) {
	        snprintf(data, sizeof data, "gop op %s %s", chan->dname, botname);
		botnet_send_zapf(i, botnetnick, bot->bot, data);
	        bots++;
	     }
	 }
     }
  }
  if (!strcasecmp(needthis, "limit")) {
     for (bot = tandbot; bot; bot = bot->next) {
         u = get_user_by_handle(userlist, bot->bot);
	 if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname))) {
	     i = nextbot(bot->bot);
	     if (i >= 0) {
	        snprintf(data, sizeof data, "gop limit %s %s", chan->dname, botname);
		botnet_send_zapf(i, botnetnick, bot->bot, data);
	        bots++;
	     }
	 }
     }
     putlog(LOG_MISC, "*", "GetOps.mod: Requested limit raise on chan %s", chan->dname);
  }
}

static int botnet_grequest(char *bot, char *com, char *par)
{
   char *subcom, *chname, *fromnick, *fromhost, data[256];
   int i, bans = 0;
   struct chanset_t *chan = NULL;
   struct userrec *u;
   masklist *b;
   memberlist *m;
   
   subcom = newsplit(&par);
   chname = newsplit(&par);
   fromnick = newsplit(&par);
   fromhost = newsplit(&par);
   if ((!chname[0]) || (!fromnick[0]) || (!(chan = findchan_by_dname(chname)))) {
      return 0;
   }
   if (!strcasecmp(subcom, "takekey")) {
      if ((chname[0]) && (fromnick[0]) && (chan = findchan_by_dname(chname)) && (!ismember(chan, botname))) {
         putlog(LOG_MISC, "*", "GetOps.mod: %s (%s) gave me the key for %s", bot, fromnick, chan->dname);
         dprintf(DP_SERVER, "JOIN %s %s\n", chan->dname, fromnick);
      }
   } else {
      if ((!ismember(chan, botname)) && (!isop(botname, chan))) {
         // putlog(LOG_MISC, "*", "GetOps.mod: %s nem vagyok opos %s csatin", bot, chan->dname);
         return 0;
      }
      if (!strcasecmp(subcom, "op")) {
         // putlog(LOG_MISC, "*", "getops.mod: opot ker %s on %s", bot, chan->dname);
         u = get_user_by_handle(userlist, bot);
         if ((m = ismember(chan, fromnick)) && (isop(botname, chan)) && (!chan_issplit(m)) && (matchattr(u, "b|-", chan->dname)) &&
               (matchattr(u, "o|o", chan->dname)) && (!matchattr(u, "d|d", chan->dname))) {
             if (!isop(fromnick, chan)) {
	        dprintf(DP_MODE, "MODE %s +o %s\n", chan->dname, fromnick);
	        i = nextbot(bot);
		if (i >= 0) {
		   snprintf(data, sizeof data, "gop_resp opping %s on %s", fromnick, chan->dname);
		   botnet_send_zapf(i, botnetnick, bot, data);
		}
		if (strcasecmp(fromnick, bot))
		   putlog(LOG_MISC, "*", "GetOps.mod: Opped %s on chan %s (using nick %s)", bot, chan->dname, fromnick);
	        else 
	           putlog(LOG_MISC, "*", "GetOps.mod: Opped %s on chan %s", bot, chan->dname);
	     }
         }
      } else if (!strcasecmp(subcom, "key")) {
         i = nextbot(bot);
         if (i < 0)
           return 0;
         if (chan->channel.mode & CHANKEY) {
               snprintf(data, sizeof data, "gop takekey %s %s", chan->dname, chan->channel.key);
	       botnet_send_zapf(i, botnetnick, bot, data);
	       putlog(LOG_MISC, "*", "GetOps.mod: Gave key for %s to %s", chan->dname, bot);
         }
      } else if (!strcasecmp(subcom, "invite")) {
            if (chan->channel.mode & CHANINV) {                                       
               if (!fromnick[0])                                                       
	           return 0;                                                             
	       dprintf(DP_SERVER, "INVITE %s %s\n", fromnick, chan->dname);          
	       if (strcasecmp(fromnick, bot))
	          putlog(LOG_MISC, "*", "GetOps.mod: invited %s (using nick %s) to %s", bot, fromnick, chan->dname);
	       else                                                                
	           putlog(LOG_MISC, "*", "GetOps.mod: invited %s to %s", bot, chan->dname);
	                                                                         
            }                                                                         
      } else if (!strcasecmp(subcom, "limit")) {
            if (chan->channel.maxmembers > -1) {                                      
	       add_mode(chan, '+', 'l', int_to_base10(chan->channel.members + 1));     
	       putlog(LOG_MISC, "*", "GetOps.mod: %s requested limit raise on chan %s", bot, chan->dname);
	       i = nextbot(bot);
	       if (i < 0)
	           return 0;
	       snprintf(data, sizeof data, "gop_spec lim %s", chan->dname);
	       botnet_send_zapf(i, botnetnick, bot, data);
	    }
      } else if (!strcasecmp(subcom, "unban")) {
         if (!fromhost[0]) {                                                           
              putlog(LOG_MISC, "*", "GetOps.mod: ERROR: %s requested unban the wrong way (update the getops.tcl in %s)", bot, bot);
	      return 0;
	 }
	 i = nextbot(bot);
	 if (i < 0)
	      return 0;
	 for (b = chan->channel.ban; b->mask[0]; b = b->next) {                       
              if (wild_match(b->mask, fromhost)) {                                       
                 add_mode(chan, '-', 'b', b->mask);                                       
                 bans++;                                                                  
              }                                                                          
         }                                                                            
         if ((bans > 0))                                            
              putlog(LOG_MISC, "*", "GetOps.mod: %s requested unban on %s (unbanning)", bot, chan->dname);
	 snprintf(data, sizeof data, "gop_spec ban %s", chan->dname);
	 botnet_send_zapf(i, botnetnick, bot, data);
         
      }  
   }
   return 0;                                                                     
} 

static int gop_gspec(char *bot, char *com, char *par)
{
   int i;
   char *subcom, *chname, *usnick, data[256];
   struct chanset_t *chan = NULL;

   subcom = newsplit(&par);
   chname = newsplit(&par);
   usnick = newsplit(&par);
   if (!strcasecmp(subcom, "lim")) {
      if ((chan = findchan_by_dname(chname)) && (!ismember(chan, botname))) {
	 if (!find_gdelay(chname))
            add_gdelay(chname, "join", 3);
         // dprintf(DP_SERVER, "JOIN %s\n", chan->dname);
      }
   }
   if (!strcasecmp(subcom, "ban")) {
      if ((chan = findchan_by_dname(chname)) && (!ismember(chan, botname))) {
         if (!find_gdelay(chname))
            add_gdelay(chname, "join", 3);
         // dprintf(DP_SERVER, "JOIN %s\n", chan->dname);
      }
   }
   if (!strcasecmp(subcom, "inv")) {
      if ((chan = findchan_by_dname(chname)) && (!ismember(chan, botname))) {
         i = nextbot(bot);
	 if (i < 0)
	    return 0;
	 putlog(LOG_MISC, "*", "GetOps.mod: %s invited me to %s. Accepting...", usnick, chname);
	 snprintf(data, sizeof data, "gop invite %s %s", chan->dname, botname);
         botnet_send_zapf(i, botnetnick, bot, data);
      }
   }
   // not implemented yet because of compatibility, maybe in future releases
   if (!strcasecmp(subcom, "needop")) {
      if ((chan = findchan_by_dname(chname)) && (ismember(chan, usnick)) && (!isop(usnick, chan)) && (isop(botname, chan))) {
         i = nextbot(bot);
	 if (i < 0)
	    return 0;
	 putlog(LOG_MISC, "*", "GetOps.mod: Offering op to %s on %s", usnick, chname);
	 snprintf(data, sizeof data, "gop_needop %s", chan->dname);
         botnet_send_zapf(i, botnetnick, bot, data);
      }
   }
   return 0;
}

static int gop_gmodechange(char *nick, char *uhost, char *hand, char *chname, char *mode, char *victim)
{
   int invt, invm, i;
   char data[256];
   struct chanset_t *chan = NULL;
   struct userrec *u;
   tand_t *bot;
   memberlist *m;
   
   chan = findchan_by_dname(chname);
   if (chan->channel.mode & CHANINV) {
      // putlog(LOG_MISC, "*", "getops.mod: tenyleg inviteos a csati %s", chname);
      if ((!strcasecmp(mode, "+o")) && (!strcasecmp(botname, victim))) {
         // putlog(LOG_MISC, "*", "getops.mod: opolas volt es en kaptam opot %s", chname);
         invt = 0;
	 invm = 2;
	 for (bot = tandbot; bot; bot = bot->next) {
	     // putlog(LOG_MISC, "*", "getops.mod: nezzuk %s botot", bot->bot);
	     u = get_user_by_handle(userlist, bot->bot);
	     if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname)) && (!ismember(chan, bot->bot))) {
		i = nextbot(bot->bot);
		if (i >= 0) {
		   snprintf(data, sizeof data, "gop_spec inv %s %s", chan->dname, botname);
		   botnet_send_zapf(i, botnetnick, bot->bot, data);
		   invt++;
		}
	     }
	     if (invt >= invm)
	        break;
	 }
         putlog(LOG_MISC, "*", "GetOps.mod: Got op, inviting other bots to %s", chname);
      }
   }
   if (victim && ((!strcasecmp(mode, "+o")) || (!strcasecmp(mode, "-o")))) {
    m = ismember(chan, victim);
    if (m->user) {
      u = get_user_by_handle(userlist, m->user->handle);
      // putlog(LOG_MISC, "*", "getops.mod: eljutva modnezesig, aldozat: %s", victim);
      if ((!strcasecmp(mode, "+o")) && (!isop(botname, chan)) && (strcasecmp(botname, victim)) && (matchattr(u, "b|-", chan->dname))) {
         i = nextbot(m->user->handle);
         // putlog(LOG_MISC, "*", "getops.mod: handle: %s i: %d", m->user->handle, i);
	 if (i >= 0) {
	    snprintf(data, sizeof data, "gop op %s %s", chname, botname);
	    botnet_send_zapf(i, botnetnick, m->user->handle, data);
            // putlog(LOG_MISC, "*", "getops.mod: ezeket kuldom el: botnak: %s cucc: %s", m->user->handle, data);
	 }
      }
      if ((!strcasecmp(mode, "-o")) && (isop(botname, chan)) && (strcasecmp(botname, victim)) && (matchattr(u, "b|-", chan->dname))) {
         i = nextbot(m->user->handle);
         if (i >= 0) {
            snprintf(data, sizeof data, "gop_needop %s", chname);
	    botnet_send_zapf(i, botnetnick, m->user->handle, data);
         }
      }	  
    }
   }
   return 0;
}

// op asking

static int gop_gneedop(char *bot, char *com, char *par)
{
   int i = 0;
   char *chname, data[256];
   struct reqop_t *r = NULL;

   chname = newsplit(&par);
   if (!(r = find_req(chname))) {
      r = add_req(chname);
      r->countop = 1;      
   } else {
      r->countop++;
      if (r->countop > go_botnum) {
         // csativaltozo idozitett torles
	 add_gdelay(chname, "delete", 3);
	 return 0;
      }
   }
   i = nextbot(bot);
   if (i < 0)
      return 0;   
   snprintf(data, sizeof data, "gop op %s %s", chname, botname);
   botnet_send_zapf(i, botnetnick, bot, data);      
   return 0;
} 
 
static int gop_gjoin(char *nick, char *uhost, char *hand, char *chname)
{
   int i;
   char data[256];
   struct chanset_t *chan = NULL;
   struct userrec *u; 

   if ((chan = findchan_by_dname(chname)) && (strcasecmp(nick, botname))) {
       u = get_user_by_handle(userlist, hand);
           if ((matchattr(u, "b|-", chan->dname)) && (matchattr(u, "o|o", chan->dname)) && (!matchattr(u, "d|d", chan->dname))) {
              i = nextbot(hand);
	      if (i < 0)
                 return 0;
	      snprintf(data, sizeof data, "gop_needop %s", chname); 
              botnet_send_zapf(i, botnetnick, hand, data);
	   }
   }
   return 0;
}

static int gop_glinkop(char *bot, char *via)
{
   int i;
   char *gnick, data[256];
   struct chanset_t *chan = NULL;
         
   if (strcasecmp(bot, botnetnick)) {
      for (chan = chanset; chan; chan = chan->next) {
          if ((gnick = nick_by_handle(bot, chan)) && (ismember(chan, gnick)) && (!isop(gnick, chan)) && (isop(botname, chan))) {
              // putlog(LOG_MISC, "*", "getops.mod: ennek kell opot adni: nick: %s csati: %s (%s)", gnick, chan->dname, botname);
              i = nextbot(bot);
	      if (i >= 0) {
	         snprintf(data, sizeof data, "gop_needop %s", chan->dname);
	         botnet_send_zapf(i, botnetnick, bot, data);
	      }
	  }      
      }
   } else {
      for (chan = chanset; chan; chan = chan->next) {
          if ((ismember(chan, botname)) && (!isop(botname, chan)))
             putlog(LOG_MISC, "*", "GetOps.mod: Linked. Requesting ops on %s", chan->dname);
      }
   }
   return 0;
} 

static int gop_gresp(char *bot, char *com, char *par)
{
   putlog(LOG_MISC, "*", "GetOps.mod: MSG from %s: %s", bot, par);
   return 0;
}


/* binds */

static cmd_t getops_bot[] =
{
  {"gop",        "", (IntFunc) botnet_grequest, "botnet_request"},
  {"gop_spec",   "", (IntFunc) gop_gspec,       "gop_spec"},
  {"gop_needop", "", (IntFunc) gop_gneedop,     "gop_needop"},
  {"gop_resp",   "", (IntFunc) gop_gresp,       "gop_resp"},
  {0,            0,  0,                         0}
};

static cmd_t getops_link[] =
{
  {"*", "", (IntFunc) gop_glinkop, "gop_linkop"},
  {0,   0,  0,                     0}
};

static cmd_t getops_mode[] =
{
  {"*", "", (IntFunc) gop_gmodechange, "gop_modechange"},
  {0,   0,  0,                         0}
};

static cmd_t getops_join[] =
{
  {"*", "", (IntFunc) gop_gjoin, "gop_join"},
  {0,   0,  0,                   0}
};

