#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> 

#define BUFFER_SIZE 512

#define VP_RATIO 1.37664
#define DOUBLESTRIKE_BONUS 11
#define REVIVAL_DISCOUNT 5.91

#define Ingrid 'M'
#define Aydae 'G'
#define Logos 'S'
#define Lucius 'O'
#define Bernard 'R'
#define YeHo 'E'
#define Brunhild 'F'
#define Captain 'C'

#define Ingrid_s "Ingrid"
#define Aydae_s "Aydae"
#define Logos_s "Logos"
#define Lucius_s "Lucius"
#define Bernard_s "Bernard"
#define YeHo_s "Ye Ho-Sung"
#define Brunhild_s "Brunhild"
#define Captain_s "Other"

struct troop{
	bool included;
	int tier;
	char ADL;
	char troop_type;
	int type;
	char type_s[13];
	int ADL_val;
	int gold;
	int silver;
	int DC;
	int SvMl;
	int SvR;
	int SvMt;
	int SvF;
	int Health;
	int Strength;
	int quantity;
	int adj_quantity;
};

struct bonus{
	char type[13];
	int Health;
	int Strength;
};

struct total{
	float Health;
	float ranked_Strength;
	float Strength;
};

struct rank{
	float Health;
	float Strength;
	float adj_Health;
	float adj_Strength;
	int Strength_rank;
};

struct sim{
	bool dead;
	bool struck;
	int turns;
	int double_strikes;
	
};

struct captain
{
	char captain;
	char troop_type;
	int level;
	int leadership;
	int dominance;
	int authority;
	bool captain_set;
};

struct troop_selection
{
	int min_guard;
	int max_guard;
	int min_monster;
	int max_monster;
	int min_spec;
	int max_spec;
	int merc_level;
	bool unlocked_all_guards;
	bool unlocked_guard_ranged;
	bool unlocked_guard_melee;
	bool unlocked_guard_mounted;
	bool unlocked_guard_flying;
	bool unlocked_all_specs;
	bool unlocked_spec_ranged;
	bool unlocked_spec_melee;
	bool unlocked_spec_mounted;
	bool unlocked_spec_flying;
	bool guards_set;
	bool specs_set;
	bool monsters_set;
	bool troops_selected;
	bool mercs_selected;
	int merc_tier;
};

struct default_vals
{
	float VP_ratio;
	int double_strike;
	float revival_discount;
};

struct cost
{
	int tot_auth;
	int tot_dom;
	int tot_lead;
	int rtot_auth;
	int rtot_dom; 
	int rtot_lead;
	int tot_silver_d;
	int rtot_silver_d;
	//int tot_rebuild_DC;
	//int rtot_rebuild_DC;
	int tot_rebuild_silv_d;
	int rtot_rebuild_silv_d;
	int tot_silver_l; 
	int rtot_silver_l;
	int tot_rebuild_silv_l; 
	int rtot_rebuild_silv_l;
	float average_gold;
	float average_silver;
	float average_VP;
	int runs;
};

struct default_vals VALS;
struct troop_selection T_SEL;
struct captain CAPTAIN;
struct cost COST;
struct troop *TROOPS;
struct bonus *BONUSES;
struct total *TOTALS;
struct rank *RANKS;
//struct sim *SIMS;

int MAX_RANK=0;
int T_CNT, B_CNT;
int num_troops=0;

FILE *TROOP_BASE, *TROOP_BONUS; 

bool troop_quantities_set=false;

int count_lines(FILE *fp);
void set_captain(struct captain *cap);
void set_troop_types(struct troop_selection *T_SELECT);

void read_in_bonuses(struct bonus *bonuses);
void read_in_troops(struct troop *troops, struct bonus *bonuses);
void set_troop_quantities();
void set_default_vals(struct default_vals *vals);
void show_selections();
void simulate();

char main_menu();

void main(int argc, char *argv[])
{
	int i;
	char quit='Y';
	
	CAPTAIN.captain_set=false;
	
	T_SEL.guards_set=false;
	T_SEL.specs_set=false;
	T_SEL.monsters_set=false;
	T_SEL.troops_selected=false;
	T_SEL.mercs_selected=false;
	
	VALS.VP_ratio=VP_RATIO;
	VALS.double_strike=DOUBLESTRIKE_BONUS;	
	VALS.revival_discount=REVIVAL_DISCOUNT;
	
	if(argc<2)
	{
		printf("usage: doomsday <troopbase.txt> <troopbonus.txt>");
		return;
	}
	TROOP_BASE = fopen(argv[1], "r");
	TROOP_BONUS = fopen(argv[2], "r");
	if(TROOP_BASE == NULL)
	{
		printf("could not open troop base file.\n");
		printf("usage: doomsday <troopbase.txt> <troopbonus.txt>");
		return;
	}
	if(TROOP_BONUS == NULL)
	{
		printf("could not open troop bonus file.\n");
		printf("usage: ./Doomsday <troop_base.txt> <troop_bonus.txt>");
		return;
	}
	T_CNT = count_lines(TROOP_BASE)-1;
	B_CNT = count_lines(TROOP_BONUS)-1;
	printf("Found %d troop types in the troop_base file.\n", T_CNT);
	printf("Found %d troop types in the troop_bonus file. \n", B_CNT);
	rewind(TROOP_BASE);
	rewind(TROOP_BONUS);
	
	if(T_CNT>0 && B_CNT>0)
	{
		struct troop troops[T_CNT];
		struct bonus bonuses[B_CNT];
		struct total totals[T_CNT];
		struct rank ranks[T_CNT];
		TROOPS=troops;
		BONUSES=bonuses;
		TOTALS=totals;
		RANKS=ranks;
		read_in_bonuses(BONUSES);
		read_in_troops(TROOPS, BONUSES);
		
		//for(i=0;i<B_CNT;i++) printf("%s %d %d\n", BONUSES[i].type, BONUSES[i].Health, BONUSES[i].Strength);
		//printf("\n\n");
		//for(i=0;i<T_CNT;i++) printf("%d %s %d %d\n", TROOPS[i].tier, TROOPS[i].type_s, TROOPS[i].Health, TROOPS[i].Strength);
		//printf("\n\n");
		
		for(i=0;i<T_CNT;i++){ TROOPS[i].included=false; TROOPS[i].quantity=0; TROOPS[i].adj_quantity=0; }
		
		while(quit!='Q')
		{
			quit = main_menu();
		}
	}
	fclose(TROOP_BASE);
	fclose(TROOP_BONUS);
}

