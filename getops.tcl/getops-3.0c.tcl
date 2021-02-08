# Getops 3.0c
# For 1.6.x (maybe others too)

# This script is used for bots to request and give ops to each other. 
# For this to work, you'll need:

# - Bots must be linked in a botnet
# - Every bot that should be ops on your channels must load this script
# - Add all bots you wanna op with this one using the .+bot nick address
#   command. The "nick" should be exactly the botnet-nick of the other bot
# - Add the hostmasks that uniquely identify this bot on IRC
# - Add a global or channel +o flag on all bots to be opped
# - Do exactly the same on all other bots

# -----------------------------------------------------------------------------

# 3.0c by Brain
# - Fixed a bug in op requesting.
# - Fixed key, limit, unban, invite requesting. Now it really works. BUT: it's now done with
#   bind need, so you MUST have 1.6.x or later to see it work.
# - TODO: if someone fixes the requests (see above) for older egg versions, please send me
#   a copy to brain@fbi.hu. Thx.

# 3.0b by Brain
# - Fixed too much CPU time usage bug. Because checkforneedop, too much CPU time was used. Now its
#   more effective and uses less time.
# - Also fixed a little bug in gop_join. (When a bot joined, it was not tested if its linked or not.)
 
# 3.0a by Brain
# - The whole thing was completely rewritten. Now when a bot joins in a channel the others
#   offer him op. With this it became really fast and with go_botnum you can set the number
#   of bots to ask op from. This should be great for bigger botnets against op flood.
# - Modechange watch included. When bots in a channel are opless, and one of them gets op,
#   the others will ask for op immediately from the opped one(s).
# - Also watches '-o' modes... (When the bot gets deopped, it asks for op.)
# - When the bot gets linked, asks other bots for op, on channels it is opless.
# - When a bot gets op on an invite only channel, it automatically sends invite to another 2 bots.
#   (Original conception was, that it invites all other bots - having chan record for that chan -,
#   but it did't worked, the bot invited all bots first, and just after that started the op procedure.
#   By now, it became much much faster... The invited and opped bots also start inviting.. and so on.)
# - TODO: maybe a bit buggy... if you find something wrong be so nice and send me an email to
#   brain@fbi.hu

# 2.6b by Flash
# - Removed all debug stuff

# 2.6 by Flash (Julien Wajsberg <flash@minet.net>)
# - Fixed a little typo
# - Removed some unused code
# - Patched the isvo function to make it work again with 1.6.x
# - TODO : maybe play around the new "bind NEED" command, but it would make
# this script 1.6.x-specific, and I don't think it's worth doing this.

# 2.5 by aerosoul
# - added random delay to give op procedure so channels with big
#   botnets won't be mode flooded on bot join
#   you can turn this off with set go_delay 0

# Previously by The_O, dtM, poptix, DakrDruid, beldin, Ernst, Cron, fantomas.

# -----------------------------------------------------------------------------

# [0/1] do you want GetOps to notice when some unknown (unauthorized) bot
#       sends request to your bot
set go_bot_unknown 0

# [0/1] do you want your bot to request to be unbanned if it becomes banned?
set go_bot_unban 1

# [0/1] do you want GetOps to notice the channel if there are no ops?
set go_cycle 0

# set this to the notice txt for the above (go_cycle)
set go_cycle_msg "Please leave the channel so the bots can get op."

# [0/1] do you want GetOps to have random delay for the give op procedure?
set go_delay 0

# if go_delay is turned on: 
# how many seconds should the bot at least wait before giving ops?
set go_mindelay 5

# How many bots do you want your bot to ask op from?
set go_botnum 3

# -----------------------------------------------------------------------------

