/*
* requests.c -- channel variable settings for getops.mod
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

static struct reqop_t *find_req(char *chan)
{
  struct reqop_t *r = NULL;

  for (r = start_reqop; r; r = r->next)
       if (!rfc_casecmp(r->chan, chan))
          return r;
  return NULL;
}

static struct reqop_t *add_req(char *chan)
{
  struct reqop_t *r = NULL;

  r = (struct reqop_t *) nmalloc(sizeof(struct reqop_t));
  if (!r)
    return NULL;
  r->next = start_reqop;
  start_reqop = r;
  strncpyz(r->chan, chan, sizeof r->chan);
  r->countop = 0;
  return r;
}

static void del_req(struct reqop_t *reqop)
{
  struct reqop_t *r = NULL, *old = NULL;

  for (r = start_reqop; r; old = r, r = r->next) {
    if (r == reqop) {
      if (old)
        old->next = r->next;
      else
        start_reqop = r->next;
      nfree(r);
      break;
    }
  }
}

static void reqop_free_mem()
{
  struct reqop_t *r = NULL, *rnext = NULL;

  for (r = start_reqop; r; r = rnext) {
    rnext = r->next;
    nfree(r);
  }
  start_reqop = NULL;
}

static int reqop_expmem()
{
  int size = 0;
  struct reqop_t *r = NULL;

  for (r = start_reqop; r; r = r->next)
    size += sizeof(struct reqop_t);
  return size;
}