char main_menu()
{
	char doneOrNot = ' ';
	
	if(CAPTAIN.captain_set) doneOrNot='X';
	else doneOrNot=' ';
	
	int menu_selection, i;
	
	//for(i=0;i<B_CNT;i++) printf("%s %d %d\n", BONUSES[i].type, BONUSES[i].Health, BONUSES[i].Strength);
	//printf("\n\n");
	//for(i=0;i<T_CNT;i++) printf("%d %s %d %d\n", TROOPS[i].tier, TROOPS[i].type_s, TROOPS[i].Health, TROOPS[i].Strength);
	//printf("\n\n");
		
	printf("\n\nMAIN MENU:\n");
	if(CAPTAIN.captain_set) doneOrNot='X';
	else doneOrNot=' ';
	printf("1 - [%c] set captain\n",doneOrNot);
	if(T_SEL.troops_selected) doneOrNot='X';
	else doneOrNot=' ';
	printf("2 - [%c] set troop types\n",doneOrNot);
	if(troop_quantities_set) doneOrNot='X';
	else doneOrNot=' ';
	printf("3 - [%c] set troop quantities\n",doneOrNot);
	printf("4 - change default values\n");
	printf("5 - show selections\n");
	printf("6 - simulate\n");
	printf("0 - exit\n");
	printf("Selection: ");
	fflush(stdout);
	scanf("%d", &menu_selection);
	fflush(stdin);
	switch(menu_selection)
	{
		case 1:
			set_captain(&CAPTAIN);
			printf("\n\nCaptain: ");
			switch(CAPTAIN.captain)
			{
				case Ingrid:
					printf(Ingrid_s);
					break;
				case Aydae:
					printf(Aydae_s);
					break;
				case Logos:
					printf(Logos_s);
					break;
				case Lucius:
					printf(Lucius_s);
					break;
				case Bernard:
					printf(Bernard_s);
					break;
				case YeHo:
					printf(YeHo_s);
					break;
				case Brunhild:
					printf(Brunhild_s);
					break;
				default:
					printf(Captain_s);	
			}
			printf("\nLevel: %d\n", CAPTAIN.level);
			printf("Leadership: %d\n", CAPTAIN.leadership);
			printf("Dominance: %d\n", CAPTAIN.dominance);
			printf("Authority: %d\n", CAPTAIN.authority);
			CAPTAIN.captain_set=true;
			break;
		
		case 2:
			set_troop_types(&T_SEL);
			num_troops=0;
			for(i=0;i<T_CNT;i++)
			{ 
				if(TROOPS[i].included==true) num_troops++; 
				else
				{
					TROOPS[i].quantity=0;
					TROOPS[i].adj_quantity=0;
					
				}
			}
			//for(i=0;i<T_CNT;i++) if(TROOPS[i].included==true)printf("(%c) t%d %s\n",TROOPS[i].ADL, TROOPS[i].tier, TROOPS[i].type_s);
			printf("\n\nSending %d stacks.\n\n", num_troops);
			break;
		case 3:
			if(CAPTAIN.captain_set) set_troop_quantities();
			else printf("You must select your captain first.\n");
			break;
			
		case 4:
			set_default_vals(&VALS);
			break;
			
		case 5: 	
			show_selections();
			break;
		case 6:
			if(troop_quantities_set)simulate();
			else printf("\n\nYou must set troop quantities!\n");
			break;
			
		default:
			printf("goodbye.");
			return 'Q';
	}
	return 'Y';
}   
	
void set_captain(struct captain *cap)
{
	int CAP;
	troop_quantities_set=false;
	printf("\n\nCaptain?\n1-Aydae\n2-Logos\n3-Ingrid\n4-Lucius\n5-Bernard\n6-Ye Ho Sung\n7-Brunhild\n0-Other\n\nSelection: ");
	fflush(stdout);
	scanf("%d",&CAP);
	fflush(stdin);
		switch(CAP)
		{
			case 0:
				cap->captain=Captain;
				cap->troop_type=Captain;
				break;
			case 1:
				cap->captain=Aydae;
				cap->troop_type=Captain;
				break;
			case 2:
				cap->captain=Logos;
				cap->troop_type=Captain;
				break;
			case 3:
				cap->captain=Ingrid;
				cap->troop_type=Captain;
				break;
			case 4:
				cap->captain=Captain;
				cap->troop_type=Lucius;
				break;
			case 5:
				cap->captain=Captain;
				cap->troop_type=Bernard;
				break;
			case 6:
				cap->captain=Captain;
				cap->troop_type=YeHo;
				break;
			case 7:
				cap->captain=Captain;
				cap->troop_type=Brunhild;
				break;
			default:
				printf("Invalid option. Default selected.\n");
				cap->captain=Captain;
				cap->troop_type=Captain;
		}
		
	printf("Level of captain? ");
	fflush(stdout);
	scanf("%d",&cap->level);
	fflush(stdin);
	printf("Leadership available? ");
	fflush(stdout);
	scanf("%d",&cap->leadership);
	fflush(stdin);
	printf("Dominance available? ");
	fflush(stdout);
	scanf("%d",&cap->dominance);
	fflush(stdin);
	printf("Authority available? ");
	fflush(stdout);
	scanf("%d",&cap->authority);
	fflush(stdin);
	cap->captain_set=true;
}

