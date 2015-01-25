#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__ 
#define __MAINSERVER
#define	__PETFILTER 
#define __QUGET_SWAP_FIX
//#define __REBIRTH
//#define __NEW_TELEPORT
#define	__VER 17

#if !defined( __TESTSERVER ) && !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#if (_MSC_VER > 1200)
#define		__VS2003		
#endif

#if __VER >= 16
#define __NEW_PROP_PARAMETER 
#define __3RD_LEGEND16 
#define __IMPROVE_MAP_SYSTEM
#define __INSTANCE_AGGRO_SYSTEM
#define __NEW_ITEM_VARUNA
#endif // __VER >= 16

#if __VER >= 17
#define	__FASHION_COMBINE
#define __NEW_WEAPON_GLOW
#define __COLOSSEUM
#endif // __VER >= 17

#define		D3DDEVICE	NULL
#define		__SERVER						
#define		__X15
#define		__VERIFY_LOOP041010
#define		__S1108_BACK_END_SYSTEM			
#define		__RES0807						
#define		__CPU_UTILDOWN_060502			
#define		__SLIDE_060502					
#define		__S8_SERVER_PORT				
#define		__EVENT_1101					
#define		__NEWYEARDAY_EVENT_COUPON		
#define		__LANG_1013						
#define		__HACK_1130						
#define		__QUEST_1208					
#define		__TRAFIC_1222					
#define		__S_SERVER_UNIFY				

#define		__BUGFIX_0326					
// 8.5
#define		__INFINITE_0227

#define		__LANG_IME_0327					
#define		__STL_0402						
// 9
#define		__TRADESYS						
#define		__EVE_BALLOON					

#define		__ITEMDROPRATE					

#define		__PK_PVP_SKILL_REGION			
#define		__PVPDEBUFSKILL					
#define		__EVENT_1101_2					
#define		__S_RECOMMEND_EVE				
#define		__EVE_MINIGAME					

#define		__ANGEL_LOG						
#define		__EXP_ANGELEXP_LOG				
#define		__S_ADD_EXP						

#define		__RULE_0615						
#define		__S_BUG_GC						
#define		__S_ADD_RESTATE					

#define		__PROP_0827						
#define		__RIGHTHAND_SKILL				
#define		__LOG_MATCHLESS					

#define		__PKSERVER_USE_ANGEL			

//	#define		__EVENTLUA_ATKDEF			


// 11
//	#define		__MA_VER11_02				
//	#define		__MA_VER11_04				
//	#define		__MA_VER11_05				
//	#define		__MA_VER11_06				

//	#define		__CSC_VER11_3				
//	#define		__CSC_VER11_5				
//	#define		__GUILDCOMBATCHIP			
//	#define		__GUILD_COMBAT_1TO1			
//	#define		__EXPITEM_TOOLTIP_CHANGE

//	#define		__REMOVE_ENDURANCE		
//	#define		__PIERCING_REMOVE		
#define		__EVENTLUA_COUPON			
#define		__NOLIMIT_RIDE_ITEM		
#define		__NPC_BUFF					
//	#define		__REFLECTDMG_AFTER		
//	#define		__CHIPI_071210			

//	#define		__SYS_POCKET			
//	#define		__SYS_COLLECTING		
//	#define		__SYS_IDENTIFY			
//	#define		__FIX_COLLISION
//	#define		__MOD_VENDOR

#define		__JEFF_11					
#define		__SYS_TICKET				
//	#define		__SYS_PLAYER_DATA		
#define		__HACK_1023					
#define		__RT_1025					
#define		__VENDOR_1106				

#define		__INVALID_LOGIN_0320		

#define		__JEFF_9_20					

#define		__EVENTLUA_GIFT				

#define		__JEFF_11_4					
#define		__JEFF_11_5					
#define		__JEFF_11_6					

#define		__DST_GIFTBOX				
#define		__EVENT_MONSTER				

#define		__CHIPI_DYO					
#define		__STL_GIFTBOX_VECTOR		
#define		__CHIPI_ITEMUPDATE_080804	

#define		__VM_0820
//#define		__VM_0819	

// 12
//	#define 	__ANGEL_NODIE				
//	#define		__SECRET_ROOM				
//	#define		__TAX						
//	#define		__HEAVEN_TOWER				
//	#define		__EXT_PIERCING				
//	#define		__MONSTER_SKILL				
//	#define		__NEW_SUMMON_RULE			
	#define		__LORA		// LORD SYSTEM
