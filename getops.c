/*
* getops.c -- the initializing part of getops.mod
*
* $Id: getops.c,v 1.0 2003/09/18 02:27:24 sup Exp $
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
/*
 * Sep 2010: Updated by pseudo to support eggdrop 1.8.x
 */
		    
#define MODULE_NAME "getops"
#define MAKING_GETOPS
#define MODULE_VERSION "1.0.1"
#include "../module.h"
#include "../irc.mod/irc.h"
#include "../server.mod/server.h"
#include "../channels.mod/channels.h"

#undef global
static Function *global = NULL, *irc_funcs = NULL, *server_funcs = NULL, *channels_funcs = NULL;

/* variables that you can set from getops.conf */
static int go_botnum = 3;
/* variables end*/


static struct delay_t *start_delay = NULL;
static struct reqop_t *start_reqop = NULL;

#include "getops.h"
#include "delay.c"
#include "requests.c"
#include "needbinds.c"
#include "bot.c"
#include "misc.c"


static void getops_hook_secondly()
{
   Context;
   check_gdelay();
} 


static int getops_expmem()
{
  int gosize = 0;  

  Context;
  return gosize + gdelay_expmem() + reqop_expmem(); 
}

static void getops_report(int idx, int details)
{
  int size;
  
  Context;
  size = getops_expmem();
  if (details)
  dprintf(idx,"    getops using %d bytes\n", size);
}
	    

static char *getops_close()
{
 del_hook(HOOK_SECONDLY, (Function) getops_hook_secondly);
 rem_builtins(H_need, getops_need);
 rem_builtins(H_bot, getops_bot);
 rem_builtins(H_mode, getops_mode);
 rem_builtins(H_join, getops_join);
 rem_builtins(H_link, getops_link);
 gdelay_free_mem();
 reqop_free_mem();
 module_undepend(MODULE_NAME);
 return NULL;
}

char *getops_start();

static Function getops_table[] =
{
  (Function) getops_start,
  (Function) getops_close,
  (Function) getops_expmem,
  (Function) getops_report,
};

char *getops_start(Function * global_funcs)
{
  global = global_funcs;
  module_register(MODULE_NAME, getops_table, 1, 0);
  if (!module_depend(MODULE_NAME, "eggdrop", 108, 0)) {
    module_undepend(MODULE_NAME);
    return "This module requires eggdrop1.8.0 or later";
  }
  if (!(irc_funcs = module_depend(MODULE_NAME, "irc", 1, 4))) {
      module_undepend(MODULE_NAME);
      return "To use the getops module you need the irc module.";
  }
  if (!(server_funcs = module_depend(MODULE_NAME, "server", 1, 4))) {
      module_undepend(MODULE_NAME);
      return "To use the getops module you need the server module.";
  }
  if (!(channels_funcs = module_depend(MODULE_NAME, "channels", 1, 2))) {
      module_undepend(MODULE_NAME);
      return "To use the getops module you need the channels module.";
  }
  add_hook(HOOK_SECONDLY, (Function) getops_hook_secondly);
  add_builtins(H_need, getops_need);
  add_builtins(H_bot, getops_bot);
  add_builtins(H_mode, getops_mode);
  add_builtins(H_join, getops_join);
  add_builtins(H_link, getops_link);
  return NULL;
}