void set_troop_types(struct troop_selection *T_SELECT)
{
	int selection=1, i;
	char YorN;
	char doneOrNot=' ';
	
	while(selection>=1 && selection<=4)
	{
		printf("\n\nEnter:\n");
		if(T_SELECT->guards_set) doneOrNot='X';
		else doneOrNot=' ';
		printf("1 - [%c] Set guards\n",doneOrNot);
		if(T_SELECT->specs_set) doneOrNot='X';
		else doneOrNot=' ';
		printf("2 - [%c] Set specialists\n",doneOrNot);
		if(T_SELECT->monsters_set) doneOrNot='X';
		else doneOrNot=' ';
		printf("3 - [%c] Set monsters\n",doneOrNot);
		if(T_SELECT->mercs_selected) doneOrNot='X';
		else doneOrNot=' ';
		printf("4 - [%c] Set mercs\n",doneOrNot);
		printf("0 - return to main menu\n");
		printf("Selection: ");
		fflush(stdout);
		scanf("%d", &selection);
		fflush(stdin);
		switch(selection)
		{
			case 1:
				troop_quantities_set=false;
				T_SELECT->unlocked_all_guards=false;
				T_SELECT->unlocked_guard_ranged=false; 
				T_SELECT->unlocked_guard_melee=false;
				T_SELECT->unlocked_guard_mounted=false;
				T_SELECT->unlocked_guard_flying=false;
				printf("\n\nminimum guards to send? (1-9, 0 for none) ");
				fflush(stdout);
				scanf("%d",&T_SELECT->min_guard);
				fflush(stdin);
				
				if(T_SELECT->min_guard>=1 && T_SELECT->min_guard<=9)
				{
					printf("Highest guards unlocked? (1-9) ");
					fflush(stdout);
					scanf("%d",&T_SELECT->max_guard);
					fflush(stdin);
					printf("All t%d guards unlocked? (Y or N) ",T_SELECT->max_guard);
					fflush(stdout);
					scanf("%c",&YorN);
					fflush(stdin);
					if(YorN=='Y' || YorN=='y')
					{ 
						T_SELECT->unlocked_all_guards=true;
						T_SELECT->unlocked_guard_ranged=true; 
						T_SELECT->unlocked_guard_melee=true;
						T_SELECT->unlocked_guard_mounted=true;
						T_SELECT->unlocked_guard_flying=true;
					}
					else
					{
						printf("Unlocked t%d ranged? (Y or N) ",T_SELECT->max_guard);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_guard_ranged=true; 
						else T_SELECT->unlocked_guard_ranged=false; 
						
						printf("Unlocked t%d melee? (Y or N) ",T_SELECT->max_guard);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_guard_melee=true; 
						else T_SELECT->unlocked_guard_melee=false; 
						
						printf("Unlocked t%d mounted? (Y or N) ",T_SELECT->max_guard);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_guard_mounted=true; 
						else T_SELECT->unlocked_guard_mounted=false; 
						if(T_SELECT->max_guard>=5)
						{
							printf("Unlocked t%d flying? (Y or N) ",T_SELECT->max_guard);
							fflush(stdout);
							scanf("%c",&YorN);
							fflush(stdin);
							if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_guard_flying=true; 
							else T_SELECT->unlocked_guard_flying=false; 
						}
						else T_SELECT->unlocked_guard_flying=false; 
					}
				}
				else T_SELECT->max_guard=0;
				T_SELECT->guards_set=true;
				break;
			case 2:
				troop_quantities_set=false;
				T_SELECT->unlocked_all_specs=false;
				T_SELECT->unlocked_spec_ranged=false; 
				T_SELECT->unlocked_spec_melee=false;
				T_SELECT->unlocked_spec_mounted=false;
				T_SELECT->unlocked_spec_flying=false;
				printf("\n\nminimum specialists to send? (1-9, 0 for none) ");
				fflush(stdout);
				scanf("%d",&T_SELECT->min_spec);
				fflush(stdin);
				if(T_SELECT->min_spec>=1 && T_SELECT->min_spec<=9)
				{
					printf("Highest specialists unlocked? (1-9) ");
					fflush(stdout);
					scanf("%d",&T_SELECT->max_spec);
					fflush(stdin);
					printf("All t%d specialists unlocked? (Y or N) ",T_SELECT->max_spec);
					fflush(stdout);
					scanf("%c",&YorN);
					fflush(stdin);
					if(YorN=='Y' || YorN=='y')
					{ 
						T_SELECT->unlocked_all_specs=true;
						T_SELECT->unlocked_spec_ranged=true; 
						T_SELECT->unlocked_spec_melee=true;
						T_SELECT->unlocked_spec_mounted=true;
						T_SELECT->unlocked_spec_flying=true;
					}
					else
					{
						printf("Unlocked t%d ranged? (Y or N) ",T_SELECT->max_spec);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_spec_ranged=true; 
						else T_SELECT->unlocked_spec_ranged=false; 
						
						printf("Unlocked t%d melee? (Y or N) ",T_SELECT->max_spec);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_spec_melee=true; 
						else T_SELECT->unlocked_spec_melee=false; 
						
						printf("Unlocked t%d mounted? (Y or N) ",T_SELECT->max_spec);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_spec_mounted=true; 
						else T_SELECT->unlocked_spec_mounted=false; 
						if(T_SELECT->max_spec>=5)
						{
							printf("Unlocked t%d flying? (Y or N) ",T_SELECT->max_spec);
							fflush(stdout);
							scanf("%c",&YorN);
							fflush(stdin);
							if(YorN=='Y' || YorN=='y') T_SELECT->unlocked_spec_flying=true; 
							else T_SELECT->unlocked_spec_flying=false; 
						}
						else T_SELECT->unlocked_spec_flying=false; 
					}
				}
				else T_SELECT->max_spec=0;
				T_SELECT->specs_set=true;
				break;
			case 3:
				troop_quantities_set=false;
				for(i=0;i<T_CNT;i++) if( TROOPS[i].ADL=='D' ) TROOPS[i].included=false;
				printf("\n\nminimum monsters to send? (1-9, 0 for none) ");
				fflush(stdout);
				scanf("%d",&T_SELECT->min_monster);
				fflush(stdin);
				if(T_SELECT->min_monster>=1 && T_SELECT->min_monster<=9)
				{
					printf("Highest monsters unlocked? (1-9) ");
					fflush(stdout);
					scanf("%d",&T_SELECT->max_monster);
					fflush(stdin);
				}
				else T_SELECT->max_monster=0;
				T_SELECT->monsters_set=true;
				
				break;

			case 4: 
				troop_quantities_set=false;
				for(i=0;i<T_CNT;i++) if( TROOPS[i].ADL=='A' ) TROOPS[i].included=false;
				printf("\n\ntier of mercs? (5-7,9, 0 for none) ");
				fflush(stdout);
				scanf("%d",&T_SELECT->merc_tier);
				fflush(stdin);
				for(i=0;i<T_CNT;i++)
				{
					if( TROOPS[i].ADL=='A' && TROOPS[i].tier==T_SELECT->merc_tier && TROOPS[i].type_s[0]=='G')
					{
						printf("t%d %s merc included? (Enter Y or N) ", TROOPS[i].tier, TROOPS[i].type_s);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') TROOPS[i].included=true;
						else TROOPS[i].included=false;
					}
					
				}
				for(i=0;i<T_CNT;i++)
				{
					if( TROOPS[i].ADL=='A' && TROOPS[i].tier==T_SELECT->merc_tier && TROOPS[i].type_s[0]=='S')
					{
						printf("t%d %s merc included? (Enter Y or N) ", TROOPS[i].tier, TROOPS[i].type_s);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') TROOPS[i].included=true;
						else TROOPS[i].included=false;
					}
					
				}
				for(i=0;i<T_CNT;i++)
				{
					if( TROOPS[i].ADL=='A' && TROOPS[i].tier==T_SELECT->merc_tier && TROOPS[i].type_s[0]=='M')
					{
						printf("t%d %s merc included? (Enter Y or N) ", TROOPS[i].tier, TROOPS[i].type_s);
						fflush(stdout);
						scanf("%c",&YorN);
						fflush(stdin);
						if(YorN=='Y' || YorN=='y') TROOPS[i].included=true;
						else TROOPS[i].included=false;
					}
					
				}
				T_SELECT->mercs_selected=true;
			default:
				break;
		}
		
	}
	if(T_SELECT->guards_set && T_SELECT->specs_set && T_SELECT->monsters_set) 
	   {
		   
		   for(i=0;i<T_CNT;i++)
		   {
			   switch(TROOPS[i].ADL)
			   {
				   case 'D':
						if(TROOPS[i].tier<=T_SELECT->max_monster && TROOPS[i].tier>=T_SELECT->min_monster) TROOPS[i].included=true;
						else TROOPS[i].included=false;
						break;
				   break;
				   case 'L':
						switch(TROOPS[i].type_s[0])
						{
							case 'G':
								if(TROOPS[i].tier<=T_SELECT->max_guard && TROOPS[i].tier>=T_SELECT->min_guard)
								{
									if(TROOPS[i].tier==T_SELECT->max_guard && T_SELECT->unlocked_all_guards) TROOPS[i].included=true;
									else if(TROOPS[i].tier==T_SELECT->max_guard)
									{
										if(TROOPS[i].troop_type=='R' && T_SELECT->unlocked_guard_ranged) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='E' && T_SELECT->unlocked_guard_melee) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='O' && T_SELECT->unlocked_guard_mounted) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='F' && T_SELECT->unlocked_guard_flying) TROOPS[i].included=true;
									}
									else TROOPS[i].included=true;
								}
								else TROOPS[i].included=false;
								break;
							case 'S':
								if(TROOPS[i].tier<=T_SELECT->max_spec && TROOPS[i].tier>=T_SELECT->min_spec)
								{
									if(TROOPS[i].tier==T_SELECT->max_spec && T_SELECT->unlocked_all_specs) TROOPS[i].included=true;
									else if(TROOPS[i].tier==T_SELECT->max_spec)
									{
										if(TROOPS[i].troop_type=='R' && T_SELECT->unlocked_spec_ranged) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='E' && T_SELECT->unlocked_spec_melee) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='O' && T_SELECT->unlocked_spec_mounted) TROOPS[i].included=true;
										else if(TROOPS[i].troop_type=='F' && T_SELECT->unlocked_spec_flying) TROOPS[i].included=true;
									}
									else TROOPS[i].included=true;
								}
								else TROOPS[i].included=false;
						}
				   break;
			   }
		   }
		   
		   
	   }
	 if(T_SELECT->guards_set && T_SELECT->specs_set && T_SELECT->monsters_set && T_SELECT->mercs_selected) T_SELECT->troops_selected=true;
}