//	#define		__PET_0519	
//	#define		__J12_0		
//	#define		__RANGDA_0521	
//	#define		__MOD_TUTORIAL
//	#define		__JHMA_VER12_1	
//	#define		__PARSKILL1001	
//	#define		__ITEMCREATEMON_S0602		
//	#define		__NEW_ITEMCREATEMON_SERVER	
//	#define		__EVENTLUA_0826
//	#define		__LEAK_0827
//	#define		__UPDATE_OPT

// 13
//	#define		__EXT_ENCHANT				
//	#define		__RAINBOW_RACE				
//	#define		__HOUSING					
//	#define		__QUEST_HELPER				
//	#define		__CHIPI_QUESTITEM_FLAG		
//	#define		__HONORABLE_TITLE			
//	#define		__COUPLE_1117				
//	#define		__COUPLE_1202				

#define		__LAYER_1015		
#define		__LAYER_1020		
#define		__LAYER_1021		
#define		__AZRIA_1023	
#define		__PET_1024		
#define		__BUFF_1107

#define		__OCCUPATION_SHOPITEM	

#define		__SYNC_1217		
#define		__SPEED_SYNC_0108		
#define		__SYS_ITEMTRANSY			 

#define		__EVENTLUA_CHEEREXP		

#define		__FUNNY_COIN			

#define		__MAP_SECURITY				 

// 14차
//	#define		__NEW_CONTINENT				
//	#define		__SMELT_SAFETY				
//	#define		__INSTANCE_DUNGEON			
//	#define		__PARTY_DUNGEON				
//	#define		__ANGEL_EXPERIENCE			
//	#define		__EQUIP_BIND				
//	#define		__EXT_ATTRIBUTE				
//	#define		__NEW_ITEM_LIMIT_LEVEL		
//	#define		__BALLOON_CODE_IMPROVEMENT	
//	#define		__PCBANG					
	#define		__QUIZ						

#define		__BUFF_TOGIFT				
#define		__EVENTLUA_SPAWN			
#define		__EVENTLUA_KEEPCONNECT		

#define		__PERIN_BUY_BUG				

#define		__ERROR_LOG_TO_DB		

#define		__EVENTLUA_RAIN				

#define		__EVENTLUA_SNOW				

#define		__ADD_RESTATE_LOW			


// 15차
//	#define		__PETVIS					
//	#define		__GUILD_HOUSE				
//	#define		__TELEPORTER				
//	#define		__IMPROVE_QUEST_INTERFACE	
//	#define		__CAMPUS					
//	#define		__HERO129_VER15				
//	#define		__IMPROVE_SYSTEM_VER15		
//	#define		__DYNAMIC_MPU				
//	#define		__USING_CONTINENT_DATA		
//	#define		__REACTIVATE_EATPET			
//	#define		__15_5TH_ELEMENTAL_SMELT_SAFETY
//	end15th

	#define		__SHOP_COST_RATE			
	#define		__PROTECT_AWAKE				
	#define		__ENVIRONMENT_EFFECT

#if	  defined(__INTERNALSERVER)	

//	#define		__VERIFY_MEMPOOL
	#define		__GUILDVOTE					
	#define		__IAOBJ0622					
	#define		__SKILL0517
//	#define		__S_NEW_SKILL_2				
	#define		__Y_CASTING_SKIP			
	#define		__YAIMONSTER_EX				
	#define		__Y_PATROL					
	#define		__V060721_TEXTDRAG			
	#define		__Y_BEAUTY_SHOP_CHARGE
//	#define		__LUASCRIPT060908			
	#define		__Y_FLAG_SKILL_BUFF			
//	#define		__EVENT_FALL				
//	#define		__LANG_1013					
//	#define		__JEFF_VER_8				
//	#define		__JHMA_VER_8_1				
//	#define		__JHMA_VER_8_2				
//	#define		__JHMA_VER_8_5				
//	#define		__JHMA_VER_8_6				
//	#define		__JHMA_VER_8_7				
//	#define		__JHMA_VER_8_5_1			
//	#define		__JHMA_VER_8_5_2			
//	#define		__CSC_VER8_3				
//	#define		__CSC_VER8_4				
//	#define		__CSC_VER8_5				
//	#define		__CSC_VER8_6				
	#define		__Y_MAX_GENERAL_LEVEL_8		
	#define		__Y_HAIR_BUG_FIX
	#define		__EVENT_0117				
