/*
 * delay.c -- timing part of getops.mod
 *
 * $Id: delay.c,v 1.0 2003/09/18 02:27:24 sup Exp $
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

static struct delay_t *find_gdelay(char *chan, char *micsinal)
{
  struct delay_t *d = NULL;

  for (d = start_delay; d; d = d->next)
    if (!egg_strcasecmp(micsinal, "join")) 
       if ((!rfc_casecmp(d->chan, chan)) && (!egg_strcasecmp(d->todowhat, "join")))
          return d;
    if (!egg_strcasecmp(micsinal, "delete")) 
       if ((!rfc_casecmp(d->chan, chan)) && (!egg_strcasecmp(d->todowhat, "delete")))
          return d;
  return NULL;
}

static struct delay_t *add_gdelay(char *chan, char *micsinal, int i)
{
  struct delay_t *d = NULL;

  d = (struct delay_t *) nmalloc(sizeof(struct delay_t));
  if (!d)
    return NULL;
  d->next = start_delay;
  start_delay = d;
  strncpyz(d->chan, chan, sizeof d->chan);
  d->reqtime = now + i;
  strncpyz(d->todowhat, micsinal, sizeof d->todowhat);
  // putlog(LOG_DEBUG, "*", "botnetop.mod: new delay record created for %s (address: %u)", chan, d);
  return d;
}

static void del_gdelay(struct delay_t *delay)
{
  struct delay_t *d = NULL, *old = NULL;

  for (d = start_delay; d; old = d, d = d->next) {
    if (d == delay) {
      if (old)
        old->next = d->next;
      else
        start_delay = d->next;
      // putlog(LOG_DEBUG, "*", "botnetop.mod: delay record removed from %s (address: %u)", d->chan, d);
      nfree(d);
      break;
    }
  }
}

static void check_gdelay()
{
  struct delay_t *d = NULL, *dnext = NULL;
  struct reqop_t *r = NULL;
  
  // ebbe meg fog kelleni az opkeres csatifuggo valtozojanak az idozitett torlese

  for (d = start_delay; d; d = dnext) {
     dnext = d->next;
     if ((d->reqtime) && (d->reqtime <= now)) {
       // bnop_askbot(d->bot, d->chan);
       if (!egg_strcasecmp(d->todowhat, "join")) {
          // putlog(LOG_MISC, "*", "getops.mod: most joinolok idozitessel");
	  dprintf(DP_SERVER, "JOIN %s\n", d->chan);
          d->reqtime = 0;
          del_gdelay(d);
       }
       if (!egg_strcasecmp(d->todowhat, "delete")) {
          // csativaltozos listabol rekord torles
	  if (!(r = find_req(d->chan))) {
	     r = NULL;
	  } else {
	     del_req(r);
          }
       }
     }
  }
}


static void gdelay_free_mem()
{
  struct delay_t *d = NULL, *dnext = NULL;

  for (d = start_delay; d; d = dnext) {
    dnext = d->next;
    nfree(d);
  }
  start_delay = NULL;
}

static int gdelay_expmem()
{
  int size = 0;
  struct delay_t *d = NULL;

  for (d = start_delay; d; d = d->next)
    size += sizeof(struct delay_t);
  return size;
}