timer 1 checkforneedop
set bns ""
proc gain_entrance {what chan} {
 global botnick botname go_bot_unban go_cycle go_cycle_msg bns go_botnum
 set valtozo 0
 switch -exact $what {
  "limit" {
   foreach bs [lobots $chan] {
    putbot $bs "gop limit $chan $botnick"
   }
   set botok [lobots $chan]
   putlog "GetOps: Requested limit raise on $chan from $botok."
  }
  "invite" {
   foreach bs [lobots $chan] {
    putbot $bs "gop invite $chan $botnick"
   }  
   set botok [lobots $chan]
   putlog "GetOps: Requested invite on $chan from $botok."
  }
  "unban" {
   if {$go_bot_unban} {
    foreach bs [lobots $chan] {
     putbot $bs "gop unban $chan $botname"
    }
   set botok [lobots $chan]
   putlog "GetOps: Requested unban on $chan from $botok."
   }
  }
  "key" {
   foreach bs [lobots $chan] {
    putbot $bs "gop key $chan $botnick"
   }
   set botok [lobots $chan]
   putlog "GetOps: Requested key on $chan from $botok."
  }
  "op" {
   if {[hasops $chan]} {
    set bot [getbot $chan]
    if {$bot == ""} {
     set bns ""
     set bot [getbot $chan]
    }
    lappend bns "$bot"
    if {$bot != ""} {
     
     putbot $bot "gop op $chan $botnick"
     putlog "Requesting ops from $bot on $chan.."
    }
   } {
    if {$go_cycle} {
     puthelp "NOTICE $chan :$go_cycle_msg"
    }
   }
  }
 }
}

proc hasops {chan} {
  foreach user [chanlist $chan] {
    if {[isop $user $chan]} {
      return 1
    }
  }
  return 0
}

proc getbot {chan} {
  global bns 
  foreach bn [bots] {
     if {[lsearch $bns $bn] < 0} {
      if {([isvo $bn $chan])} {
        if {([onchan [hand2nick $bn $chan] $chan]) && ([isop [hand2nick $bn $chan] $chan])} {
          return $bn
          break
        }
      }
    }
  }
}

proc botnet_request {bot com args} {
 global botnick go_bot_unban go_bot_unknown go_delay go_mindelay
 set args [lindex $args 0]
 set subcom [lindex $args 0]
 set chan [string tolower [lindex $args 1]]
 set nick [lindex $args 2]
 putloglev d * "botnet_request: bot $bot chan $chan nick $nick com $subcom"
  if {([matchattr $bot b] == 0 || [isvo $bot $chan] == 0) && ($subcom != "takekey" ) } {
   if { $go_bot_unknown == 1} {
   putlog "GetOps: Request for $subcom from $bot - unauthorised bot (IGNORED)"
  }
  return 0
  }
 switch -exact $subcom {
  "op" {
   if {[onchan $nick $chan] == 0} {
    putbot $bot "gop_resp You're not on $chan."
    return 1
   }
   set bothand [finduser $nick![getchanhost $nick $chan]]
   if {$bothand == "*"} {
    putlog "GetOps: $bot requested ops on $chan. Ident not recognized: $nick![getchanhost $nick $chan]."
    putbot $bot "gop_resp I don't know you from this ident: $nick![getchanhost $nick $chan]"
    return 1
   }
   if {[string tolower $bothand] == [string tolower $nick]} {
    putlog "GetOps: $bot requested for op on $chan."
   } {
    putlog "GetOps: $bot requested for op as $nick on $chan."
   }
   if {[iso $nick $chan] && [matchattr $bothand b]} {
    if {[botisop $chan]} {
     if {[isop $nick $chan] == 0} {
	if {$go_delay == 1} {
	 utimer [expr $go_mindelay + [rand 10]] [split "ngop_op $chan $nick $bot"]
	} else {
	 putlog "GetOps: $nick opped on $chan."
	 putbot $bot "gop_resp opping $nick on $chan."
	 pushmode $chan +o $nick
	}	
     }
    } {
     putbot $bot "gop_resp I am not +o on $chan."
    }
   } {
    putbot $bot "gop_resp You're not in my +o list on $chan."
   }
   return 1
  }
  "unban" {
   if {$go_bot_unban} {
    putlog "GetOps: $bot requested for unban on $chan (unbanning)."
    foreach ban [chanbans $chan] {
     if {[string compare $nick $ban]} {
      set bug_1 [lindex $ban 0]
      pushmode $chan -b $bug_1
      putbot $bot "gop_spec ban $chan"
     }
    }
    return 1
   } {
    putlog "GetOps: Refused request to unban $bot ($nick) on $chan."
    putbot $bot "gop_resp I won't unban you on $chan."
   }
  }
  "invite" {
   putlog "GetOps: $bot requested for invite on $chan."
   if {[matchattr $bot b]} {
    putserv "invite $nick $chan"
   }
   return 1
  }
  "limit" {
   putlog "GetOps: $bot requested for limit raise on $chan."
   if {[matchattr $bot b]} {
    pushmode $chan +l [expr [llength [chanlist $chan]] + 1]
   putbot $bot "gop_spec lim $chan"
   }
   return 1
  }
  "key" {
   putlog "GetOps: $bot requested for key for $chan."
   if {[onchan $botnick $chan] == 0} {
    putbot $bot "gop_resp I'm not on $chan."
    return 1
    }
   if {[string match *k* [lindex [getchanmode $chan] 0]]} {
    putbot $bot "gop takekey $chan [lindex [getchanmode $chan] 1]"
   } {
    putbot $bot "gop_resp There's no key needed on $chan!"
   }
   return 1
  }
  "takekey" {
   putlog "GetOps: $bot ($nick) gave me key for $chan."
   foreach channel [string tolower [channels]] {
    if {$chan == $channel} {
     putserv "JOIN $channel $nick"
     return 1
    }
   }
  }
  default {
   putlog "GetOps: ALERT! $bot send fake 'gop' message! ($subcom)"
  }
 }
}