void set_default_vals(struct default_vals *vals)
{
	int selection=1, i;
	char YorN;
	char doneOrNot=' ';
	
	while(selection>=1 && selection<=3)
	{
		printf("\n\nEnter:\n");
		printf("1 - Set VP ratio [%0.3f]\n",VALS.VP_ratio);
		printf("2 - Set Doublestrike bonus [%d]\n",VALS.double_strike);
		printf("3 - Set Revival discount [%0.2f]\n", VALS.revival_discount);
		printf("0 - return to main menu\n");
		printf("Selection: ");
		fflush(stdout);
		scanf("%d", &selection);
		fflush(stdin);
		switch(selection)
		{
			case 1:
				printf("Enter new value: ");
				fflush(stdout);
				scanf("%f", &VALS.VP_ratio);
				fflush(stdin);
			break;
			case 2:
				printf("Enter new value: ");
				fflush(stdout);
				scanf("%d", &VALS.double_strike);
				fflush(stdin);
			break;
			case 3:
				printf("Enter new value: ");
				fflush(stdout);
				scanf("%d", &VALS.revival_discount);
				fflush(stdin);
			break;
				
		}
	}
}

int rank_troops(int num_stacks)
{
	int cap=0;
	int i, j, type, tier, str_bonuses;
	double str_bonus, hth_bonus, base_health, base_strength;
	int strongest;
	int max_rank=0;
	
	for(i=0; i<T_CNT; i++)
	{
		if(TROOPS[i].included)
		{
			str_bonuses=0;
			str_bonus=0.0;
			//printf("herea");
			//fflush(stdout);
			type = TROOPS[i].type;
			//printf("hereb");
			//fflush(stdout);
			base_health = (float)TROOPS[i].Health;
			//printf("%d[%d]",type, i);
			//fflush(stdout);
			hth_bonus = (float)BONUSES[type].Health/100.0;
			if(TROOPS[i].type_s[0]==CAPTAIN.captain) hth_bonus = hth_bonus+(float)(CAPTAIN.level)/100.0;
			else if(TROOPS[i].troop_type==CAPTAIN.troop_type) hth_bonus = hth_bonus+(float)(CAPTAIN.level)/100.0;
			//printf("hered");
			//fflush(stdout);
			TOTALS[i].Health = base_health*(1.0 + hth_bonus);
			//printf("%f, %f, %d\n",base_health, hth_bonus, ADL);
			//fflush(stdout);
			if(TROOPS[i].SvMl > 0)
			{
				//printf("here1");
				//fflush(stdout);
				str_bonuses++;
				str_bonus+=TROOPS[i].SvMl;
			}
			if(TROOPS[i].SvR > 0 && num_stacks>2)
			{
				//printf("here11");
				//fflush(stdout);
				str_bonuses++;
				str_bonus+=TROOPS[i].SvR;
			}
			if(TROOPS[i].SvMt > 0 && num_stacks>3)
			{
				//printf("here111");
				//fflush(stdout);
				str_bonuses++;
				str_bonus+=TROOPS[i].SvMt;
			}
			if(TROOPS[i].SvF > 0)
			{
				//printf("here2");
				//fflush(stdout);
				str_bonuses++;
				str_bonus+=TROOPS[i].SvF;
			}
			
			//printf("here3");
			//fflush(stdout);
			base_strength=(float)TROOPS[i].Strength;
			//printf("here4");
			//fflush(stdout);
			if(str_bonuses>0) str_bonus=((str_bonus/str_bonuses)+((float)BONUSES[type].Strength))/100.0;
			else str_bonus=(float)BONUSES[type].Strength/100.0;
			if(TROOPS[i].type_s[0]==CAPTAIN.captain) str_bonus=str_bonus+(float)CAPTAIN.level/100.0;
			else if(TROOPS[i].troop_type==CAPTAIN.troop_type) str_bonus = str_bonus+(float)(CAPTAIN.level)/100.0;
			TOTALS[i].ranked_Strength = base_strength * (1+str_bonus)/TROOPS[i].ADL_val;
			TOTALS[i].Strength=base_strength*(1+(float)BONUSES[type].Strength/100.0);
			//printf("here5");
			//fflush(stdout);
			//printf("Type: t%d %s, Health/ADL: %f, Strength/ADL: %f\n", TROOPS[i].tier, TROOPS[i].type_s, TOTALS[i].Health, TOTALS[i].Strength);
			//fflush(stdout);
		}
	}		
	
	for(i=0;i<T_CNT;i++)
	{
		if(TROOPS[i].included)
		{
			tier = TROOPS[i].tier;
			RANKS[i].Strength_rank=1;
			for(j=0;j<T_CNT;j++)
			{
				//printf("%d %f %d %f\n", i, TOTALS[i].ranked_Strength, j, TOTALS[j].ranked_Strength);
				 if(TOTALS[j].ranked_Strength > TOTALS[i].ranked_Strength && TROOPS[j].included)
				 {
					 //printf("added");
					 RANKS[i].Strength_rank=RANKS[i].Strength_rank+1;
					 //printf("%d",RANKS[i].Strength_rank);
				 }
			}
			if (RANKS[i].Strength_rank==1) strongest=i;
			if (RANKS[i].Strength_rank>max_rank) max_rank=RANKS[i].Strength_rank;
			//printf("t%d %s: Strength/L %f, rank %d\n",tier, TROOPS[i].type_s, TOTALS[i].Strength, RANKS[i].Strength_rank);
		}
	}
	MAX_RANK=max_rank;
	return strongest;
}

void calculate_costs()
{
	int j;
	
	COST.tot_auth=0; 
	COST.tot_dom=0; 
	COST.tot_lead=0;
	COST.tot_silver_d=0;
	COST.tot_rebuild_silv_d=0; 
	COST.tot_silver_l=0; 
	COST.tot_rebuild_silv_l=0; 
	
	for(j=0;j<T_CNT;j++)
	{
		if(TROOPS[j].ADL=='A') COST.tot_auth+=TROOPS[j].quantity*TROOPS[j].ADL_val;
		else if(TROOPS[j].ADL=='D'){
			COST.tot_dom+=TROOPS[j].quantity*TROOPS[j].ADL_val;
			COST.tot_rebuild_silv_d+=(int)((float)TROOPS[j].quantity*TROOPS[j].silver*0.1);
			COST.tot_silver_d+=TROOPS[j].quantity*TROOPS[j].silver;
		}
		else 
		{
			COST.tot_lead+=TROOPS[j].quantity*TROOPS[j].ADL_val;
			COST.tot_rebuild_silv_l+=(int)((float)TROOPS[j].quantity*TROOPS[j].silver*0.1);
			COST.tot_silver_l+=TROOPS[j].quantity*TROOPS[j].silver;
		}
	}
	//COST.tot_rebuild_DC=(int)((float)COST.tot_dom*40*0.1);
}

void print_cost()
{
	printf("\nTotal Authority: %d, Total Dominance: %d, Total Leadership: %d\n", COST.tot_auth, COST.tot_dom, COST.tot_lead);
	printf("Gold revive: mercs %d, monsters %d, regular troops %d\n", (int)((float)COST.tot_auth*8/VALS.revival_discount), (int)((float)COST.tot_dom*16/VALS.revival_discount), (int)((float)COST.tot_lead*4/VALS.revival_discount));
	printf("Silver to build: monsters %d, regular troops %d\n", COST.tot_silver_d, COST.tot_silver_l);
	printf("Silver to rebuild: monsters %d, regular troops %d\n", COST.tot_rebuild_silv_d, COST.tot_rebuild_silv_l);
	printf("Dragon coins: build %d, rebuild %d\n", COST.tot_dom*40, (int)((float)COST.tot_dom*40.0*0.1));
	printf("Revive mercs and monsters, rebuild the rest:\n%d gold + %d silver\n\n", (int)((float)COST.tot_auth*8/VALS.revival_discount)+(int)((float)COST.tot_dom*16/VALS.revival_discount), COST.tot_rebuild_silv_d+COST.tot_silver_l);
}