//	#define		__NEWYEARDAY_EVENT_COUPON	
	#define		__TRAFIC_1215
//	#define     __Y_NEW_ENCHANT				

//	#define		__INVALID_LOGIN_0320		
	#define		__BUGFIX_0326				
// 10
//	#define		__LEGEND					
// 10
//	#define		__ULTIMATE					
	
//	#define		__PET_0410					
//	#define		__JEFF_9					
//	#define		__AI_0509					
	#define		__HACK_0516					
//	#define		__LUASCRIPT					
//	#define		__EVENTLUA					

//	#define		__FLYBYATTACK0608			
//	#define		__PVPDEMAGE0608				
//	#define		__BLADELWEAPON0608			 
//	#define		__METEONYKER_0608
//	#define		__Y_DRAGON_FIRE
//	#define		__CSC_VER9_5				

	#define		__GLOBAL_COUNT_0705			

//	#define		__SKILL_0706				
//	#define		__AI_0711					
//	#define		__HACK_0720					
//11
//	#define		__JEFF_11					
	
//	#define		__SYS_TICKET				
//	#define		__SYS_PLAYER_DATA			
//	#define		__HACK_1023					
//	#define		__RT_1025					

//	#define		__VENDOR_1106				

	#define		__JEFF_11_1
	#define		__JEFF_11_3					
//	#define		__JEFF_11_4					

//	#define		__JEFF_11_5					

//	#define		__DST_GIFTBOX				

	#define		__PERF_0226

	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
//	#define		__NEW_PROFILE		

	#define		__VTN_TIMELIMIT				

	#define		__PROTECT_AWAKE				


	#undef		__VER
	#define		__VER 16

	#define		__GUILD_HOUSE_MIDDLE		

	#define		__MOVER_STATE_EFFECT		

	#define		__NEW_ITEM_VARUNA			


#elif defined(__TESTSERVER)		
	
	#define		__GUILDVOTE					
//	#define		__SKILL0517					
	#define		__Y_CASTING_SKIP			
	#define		__YAIMONSTER_EX				
	#define		__Y_PATROL					
	#define		__V060721_TEXTDRAG			
	#define		__Y_BEAUTY_SHOP_CHARGE
	#define		__Y_FLAG_SKILL_BUFF			
//	#define		__EVENT_FALL				
	#define		__Y_MAX_GENERAL_LEVEL_8		
	#define		__TRAFIC_1215

	// 10
//	#define		__LEGEND					

//	#define		__CSC_VER9_2				

	#define		__HACK_0516					

//	#define		__FLYBYATTACK0608			
//	#define		__PVPDEMAGE0608				
//	#define		__BLADELWEAPON0608			

//	#define		__Y_DRAGON_FIRE
//	#define		__LUASCRIPT					
//	#define		__EVENTLUA					
//	#define		__CSC_VER9_5				

//	#define		__PET_0410					
//	#define		__METEONYKER_0608
//	#define		__SKILL_0706				
//	#define		__AI_0711					
//	#define		__JEFF_9					
//	#define		__AI_0509					
	#define		__GLOBAL_COUNT_0705			

//	#define		__JEFF_11					
//	#define		__SYS_TICKET				
//	#define		__SYS_PLAYER_DATA			
//	#define		__HACK_1023					
//	#define		__RT_1025					
//	#define		__VENDOR_1106				

//	#undef	__VER
//	#define	__VER	11						

	#define		__JEFF_11_1
	#define		__JEFF_11_3					
//	#define		__JEFF_11_4					
//	#define		__JEFF_11_5					

//	#define		__JAPAN_SAKURA				

	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
	
//	#define		__NEW_PROFILE		

	#define		__GUILD_HOUSE_MIDDLE		

#elif defined(__MAINSERVER)	
 
	#define		__ON_ERROR
	#define		__IDC
	#define		__Y_BEAUTY_SHOP_CHARGE		
	#define		__TRAFIC_1215
//	#define		__EVENT_FALL		

//	#define     __Y_NEW_ENCHANT				

	#define		__JEFF_11_1
	#define		__JEFF_11_3		

	#define		__EVENT_0117				
//	#define		__JAPAN_SAKURA				
//	#define		__RAIN_EVENT		 

	#define		__OPT_MEM_0811
//	#define		__MEM_TRACE
//	#define		__NEW_PROFILE		
	#define		__GLOBAL_COUNT_0705			

#endif	
#if __VER >= 7
	#define		__REMOVE_SCIRPT_060712		
#endif

#endif