proc gop_resp {bot com msg} {
 putlog "GetOps: MSG from $bot: $msg"
 return 1
}

proc gop_spec {bot com msg} {
 global botnick
 set subcom [lindex [split $msg] 0]
 if {$subcom == "inv"} {
     set chan [lindex [split $msg] 1]
     set fromnick [lindex [split $msg] 2]
     if {[validchan $chan] && ![onchan $botnick $chan]} {
         putbot $bot "gop invite $chan $botnick"
         putlog "GetOps: $bot invited me to $chan. Accepting..."
        }
    }
 if {$subcom == "lim"} {
    set chan [lindex [split $msg] 1]
    putserv "JOIN $chan"
    }
 if {$subcom == "ban"} {
    set chan [lindex [split $msg] 1]
    putserv "JOIN $chan"
    }
 return 1
}

# Returns list of bots in the botnet and +o in my userfile on that channel
proc lobots { channel } {
 set unf ""
 foreach users [userlist b] {
  if {[isvo $users $channel] == 0} { continue }
  foreach bs [bots] {
   if {$users == $bs} {	lappend unf $users }
  }
 }
 return $unf
}

proc isvo {hand chan} {
 if {[matchattr $hand o] || [matchattr $hand |o $chan]} {
  return 1
 } {
  return 0
 }
}

proc iso {nick chan1} {
 return [isvo [nick2hand $nick $chan1] $chan1]
}

proc isinvonly {c} {
    if [string match *i* [lindex [getchanmode $c] 0]] {
     return 1
     } {
     return 0
    }
}

proc do_channels {} {
 global go_chanset
 foreach a [string tolower [channels]] {
  if {[info exist go_chanset($a)] == 0} {
   channel set $a need-op "gain_entrance op $a"
   channel set $a need-key "gain_entrance key $a"
   channel set $a need-invite "gain_entrance invite $a"
   channel set $a need-unban "gain_entrance unban $a"
   channel set $a need-limit "gain_entrance limit $a"
   set go_chanset($a) 1
  }
 }

 if {![string match "*do_channels*" [timers]]} { timer 5 do_channels }
}

proc kill_channels { } {
    global chanset
    
        foreach chan [channels] {
	    if {[info exist chanset($chan)]} {
	    unset chanset($chan)
	    }
	}
	
	foreach i [timers] {
	    if {[lindex $i 1] == "do_channels"} {
	    killtimer [lindex $i 2]
	    }
	}
    do_channels
} 

utimer 2 kill_channels

bind bot - gop botnet_request
bind bot - gop_resp gop_resp
bind bot - gop_spec gop_spec
bind bot - gop_needop gop_needop
bind mode - * gop_modechange
bind link - * gop_linkop
bind join - * gop_join