void calculate_adjusted_quantity(float ratio, char add_strength, int num_str_turns, int num_stacks)
{
	int i;
	int current_rank=MAX_RANK;
	float current_health;
	float current_strength;
	
	for(i=0;i<T_CNT;i++)
	{
		TROOPS[i].adj_quantity=(int)((float)TROOPS[i].quantity*ratio);
		if(RANKS[i].Strength_rank==MAX_RANK)
		{
			current_health=TROOPS[i].adj_quantity*TOTALS[i].Health+100;
			current_strength=TROOPS[i].adj_quantity*TOTALS[i].Strength+100;
		}
	}
	
	while(current_rank > 0)
	{
		for(i=0;i<T_CNT;i++)
		{
			if(RANKS[i].Strength_rank == current_rank)
			{
				RANKS[i].adj_Health = TROOPS[i].adj_quantity*TOTALS[i].Health;
				RANKS[i].adj_Strength = TROOPS[i].adj_quantity*TOTALS[i].Strength;
				//printf("adjusting down");
				//fflush(stdout);
				while(RANKS[i].adj_Health >= current_health) 
				{
					
					TROOPS[i].adj_quantity-=1;
					RANKS[i].adj_Health = TROOPS[i].adj_quantity*TOTALS[i].Health;
					RANKS[i].adj_Strength = TROOPS[i].adj_quantity*TOTALS[i].Strength;
					
					if( (add_strength=='y' || add_strength=='Y') && RANKS[i].Strength_rank>MAX_RANK-num_stacks*num_str_turns && RANKS[i].adj_Strength >= current_strength)
					{
						printf("Adjusting down: t%d %s\n", TROOPS[i].tier, TROOPS[i].type_s);
						while(RANKS[i].adj_Strength >= current_strength)
						{
							TROOPS[i].adj_quantity-=1;
							RANKS[i].adj_Health=TROOPS[i].adj_quantity*TOTALS[i].Health;
							RANKS[i].adj_Strength=TROOPS[i].adj_quantity*TOTALS[i].Strength;
						}
					}
					
					//printf("%d %d %d %s %f %f %f \n",i, current_rank,TROOPS[i].quantity, RANKS[i].Health, TOTALS[i].Health, current_health);
					//printf("%d %d %d %s %f %f %f \n\n",i, current_rank, TROOPS[i].quantity, RANKS[i].Strength, TOTALS[i].Strength, current_strength);
					//fflush(stdout);
					//getchar();
					//fflush(stdin);
				}
				//printf("%d %d %d %f %f %d %f \n",j, current_rank, TROOPS[j].quantity RANKS[j]->Health, TOTALS[j]->Health, increment, current_health);
				//printf("%d %d %d %f %f %d %f \n\n",j, current_rank, TROOPS[j].quantity, RANKS[j]->Strength, TOTALS[j]->Strength, increment, current_strength);
				//fflush(stdout);
				//getchar();
				//fflush(stdin);
				//temp_current_health = ranks[j].Health;
				//temp_current_strength = ranks[j].Strength;
				current_health = RANKS[i].adj_Health;
				current_strength = RANKS[i].adj_Strength;
				
			}
		}
		current_rank--;
	}
	
}

void calculate_adjusted_costs()
{
	COST.rtot_auth=0; 
	COST.rtot_dom=0; 
	COST.rtot_lead=0;
	COST.rtot_silver_d=0;
	COST.rtot_rebuild_silv_d=0;
	COST.rtot_silver_l=0;
	COST.rtot_rebuild_silv_l=0;
	
	int i;
	for(i=0;i<T_CNT;i++)
	{
		if(TROOPS[i].ADL=='A') COST.rtot_auth+=TROOPS[i].adj_quantity*TROOPS[i].ADL_val;
		else if(TROOPS[i].ADL=='D')
		{
			COST.rtot_dom+=TROOPS[i].adj_quantity*TROOPS[i].ADL_val;
			COST.rtot_rebuild_silv_d+=(int)((float)TROOPS[i].adj_quantity*TROOPS[i].silver*0.1);
			COST.rtot_silver_d+=(int)((float)TROOPS[i].adj_quantity*TROOPS[i].silver);
		}
		else
		{
			COST.rtot_lead+=TROOPS[i].adj_quantity*TROOPS[i].ADL_val;
			COST.rtot_rebuild_silv_l+=(int)((float)TROOPS[i].adj_quantity*TROOPS[i].silver*0.1);
			COST.rtot_silver_l+=(int)((float)TROOPS[i].adj_quantity*TROOPS[i].silver);
		}
	}
	//COST.rtot_rebuild_DC=(int)((float)COST.rtot_dom*40*0.1);
}

void print_adj_cost()
{
	printf("Adjusted:\n");
	printf("Total Authority: %d, Total Dominance: %d, Total Leadership: %d\n", COST.rtot_auth, COST.rtot_dom, COST.rtot_lead);
	printf("Gold revive: mercs %d, monsters %d, regular troops %d\n", (int)((float)COST.rtot_auth*8/VALS.revival_discount), (int)((float)COST.rtot_dom*16/VALS.revival_discount), (int)((float)COST.rtot_lead*4/VALS.revival_discount));
	printf("Silver to build: monsters %d, regular troops %d\n", COST.rtot_silver_d, COST.rtot_silver_l);
	printf("Silver to rebuild: monsters %d, regular troops %d\n", COST.rtot_rebuild_silv_d, COST.rtot_rebuild_silv_l);
	printf("Dragon coins: build %d, rebuild %d\n", COST.rtot_dom*40, (int)((float)COST.rtot_dom*40.0*0.1));
	printf("Revive mercs and monsters, rebuild the rest:\n%d gold + %d silver\n\n", (int)((float)COST.rtot_auth*8/VALS.revival_discount)+(int)((float)COST.rtot_dom*16/VALS.revival_discount), COST.rtot_rebuild_silv_d+COST.rtot_silver_l);
		
}

void print_troops()
{
	int i;
	printf("\n\n");
	for(i=0;i<T_CNT;i++)
	{
		if(TROOPS[i].included) printf("(%c) rank: %2d/%d t%d %*s: %*d\t adjusted: %*d\n",TROOPS[i].ADL, RANKS[i].Strength_rank, MAX_RANK, TROOPS[i].tier, -12, TROOPS[i].type_s, -5, TROOPS[i].quantity, -4, TROOPS[i].adj_quantity);
	}
}

void print_adj_troops()
{
	int i;
	printf("\n\n");
	for(i=0;i<T_CNT;i++)
	{
		if(TROOPS[i].included) printf("(%c) rank: %2d/%d t%d %*s: %*d\n", TROOPS[i].ADL, RANKS[i].Strength_rank, MAX_RANK, TROOPS[i].tier, -12, TROOPS[i].type_s, -5, TROOPS[i].adj_quantity);
	}
}


float calculate_ratio()
{
	
	float ratio=0.0, ratio_d, ratio_a, ratio_l;
	if(COST.tot_dom==0) ratio_d=100000000.0;
	else ratio_d=(float)CAPTAIN.dominance/COST.tot_dom;
	if(COST.tot_auth==0) ratio_a=100000000.0;
	else ratio_a=(float)CAPTAIN.authority/COST.tot_auth;
	if(COST.tot_lead==0) ratio_l=100000000.0;
	else ratio_l=(float)CAPTAIN.leadership/COST.tot_lead;
	
	//printf("ratio_l %f, ratio_d %f, ratio_a %f\n", ratio_l, ratio_d, ratio_a);
	
	ratio=ratio_l;
	if(ratio_d < ratio) ratio=ratio_d;
	if(ratio_a < ratio) ratio=ratio_a;
	return ratio;
}

