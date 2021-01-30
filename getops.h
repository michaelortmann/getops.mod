#ifndef _EGG_MOD_GETOPS_GETOPS_H
#define _EGG_MOD_GETOPS_GETOPS_H

#ifdef MAKING_GETOPS

/* delay.c */
struct delay_t {
struct delay_t *next;  /* Next delay record                      */
char chan[81];         /* Channel name                           */
int reqtime;           /* Time to wait before running            */
char todowhat[80];     /* what to do                             */
};
static struct delay_t *find_gdelay(char *, char *);
static struct delay_t *add_gdelay(char *, char *, int);
static void del_gdelay(struct delay_t *);
static void check_gdelay();
static void gdelay_free_mem();
static int gdelay_expmem();

/* misc.c */
static int matchattr(struct userrec *, char *, char *);
static int isop(char *nick, struct chanset_t *chan);
static char *nick_by_handle(char *handle, struct chanset_t *chan);

/* needbinds.c */
static int gnop_need(char *chname, char *type);

/* bot.c */
static void gaing_entrance(char *needthis, char *chname);
static int botnet_grequest(char *bot, char *com, char *par);
static int gop_gspec(char *bot, char *com, char *par);
static int gop_gmodechange(char *nick, char *uhost, char *hand, char *chname, char *mode, char *victim);
static int gop_gneedop(char *bot, char *com, char *par);
static int gop_gjoin(char *nick, char *uhost, char *hand, char *chname);
static int gop_glinkop(char *bot, char *via);
static int gop_gresp(char *bot, char *com, char *par);

/* requests.c */
struct reqop_t {
struct reqop_t *next;        /* Next reqop record               */
char chan[81];               /* Channel name                    */
int countop;                 /* Number of op offers             */
};
static struct reqop_t *find_req(char *chan);
static struct reqop_t *add_req(char *chan);
static void del_req(struct reqop_t *reqop);
static void reqop_free_mem();
static int reqop_expmem();

/* stuff */
#ifndef strncpyz
#define strncpyz(target, source, len) do {      \
        strncpy((target), (source), (len) - 1); \
	        (target)[(len) - 1] = 0;                \
		} while (0)
#endif
		
#endif /* MAKING_GETOPS */
	
#endif /* _EGG_MOD_GETOPS_GETOPS_H */

