/*
*
* misc.c -- miscellaneous functions for getops.mod
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

static int matchattr(struct userrec *u, char *flags, char *chan) {
  struct flag_record plus, minus, user;
  int ok = 0, f;
    
  if (u && (!chan || findchan_by_dname(chan))) {
      user.match = FR_GLOBAL | (chan ? FR_CHAN : 0) | FR_BOT;
      get_user_flagrec(u, &user, chan);
      plus.match = user.match;
      break_down_flags(flags, &plus, &minus);
      f = (minus.global || minus.udef_global || minus.chan ||
      minus.udef_chan || minus.bot);
      if (flagrec_eq(&plus, &user)) {
         if (!f)
             ok = 1;
         else {
         minus.match = plus.match ^ (FR_AND | FR_OR);
           if (!flagrec_eq(&minus, &user))
              ok = 1;
         }
      }
  }
  return ok;
}

static int isop(char *nick, struct chanset_t *chan)
{
  memberlist *m;
  
  if ((m = ismember(chan, nick)) && (chan_hasop(m)))
     return 1;
  else
     return 0;
}

static char *nick_by_handle(char *handle, struct chanset_t *chan)
{
  char nuh[1024];
  memberlist *m;
      
  Context;
  for (m = chan->channel.member; m && m->nick[0]; m = m->next) {
      if (!m->user) {
         egg_snprintf(nuh, sizeof nuh, "%s!%s", m->nick, m->userhost);
         m->user = get_user_by_host(nuh);
      }
      if (m->user && !rfc_casecmp(m->user->handle, handle)) {
         return m->nick;
      }
  }
  return NULL;
}