void set_troop_quantities()
{
	
	int cap=0;
	int i, j, type, tier, str_bonuses, ADL;
	int num_stacks, selection=1, num_str_turns;
	int strongest, starting, current_rank, max_rank=MAX_RANK;
	
	char add_strength;
	int increment;
	
	double current_health, temp_current_health;
	double current_strength, temp_current_strength;
	float ratio;
	
	char adjust, adjust_q;
	
	while(selection>=1 && selection<=4)
	{
		//char done='N';
		printf("\n\nEnter:\n");
		printf("1 - Display troops\n");
		printf("2 - Calculate stacks\n");
		printf("3 - Adjust stacks\n");
		printf("4 - Reset all to zero\n");
		printf("0 - return to main menu\n");
		printf("Selection: ");
		fflush(stdout);
		scanf("%d", &selection);
		fflush(stdin);
		switch(selection)
		{
			case 1:
				if(T_SEL.troops_selected) print_troops();
				else printf("\nNo troops have been selected.\n");
				break;
			
			case 2:
			
				if(T_SEL.troops_selected)
				{
					printf("\n\nHow many DD stacks to set up for? ");
					fflush(stdout);
					scanf("%d",&num_stacks);
					fflush(stdin);
					
					strongest = rank_troops(num_stacks);
					max_rank=MAX_RANK;
					//printf("strongest %d\n",strongest);
					//fflush(stdout);
		
					printf("Strongest troop is (%c) t%d %s. How many to start with? ", TROOPS[strongest].ADL, TROOPS[strongest].tier, TROOPS[strongest].type_s);
					fflush(stdout);
					scanf("%d",&starting);
					fflush(stdin);
					
					printf("Sort by both strength and health (Y or N, N for health only)? ");
					fflush(stdout);
					scanf("%c",&add_strength);
					fflush(stdin);
		
					if(add_strength == 'Y' || add_strength == 'y')
					{
						printf("How many turns to use strength? (-1 for all) ");
						fflush(stdout);
						scanf("%d",&num_str_turns);
						fflush(stdin);
						if(num_str_turns < 0) num_str_turns=T_CNT;
					}
					
					current_health = (starting)*TOTALS[strongest].Health-100;
					current_strength = (starting)*TOTALS[strongest].Strength-100;
					//printf("starting quantity: %d\n", starting);
					//printf("starting health: %f\n", current_health);
					//fflush(stdout);
					current_rank=1;
					while(current_rank<=MAX_RANK)
					{
						//printf("Current rank: %d, Current health: %f\n", current_rank, current_health);
						//fflush(stdout);
						for(j=0;j<T_CNT;j++)
						{
							//printf("j: %d, rank: %d, %d, %s\n",j,RANKS[j].Strength_rank,(RANKS[j].Strength_rank==current_rank),TROOPS[j].type_s);
							//fflush(stdout);
							if(RANKS[j].Strength_rank == current_rank)
							{
								RANKS[j].Health = 0.0;
								RANKS[j].Strength = 0.0;
								TROOPS[j].quantity=0;
								if(TROOPS[j].ADL == 'D') increment = 1;
								else if(TROOPS[j].ADL == 'A') increment = 10;
								else increment = 10; 
								//printf("increment: %d\n", increment);
								//fflush(stdout);
								while(RANKS[j].Health <= current_health) 
								{
									
									TROOPS[j].quantity+=increment;
									RANKS[j].Health=TROOPS[j].quantity*TOTALS[j].Health;
									RANKS[j].Strength=TROOPS[j].quantity*TOTALS[j].Strength;
									
									//if(troops[j].merc=='L')
									//if(troops[j].ADL<=2)
										
									if( (add_strength=='y' || add_strength=='Y') && RANKS[j].Strength_rank>max_rank-num_stacks*num_str_turns && RANKS[j].Strength <= current_strength)
									{
										printf("Adjusting up: t%d %s\n",TROOPS[j].tier, TROOPS[j].type_s);
										while(RANKS[j].Strength <= current_strength)
										{
											TROOPS[j].quantity+=increment;
											RANKS[j].Health=TROOPS[j].quantity*TOTALS[j].Health;
											RANKS[j].Strength=TROOPS[j].quantity*TOTALS[j].Strength;
										}
									}
									
									//printf("%d %d %d %f %f %d %f \n",j, current_rank,TROOPS[j].quantity, RANKS[j].Health, TOTALS[j].Health, increment, current_health);
									//printf("%d %d %d %f %f %d %f \n\n",j, current_rank, TROOPS[j].quantity, RANKS[j].Strength, TOTALS[j].Strength, increment, current_strength);
									//fflush(stdout);
									//getchar();
									//fflush(stdin);
								}
								//printf("%d %d %d %f %f %d %f \n",j, current_rank, TROOPS[j].quantity RANKS[j]->Health, TOTALS[j]->Health, increment, current_health);
								//printf("%d %d %d %f %f %d %f \n\n",j, current_rank, TROOPS[j].quantity, RANKS[j]->Strength, TOTALS[j]->Strength, increment, current_strength);
								//fflush(stdout);
								//getchar();
								//fflush(stdin);
								//temp_current_health = ranks[j].Health;
								//temp_current_strength = ranks[j].Strength;
								current_health = RANKS[j].Health;
								current_strength = RANKS[j].Strength;
								
							}
							
							//printf("t%d %s: %d\n", TROOPS[j].tier, TROOPS[j].type_s, TROOPS[j].quantity);
							//fflush(stdout);
							
							//getchar();
							//fflush(stdin);
						}
						current_rank++;
						//current_health=temp_current_health;
						//current_strength=temp_current_strength;
					}
					calculate_costs();
					
					ratio=calculate_ratio();
					calculate_adjusted_quantity(ratio, add_strength, num_str_turns, num_stacks);
					calculate_adjusted_costs();
					
					print_troops(max_rank);
					print_cost();
					print_adj_cost();
					troop_quantities_set=true;
				}
				else printf("You must select troop types to include before you can use this option.\n");
				break;
				
				
			case 3:
				
				while(1)
				{
					int adj_tier;
					char adj_type;
		
					printf("What tier? (0 to quit) ");
					fflush(stdout);
					scanf("%d",&adj_tier);
					fflush(stdin);
					
					
		
					if(adj_tier>=1 && adj_tier<=9)
					{
						printf("Guard, Specialist, or Monster? (G,S or M) ");
						fflush(stdout);
						scanf("%c",&adj_type);
						fflush(stdin);
						
						for(i=0;i<T_CNT;i++)
						{
							if(TROOPS[i].tier==adj_tier && (TROOPS[i].type_s[0]==adj_type || adj_type==(int)TROOPS[i].type_s[0]+32))
							{
								printf("New quantity of (%c) t%d %s? ",TROOPS[i].ADL ,TROOPS[i].tier, TROOPS[i].type_s);
								fflush(stdout);
								scanf("%d",&TROOPS[i].adj_quantity);
								fflush(stdin);
								if(TROOPS[i].adj_quantity<=0) TROOPS[i].included=false;
								else TROOPS[i].included=true; 
							}
						}
					}
					else break;
				}
				
				calculate_adjusted_costs();
				print_adj_troops(max_rank);
				print_adj_cost();
				num_troops=0;
				for(i=0;i<T_CNT;i++) if(TROOPS[i].included) num_troops++;
				if(num_troops>0)
				{
					troop_quantities_set=true;
					T_SEL.troops_selected=true;
				}
				break;
			case 4:
				troop_quantities_set=false;
				T_SEL.troops_selected=false;
				for(i=0;i<T_CNT;i++)
				{
					TROOPS[i].adj_quantity=0;
					TROOPS[i].quantity=0;
					TROOPS[i].included=false;
				}
				break;
		}
	}	
}