if {$numversion >= 1060000} {
bind need - * gop_need
}
  
proc gop_need {chan need} {
  global botnick
  switch -exact $need {
  
  "key" {
  gain_entrance key $chan
  }
  
  "limit" {
  gain_entrance limit $chan
  }
  
  "invite" {
  gain_entrance invite $chan
  }

  "op" {
  utimer 3 "checkforneedop"  
  }
  
  "unban" {
  gain_entrance unban $chan
  }
  
  default {
  putlog "GetOps: Something is going wrong.. I shouldn't receive $need as a channel need..."
  }
 
  }
}

proc askoplater {elso masodik harmadik} {
 putbot $elso "gop op $masodik $harmadik"
}


proc gop_modechange {nick uhost hand chan mode opped} {
  global botnick
  set bhand [nick2hand $opped $chan]
  if {$mode == "+o"} {
       if {($opped != $botnick) && ![botisop $chan] && [islinked $bhand]} {
            utimer 1 "askoplater $bhand $chan $botnick"
	  }
     }

  if {$mode == "-o"} {
     set bhand [nick2hand $opped $chan]
     if {($opped != $botnick) && ([botisop $chan] && [islinked $bhand])} {
        putbot $bhand "gop_needop $chan"
     }
  }
  if {[isinvonly $chan] && $mode == "+o" && $opped == $botnick} {
         set invtemp 0
         set invmax 2
         foreach b [bots] {
             if {[matchattr $b o|o $chan] && ![onchan [hand2nick $b $chan] $chan]} {
                 lappend invoffer $b
                 incr invtemp
                 putbot $b "gop_spec inv $chan $botnick"
                 if {$invtemp == $invmax} { break }
                }
         }
         unset invtemp
         unset invmax
         if [info exists invoffer] {
             putlog "GetOps: Got OP on $chan, +i, inviting other bots!"
             putlog "GetOps: Sending invite offer to: [join $invoffer ", "]"
             unset invoffer
         }
  }
}

proc gop_linkop {bot via} {
  global botnick botnet-nick
  if {$bot != ${botnet-nick}} {
     utimer 1 checkforneedop
  }
}

proc gop_needop {bot com chan } {
 global botnick go_botnum dontwantop
 set chan [lindex $chan 0] 
 if ![info exists dontwantop($chan)] {
    set dontwantop($chan) 0
 }
 incr dontwantop($chan)
 if {$dontwantop($chan) > $go_botnum} { 
    utimer 5 "set dontwantop($chan) 0"
    return 0
 }
 putbot $bot "gop op $chan $botnick"
 putloglev d * "GetOps _v3.0_: Opot kertem $bot-tol $chan csatin, mert felajanlotta... Ennyiedik: $dontwantop($chan)"
 if {![string match *checkforneedop* [utimers]]} {
   timer 1 checkforneedop
 }
}

proc gop_join { nick uhost hand chan } {
 global botnick
 set joinnick [nick2hand $nick $chan]
 if {$nick != $botnick && [matchattr $joinnick b] && [islinked $joinnick]} {
      putloglev d * "GetOps _v3.0_: Op felajanlas $nick nek $chan csatin... (JOIN)"
      putbot $joinnick "gop_needop $chan"
 return 0 
 }
 do_channels
 return 0
}

proc checkforneedop {} {
 global botnick
 foreach chan [channels] {
     foreach b [chanlist $chan b] {
                 set bhand [nick2hand $b $chan]
                 if {[matchattr $bhand o|o $chan] && ![isop $b $chan] && ($b!=$botnick) && [islinked $bhand] && [botisop $chan]} {        
                     putloglev d * "GetOps _v3.0_: Opnelkul maradt $bhand, felajanlas $chan csatin"
                     putbot $bhand "gop_needop $chan"
                 }
     }
 }
}

proc ngop_op {chan nick bot} {
 if {![isop $nick $chan] && [botisop $chan]} {
        putlog "GetOps: gave $nick delayed op on $chan."
        putbot $bot "gop_resp opping $nick on $chan."
        pushmode $chan +o $nick
 }
}

set getops_loaded 1

putlog "GetOps v3.0c loaded."