void simulate()
{
	int num_stacks;
	char adjust, YorN;
	bool adjusted;
	COST.average_gold=0; COST.average_silver=0; COST.average_VP=0;
	COST.runs=0;
		
	printf("Number of monster stacks? ");
	fflush(stdout);
	scanf("%d",&num_stacks);
	fflush(stdin);
	rank_troops(num_stacks);
		
		
	printf("Use adjusted troop stacks? (Y or N) ");
	fflush(stdout);
	scanf("%c",&adjust);
	fflush(stdin);
		
	while(1) 
	{
		struct sim sims[T_CNT];
		double total_strength=0;
		double temp_strength=0;
		float max_hth, max_str;
		int index_hth, index_str, i, j;
		int stacks_remaining=num_troops;
		char first_strike;
		
		int DS_mounted;
		int rando;
		
		int turn=0;
		float gold_s;
		float silver_s;
		int adjusted_quantity[T_CNT];
		
		srand((int)clock());
		
		
		for(i=0;i<T_CNT;i++){ 
		sims[i].dead=false; sims[i].turns=0; sims[i].struck=false; sims[i].double_strikes=0;}
		
		total_strength=0;
		
		
		if(adjust=='y' || adjust=='Y')
		{
			adjusted=true;
			gold_s=(((float)COST.rtot_auth*8/5.91)+((float)COST.rtot_dom*16/5.91))/1000.0;
			//printf("%f gold\n", gold_s);
			silver_s=(float)(COST.rtot_rebuild_silv_d+COST.rtot_silver_l)/1000000.0;
		}
		else
		{
			adjusted=false;
			gold_s=(((float)COST.tot_auth*8/5.91)+((float)COST.tot_dom*16/5.91))/1000.0;
			//printf("%f gold\n", gold_s);
			silver_s=(float)(COST.tot_rebuild_silv_d+COST.tot_silver_l)/1000000.0;
		}
		
		for(i=0;i<T_CNT;i++)
		{
			if(adjusted) adjusted_quantity[i] = TROOPS[i].adj_quantity;
			else adjusted_quantity[i] = TROOPS[i].quantity;
		}
		
		printf("Player gets first strike? (Y or N) ");
		fflush(stdout);
		scanf("%c",&first_strike);
		fflush(stdin);
		
		while(stacks_remaining>0)
		{
			printf("\n\n******************* turn %d *******************\n", turn);
			fflush(stdout);
			for(j=0; j<num_stacks; j++)
			{
				//printf("a "); fflush(stdout);
				max_hth=0;
				max_str=0;
				if( ((first_strike=='Y' || first_strike=='y') && (j!=0 || stacks_remaining!=T_CNT)) ||
					 (first_strike!='Y' && first_strike!='y') )
					 {
						 //printf("b "); fflush(stdout);
						for(i=0;i<T_CNT;i++)
						{
							//printf("c "); fflush(stdout);
							if(TROOPS[i].included)
							{
								//printf("d "); fflush(stdout);
								if( sims[i].dead==false)
								{		
									//printf("e "); fflush(stdout);
									if( adjusted_quantity[i]*TOTALS[i].Health > max_hth )
									{ 
										//printf("f "); fflush(stdout);
										max_hth = adjusted_quantity[i]*TOTALS[i].Health; 
										index_hth=i; 
										//printf("g "); fflush(stdout);
									}
									
								}
							}
						}
						//printf("i "); fflush(stdout);
						sims[index_hth].dead=true;
						//printf("j \n"); fflush(stdout);
						printf("(%c) t%d %s is killed\n", TROOPS[index_hth].ADL, TROOPS[index_hth].tier, TROOPS[index_hth].type_s); 
						
						stacks_remaining--;
						//printf("stacks remaining: %d", stacks_remaining); fflush(stdout);
						if(stacks_remaining<=0) break;
					 }
				
				for(i=0;i<T_CNT;i++)
				{
					//printf("1 "); fflush(stdout);
					if(TROOPS[i].included)
					{
						//printf("2 "); fflush(stdout);
						if( sims[i].dead==false && sims[i].struck==false)
						{					
							//printf("3 "); fflush(stdout);
							if( adjusted_quantity[i]*TOTALS[i].Strength > max_str )
							{
								max_str = adjusted_quantity[i]*TOTALS[i].Strength; 
								index_str=i; 
								//printf("4 "); fflush(stdout);
							}
						}
					}
				}
				//printf("5 "); fflush(stdout);
				sims[index_str].struck=true;
				//printf("6 \n"); fflush(stdout);
				printf("(%c) t%d %s strikes\n", TROOPS[index_hth].ADL, TROOPS[index_str].tier, TROOPS[index_str].type_s); 
				//printf("7 "); fflush(stdout);
				sims[index_str].turns++;
				//printf("8 "); fflush(stdout);


				rando = rand()%100;
				//printf("rando: %d\n", rando);
				if(TROOPS[index_str].troop_type=='O' && TROOPS[index_str].type_s[0]=='G')
				{
					//printf("9 "); fflush(stdout);
					//printf("Mounted guard +5\n");
					DS_mounted=5;
				}
				else DS_mounted=0;
				if( rando < (VALS.double_strike + DS_mounted) )
				{
					//printf("A \n"); fflush(stdout);
					printf("(%c) t%d %s doublestrikes!\n", TROOPS[index_hth].ADL, TROOPS[index_str].tier, TROOPS[index_str].type_s);
					sims[index_str].double_strikes++;
				}
				
				
			}
			//printf("B \n"); fflush(stdout);
			if(stacks_remaining>1) printf("rest of troops strike... \n");
			for(i=0;i<T_CNT;i++)
			{ 
				//printf("C "); fflush(stdout);
				if(sims[i].dead==false && sims[i].struck==false && TROOPS[i].included)
				{
					//printf("D "); fflush(stdout);
					sims[i].turns++; 
					
					//printf("E "); fflush(stdout);
					rando = rand()%100;
					//printf("rando: %d\n", rando);
					if(TROOPS[i].troop_type=='O' && TROOPS[i].type_s[0]=='G')
					{
						//printf("Mounted guard +5\n");
						DS_mounted=5;
					}
					else DS_mounted=0;
					if( rando < (VALS.double_strike + DS_mounted) )
					{
						printf("(%c) t%d %s doublestrikes!\n", TROOPS[i].ADL, TROOPS[i].tier, TROOPS[i].type_s);
						sims[i].double_strikes++;
					}
				}
				sims[i].struck=false;
			}
			turn++;
			fflush(stdout);
		}
		printf("\n******************* end: %d turns *******************\n\n", turn-1);
		for(i=0;i<T_CNT;i++){
			if(TROOPS[i].included)
			{
				temp_strength=(sims[i].turns+sims[i].double_strikes)*TOTALS[i].ranked_Strength*TROOPS[i].ADL_val*(float)adjusted_quantity[i];
				printf("(%c) rank: %2d/%d t%d %*s: %3d turns\tdoublestrikes: %2d\tdamage: %-2f\n", TROOPS[i].ADL, RANKS[i].Strength_rank, MAX_RANK, TROOPS[i].tier, -12, TROOPS[i].type_s, sims[i].turns, sims[i].double_strikes, temp_strength);
				total_strength=total_strength+temp_strength;
				//printf("tot_str %f\n",total_strength);
			}
		}
		printf("total health destroyed: %0.2f billion\n", total_strength/1000000000.0);
		printf("VP estimate: %0.2f billion\n", total_strength*VALS.VP_ratio/1000000000.0);
		printf("%0.3f k gold and %0.3f M silver per billion VP\n", gold_s/(total_strength*VALS.VP_ratio/1000000000.0), silver_s/(total_strength*VALS.VP_ratio/1000000000.0));
		COST.average_gold=COST.average_gold+gold_s/(total_strength*VALS.VP_ratio/1000000000.0);
		COST.average_silver=COST.average_silver+silver_s/(total_strength*VALS.VP_ratio/1000000000.0);
		COST.average_VP=COST.average_VP+(total_strength*VALS.VP_ratio/1000000000.0);
		COST.runs++;
		printf("runs: %d, average VP: %f B, average gold/B: %0.3f, average silver/B: %0.3f\n",COST.runs, COST.average_VP/COST.runs, COST.average_gold/COST.runs, COST.average_silver/COST.runs);
		printf("\nSimulate again? (Y or N) ");
		fflush(stdout);
		scanf("%c",&YorN);
		fflush(stdin);
		if(YorN!='Y' && YorN!='y') break;
	}
	
}

void show_selections()
{
	int selection=1, i;
	char YorN;
	char doneOrNot=' ';
	
	while(selection>=1 && selection<=4)
	{
		printf("\n\nEnter:\n");
		printf("1 - Show captain\n");
		printf("2 - Show troops\n");
		printf("3 - Show costs\n");
		printf("4 - Show default vals\n");
		printf("0 - return to main menu\n");
		printf("Selection: ");
		fflush(stdout);
		scanf("%d", &selection);
		fflush(stdin);
		switch(selection)
		{
			case 1:
				if(CAPTAIN.captain_set==false) printf("\n\nNo captain selected.\n\n");
				else
				{
					printf("\n\nCaptain: ");
					switch(CAPTAIN.captain)
					{
						case Ingrid:
							printf(Ingrid_s);
							break;
						case Aydae:
							printf(Aydae_s);
							break;
						case Logos:
							printf(Logos_s);
							break;
						case Lucius:
							printf(Lucius_s);
							break;
						case Bernard:
							printf(Bernard_s);
							break;
						case YeHo:
							printf(YeHo_s);
							break;
						case Brunhild:
							printf(Brunhild_s);
							break;
						default:
							printf(Captain_s);	
					}
					printf("\nLevel: %d\n", CAPTAIN.level);
					printf("Leadership: %d\n", CAPTAIN.leadership);
				}
				break;
			case 2:
				if(num_troops==0) printf("\n\nNo troops selected.\n");
				else 
				{
					printf("\n\nShow adjusted only? (Y or N) ");
					fflush(stdout);
					scanf("%c", &YorN);
					fflush(stdin);
					printf("Sending %d stacks:\n\n", num_troops);
					if(YorN=='Y' || YorN=='y')
					{
						for(i=0;i<T_CNT;i++) if(TROOPS[i].included==true)printf("(%c) t%d %-13s: %6d\n",TROOPS[i].ADL, TROOPS[i].tier, TROOPS[i].type_s, TROOPS[i].adj_quantity);
					}
					else for(i=0;i<T_CNT;i++) if(TROOPS[i].included==true)printf("(%c) t%d %-13s: %6d\t adj.: %6d\n",TROOPS[i].ADL, TROOPS[i].tier, TROOPS[i].type_s, TROOPS[i].quantity, TROOPS[i].adj_quantity);
				}
				break;
				
			case 3:
				printf("\n\nShow adjusted costs? (Y or N) ");
				fflush(stdout);
				scanf("%c", &YorN);
				fflush(stdin);
				if(YorN=='Y' || YorN=='y') print_adj_cost();
				else print_cost();
				break;
				
			case 4:
				printf("\n\nVP ratio: %0.3f\n",VALS.VP_ratio);
				printf("Doublestrike bonus: %d\n",VALS.double_strike);
				printf("Revival discount: %0.2f\n",VALS.revival_discount);
				break;
		}
	}
}

void read_in_troops(struct troop *troops, struct bonus *bonuses)
{
	char line[BUFFER_SIZE];
	int i=0, j=0;
	char *token;
	bool match=false;
	
	while(fgets(line, sizeof line, TROOP_BASE) != NULL)
	{
		if(i>0)
		{
			token = strtok(line, ",");
			//printf("%s\t",token);
			troops[i-1].tier=atoi(token);
			//printf("%d\t",troops[i-1].tier);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].ADL=token[0];
			//printf("%c\t",troops[i-1].merc);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].troop_type=token[0];
			//printf("%c\t",troops[i-1].troop_type);
			
			token = strtok(NULL, ",");
			//printf("%s,",token);
			strcpy(troops[i-1].type_s, token);
			for(j=0;j<B_CNT;j++)
			{
				if(strcmp(troops[i-1].type_s, bonuses[j].type)==0)
				{
					match=true;
					//printf("%d found:",j);
					//fflush(stdout);
					//printf("%s,",bonuses[j].type);
					troops[i-1].type=j;
					break;
				}
			}
			if(!match)
			{
				troops[i-1].type=0;
				printf("Troop type unknown: %s\n", troops[i-1].type_s);
			}
			else match=false;
						
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].ADL_val=atoi(token);
			//printf("%d\t",troops[i-n].ADL);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].gold=atoi(token);
			//printf("%d\t",troops[i-n].gold);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].silver=atoi(token);
			//printf("%d\t",troops[i-n].silver);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].DC=atoi(token);
			//printf("%d\t",troops[i-n].DC);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].SvMl=atoi(token);
			//printf("%d\t",troops[i-n].SvMl);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].SvR=atoi(token);
			//printf("%d\t",troops[i-n].SvR);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].SvMt=atoi(token);
			//printf("%d\t",troops[i-n].SvMt);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].SvF=atoi(token);
			//printf("%d\t",troops[i-n].SvF);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].Health=atoi(token);
			//printf("%d\t",troops[i-n].Health);
			
			token = strtok(NULL, ",");
			//printf("%s\t",token);
			troops[i-1].Strength=atoi(token);
			//printf("%d\t",troops[i-n].Strength);
			
			//printf("\n");
		}
		i++;
	}

}




void read_in_bonuses(struct bonus *bonuses)
{
	char line[BUFFER_SIZE];
	int i=0;
	char *token;
	
	while(fgets(line, sizeof line, TROOP_BONUS) != NULL)
		{
			if(i>0)
			{
				token = strtok(line, ",");
				//printf("%s,",token);
				strcpy(bonuses[i-1].type, token);
				//printf("%s\t",bonuses[i-1].type);
				
				token = strtok(NULL, ",");
				//printf("%s,",token);
				bonuses[i-1].Health=atoi(token);
				//printf("%d\t",bonuses[i-1].Health);
				
				token = strtok(NULL, ",");
				//printf("%s,",token);
				bonuses[i-1].Strength=atoi(token);
				//printf("%d\t",bonuses[i-1].Strength);
				
				//printf("\n");
			}
			i++;
		}
}

// counts the number of lines in the file
int count_lines(FILE *fp)
{
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
  
    // Check if file exists
    if (fp == NULL)
    {
        return 0;
    }
  
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
  
    //printf("The file has %d lines\n ", count);
  
    return count;
}


