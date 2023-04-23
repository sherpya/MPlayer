// Translated by:  Jiri Svoboda, jiri.svoboda@seznam.cz
// Updated by:     Tomas Blaha,  tomas.blaha at kapsa.cz
//                 Jiri Heryan
// Synced with r28122


// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<č_stopy>  přehraje (S)VCD (Super Video CD) stopu (z nepřipojeného\n"\
                      "                  zařízení)\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<č_tit>    přehraje DVD titul ze zařízení (mechaniky), místo ze souboru\n"\
                      " -alang/-slang    zvolí jazyk zvuku/titulků na DVD (dvouznakový kód země)\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Použití:          mplayer [volby] [url|cesta/]jméno_souboru\n"\
"\n"\
"Základní volby: (úplný seznam najdete v manuálové stránce)\n"\
" -vo <rozhraní>   vybere výstupní video rozhraní (seznam: -vo help)\n"\
" -ao <rozhraní>   vybere výstupní audio rozhraní (seznam: -ao help)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -ss <pozice>     převine na zadanou pozici (sekundy nebo hh:mm:ss)\n"\
" -nosound         přehrávání beze zvuku\n"\
" -fs              celoobrazovkové přehrávání (nebo -vm -zoom, viz manuál)\n"\
" -x <x> -y <y>    rozlišení obrazu (pro použití s -vm nebo -zoom)\n"\
" -sub <soubor>    zvolí soubor s titulky (viz také -subfps, -subdelay)\n"\
" -playlist <soubor> určí soubor s playlistem\n"\
" -vid x -aid y    vybere video (x) a audio (y) proud pro přehrání\n"\
" -fps x -srate y  změnit video (x fps) a audio (y Hz) frekvence\n"\
" -pp <kvalita>    aktivovat postprocessing (podrobnosti v manuálu)\n"\
" -framedrop       povolit zahazování snímků (pro pomalé stroje)\n"\
"\n"\
"Základní klávesy: (úplný seznam je v manuálu, viz také input.conf)\n"\
" <-  nebo  ->     převíjení vzad/vpřed o 10 sekund\n"\
" dolů či nahoru   převíjení vzad/vpřed o  1 minutu\n"\
" pgdown či pgup   převíjení vzad/vpřed o 10 minut\n"\
" < nebo >         posun na předchozí/další soubor v playlistu\n"\
" p nebo mezerník  pozastaví přehrávání (pokračuje po stisku jakékoliv klávesy)\n"\
" q nebo ESC       konec přehrávání a ukončení programu\n"\
" + nebo -         upraví zpoždění zvuku v krocích +/- 0,1 sekundy\n"\
" o                cyklická změna režimu OSD: nic / pozice / pozice a čas\n"\
" * nebo /         přidá nebo ubere PCM hlasitost\n"\
" x nebo z         upraví zpoždění titulků v krocích +/- 0,1 sekundy\n"\
" r nebo t         upraví polohu titulků nahoru/dolů, viz také -vf expand\n"\
"\n"\
" * * * V MAN STRÁNCE NAJDETE PODROBNOSTI, DALŠÍ VOLBY A KLÁVESY * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\nKončím...\n"
#define MSGTR_ExitingHow "\nKončím... (%s)\n"
#define MSGTR_Exit_quit "Konec"
#define MSGTR_Exit_eof "Konec souboru"
#define MSGTR_Exit_error "Kritická chyba"
#define MSGTR_IntBySignal "\nMPlayer přerušen signálem %d v modulu %s.\n"
#define MSGTR_NoHomeDir "Nemohu nalézt domácí adresář.\n"
#define MSGTR_GetpathProblem "Nastal problém s get_path(\"config\")\n"
#define MSGTR_CreatingCfgFile "Vytvářím konfigurační soubor: %s\n"
#define MSGTR_CantLoadFont "Nemohu načíst bitmapový font: %s\n"
#define MSGTR_CantLoadSub "Nemohu načíst titulky: %s\n"
#define MSGTR_DumpSelectedStreamMissing "dump: Kritická chyba: Chybí požadovaný datový proud!\n"
#define MSGTR_CantOpenDumpfile "Nelze otevřít soubor pro dump.\n"
#define MSGTR_CoreDumped "Jádro odhozeno ;)\n"
#define MSGTR_FPSnotspecified "Údaj o FPS v hlavičce souboru je špatný nebo chybí, použijte volbu -fps!\n"
#define MSGTR_TryForceAudioFmtStr "Pokouším se vynutit rodinu audiokodeku %s...\n"
#define MSGTR_CantFindAudioCodec "Nemohu nalézt kodek pro audio formát 0x%X!\n"
#define MSGTR_TryForceVideoFmtStr "Pokouším se vynutit rodinu videokodeku %s...\n"
#define MSGTR_CantFindVideoCodec "Nemohu nalézt kodek pro vybraný -vo a video formát 0x%X.\n"
#define MSGTR_CannotInitVO "Kritická chyba: Nemohu inicializovat video rozhraní!\n"
#define MSGTR_CannotInitAO "Nepodařilo se otevřít/inicializovat audio zařízení -> nebude zvuk.\n"
#define MSGTR_StartPlaying "Začínám přehrávat...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         ***********************************************************\n"\
"         ****  Váš systém je příliš POMALÝ pro toto přehrávání! ****\n"\
"         ***********************************************************\n\n"\
"Možné příčiny, problémy a řešení:\n"\
"- Nejčastější: špatný/chybný _zvukový_ ovladač!\n"\
"  - Zkuste -ao sdl nebo použijte OSS emulaci z ALSA.\n"\
"  - Pohrajte si s různými hodnotami -autosync, pro začátek třeba 30.\n"\
"- Pomalý obrazový výstup\n"\
"  - Zkuste jiný -vo ovladač (seznam: -vo help) nebo zkuste -framedrop!\n"\
"- Pomalá CPU\n"\
"  - Nezkoušejte přehrát velké DVD/DivX na pomalé CPU! Zkuste některé lavdopts,\n"\
"    jako -vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all.\n"\
"- Poškozený soubor.\n"\
"  - Zkuste různé kombinace voleb -nobps -ni -forceidx -mc 0.\n"\
"- Přehráváte z pomalého média (NFS/SMB, DVD, VCD, atd.)\n"\
"  - Zkuste -cache 8192.\n"\
"- Používáte -cache pro neprokládané AVI soubory?\n"\
"  - Zkuste -nocache.\n"\
"Tipy na vyladění a zrychlení najdete v DOCS/HTML/en/devices.html.\n"\
"Pokud nic z toho nepomůže, přečtěte si DOCS/HTML/en/bugreports.html.\n\n"

#define MSGTR_NoGui "MPlayer byl přeložen BEZ podpory GUI.\n"
#define MSGTR_GuiNeedsX "GUI MPlayeru vyžaduje X11.\n"
#define MSGTR_Playing "\nPřehrávám %s\n"
#define MSGTR_NoSound "Audio: žádný zvuk\n"
#define MSGTR_FPSforced "FPS vynuceno na hodnotu %5.3f  (vyn. čas: %5.3f)\n"
#define MSGTR_AvailableVideoOutputDrivers "Dostupná video rozhraní:\n"
#define MSGTR_AvailableAudioOutputDrivers "Dostupná audio rozhraní:\n"
#define MSGTR_AvailableAudioCodecs "Dostupné audio kodeky:\n"
#define MSGTR_AvailableVideoCodecs "Dostupné video kodeky:\n"
#define MSGTR_AvailableAudioFm "Dostupné (zakompilované) rodiny audio kodeků/ovladačů:\n"
#define MSGTR_AvailableVideoFm "Dostupné (zakompilované) rodiny video kodeků/ovladačů:\n"
#define MSGTR_AvailableFsType "Dostupné režimy změny hladiny při celoobrazovkovém zobrazení:\n"
#define MSGTR_CannotReadVideoProperties "Video: Nelze přečíst vlastnosti.\n"
#define MSGTR_NoStreamFound "Nenalezen žádný datový proud.\n"
#define MSGTR_ErrorInitializingVODevice "Chyba při otevírání/inicializaci vybraného video_out (-vo) zařízení.\n"
#define MSGTR_ForcedVideoCodec "Vynucen video kodek: %s\n"
#define MSGTR_ForcedAudioCodec "Vynucen audio kodek: %s\n"
#define MSGTR_Video_NoVideo "Video: Žádné video\n"
#define MSGTR_NotInitializeVOPorVO "\nKritická chyba: Nemohu inicializovat video filtry (-vf) nebo video výstup (-vo)!\n"
#define MSGTR_Paused "===== POZASTAVENO ====="
#define MSGTR_PlaylistLoadUnable "\nNemohu načíst playlist %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer havaroval kvůli 'Illegal Instruction'.\n"\
"  To může být chyba v kódu pro rozpoznání CPU za běhu...\n"\
"  Prosím, přečtěte si DOCS/HTML/en/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer havaroval kvůli 'Illegal Instruction'.\n"\
"  To se obvykle stává, když se ho pokusíte spustit na CPU odlišném, než pro který\n"\
"  byl přeložen/optimalizován.\n  Ověřte si to!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer havaroval kvůli špatnému použití CPU/FPU/RAM.\n"\
"  Přeložte MPlayer s volbou --enable-debug , proveďte 'gdb' backtrace\n"\
"  a disassembly. Detaily najdete v DOCS/HTML/en/bugreports_what.html#bugreports_crash.\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer havaroval. To by se nemělo stát.\n"\
"  Může to být chyba v kódu MPlayeru _nebo_ ve vašich ovladačích _nebo_ ve verzi\n"\
"  vašeho gcc. Pokud si myslíte, že je to chyba MPlayeru, přečtěte si, prosím,\n"\
"  DOCS/HTML/en/bugreports.html a pokračujte podle tam uvedeného návodu. My vám nemůžeme\n"\
"  pomoci, pokud tyto informace neuvedete při ohlašování možné chyby.\n"
#define MSGTR_LoadingConfig "Načítám konfiguraci '%s'\n"
#define MSGTR_LoadingProtocolProfile "Načítám profil protokolu '%s'\n"
#define MSGTR_LoadingExtensionProfile "Načítám profil rozšíření '%s'\n"
#define MSGTR_AddedSubtitleFile "SUB: Přidán soubor s titulky (%d): %s\n"
#define MSGTR_RemovedSubtitleFile "SUB: Odebrán soubor s titulky (%d): %s\n"
#define MSGTR_RTCDeviceNotOpenable "Selhalo otevření %s: %s (by mělo být čitelné uživatelem.)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Chyba inicializace Linuxových RTC v ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Zkuste přidat \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" do startovacích\n skriptů vašeho systému.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Chyba inicializace Linuxových RTC v ioctl (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "VAROVÁNÍ: getch2_init volána dvakrát!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Nemohu otevřít video filtr libmenu s kořenovým menu %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Chyba při předinicializaci řetězce audio filtrů!\n"
#define MSGTR_LinuxRTCReadError "Chyba při čtení z Linuxových RTC: %s\n"
#define MSGTR_SoftsleepUnderflow "Varování! Podtečení softsleep!\n"

// --- edit decision lists (seznamy editačních zásahů)
#define MSGTR_EdlOutOfMem "Nelze alokovat dostatek paměti pro vložení EDL dat.\n"
#define MSGTR_EdlRecordsNo "Načítám %d EDL akcí.\n"
#define MSGTR_EdlQueueEmpty "Veškeré EDL akce již byly provedeny.\n"
#define MSGTR_EdlCantOpenForWrite "Nelze otevřít EDL soubor [%s] pro zápis.\n"
#define MSGTR_EdlNOsh_video "EDL nelze použít bez videa, vypínám.\n"
#define MSGTR_EdlNOValidLine "Chybná EDL na řádku: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Špatně formátovaná EDL na řádku [%d], zahazuji.\n"
#define MSGTR_EdlBadLineOverlap "Poslední stop značka byla [%f]; další start je [%f].\n"\
"Vstupy musí být v chronologickém pořadí a nesmí se překrývat. Zahazuji.\n"
#define MSGTR_EdlBadLineBadStop "Časová značka stop má být za značkou start.\n"
#define MSGTR_EdloutBadStop "EDL: Vynechání zrušeno, poslední start > stop\n"
#define MSGTR_EdloutStartSkip "EDL: Začátek vynechaného bloku, stiskněte znovu 'i' pro ukončení bloku.\n"
#define MSGTR_EdloutEndSkip "EDL: Konec vynechaného bloku, řádek zapsán.\n"

// mplayer.c OSD
#define MSGTR_OSDenabled "zapnuto"
#define MSGTR_OSDdisabled "vypnuto"
#define MSGTR_OSDAudio "Zvuk: %s"
#define MSGTR_OSDChannel "Kanál: %s"
#define MSGTR_OSDSubDelay "Zpoždění tit: %d ms"
#define MSGTR_OSDSpeed "Rychlost: x %6.2f"
#define MSGTR_OSDosd "OSD: %s"
#define MSGTR_OSDChapter "Kapitola: (%d) %s"
#define MSGTR_OSDAngle "Úhel: %d/%d"

// property values
#define MSGTR_Enabled "zapnuto"
#define MSGTR_EnabledEdl "zapnuto (EDL)"
#define MSGTR_Disabled "vypnuto"
#define MSGTR_HardFrameDrop "intenzivní"
#define MSGTR_Unknown "neznámé"
#define MSGTR_Bottom "dolů"
#define MSGTR_Center "na střed"
#define MSGTR_Top "nahoru"
#define MSGTR_SubSourceFile "soubor"
#define MSGTR_SubSourceVobsub "vobsub"
#define MSGTR_SubSourceDemux "vestavěné"

// OSD bar names
#define MSGTR_Volume "Hlasitost"
#define MSGTR_Panscan "Panscan"
#define MSGTR_Gamma "Gama"
#define MSGTR_Brightness "Jas"
#define MSGTR_Contrast "Kontrast"
#define MSGTR_Saturation "Sytost"
#define MSGTR_Hue "Odstín"
#define MSGTR_Balance "Stereováha"

// property state
#define MSGTR_LoopStatus "Loop: %s"
#define MSGTR_MuteStatus "Ztišení: %s"
#define MSGTR_AVDelayStatus "A-V odchylka: %s"
#define MSGTR_OnTopStatus "Zůstat navrchu: %s"
#define MSGTR_RootwinStatus "Kořenové okno: %s"
#define MSGTR_BorderStatus "Rámeček: %s"
#define MSGTR_FramedroppingStatus "Zahazování snímků: %s"
#define MSGTR_VSyncStatus "Vertikální synchronizace: %s"
#define MSGTR_SubSelectStatus "Titulky: %s"
#define MSGTR_SubSourceStatus "Zdroj titulků: %s"
#define MSGTR_SubPosStatus "Umístění titulků: %s/100"
#define MSGTR_SubAlignStatus "Zarovnání titulků: %s"
#define MSGTR_SubDelayStatus "Zpoždění titulků: %s"
#define MSGTR_SubScale "Zvětšení titulků: %s"
#define MSGTR_SubVisibleStatus "Titulky: %s"
#define MSGTR_SubForcedOnlyStatus "Pouze vynucené titulky: %s"

// mencoder.c
#define MSGTR_UsingPass3ControlFile "Řídicí soubor pro tříprůchodový režim: %s\n"
#define MSGTR_MissingFilename "\nChybí jméno souboru.\n\n"
#define MSGTR_CannotOpenFile_Device "Nelze otevřít soubor/zařízení.\n"
#define MSGTR_CannotOpenDemuxer "Nelze otevřít demuxer.\n"
#define MSGTR_NoAudioEncoderSelected "\nNebyl vybrán audio enkodér (-oac). Nějaký vyberte (viz -oac help) nebo použijte -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nNebyl vybrán video enkodér (-ovc). Nějaký vyberte (viz  -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Nelze otevřít výstupní soubor '%s'\n"
#define MSGTR_EncoderOpenFailed "Selhalo spuštění enkodéru\n"
#define MSGTR_MencoderWrongFormatAVI "\nVAROVÁNÍ: FORMÁT VÝSTUPNÍHO SOUBORU JE _AVI_. Viz -of help.\n"
#define MSGTR_MencoderWrongFormatMPG "\nVAROVÁNÍ: FORMÁT VÝSTUPNÍHO SOUBORU JE _MPEG_. Viz -of help.\n"
#define MSGTR_MissingOutputFilename "Nebyl nastaven výstupní soubor, prostudujte si volbu -o."
#define MSGTR_ForcingOutputFourcc "Vynucuji výstupní FourCC na %x [%.4s].\n"
#define MSGTR_ForcingOutputAudiofmtTag "Vynucuji značku výstupního zvukového formátu 0x%x\n"
#define MSGTR_DuplicateFrames "\n%d opakujících se snímků!\n"
#define MSGTR_SkipFrame "\nPřeskakuji snímek!\n"
#define MSGTR_ResolutionDoesntMatch "\nNový video soubor má jiné rozlišení nebo barevný prostor než jeho předchůdce.\n"
#define MSGTR_FrameCopyFileMismatch "\nVšechny video soubory musí mít shodné fps, rozlišení a kodek pro -ovc copy.\n"
#define MSGTR_AudioCopyFileMismatch "\nVšechny soubory musí používat identický audio kodek a formát pro -oac copy.\n"
#define MSGTR_NoAudioFileMismatch "\nNelze kombinovat neozvučené video soubory s ozvučenými. Zkuste -nosound.\n"
#define MSGTR_NoSpeedWithFrameCopy "VAROVÁNÍ: volba -speed nemá zaručenou správnou funkčnost spolu s -oac copy!\n"\
"Výsledný film může být vadný!\n"
#define MSGTR_ErrorWritingFile "%s: chyba při zápisu souboru.\n"
#define MSGTR_FlushingVideoFrames "\nVylévám video snímky.\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "Filtry nebyly nakonfiguovány! Prázdný soubor?\n"
#define MSGTR_RecommendedVideoBitrate "Doporučený datový tok videa pro CD %s: %d\n"
#define MSGTR_VideoStreamResult "\nVideo proud: %8.3f kbit/s  (%d B/s)  velikost: %"PRIu64" bajtů  %5.3f sekund  %d snímků\n"
#define MSGTR_AudioStreamResult "\nAudio proud: %8.3f kbit/s  (%d B/s)  velikost: %"PRIu64" bajtů  %5.3f sekund\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL SKIP: Start: %.2f  Konec: %.2f   Současná: V: %.2f  A: %.2f     \r"
#define MSGTR_OpenedStream "úspěch: formát: %d  data: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videokodek: framecopy (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "audiokodek: framecopy (formát=%x kanálů=%d frekvence=%d bitů=%d B/s=%d vzorek-%d)\n"
#define MSGTR_MP3AudioSelected "Vybrán MP3 zvuk.\n"
#define MSGTR_SettingAudioDelay "Nastavuji zpoždění zvuku na %5.3fs.\n"
#define MSGTR_SettingVideoDelay "Nastavuji zpoždění videa na %5.3fs.\n"
#define MSGTR_LimitingAudioPreload "Omezuji přednačítání zvuku na 0.4s.\n"
#define MSGTR_IncreasingAudioDensity "Zvyšuji hustotu audia na 4.\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Vynucuji přednačítání zvuku na 0, max korekci pts  na 0.\n"
#define MSGTR_LameVersion "LAME ve verzi %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Chyba: Specifikovaný datový tok je mimo rozsah pro tento preset režim.\n"\
"\n"\
"Pokud používáte tento režim, musíte zadat hodnotu od \"8\" do \"320\".\n"\
"\n"\
"Další informace viz: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Chyba: Nezadali jste platný profil a/nebo volby s preset režimem.\n"\
"\n"\
"Dostupné profily jsou:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - Implikuje režim ABR. Pro jeho použití,\n"\
"                      jednoduše zadejte datový tok. Například:\n"\
"                      \"preset=185\" aktivuje tento režim\n"\
"                      a použije průměrný datový tok 185 kbps.\n"\
"\n"\
"    Několik příkladů:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Další informace viz: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Preset režimy jsou navrženy tak, aby poskytovaly co nejvyšší možnou kvalitu.\n"\
"\n"\
"Většina z nich byla testována a vyladěna pomocí zevrubných zdvojených slepých\n"\
"poslechových testů, za účelem dosažení a ověření této kvality.\n"\
"\n"\
"Nastavení jsou neustále aktualizována v souladu s nejnovějším vývojem\n"\
"a měla by poskytovat prakticky nejvyšší možnou kvalitu, jaká je v současnosti \n"\
"s kodekem LAME dosažitelná.\n"\
"\n"\
"Aktivace preset režimů:\n"\
"\n"\
"   Pro režimy VBR (všeobecně nejvyšší kvalita):\n"\
"\n"\
"     \"preset=standard\" Tento režim by měl být jasnou volbou\n"\
"                             pro většinu lidí a hudebních žánrů a má\n"\
"                             již vysokou kvalitu.\n"\
"\n"\
"     \"preset=extreme\" Pokud máte výjimečně dobrý sluch a odpovídající\n"\
"                             vybavení, tento režim obecně poskytuje\n"\
"                             mírně vyšší kvalitu než režim \"standard\".\n"\
"\n"\
"   Pro CBR 320kbps (nejvyšší možná kvalita ze všech preset režimů):\n"\
"\n"\
"     \"preset=insane\"  Tento režim je pro většinu lidí a situací\n"\
"                             předimenzovaný, ale pokud vyžadujete\n"\
"                             absolutně nejvyšší kvalitu bez ohledu na\n"\
"                             velikost souboru, je toto vaše volba.\n"\
"\n"\
"   Pro režimy ABR (vysoká kvalita při daném datovém toku, ale ne jako VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  Použitím tohoto režimu obvykle dosáhnete dobré\n"\
"                             kvality při daném datovém toku. V závislosti\n"\
"                             na zadaném toku tento preset odvodí optimální\n"\
"                             nastavení pro danou situaci.\n"\
"                             Ačkoli tento přístup funguje, není ani zdaleka\n"\
"                             tak flexibilní jako VBR, a obvykle nedosahuje\n"\
"                             stejné úrovně kvality jako VBR na vyšších dato-\n"\
"                             vých tocích.\n"\
"\n"\
"Pro odpovídající profily jsou také dostupné následující volby:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR režim) - Implikuje režim ABR. Pro jeho použití,\n"\
"                      jednoduše zadejte datový tok. Například:\n"\
"                      \"preset=185\" aktivuje tento režim\n"\
"                      a použije průměrný datový tok 185 kbps.\n"\
"\n"\
"   \"fast\" - V daném profilu aktivuje novou rychlou VBR kompresi.\n"\
"            Nevýhodou je obvykle mírně vyšší datový tok než v normálním\n"\
"            režimu a také může dojít k mírnému poklesu kvality.\n"\
"   Varování:v současné verzi může nastavení \"fast\" vést k příliš\n"\
"            vysokému datovému toku ve srovnání s normálním nastavením.\n"\
"\n"\
"   \"cbr\"  - Pokud použijete režim ABR (viz výše) s významným\n"\
"            datovým tokem, např. 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            můžete použít volbu \"cbr\" k vynucení kódování v režimu CBR\n"\
"            (konstantní tok) namísto standardního ABR režimu. ABR poskytuje\n"\
"            lepší kvalitu, ale CBR může být užitečný v situacích jako je\n"\
"            vysílání MP3 proudu po internetu.\n"\
"\n"\
"    Například:\n"\
"\n"\
"      \"-lameopts fast:preset=standard  \"\n"\
" nebo \"-lameopts  cbr:preset=192       \"\n"\
" nebo \"-lameopts      preset=172       \"\n"\
" nebo \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Pro ABR režim je k dispozici několik zkratek:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"Nelze nastavit volby pro LAME, ověřte datový_tok/vzorkovou_rychlost. Některé"\
"velmi nízké datové toky (<32) vyžadují nižší vzorkovou rychlost (např. -srate 8000).\n"\
"Pokud vše selže, zkuste preset."
#define MSGTR_ConfigFileError "chyba konfiguračního souboru"
#define MSGTR_ErrorParsingCommandLine "chyba při zpracovávání příkazového řádku"
#define MSGTR_VideoStreamRequired "Videoproud je povinný!\n"
#define MSGTR_ForcingInputFPS "Vstupní fps bude interpretováno jako %5.3f\n"
#define MSGTR_DemuxerDoesntSupportNosound "Tento demuxer zatím nepodporuje -nosound.\n"
#define MSGTR_MemAllocFailed "Alokace paměti selhala.\n"
#define MSGTR_NoMatchingFilter "Nemohu najít odpovídající filtr/ao formát!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, možná je vadný překladač C?\n"
#define MSGTR_NoLavcAudioCodecName "Audio LAVC, chybí jméno kodeku!\n"
#define MSGTR_LavcAudioCodecNotFound "Audio LAVC, nemohu najít enkodér pro kodek %s.\n"
#define MSGTR_CouldntAllocateLavcContext "Audio LAVC, nemohu alokovat kontext!\n"
#define MSGTR_CouldntOpenCodec "Nelze otevřít kodek %s, br=%d.\n"
#define MSGTR_CantCopyAudioFormat "Audio formát 0x%x je nekompatibilní s '-oac copy', zkuste prosím '-oac pcm',\n nebo použijte '-fafmttag' pro jeho přepsání.\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     metoda proměnného datového toku\n"\
"                0: cbr  (konstantní tok)\n"\
"                1: mt   (VBR algoritmus Mark Taylor)\n"\
"                2: rh   (VBR algoritmus Robert Hegemann - výchozí)\n"\
"                3: abr  (průměrný tok)\n"\
"                4: mtrh (VBR alogoritmus Mark Taylor Robert Hegemann)\n"\
"\n"\
" abr           průměrný datový tok\n"\
"\n"\
" cbr           konstantní datový tok\n"\
"               Vynutí také metodu CBR pro následné ABR preset režimy\n"\
"\n"\
" br=<0-1024>   určení datového toku v kBit (pouze CBR a ABR)\n"\
"\n"\
" q=<0-9>       kvalita (0-nejvyšší, 9-nejnižší) (pouze pro VBR)\n"\
"\n"\
" aq=<0-9>      kvalita algoritmu (0-nejlepší/nejpomalejší, 9-nejhorší/nejrychlejší)\n"\
"\n"\
" ratio=<1-100> kompresní poměr\n"\
"\n"\
" vol=<0-10>    zesílení zvuku\n"\
"\n"\
" mode=<0-3>    (výchozí: auto)\n"\
"                0: stereo\n"\
"                1: joint-stereo\n"\
"                2: dualchannel\n"\
"                3: mono\n"\
"\n"\
" padding=<0-2>\n"\
"                0: ne\n"\
"                1: vše\n"\
"                2: upravit\n"\
"\n"\
" fast          Zapíná rychlejší enkódování pro následné VBR preset režimy,\n"\
"               poskytuje o něco nižší kvalitu a vyšší datový tok.\n"\
"\n"\
" preset=<hodnota> Přednastavené profily poskytující maximální kvalitu.\n"\
"                  medium: enkódování metodou VBR, dobrá kvalita\n"\
"                   (datový tok 150-180 kbps)\n"\
"                  standard: enkódování metodou VBR, vysoká kvalita\n"\
"                   (datový tok 170-210 kbps)\n"\
"                  extreme: enkódování metodou VBR, velmi vysoká kvalita\n"\
"                   (datový tok 200-240 kbps)\n"\
"                  insane: enkódování metodou CBR, nejvyšší preset kvalita\n"\
"                   (datový tok 320 kbps)\n"\
"                  <8-320>: hodnota průměrného datového toku pro metodu ABR.\n\n"

//codec-cfg.c
#define MSGTR_DuplicateFourcc "zdvojené FourCC"
#define MSGTR_TooManyFourccs "příliš mnoho FourCC/formátů..."
#define MSGTR_ParseError "chyba interpretace (parse)"
#define MSGTR_ParseErrorFIDNotNumber "chyba interpretace (ID formátu, nikoli číslo?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "chyba interpretace (alias ID formátu, nikoli číslo?)"
#define MSGTR_DuplicateFID "zdvojené ID formátu"
#define MSGTR_TooManyOut "příliš mnoho výstupu..."
#define MSGTR_InvalidCodecName "\njméno kodeku(%s) není platné!\n"
#define MSGTR_CodecLacksFourcc "\nkodek(%s) nemá FourCC/formát!\n"
#define MSGTR_CodecLacksDriver "\nkodek(%s) nemá driver!\n"
#define MSGTR_CodecNeedsDLL "\nkodek(%s) vyžaduje 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\nkodek(%s) vyžaduje 'outfmt'!\n"
#define MSGTR_CantAllocateComment "Nelze alokovat paměť pro komentář. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Nemám paměť pro 'line': %s\n"
#define MSGTR_CantReallocCodecsp "Nelze realokovat '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "Jméno kodeku '%s' není jedinečné."
#define MSGTR_CantStrdupName "Nelze provést strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Nelze provést strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Nelze provést strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Nelze provést strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d audio & %d video kodeků\n"
#define MSGTR_CodecDefinitionIncorrect "Kodek není správně definován."
#define MSGTR_OutdatedCodecsConf "Tento codecs.conf je příliš starý a nekompatibilní s tímto sestavením  MPlayeru!"

// fifo.c

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "'--' označuje konec voleb, ale nebyl zadán žádný název souboru na příkazovém řádku.\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "Hodnota volby loop musí být selé číslo: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "Neznámá volba na příkazovém řádku: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "Chyba při zpracování volby na příkazovém řádku: -%s\n"
#define MSGTR_InvalidPlayEntry "Neplatná položka pro přehrávání %s\n"
#define MSGTR_NotAnMEncoderOption "-%s není volbou MEncoderu\n"
#define MSGTR_NoFileGiven "Nezadán soubor\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "Nalezený save slot z lvl %d je příliš starý: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "Volbu %s nelze použít v konfiguračním souboru\n"
#define MSGTR_InvalidCmdlineOption "Volbu %s nelze použít z příkazového řádku\n"
#define MSGTR_InvalidSuboption "Chyba: volba '%s' nemá žádnou podvolbu '%s'\n"
#define MSGTR_MissingSuboptionParameter "Chyba: podvolba '%s' volby '%s' musí mít parametr!\n"
#define MSGTR_MissingOptionParameter "Chyba: volba '%s' musí mít parametr!\n"
#define MSGTR_OptionListHeader "\n Název                Typ             Min        Max      Globál  CL    Konfig\n\n"
#define MSGTR_TotalOptions "\nCelkem: %d voleb\n"
#define MSGTR_ProfileInclusionTooDeep "VAROVÁNÍ: Příliš hluboké vnořování profilů.\n"
#define MSGTR_NoProfileDefined "Žádný profil nebyl definován.\n"
#define MSGTR_AvailableProfiles "Dostupné profily:\n"
#define MSGTR_UnknownProfile "Neznámý profil '%s'.\n"
#define MSGTR_Profile "Profil %s: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n Název                Typ             Min        Max\n\n"
#define MSGTR_TotalProperties "\nCelkem: %d nastavení\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "VAROVÁNÍ: Pokouším se použít DLL kodeky, ale proměnná prostředí\n        DYLD_BIND_AT_LAUNCH není nastavena. Nejspíš havaruji.\n"


// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "O aplikaci MPlayer"
#define MSGTR_GUI_Add "Přidat"
#define MSGTR_GUI_AspectRatio "Poměr stran"
#define MSGTR_GUI_Audio "Zvuk"
#define MSGTR_GUI_AudioDelay "Zpoždění zvuku"
#define MSGTR_GUI_AudioDriverConfiguration "Konfigurace ovladače zvuku"
#define MSGTR_GUI_AudioTrack "Načíst externí soubor se zvukem"
#define MSGTR_GUI_AudioTracks "Audio stopa"
#define MSGTR_GUI_AvailableDrivers "Dostupné ovladače:"
#define MSGTR_GUI_AvailableSkins "Témata"
#define MSGTR_GUI_Bass "Basový"
#define MSGTR_GUI_Blur "Rozmazání"
#define MSGTR_GUI_Bottom "Dolní"
#define MSGTR_GUI_Brightness "Jas"
#define MSGTR_GUI_Browse "Prohlížet"
#define MSGTR_GUI_Cache "Vyrovnávací paměť"
#define MSGTR_GUI_CacheSize "Velikost vyrovnávací paměti"
#define MSGTR_GUI_Cancel "Zrušit"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Středový"
#define MSGTR_GUI_Channel1 "Kanál 1"
#define MSGTR_GUI_Channel2 "Kanál 2"
#define MSGTR_GUI_Channel3 "Kanál 3"
#define MSGTR_GUI_Channel4 "Kanál 4"
#define MSGTR_GUI_Channel5 "Kanál 5"
#define MSGTR_GUI_Channel6 "Kanál 6"
#define MSGTR_GUI_ChannelAll "Vše"
#define MSGTR_GUI_ChapterN "Kapitola %d"
#define MSGTR_GUI_ChapterNN "Kapitola %2d"
#define MSGTR_GUI_Chapters "Kapitoly"
#define MSGTR_GUI_Clear "Vynulovat"
#define MSGTR_GUI_CodecFamilyAudio "Rodina audio kodeku"
#define MSGTR_GUI_CodecFamilyVideo "Rodina video kodeku"
#define MSGTR_GUI_CodecsAndLibraries "Kodeky a knihovny třetích stran"
#define MSGTR_GUI_Coefficient "Koeficient"
#define MSGTR_GUI_ConfigFileError "chyba konfiguračního souboru\n"
#define MSGTR_GUI_Configure "Konfigurace"
#define MSGTR_GUI_ConfigureDriver "Konfigurovat ovladač"
#define MSGTR_GUI_Contrast "Kontrast"
#define MSGTR_GUI_Contributors "Přispěvatelé kódu a dokumentace"
#define MSGTR_GUI_Cp874 "Thajština (CP874)"
#define MSGTR_GUI_Cp936 "Jednoduchá čínština (CP936)"
#define MSGTR_GUI_Cp949 "Korejština (CP949)"
#define MSGTR_GUI_Cp1250 "Slovanské/středoevropské Windows (CP1250)"
#define MSGTR_GUI_Cp1251 "Cyrilické Windows (CP1251)"
#define MSGTR_GUI_Cp1256 "Arabské Windows (CP1256)"
#define MSGTR_GUI_CpBIG5 "Tradiční čínština (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Západoevropské jazyky (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Slovanské/středoevropské jazyky (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Esperanto, gaelština, maltézština, turečtina (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Staré Baltské kódování (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Cyrilice (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Arabština (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Moderní řečtina (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Hebrejština (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Turečtina (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Baltické (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Keltština (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Západoevropské jazyky s Eurem (ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Ruština (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Ukrajinština, běloruština (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Japonština (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_DefaultSetting "výchozí nastavení"
#define MSGTR_GUI_Delay "Zpoždění"
#define MSGTR_GUI_Demuxers_Codecs "Kodeky & demuxer"
#define MSGTR_GUI_Device "Zařízení"
#define MSGTR_GUI_DeviceCDROM "Zařízení CD-ROM"
#define MSGTR_GUI_DeviceDVD "Zařízení DVD"
#define MSGTR_GUI_Directory "Cesta"
#define MSGTR_GUI_DirectoryTree "Adresáře"
#define MSGTR_GUI_DropSubtitle "Zahodit titulky..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAssSubtitle "SSA/ASS renderování titulku"
#define MSGTR_GUI_EnableAutomaticAVSync "Zapnout automatickou synchronizaci"
#define MSGTR_GUI_EnableCache "Zapnout vyrovnávací paměť"
#define MSGTR_GUI_EnableDirectRendering "Aktivovat direct rendering"
#define MSGTR_GUI_EnableDoubleBuffering "Aktivovat dvojitou vyrovnávací paměť"
#define MSGTR_GUI_EnableEqualizer "Aktivovat ekvalizér"
#define MSGTR_GUI_EnableExtraStereo "Aktivovat extra stereo"
#define MSGTR_GUI_EnableFrameDropping "Aktivovat zahazování snímků"
#define MSGTR_GUI_EnableFrameDroppingIntense "Aktivovat TVRDÉ zahazování snímků (nebezpečné)"
#define MSGTR_GUI_EnablePlaybar "Aktivovat playbar"
#define MSGTR_GUI_EnablePostProcessing "Aktivovat postprocessing"
#define MSGTR_GUI_EnableSoftwareMixer "Aktivovat softwarový směšovač"
#define MSGTR_GUI_Encoding "Kódování"
#define MSGTR_GUI_Equalizer "Ekvalizér"
#define MSGTR_GUI_EqualizerConfiguration "Konfigurace ekvalizéru"
#define MSGTR_GUI_Error "Chyba!"
#define MSGTR_GUI_ErrorFatal "Kritická chyba!"
#define MSGTR_GUI_File "Přehrát soubor"
#define MSGTR_GUI_Files "Soubory"
#define MSGTR_GUI_Flip "Převrátit obraz vzhůru nohama"
#define MSGTR_GUI_Font "Font"
#define MSGTR_GUI_FrameRate "FPS"
#define MSGTR_GUI_FrontLeft "Levý přední"
#define MSGTR_GUI_FrontRight "Pravý přední"
#define MSGTR_GUI_HideVideoWindow "Zobrazovat video okno při nečinnosti"
#define MSGTR_GUI_Hue "Odstín"
#define MSGTR_GUI_Lavc "Použít LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Automatické řízení kvality"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Ostatní"
#define MSGTR_GUI_Mixer "Mixér"
#define MSGTR_GUI_MixerChannel "Kanál mixéru"
#define MSGTR_GUI_MSG_AddingVideoFilter "[GUI] Přidávám video filtr: %s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "Promiňte, ale barevná hloubka je příliš malá.\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "D&D: Nic se nevrátilo!\n"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Bohužel nelze přehrávat ne-MPEG s kartou DXR3/H+ bez přeenkódování.\nProsím, zapněte lavc v konfiguraci DXR3/H+.\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "[GUI] Načítám titulky: %s\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "Promiňte, nedostatek paměti pro vykreslení bufferu.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Bohužel není dostatek paměti pro vykreslovací mezipaměť.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Není načten žádný soubor."
#define MSGTR_GUI_MSG_NoMediaOpened "Nic není otevřeno."
#define MSGTR_GUI_MSG_NotAFile0 "Toto nevypadá jako soubor...\n"
#define MSGTR_GUI_MSG_NotAFile1 "Toto nevypadá jako soubor: %s !\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Pozor, některá nastavení potřebují pro svou funkci restartovat přehrávání!"
#define MSGTR_GUI_MSG_RemoteDisplay "Vzdálený display, vypínám XMITSHM.\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "[GUI] Mažu titulky.\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "chyba konverze z 24 do 32 bitů (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "Soubor nenalezen (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "chyba čtení PNG (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Téma nenalezeno (%s).\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "Vybraný skin ( %s ) nenalezen, zkouším 'výchozí'...\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "Bitmapy s hloubkou 16 bitů a méně nejsou podporovány (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[témata] chyba v konfiguračním souboru témat na řádce %d: %s"
#define MSGTR_GUI_MSG_SkinFileNotFound "[témata] soubor ( %s ) nenalezen.\n"
#define MSGTR_GUI_MSG_SkinFileNotReadable "[témata] soubor ( %s ) nelze přečíst.\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "soubor fontu nebyl nalezen\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "soubor obrazu fontu nebyl nalezen\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "neexistující identifikátor fontu (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "nedostatek paměti\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "deklarováno příliš mnoho fontů\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "neznámá zpráva: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "neznámý parametr (%s)\n"
#define MSGTR_GUI_MSG_TooManyWindows "Příliš mnoho otevřených oken.\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "[cfg] Nelze uložit volbu '%s'.\n"
#define MSGTR_GUI_MSG_VideoOutError "Bohužel nebyl nalezen video ovladač kompatibilní s GUI.\n"
#define MSGTR_GUI_MSG_XShapeError "Promiňte, ale váš systém nepodporuje rozšíření XShape.\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "chyba rozšíření shared memory\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "Promiňte, ale váš systém nepodporuje rozšíření X shared memory.\n"
#define MSGTR_GUI_Mute "Ztlumit"
#define MSGTR_GUI_NetworkStreaming "Síťové vysílání..."
#define MSGTR_GUI_Next "Další proud"
#define MSGTR_GUI_NoChapter "Žádná kapitola" //bez kapitoly?
#define MSGTR_GUI__none_ "(žádné)"
#define MSGTR_GUI_NonInterleavedParser "Použít parser pro neprokládaný AVI formát"
#define MSGTR_GUI_NormalizeSound "Normalizovat zvuk"
#define MSGTR_GUI_Ok "OK"
#define MSGTR_GUI_Open "Otevřít..."
#define MSGTR_GUI_Original "Původní"
#define MSGTR_GUI_OsdLevel "Typ OSD"
#define MSGTR_GUI_OSD_Subtitles "Titulky & OSD"
#define MSGTR_GUI_Outline "Obrys"
#define MSGTR_GUI_PanAndScan "Panscan"
#define MSGTR_GUI_Pause "Pozastavit"
#define MSGTR_GUI_Play "Přehrát"
#define MSGTR_GUI_Playback "Ovládání přehrávání"
#define MSGTR_GUI_Playlist "Playlist"
#define MSGTR_GUI_Position "Pozice"
#define MSGTR_GUI_PostProcessing "Postprocessing"
#define MSGTR_GUI_Preferences "Nastavení"
#define MSGTR_GUI_Previous "Předchozí proud"
#define MSGTR_GUI_Quit "Konec"
#define MSGTR_GUI_RearLeft "Levý zadní"
#define MSGTR_GUI_RearRight "Pravý zadní"
#define MSGTR_GUI_Remove "Odebrat"
#define MSGTR_GUI_Saturation "Sytost"
#define MSGTR_GUI_SaveWindowPositions "Uložit pozici okna"
#define MSGTR_GUI_ScaleMovieDiagonal "Proporčně dle úhlopříčky"
#define MSGTR_GUI_ScaleMovieHeight "Proporčně dle výšky obrazu"
#define MSGTR_GUI_ScaleMovieWidth "Proporčně dle šířky obrazu"
#define MSGTR_GUI_ScaleNo "Bez automatické velikosti"
#define MSGTR_GUI_SeekingInBrokenMedia "Znovu sestavit tabulku indexů, pokud je to třeba"
#define MSGTR_GUI_SelectAudioFile "Vybrat externí zvukový kanál..."
#define MSGTR_GUI_SelectedFiles "Vybrané soubory"
#define MSGTR_GUI_SelectFile "Vybrat soubor..."
#define MSGTR_GUI_SelectFont "Vybrat font..."
#define MSGTR_GUI_SelectSubtitle "Vybrat titulky..."
#define MSGTR_GUI_SizeDouble "Dvojnásobná velikost"
#define MSGTR_GUI_SizeFullscreen "Celá obrazovka"
#define MSGTR_GUI_SizeHalf "Poloviční velikost"
#define MSGTR_GUI_SizeNormal "Normální velikost"
#define MSGTR_GUI_SizeOSD "Velikost OSD"
#define MSGTR_GUI_SizeSubtitles "Velikost textu"
#define MSGTR_GUI_SkinBrowser "Prohlížeč témat"
#define MSGTR_GUI_Skins "Skiny"
#define MSGTR_GUI_Sponsored "Vývoj GUI je sponzorován firmou UHU Linux"
#define MSGTR_GUI_StartFullscreen "Spustit přes celou obrazovku"
#define MSGTR_GUI_Stop "Stop"
#define MSGTR_GUI_Subtitle "Titulky"
#define MSGTR_GUI_SubtitleAddMargins "Použijí se okraje"
#define MSGTR_GUI_SubtitleAllowOverlap "Zapnout překrývání titulků"
#define MSGTR_GUI_SubtitleAutomaticLoad "Vypnout automatické načtení titulků"
#define MSGTR_GUI_SubtitleConvertMpsub "Převést dané titulky do vlastního formátu MPlayeru"
#define MSGTR_GUI_SubtitleConvertSrt "Převést dané titulky do časově orientovaného formátu SubViewer (SRT)"
#define MSGTR_GUI_Subtitles "Titulky"
#define MSGTR_GUI_SyncValue "Automatická synchronizace"
#define MSGTR_GUI_TitleNN "Titul %2d"
#define MSGTR_GUI_Titles "Tituly"
#define MSGTR_GUI_Top "Horní"
#define MSGTR_GUI_TrackN "Stopa %d"
#define MSGTR_GUI_Translations "Překlady"
#define MSGTR_GUI_TurnOffXScreenSaver "Zastavit XScreenSaver"
#define MSGTR_GUI_URL "Přehrát z URL"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Obraz"
#define MSGTR_GUI_VideoEncoder "Video enkodér"
#define MSGTR_GUI_VideoTracks "Video stopa"
#define MSGTR_GUI_Volume MSGTR_Volume
#define MSGTR_GUI_Warning "Varování!"

// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "Vybrané video_out zařízení je nekompatibilní s tímto kodekem.\n"\
                "Zkuste přidat filtr scale do svého řetězce filtrů,\n"\
                "čili -vf spp,scale namísto -vf spp.\n"
#define MSGTR_VO_GenericError "Tato chyba nastala"
#define MSGTR_VO_UnableToAccess "Nemám přístup k"
#define MSGTR_VO_ExistsButNoDirectory "již existuje, ale není to adresář."
#define MSGTR_VO_DirExistsButNotWritable "Výstupní adresář již existuje, ale nelze do něj zapisovat."
#define MSGTR_VO_CantCreateDirectory "Nelze vytvořit výstupní adresář."
#define MSGTR_VO_CantCreateFile "Nelze vytvořit výstupní soubor."
#define MSGTR_VO_DirectoryCreateSuccess "Úspěšně vytvořen výstupní adresář."
#define MSGTR_VO_ValueOutOfRange "hodnota mimo rozsah"

// aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] Varování: Nenalezeno vhodné nové rozlišení!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] Chyba: Nenalezen nový rozměr vhodný pro dané rozlišení!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "New_Face selhalo. Možná je chybná cesta k fontu.\nDodejte prosím soubor fontu pro texty (~/.mplayer/subfont.ttf).\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "New_Memory_Face selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "font titulků: load_sub_face selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "font titulků: prepare_charset selhalo.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "Nelze připravit font titulků.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "Nelze připravit font pro OSD.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "Nelze generovat tabulky fontu.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "FT_Done_FreeType selhalo.\n"

// sub.c
#define MSGTR_VO_SUB_Seekbar "Pozice"
#define MSGTR_VO_SUB_Play "Přehrávání"
#define MSGTR_VO_SUB_Pause "Pauza"
#define MSGTR_VO_SUB_Stop "Stop"
#define MSGTR_VO_SUB_Rewind "Zpět"
#define MSGTR_VO_SUB_Forward "Vpřed"
#define MSGTR_VO_SUB_Clock "Hodiny"
#define MSGTR_VO_SUB_Contrast "Kontrast"
#define MSGTR_VO_SUB_Saturation "Sytost"
#define MSGTR_VO_SUB_Volume "Hlasitost"
#define MSGTR_VO_SUB_Brightness "Jas"
#define MSGTR_VO_SUB_Hue "Tónování"
#define MSGTR_VO_SUB_Balance "Váha"

// vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] Podporována pouze 16bit barevná hloubka!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] Visual ID je %lx.\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] Nelze otevřít /dev/3dfx.\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] Chyba: %d.\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] Nelze mapovat 3dfx paměťové prostory: %p,%p,%d.\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] Inicializováno: %p.\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] Neznámé podzařízení: %s.\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\nZde jsou podvolby aalib vo_aa:\n"
#define MSGTR_VO_AA_AdditionalOptions "Dodatečné volby vo_aa zajišťují:\n" \
"  help        vypíše tuto nápovědu\n" \
"  osdcolor    nastaví barvu OSD\n  subcolor    nastaví barvu titulků\n" \
"        parametry barev jsou:\n           0 : normal\n" \
"           1 : dim\n           2 : bold\n           3 : boldfont\n" \
"           4 : reverse\n           5 : special\n\n\n"

// vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] Nelze nahrát novou SPU paletu!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] Nelze nastavit přehrávací režim!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] Nelze nastavit režim subpicture!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] Nelze získat TV normu!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] TV norma zvolena automaticky podle snímkové rychlosti: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] Nelze nastavit TV normu!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] Nastavuji NTSC.\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] Nastavuji PAL/SECAM.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] Nastavuji poměr stran na 4:3.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] Nastavuji poměr stran na 16:9.\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] došla paměť\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] Nelze alokovat klíčovací barvu!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] Nelze alokovat přesně klíčovací barvu,\n    používám nejbližší možnou (0x%lx).\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] Odinicializuji.\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] Selhalo obnovení TV normy!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] Zapínám předsunutou vyrovnávací paměť.\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] Používám nový synchronizační mechanizmus.\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] Používám overlay.\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] Chyba: Overlay vyžaduje kompilaci s nainstalovanými knihovnami\n    a hlavičkovými soubory X11.\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] Nastavím TV normu na: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "samopřizpůsobení snímkové rychlosti filmu (PAL/PAL-60)"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "samopřizpůsobení snímkové rychlosti filmu (PAL/NTSC)"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "Použiji současnou normu."
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "Zadanou normu neznám. Použiji současnou."
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300_mv.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] Ani /dev/em8300 nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] Ani /dev/em8300_mv nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] Otevřeno: %s.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] Nelze otevřít %s pro zápis, zkouším /dev/em8300_sp instead.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] Ani /dev/em8300_sp nelze otevřít pro zápis!\nVzdávám to.\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] Nelze otevřít obrazovku během overlay hack nastavení!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] Nelze inicializovat X11!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] Nelze nastavit overlay atribut.\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] Selhalo nastavení overlay obrazu!\nKončím.\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] Selhalo zapnutí overlay!\nKončím.\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] Selhala změna velikosti overlay okna!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] Selhalo nastavení overlay bcs!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] Selhalo nastavení overlay Y offsetů!\nKončím.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] Selhalo nastavení overlay X offsetů!\nKončím.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] Selhalo nastavení overlay X korekce rozlišení!\nKončím.\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y offset: %d.\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X offset: %d.\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X korekce: %d.\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] Selhalo nastavení signálové mixáže!\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Zapnut progresivní JPEG."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Vypnut progresivní JPEG."
#define MSGTR_VO_JPEG_BaselineJPEG "Zapnut základní JPEG."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Vypnut základní JPEG."

// vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): škálováno na %dx%d.\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] odinicializace!\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] chyba v mga_vid_config ioctl (špatná verze mga_vid.o?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] Nelze získat jasové hodnoty z jaderného modulu!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] Nelze nastavit jasové hodnoty z jaderného modulu!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] Neznámá šířka/výška obrazovky!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] neplatný výstupní formát %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] Verze vašeho mga_vid ovlůadače je nekompatibilní s touto verzí MPlayeru!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] Nelze otevřít: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] Rozlišení zdroje přesahuje 1023x1023 nejméně v jednom směru.\n[MGA] Přeškálujte softwarově, nebo použijte -lavdopts lowres=1.\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] neshoda verzí jaderného (%u) a MPlayerova (%u) mga_vid ovladače\n"

// vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] Neznámé podzařízení: %s.\n"

// vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] Varování: kompresní úroveň nastavena na 0, komprese vypnuta!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] Info: Použijte -vo png:z=<n> pro nastavení kompresní úrovně od 0 do 9.\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] Info: (0 = bez komprese, 1 = nejrychlejší, nejmenší - 9 nejlepší, nejpomalejší koprese)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] Nelze otevřít '%s' pro zápis!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] Chyba v create_png.\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "Zapnut ASCII režim."
#define MSGTR_VO_PNM_RawMode "Zapnut surový (Raw) režim."
#define MSGTR_VO_PNM_PPMType "Budou zapisovány PPM soubory."
#define MSGTR_VO_PNM_PGMType "Budou zapisovány PGM soubory."
#define MSGTR_VO_PNM_PGMYUVType "Budou zapisovány PGMYUV soubory."

// vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] Nelze získat přijatelný SDL Mode pro výstup.\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] Nepodporovaný formát obrazu (0x%X).\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] Info - použijte prosím -vm nebo -zoom pro přepnutí do nejlepřího rozlišení.\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] Selhalo nbastavení video režimu: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] Nelze vytvořit YUV overlay: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] Nelze vytvořit RGB podklad: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] Použití konverze hloubky na barevnou paletu vše zpomalí (%ibpp -> %ibpp).\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] Nepodporovaný formát obrazu v draw_slice, kontaktujte vývojáře MPlayeru!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Blit selhalo: %s.\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] Inicializace SDL selhala: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] Používám ovladač: %s.\n"

// vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] Vynucený vid_mode %d (%s) není k dispozici.\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] Vynucený vid_mode %d (%s) je moc malý.\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode: %d, %dx%d %dbpp.\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode(%d) selhalo.\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] Videorežim je lineární a memcpy lze použít pro transfer obrazu.\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] Videorežim má hardwarovou akceleraci a put_image lze použít.\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] Rád bych věděl zda vám to funguje.\n[VO_SVGA] (pošlete log získaný takto: `mplayer test.avi -v -v -v -v &> svga.log`). Thx!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] Videorežim má %d stran.\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] Centruji obraz. Počátek na (%d,%d)\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] Používám VIDIX. w=%i h=%i  mw=%i mh=%i\n"

// vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Přesun %d(%d) x %d => %d.\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] AGP přesun nesmazal obrazovku.\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Blit selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] Nenativní overlay formát vyžaduje konverzi.\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] Nepodporovaný vstupní formát 0x%x.\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] Nastavení overlay selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] Zapnutí overlay selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] Overlay připraveno: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] Texture blit připraveno: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] Vypnutí overlay selhalo\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] Nelze otevřít %s: %s.\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] Nelze získat současnou konfiguraci: %s.\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap selhalo !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "Get image todo.\n" //I have no idea how to translate this.
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP přesun selhal.\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] Nastavení YUV selhalo.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] AGP přesun selhal v kanálu Y.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] AGP přesun selhal v kanálu U.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] AGP přesun selhal v kanálu V.\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] neznámý formát: 0x%x.\n"

// vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] Nelze otevřít %s: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] Problém s FBITGET_FSCREENINFO ioctl: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] Problém s FBITGET_VSCREENINFO ioctl: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] Toto rozhraní podporuje pouze 3Dfx Banshee, Voodoo3 a Voodoo 5.\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp výstup není podporován.\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] Nelze mapovat paměťové rozsahy: %s.\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp výstup není podporován (Tak to by se nikdy nemělo stát.).\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Jejda! Něco je špatně s control().\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] Není dost paměti pro přehrátí tohoto filmu. Zkuste nižší rozlišení.\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] Obraz má %dx%d při %d bpp, vstup je %dx%d při %d bpp, norma je %dx%d.\n"

// vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] Neznámé podzařízení: %s.\n"

// vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] Nastala fatální chyba! Nelze pokračovat.\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] neznámé podzařízení: '%s'.\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] Máte malou video paměť pro tento režim:\n[VO_VESA] Vyžadováno: %08lX poskytnuto: %08lX.\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] Musíte poskytnout schopnosti monitoru. Neměním obnovovací kmitočet.\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] Režim přesahuje schopnosti monitoru. Neměním obnovovací kmitočet.\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] Zjištěna interní fatální chyba: init je voláno před preinit.\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] Volba -flip není podporována.\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] Možný důvod: Nenalezen VBE2 BIOS.\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] Nalezen VESA VBE BIOS Verze %x.%x Revize: %x.\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] Video paměť: %u Kb.\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA Capabilities: %s %s %s %s %s.\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! OEM info bude vypsáno níže !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] Níže byste měli vidět 5 řádků k OEM; Pokud ne, máte vadný vm86.\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM info: %s.\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM Revize: %x.\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM vydavatel: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM Název produktu: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM Revize produktu: %s.\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] Tip: Pro funkční TV-Out byste měli mít zapojen TV konektor\n"\
"[VO_VESA] před bootem, jelikož VESA BIOS se inicializuje pouze během POST.\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] Používám VESA režim (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] Nelze inicializovat softwarový škálovač.\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] Nelze použít DGA. Vynucuji režim bank switching. :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] Používám DGA (fyzické zdroje: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] Nelze použít dvojitý buffer: nedostatek video paměti.\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] Nelze nalézt ani DGA ani relokovatelné okno.\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] Vynutili jste DGA. Končím\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] Nelze najít platnou adresu okna.\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] Používám bank switching režim (fyzické zdroje: %08lXh, %08lXh).\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] Nelze alokovat dočasný buffer.\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] Promiňte, nepodporovaný režim -- zkuste -x 640 -zoom.\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] Páni, vážně máte obraz na TV!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] Nelze inicializovat Linux Video Overlay.\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] Používám video overlay: %s.\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] Nelze inicializovat VIDIX ovladač.\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] Používám VIDIX.\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] Nelze najít režim pro: %ux%u@%u.\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] Inicializace VESA je dokončena.\n"

// vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] Tato řada již není podporována.\n[VESA_LVO] Použijte prosím -vo vesa:vidix.\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] Nelze otevřít: '%s'\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] Neplatný výstupní formát: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] Verze vašeho fb_vid ovladače je nekompatibilní s touto verzí MPlayeru!\n"

// vo_x11.c

// vo_xv.c
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] Sdílená paměť není podporována\nVracím se k běžnému Xv.\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] Promiňte, Xv není touto verzí/ovladačem X11 podporováno\n[VO_XV] ******** Zkuste -vo x11  or  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors selhalo.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] Zadán neplatný port, měním na port 0.\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] Nelze zabrat port %i.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] Nelze nalézt volný Xvideo port - možná je využit jiným procesem\n"\
"[VO_XV] Zavřete všechny video aplikace a zkuste to znovu. Pokud to nepomůže,\n"\
"[VO_XV] vyberte si pomocí 'mplayer -vo help' jiné (ne-xv) video rozhraní.\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] Zdá se, že popora Xvideo pro vaši video kartu není k dispozici.\n"\
"[VO_XV] Spusťte 'xvinfo' pro ověření podpory Xv a přečtěte si\n"\
"[VO_XV] DOCS/HTML/cs/video.html#xv!\n"\
"[VO_XV] Viz také 'mplayer -vo help' pro jiné (ne-xv) video rozhraní.\n"\
"[VO_XV] Zkuste -vo x11.\n"
#define MSGTR_VO_XV_ImagedimTooHigh "Rozměry zdrojového obrazu jsou příliš velké: %ux%u (maximum je %ux%u)\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Prokládaný režim obrazu vyžaduje výšku obrazu dělitelnou 4."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Šířka obrazu musí být dělitelná 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Nelze získat paměť nebo ukazatel souboru pro zápis \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Chyba při zápisu obrázku na výstup!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Neznámé podzařízení: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Používám prokládaný výstupní režim, horní pole napřed."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Používám prokládaný výstupní režim, dolní pole napřed."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Používám (výchozí) neprokládaný snímkový režim."

// vosub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] Nelze začít přehrávat: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] Nelze skončit přehrávání: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] Prokládané UV pro YUV410P není podporováno.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] Dummy vidix_draw_slice() zavolána.\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] Nepodporované FourCC pro tento VIDIX ovladač: %x (%s).\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] Video server nepodporuje rozlišení (%dx%d), podporuje: %dx%d-%dx%d.\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] Video server nepodporuje ve vidix barevnou hloubku (%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX ovladač nemůže zvětšit obraz (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX ovladač nemůže zmenšit obraz (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] Nelze konfigurovat přehrávání: %s.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] Nelze najít funkční VIDIX ovladač.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] Nelze získat schopnost: %s.\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Nelze odeslat událost EWMH fullscreen!\n"
#define MSGTR_SelectedVideoMode "XF86VM: Vybraný video režim %dx%d pro velikost obrazu %dx%d.\n"

#define MSGTR_InsertingAfVolume "[Mixer] Hardwarová mixáž není k dispozici, vkládám filtr volume.\n"
#define MSGTR_NoVolume "[Mixer] Ovládání hlasitosti není k dislozici.\n"
#define MSGTR_NoBalance "[Mixer] Stereováha není k dispozici.\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "Výstupní videorozhraní pgm bylo nahrazeno -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "Výstupní videorozhraní md5 bylo nahrazeno -vo md5sum.\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: moduly alsa9 a alsa1x byly odstraněny, místo nich použijte -ao alsa.\n"
#define MSGTR_AO_NoSuchDriver "Takové audio rozhraní není '%.*s'\n"
#define MSGTR_AO_FailedInit "Selhala inicializace audio rozhraní '%s'\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Nelze otevřít mixážní zařízení %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup: Mixer zvukové karty nemá kanál '%s', používám výchozí.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Nelze otevřít zvukové zařízení %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Nelze provést blokování souborového deskriptoru: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] Zvukové zařízení %s nelze nastavit na výstup %s, zkouším %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Selhalo nastavení výstupního zvukového zařízení na %d kanálů.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: Ovladač nepodporuje SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Ovladač Vaší zvukové karty NEPODPORUJE select()  ***\n Překompilujte MPlayer s #undef HAVE_AUDIO_SELECT v config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\nKritická chyba: *** NELZE ZNOVUOTEVŘÍT / RESTARTOVAT ZVUKOVÉ ZAŘÍZENÍ *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] Neznámý/nepodporovaný OSS formát: %x.\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Připojen ke zvukovému serveru.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Nelze otevřít datový proud.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Datový proud otevřen.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] velikost vyrovnávací paměti: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Nastavení hlasitosti na %d selhalo.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] %d Hz není podporováno, zkuste převzorkovat.\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound selhalo: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] latence: [server: %0.2fs, síť: %0.2fs] (upravuji %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] selhalo otevření datového proudu ESD pro přehrávání: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] selhalo nastavení DVB zvukového mixeru: %s.\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Hz není podporováno, zkuste převzorkovat.\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Soubor: %s (%s)\nPCM: Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Info:  Nejrychlejší extrakce dosáhnete s -vc null -vo null -ao pcm:fast\n[AO PCM] Info: Pro zápis WAVE souborů použijte -ao pcm:waveheader (výchozí).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Selhalo otevření %s pro zápis!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] používám zvukový ovladač %s.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Nepodporovaný formát zvuku: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] Inicializace SDL Audio selhala: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Nelze otevřít zvuk: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] ovládání.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] init: Vzorkování: %iHz Kanál(y): %s Formát %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] přehrávání: neplatné zařízení.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] init: selhalo setparams: %s\nNelze nastavit požadované vzorkování.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] init: AL_RATE nebyl přijat daným zdrojem.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] init: selhalo getparams: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] init: vzorkování je nyní %f (požadovaný kmitočet je %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] init: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] init: Nelze otevřít zvukový kanál: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] uninit: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] reset: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume: ...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: selhalo SETINFO.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: zápis selhal.\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Nelze otevřít zvukové zařízení %s, %s  -> nebude zvuk.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] audio_setup: Vaše karta nepodporuje %d kanálové, %s, %d Hz vzorkování.\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] Neplatný index směšovačů. Používám výchozí 0.\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] Chyba otevření směšovače: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] Připojení směšovače %s skončilo chybou: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] Chyba registrace směšovače: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] Chyba načtení směšovače: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] Nemohu nalézt přepínač '%s',%i.\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] Chyba nastavení levého kanálu, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] Chyba nastavení pravého kanálu, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa nápověda:\n"\
"[AO_ALSA] Příklad: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   Nastaví první kartu čtvrté hardwarové zařízení.\n\n"\
"[AO_ALSA] Volby:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     Otevře zařízení v neblokujícím režimu.\n"\
"[AO_ALSA]   device=<název_zařízení>\n"\
"[AO_ALSA]     Nastaví zařízení (změňte , za . a : za =)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] %d kanálů není podporováno.\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] Otevření v neblokujícím režimu selhalo, zkouším otevřít v blokujícím.\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] Chyba otevření přehrávání: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] Chyba nastavení blokujícího režimu %s.\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] Nelze získat počáteční parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] Nelze nastavit typ přístupu: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] Formát %s není hardwarově podporován, zkouším výchozí.\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] Nelze nastavit formát: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] Nelze nastavit kanály: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] Nelze vypnout převzorkování: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] Nelze nastavit vzorkovací kmitočet-2: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] Nelze nastavit časový buffer okolo: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] Nelze získat velikost periody: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] Nelze nastavit periody: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] Nelze nastavit hw parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] Nelze získat velikost bufferu: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] Nelze získat sw parametry: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] Nelze získat boundary: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] Nelze nastavit startovací práh: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] Nelze nastavit stop práh: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] Nelze nastavit velikost ticha: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] chyba zavření pcm: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] Nebyl definován handler!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] chyba přípravy pcm: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] chyba pauzy pcm: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] chyba shození pcm: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] chyba obnovení pcm: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] Chyba konfigurace zařízení."
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm v režimu suspend, zkouším obnovit.\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] Chyba zápisu: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] Zkouším resetovat zvukovou kartu.\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] Nelze získat status pcm: %s\n"

// ao_plugin.c


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "dostupné názvy v"
#define MSGTR_AF_LADSPA_WarnNoInputs "VAROVÁNÍ! Tento LADSPA plugin nemá audio vstupy.\n  Vstupní audio signál bude ztracen."
#define MSGTR_AF_LADSPA_ErrNoOutputs "Tento LADSPA plugin nemá audio výstupy."
#define MSGTR_AF_LADSPA_ErrInOutDiff "Počet audio vstupů LADSPA pluginu je odlišný od počtu audio výstupů."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "selhalo načtení"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Nelze nalézt funkci ladspa_descriptor() v uvedené knihovně."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Nelze nalézt požadovaný název v knihovně pluginů."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Nebyla zadány žádné podvolby."
#define MSGTR_AF_LADSPA_ErrNoLibFile "Nebyla zadána žádná knihovna."
#define MSGTR_AF_LADSPA_ErrNoLabel "Nebyl zadán název žádného filtru."
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Na příkazovém řádku bylo uvedeno málo voleb."
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: Vstupní volič #%d je nižší než minimální hodnota %0.4f.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: Vstupní volič #%d je vyšší než maximální hodnota %0.4f.\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "neznámý formát "


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "Nelze otevřít zařízení joysticku %s: %s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "Chyba při čtení zařízení joysticku: %s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "Joystick: Uvolnili jsme %d bajtů dat\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "Joystick: warning init event, ztratili jsme synchronizaci s ovladačem.\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "Joystick: varování, neznámý typ události %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "Nelze otevřít Apple IR zařízení: %s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "Příliš mnoho souborových deskriptorů příkazů, nelze registrovat\n deskriptor souboru %d.\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "Příliš mnoho souborových deskriptorů klávesnice, nelze registrovat\n deskriptor souboru %d.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "Příkaz %s: argument %d není typu integer.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "Příkaz %s: argument %d není typu float.\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "Příkaz %s: argument %d není ukončen.\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "Neznámý argument %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "Příkaz %s vyžaduje aspoň %d argumentů, nalezli jsme jich však pouze %d.\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "Chyba při čtení příkazového deskriptoru souboru %d: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "Vyrovnávací paměť deskriptoru souboru příkazů %d je plná: zahazuji obsah.\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "Špatný příkaz pro přiřazení klávese %s"
#define MSGTR_INPUT_INPUT_ErrSelect "Chyba výběru: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "Chyba v deskriptoru souboru klávesového vstupu %d\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "Vstup mrtvé klávesy z deskriptoru souboru %d\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "Příliš mnoho současně stisknutých kláves\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "Chyba na cmd fd %d\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "Chyba při čtení input konfiguračního souboru %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "Neznámá klávesa '%s'\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "Příliš malá vyrovnávací paměť pro tento název klávesy: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "Nenalezen příkaz pro tlačítko %s"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "Příliš malá vyrovnávací paměť pro příkaz %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "Nelze inicializovat vstupní joystick\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "Nelze otevřít %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "Nelze inicializovat vstupní Apple Remote.\n"

// lirc.c
#define MSGTR_LIRCopenfailed "Selhalo otevření podpory LIRC. Nebudete moci používat dálkové ovládání.\n"
#define MSGTR_LIRCcfgerr "Nepovedlo se přečíst konfigurační soubor LIRC %s.\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c:
#define MSGTR_TooManyStreams "Příliš mnoho datových proudů!"
#define MSGTR_RawMuxerOnlyOneStream "Muxer surového zvuku podporuje pouze jeden zvukový proud!\n"
#define MSGTR_IgnoringVideoStream "Ignoruji video proud!\n"
#define MSGTR_UnknownStreamType "Varování, neznámý typ datového proudu: %d\n"
#define MSGTR_WarningLenIsntDivisible "Varování, délka není násobkem velikosti vzorku!\n"
#define MSGTR_MuxbufMallocErr "Nelze alokovat paměť pro snímkovou vyrovnávací paměť muxeru!\n"
#define MSGTR_MuxbufReallocErr "Nelze realokovat paměť pro snímkovou vyrovnávací paměť muxeru!\n"
#define MSGTR_WritingHeader "Zapisuji hlavičku...\n"
#define MSGTR_WritingTrailer "Zapisuji index...\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "VAROVÁNÍ: Hlavička audio proudu %d předefinována!\n"
#define MSGTR_VideoStreamRedefined "VAROVÁNÍ: Hlavička video proudu %d předefinována!\n"
#define MSGTR_TooManyAudioInBuffer "\nPříliš mnoho audio paketů ve vyrovnávací paměti: (%d v %d bajtech)\n"
#define MSGTR_TooManyVideoInBuffer "\nPříliš mnoho video paketů ve vyrovnávací paměti: (%d v %d bajtech)\n"
#define MSGTR_MaybeNI "Možná přehráváte neprokládaný proud/soubor nebo kodek selhal?\n"\
                      "V AVI souborech zkuste vynutit neprokládaný režim pomocí volby -ni.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: Obcházím chybu CBR-MP3 nBlockAlign header!\n"
#define MSGTR_SwitchToNi "\nDetekován špatně prokládaný AVI soubor - přepínám do -ni režimu...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: neplatný audio proud ID: %d - ignoruji (nebude zvuk)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: neplatný video proud ID: %d - ignoruji (používám výchozí)\n"
#define MSGTR_ON2AviFormat "ON2 AVI formát"
#define MSGTR_Detected_XXX_FileFormat "Detekován formát souboru %s.\n"
#define MSGTR_FormatNotRecognized "======= Bohužel, formát tohoto souboru nebyl rozpoznán/není podporován =======\n"\
                                  "==== Pokud je soubor AVI, ASF nebo MPEG proud, kontaktujte prosím autora! ====\n"
#define MSGTR_SettingProcessPriority "Nastavuji prioritu procesu: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] fmtsouboru:%d  fourcc:0x%X  velikost:%dx%d  fps:%5.3f  fčas:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Nelze inicializovat muxer."
#define MSGTR_MissingVideoStream "Nebyl nalezen video proud.\n"
#define MSGTR_MissingAudioStream "Nebyl nalezen audio proud -> bez zvuku.\n"
#define MSGTR_MissingVideoStreamBug "Chybí video proud!? Kontaktujte autora, může to být chyba :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: Soubor neobsahuje zvolený audio nebo video proud.\n"

#define MSGTR_NI_Forced "Vynucen"
#define MSGTR_NI_Detected "Detekován"
#define MSGTR_NI_Message "%s NEPROKLÁDANÝ formát AVI souboru.\n"

#define MSGTR_UsingNINI "Používám NEPROKLÁDANÉ vadné formátování AVI souboru.\n"
#define MSGTR_CouldntDetFNo "Nelze určit počet snímků (pro absolutní posun)\n"
#define MSGTR_CantSeekRawAVI "Nelze se posouvat v surových (raw) AVI proudech! (Potřebuji index, zkuste použít volbu -idx.)\n"
#define MSGTR_CantSeekFile "Nemohu se posouvat v tomto souboru.\n"

#define MSGTR_MOVcomprhdr "MOV: Komprimované hlavičky vyžadují ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: VAROVÁNÍ: Proměnná FourCC detekována!?\n"
#define MSGTR_MOVtooManyTrk "MOV: VAROVÁNÍ: příliš mnoho stop"
#define MSGTR_ErrorOpeningOGGDemuxer "Nelze otevřít Ogg demuxer.\n"
#define MSGTR_CannotOpenAudioStream "Nemohu otevřít audio proud: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Nemohu otevřít proud s titulky: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Nepovedlo se otevřít audio demuxer: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Nepovedlo se otevřít demuxer pro titulky: %s\n"
#define MSGTR_TVInputNotSeekable "TV vstup neumožňuje posun! (\"Posun\" bude pravděpodobně použit pro změnu kanálů ;)\n"
#define MSGTR_DemuxerInfoChanged "Info demuxeru %s změněno na %s\n"
#define MSGTR_ClipInfo "Informace o klipu:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: detekováno 30000/1001 fps NTSC, přepínám frekvenci snímků.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: detekováno 24000/1001 fps progresivní NTSC, přepínám frekvenci snímků.\n"

#define MSGTR_CacheFill "\rNaplnění cache: %5.2f%% (%"PRId64" bajtů)   "
#define MSGTR_NoBindFound "Tlačítko '%s' nemá přiřazenu žádnou funkci.\n"
#define MSGTR_FailedToOpen "Selhalo otevření %s.\n"

#define MSGTR_VideoID "Nalezen video proud [%s], -vid %d\n"
#define MSGTR_AudioID "Nalezen audio proud [%s], -aid %d\n"
#define MSGTR_SubtitleID "Nalezen titulkový proud [%s], -sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "FATAL: velikost hlavičky je větší než 1 MB (%d)!\nKontaktujte prosím tvůrce MPlayeru a nahrajte/pošlete jim tento soubor.\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "Nemohu alokovat %d bajtů pro hlavičku.\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "konec souboru při čtení ASF hlavičky, poškozený/neúplný soubor?\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR bude pravděpodobně pracovat pouze s libavformat, v případě problémů zkuste -demuxer 35\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "Po hlavičce nenásleduje žádný datový chunk!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: ani audio ani video hlavičky nebyly nalezeny - vadný soubor?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "Nesprávná délka v hlavičce ASF!\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "DRM Licence URL: %s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "Tento soubor byl zatížen DRM šifrou, v MPlayeru nelze přehrát!\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "** prázdný seznam?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** Varování: toto není rozšířená AVI hlavička..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: Vytvářím ODML index (%d superindexchunků).\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: Detekován vadný (neúplný?) soubor. Použije se tradiční index.\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "Nelze číst indexový soubor %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s není planý indexový soubor pro MPlayer.\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "Nemohu alokovat paměť pro data indexu od %s.\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "předčasný konec indexového souboru %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "Nahrán indexový soubor: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "Generuji index: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: Vygenerována tabulka indexu pro %d chunků!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "Nelze zapsat indexový soubor %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "Uložen indexový soubor: %s\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "Audio demuxer: neznámý formát %d.\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "fill_buffer chyba: špatný demuxer: ani vd, ad nebo sd.\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] inicializace zlib selhala.\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib dekomprese selhala.\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo dekomprese selhala.\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] Stopa číslo %u byla zašifrována a dešifrování nebylo dosud\n[mkv] implementováno. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] Nezanámý typ enkódování stopy %u. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] Stopa %u byla komprimována neznámým/nepodporovaným kompresním\n[mkv] algoritmem (%u). Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] Stopa %u byla komprimována pomocí zlib, ale MPlayer nebyl kompilován\n[mkv] s podporou zlib komprese. Přeskakuji stopu.\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] Stopa ID %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] Stopa ID %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] Neznámé/nepodporované ID kodeku (%s) nebo chybějící/vadná CodecPrivate\n[mkv] data (stopa %u).\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] Stopa FLAC neobsahuje platné hlavičky.\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] Neznámé/nepodporované ID audio kodeku '%s' pro stopu %u nebo chybějící/chybná\n[mkv] private codec data.\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] Typ titulků '%s' není podporován.\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] Budu přehrávat video stopu %u.\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] Žádná video stopa nenalezena/požadována.\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] Žádná audio stopa nenalezena/požadována.\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] Varování: Nebylo nalezeno žádné BlockDuration pro titulkovou stopu.\n"

// demux_nuv.c

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "Nalezen plugin: %s (%s).\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "Uzavírám plugin: %s.\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "Čekám až XMMS plugin zahájí přehrávání '%s'...\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] V definici menu není žádná položka.\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] syntaktická chyba na řádku: %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] V definici menu je potřeba jmenný atribut (řádek %d)\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] špatný atribut %s=%s v menu '%s' na řádku %d\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] neznámý typ menu '%s' na řádce %d\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] Nemohu otevřít konfigurační soubor menu: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] Konfigurační soubor je příliš velký. (> %d KB)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] Konfigurační soubor je prázdný.\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] Menu %s nebylo nalezeno.\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] Menu '%s': Selhala inicializace.\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] Nepodporovaný výstupní formát!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] Položky typu seznam vyžadují název (řádek %d).\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] Položka typu seznam vyžaduje argument.\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Chyba při čekání na PID: %s.\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Chyba výběru.\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] Chyba čtení na popisovači souboru potomka: %s.\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] Spuštění v konsoli: %s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] Potomek už běží.\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Forkování selhalo!!!\n"
#define MSGTR_LIBMENU_WriteError "[MENU] chyba při zápisu.\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] chyba při otevírání adresáře: %s\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] chyba při relokaci: %s\n"
#define MSGTR_LIBMENU_MallocError "[MENU] chyba při alokaci paměti: %s\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] chyba čtení adresáře: %s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] Nelze otevřít adresář %s.\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] Při definici podmenu je potřeba uvést atribut 'menu'.\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] Neplatná vlastnost '%s' v pref menu. (řádek %d).\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] Preferenční položka menu vyžaduje korektní atribut 'property' nebo 'txt' (řádek %d).\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] Preferenční menu vyžaduje argument.\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] Nemohu nalézt cílovou položku??\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] Selhalo sestavení příkazu: %s.\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] Textové menu vyžaduje název souboru txt (parametrický soubor).\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] Nelze otevřít: %s.\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] Pozor, řádka je příliš dlouhá. Rozděluju ji.\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] Zpracováno %d řádků.\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] Neznámý příkaz: '%s'.\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] Nemohu otevřít menu: '%s'.\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c
#define MSGTR_CantOpenCodec "Nelze otevřít kodek.\n"
#define MSGTR_CantCloseCodec "Nelze uzavřít kodek.\n"

#define MSGTR_MissingDLLcodec "CHYBA: Nelze otevřít požadovaný DirectShow kodek %s.\n"
#define MSGTR_ACMiniterror "Nemohu načíst/inicializovat Win32/ACM audio kodek (chybí DLL soubor?).\n"
#define MSGTR_MissingLAVCcodec "Nemohu najít kodek '%s' v libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: KRITICKÁ CHYBA: Konec souboru v průběhu vyhledávání hlavičky sekvence.\n"
#define MSGTR_CannotReadMpegSequHdr "KRITICKÁ CHYBA: Nelze přečíst hlavičku sekvence.\n"
#define MSGTR_CannotReadMpegSequHdrEx "KRITICKÁ CHYBA: Nelze přečíst rozšíření hlavičky sekvence.\n"
#define MSGTR_BadMpegSequHdr "MPEG: Špatná hlavička sekvence.\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Špatné rozšíření hlavičky sekvence.\n"

#define MSGTR_ShMemAllocFail "Nelze alokovat sdílenou paměť\n"
#define MSGTR_CantAllocAudioBuf "Nelze alokovat vyrovnávací paměť pro zvukový výstup\n"

#define MSGTR_UnknownAudio "Neznámý/chybějící audio formát -> nebude zvuk.\n"

#define MSGTR_UsingExternalPP "[PP] Používám externí filtr pro postprocessing, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Používám integrovaný postprocessing kodeku, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Požadovaná rodina video kodeku [%s] (vfm=%s) není dostupná.\nAktivujte ji při kompilaci.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Požadovaná rodina audio kodeku [%s] (afm=%s) není dostupná.\nAktivujte ji při kompilaci.\n"
#define MSGTR_OpeningVideoDecoder "Otevírám video dekodér: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Vybrán video kodek: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Otevírám audio dekodér: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Vybrán audio kodek: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "Video dekodér - inicializace selhala :(\n"
#define MSGTR_ADecoderInitFailed "Audio dekodér - inicializace selhala :(\n"
#define MSGTR_ADecoderPreinitFailed "Audio dekodér - předinicializace selhala :(\n"

// ad_dvdpcm.c:
#define MSGTR_SamplesWanted "Vzorky tohoto formátu potřebujeme pro zlepšení podpory. Kontaktujte prosím vývojáře.\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] Varování! Velikost rámce zvuku se liší! přečteno=%d  hlavička=%d.\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Nemohu nalézt společný barevný prostor - zkouším to znovu s -vf scale...\n"
#define MSGTR_MovieAspectIsSet "Poměr stran obrazu filmu je %.2f:1 - škáluji na správný poměr.\n"
#define MSGTR_MovieAspectUndefined "Poměr stran obrazu filmu není definován - neměním velikost.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Potřebujete aktualizovat nebo nainstalovat binární kodeky.\nJděte na http://www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] Nemohu alokovat obraz pro kodek cinepak.\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] Aritmetický průměr QP: %2.4f, harmonický průměr QP: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI selhalo.\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] Nemohu alokovat obraz pro kodek.\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] XVMC-akcelerovaný MPEG-2.\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] Zkouším pixfmt=%d.\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] mc_get_buffer by mělo fungovat jen s XVMC akcelerací!"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] Povoleny jsou jen buffery alokované pomocí vo_xvmc.\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] Vybráno vysoce kvalitní kódování (nebude probíhat v reálném čase)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] Používám konstantní qscale = %f (VBR).\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] Surový výstup s fourcc [%x] není podporován!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] Požadovaný VfW kodek nebyl specifikován!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Nemohu nalézt video filtr '%s'\n"
#define MSGTR_CouldNotOpenVideoFilter "Nemohu otevřít video filtr '%s'\n"
#define MSGTR_OpeningVideoFilter "Otevírám video filtr: "
#define MSGTR_CannotFindColorspace "Ani při vložení 'scale' nemohu nalézt společný barevný prostor :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] Špatná pozice/šířka/výška - ořezová oblast zasahuje mimo originál!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] Ořezová oblast: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d).\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] Neznámý název formátu: '%s'.\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] Chyba při zpracování argumentu.\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "Typ komprese: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "Podtyp komprese: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "Příznaky kompresoru: %lu, verze %lu, verze ICM: %lu\n"
#define MSGTR_MPCODECS_Flags "Příznaky:"
#define MSGTR_MPCODECS_Quality " kvalita"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "Plný DR není možný, zkouším místo něj SLICES!\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "Proč jsme dostali NULL??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "%s není dalším filtrem/vo podporován :(\n"

// ================================== LIBASS ====================================

// ass_bitmap.c

// ass.c
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): fopen selhalo\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): Odmítám nahrát titulky delší než 100M\n"

// ass_cache.c

// ass_fontconfig.c

// ass_render.c

// ass_font.c


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "Nelze nastavit vzorkovací kmitočet.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "Nelze nastavit čas bufferu.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "Nelze nastavit čas periody.\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "Vadná konfigurace pro toto PCM: žádná dostupná konfigurace.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "Nedostupný typ přístupu.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "Nedostupný formát vzorku.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "Nedostupný počet kanálů - použiji výchozí: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "Nelze instalovat hardwarové paramametry: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "Nelze použít periodu odpovídající velikosti bufferu (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "Nelze instalovat softwarové parametry:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "Nelze otevřít audio: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (aspoň %.3f ms dlouhý)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: připravuji chybu: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA chyba čtení/zápisu"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "Nelze nastavit počet kanálů: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "Nelze nastavit stereo: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "Nelze otevřít '%s': %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "nepodporovaný formát\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "Nelze nastavit audio formát."
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "Nelze nastavit vzorkovací kmitočet: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "Nelze nastavit spoušť: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "Nelze zjistit velikost bloku!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "Velikost zvukového bloku je nulová, nastavuji ji na %d!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "Velikost zvukového bloku je příliš malá, nastavuji ji na %d!\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "chyba zápisu\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\nVýstraha! EOF\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "čtení pre-hlavičky selhalo\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "Špatná velikost hlavičky, vzdávám to.\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "Čtení dat hlavičky selhalo.\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "Selhalo čtení packet_len.\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "Špatná velikost RTSP paketu, vzdávám to.\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "Selhalo čtení příkazových dat.\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "hlavičkový objekt\n"
#define MSGTR_MPDEMUX_MMST_DataObject "datový objekt\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "souborový objekt, délka paketu = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "proudový objekt, ID datového proudu: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "Příliš mnoho ID, proud přeskočen."
#define MSGTR_MPDEMUX_MMST_UnknownObject "neznámý objekt\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "Čtení media dat selhalo.\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "chybí signatura\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "Vše hotovo. Děkujeme, že jste si stáhli mediální soubor obsahující proprietární a patentovanou technologii.\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "neznámý příkaz %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "chyba get_media_packet: %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "Připojeno\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "Ahhhh, velikost stream_chunck je příliš malá: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "size_confirm nesouhlasí!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "Varování: zahozena hlavička ????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "Chyba při parsování hlavičky chunku\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "Hlavička nedošla jako první chunk !!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "Chyba: nelze alokovat %d bajtů vyrovnávací paměti.\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "Chyba při čtení proudu ze sítě.\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "Chyba: chunk je příliš malý.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "Příliš malá přenosová rychlost, soubor nelze přehrávat!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "Příliš malá přenosová rychlost, odvolaný audio proud.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "Příliš malá přenosová rychlost, odvolaný video proud.\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "Nesprávná délka v ASF hlavičce!\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "Chyba: chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF Redirector\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "neplatná proxy URL\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "neznámý typ ASF proudu\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "Selhalo parsování HTTP odpovědi.\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "Server vrátil %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP PARSE VAROVÁNÍ: Pragma %s zkrácena z %zu bajtů na %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Chyba zápisu soketu: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "Selhalo parsování hlavičky\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "Nenalezen datový proud\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "Neznámý typ ASF proudu\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "Selhalo, končím.\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\nChyba při čtení audia: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "Zotaveno z cross-run, některé snímky mohly být vynechány!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "Kritická chyba, nelze zotavit!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\nNedostatek audio vzorků!\n"

// cache2.c

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "Neznámá rodina adres %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "Resolvuji %s pro %s...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "Nelze resolvovat jméno pro %s: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "Připojuji se k serveru %s[%s]: %d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "Selhalo připojení k serveru pomocí %s\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "Select selhal.\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "spojení vypršelo\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt selhal: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "chyba spojení: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "Špatné nastavení proxy... Zkouším bez proxy.\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "Nelze resolvovat jméno vzdáleného systému pro AF_INET. Zkouším bez proxy.\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "Chyba při odesílání HTTP požadavku: Nebyl odeslán celý požadavek.\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "Chyba při čtení.\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response přečetlo 0 (to je EOF).\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "Autentifikace selhala. Použijte volby -user a -passwd pro zadání svého\n"\
"uživatelského_jména/hesla pro seznam URL, nebo URL v následující formě:\n"\
"http://uživatelské_jméno:heslo@jméno_serveru/soubor\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "Pro %s je vyžadována autentifikace\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "Vyžadována autentifikace.\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "Nezadáno heslo, zkouším prázdné heslo.\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "Server vrátil %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "Vyrovnávací paměť nastavena na %d KBajtů\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM zařízení '%s' nebylo nalezeno.\n"
#define MSGTR_ErrTrackSelect "Chyba při výběru VCD stopy."
#define MSGTR_ReadSTDIN "Čtu ze std. vstupu...\n"
#define MSGTR_FileNotFound "Soubor nenalezen: '%s'\n"

#define MSGTR_SMBInitError "Nelze inicializovat knihovnu libsmbclient: %d\n"
#define MSGTR_SMBFileNotFound "Nemohu otevřít soubor ze sítě: '%s'\n"

#define MSGTR_CantOpenDVD "Nelze otevřít DVD zařízení: %s (%s)\n"

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "Nelze otevřít CDDA zařízení.\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "Nelze otevřít disk.\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "Nalezeno audio CD s %d stopami\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "Selhalo čtení TOC.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "Selhalo otevření zařízení %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "neplatná URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "Selhalo odeslání HTTP požadavku.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "Selhalo čtení HTTP odpovědi.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "Není k dispozici.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "neznámý error kód\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "Vyrovnávací paměť nenalezena.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "Nebyl přečten celý xmcd soubor.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "Selhalo vytvoření adresáře %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "Nebyl zapsán celý xmcd soubor.\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "Vrácen chybný soubor xmcd databáze.\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "neočekávané UROB-SI-SÁM\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "neošetřený kód\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "Nelze nalést konec řádku.\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "Parsování OK, nalezeno: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "Album nenalezeno.\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "Server vrátil: Syntaktická chyba příkazu\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "Selhalo získání úrovně protokolu.\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "V mechanice není CD.\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] Neočekávaný řádek v cue souboru: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] otestován bin soubor: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] Nelze nalézt bin soubor - vzdávám to.\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] Používám bin soubor %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] neznámý režim pro bin soubor. To by se nemělo stát. Končím.\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] Nelze otevřít %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] Chyba čtení z  %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] Chyba získání velikosti bin souboru.\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "stopa %02d:  format=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] neočekávaný konec bin souboru\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] Nelze přečíst %d bajtů 'payloadu'.\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE stream_open, soubor=%s, stopa=%d, dostupné stopy: %d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "Nemohu otevřít DVD zařízení pro zápis, změna DVD rychlosti vyžaduje právo zápisu.\n"
#define MSGTR_DVDrestoreSpeed "Obnovuji DVD rychlost... "
#define MSGTR_DVDlimitSpeed "Omezuji DVD rychlost na %dKB/s... "
#define MSGTR_DVDlimitFail "selhalo\n"
#define MSGTR_DVDlimitOk "úspěch\n"
#define MSGTR_NoDVDSupport "MPlayer byl zkompilován bez podpory DVD, končím.\n"
#define MSGTR_DVDnumTitles "Na tomto DVD je %d titul(ů).\n"
#define MSGTR_DVDinvalidTitle "Neplatné číslo DVD titulu: %d\n"
#define MSGTR_DVDinvalidChapterRange "Nesprávně nastavený rozsah kapitol %s\n"
#define MSGTR_DVDnumAngles "Tento DVD titul má %d úhlů pohledu.\n"
#define MSGTR_DVDinvalidAngle "Neplatné číslo DVD úhlu pohledu: %d\n"
#define MSGTR_DVDnoIFO "Nelze otevřít IFO soubor pro DVD titul %d.\n"
#define MSGTR_DVDnoVMG "Nelze otevřít VMG info!\n"
#define MSGTR_DVDnoVOBs "Nelze otevřít VOBy titulu (VTS_%02d_1.VOB).\n"
#define MSGTR_DVDnoMatchingAudio "DVD zvuk v požadovaném jazyce nebyl nalezen!\n"
#define MSGTR_DVDaudioChannel "Vybrán DVD zvukový kanál: %d jazyk: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "audio proud: %d formát: %s (%s) jazyk: %s aid: %d.\n"
#define MSGTR_DVDnumAudioChannels "počet zvukových kanálů na disku: %d.\n"
#define MSGTR_DVDnoMatchingSubtitle "DVD titulky v požadovaném jazyce nebyly nalezeny!\n"
#define MSGTR_DVDsubtitleChannel "Vybrán DVD titulkový kanál: %d jazyk: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "titulky ( sid ): %d jazyk: %s\n"
#define MSGTR_DVDnumSubtitles "počet sad titulků na disku: %d\n"

// stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] Detekovány názvy stanic.\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] Nesprávná frekvence pro stanici %s\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] Nesprávné číslo kanálu: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] Nesprávné číslo kanálu: %d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] Nesprávné jméno kanálu: %s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] Radio parametr detekován jako frekvence.\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Varování: ioctl get tuner selhala: %s. Nastavuji frac na %d.\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s není rádiovým zařízením!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl set frequency 0x%x (%.2f) selhala: %s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl get frequency selhala: %s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl set mute selhala: %s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl query control selhala: %s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl get volume selhala: %s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl set volume selhala: %s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] nemohu alokovat vyrovnávací paměť zvuku (blok=%d,buf=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] Současná frekvence: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] Zvolený kanál: %d - %s (frekv: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] Nelze změnit kanál: nezadán seznam kanálů.\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] Nelze otevřít '%s': %s\n"
#define MSGTR_RADIO_WrongFreq "[radio] Špatná frekvence: %.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] Používám frekvuenci: %.2f.\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init selhala.\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] volání audio_in_setup selhalo: %s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] Vypráznění vyrovnávací paměti selhalo: %s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] Volání do stream_enable_cache selhalo: %s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] Neznámé jméno ovladače: %s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] Používám V4Lv2 rádio rozhraní.\n"
#define MSGTR_RADIO_DriverV4L "[radio] Používám V4Lv1 rádio rozhraní.\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] Používám *BSD BT848 rádio rozhraní.\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c: norm_from_string(%s): Bogus norm parametr, nastaveno %s.\n"
#define MSGTR_TV_NoVideoInputPresent "Chyba: Žádný video vstup není k dispozici!\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" VAROVÁNÍ: VYŽADOVÁN NEZNÁMÝ NEBO NETESTOVANÝ FORMÁT OBRAZU (0x%x)\n"\
" To může způsobit vadné přehrávání, nebo pád programu! Hlášení\n"\
" chyb budou ignorována. Měli byste zkusit YV12 (což je výchozí\n"\
" barevný prostor) a přečíst si dokumentaci!\n"\
"==================================================================\n"
#define MSGTR_TV_CannotSetNorm "Chyba: Nelze nastavit normu!\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP: šířka %d výška %d\n"
#define MSGTR_TV_UnableToSetWidth "Nelze nastavit požadovanou šířku: %d\n"
#define MSGTR_TV_UnableToSetHeight "Nelze nastavit požadovanou výšku: %d\n"
#define MSGTR_TV_NoTuner "Vybraný vstup nemá tuner!\n"
#define MSGTR_TV_UnableFindChanlist "Nelze nalézt vybraný seznam kanálů! (%s)\n"
#define MSGTR_TV_ChannelFreqParamConflict "Nemůžete nastavit kmitočet a kanál současně!\n"
#define MSGTR_TV_ChannelNamesDetected "Detekovány názvy TV kanálů.\n"
#define MSGTR_TV_NoFreqForChannel "Nelze nalézt kmitočet pro kanál %s (%s)\n"
#define MSGTR_TV_SelectedChannel3 "Zvolený kanál: %s - %s (kmit: %.3f)\n"
#define MSGTR_TV_SelectedChannel2 "Zvolený kanál: %s (kmit: %.3f)\n"
#define MSGTR_TV_UnsupportedAudioType "Audio typu '%s (%x)' nepodporováno!\n"
#define MSGTR_TV_AvailableDrivers "Dostupné ovladače:\n"
#define MSGTR_TV_DriverInfo "Zvolený ovladač: %s\n název: %s\n autor: %s\n popis: %s\n"
#define MSGTR_TV_NoSuchDriver "Chybí ovladač: %s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "Autodetekce TV ovladače selhala.\n"
#define MSGTR_TV_UnknownColorOption "Zadána neznámá color volba (%d)!\n"
#define MSGTR_TV_NoTeletext "Žádný teletext"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848: Volání %s ioctl selhalo. Chyba: %s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848: Špatný vzorkovací kmitočet zvuku. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848: Nelze otevřít bktr zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848: Nelze otevřít tuner zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848: Nelze otevřít dsp zařízení. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848: Konfigurace dsp selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848: Chyba čtení audio dat. Chyba: %s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848: mmap selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848: Alokace Frame bufferu selhala. Chyba: %s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848: Chyba nastavení šířky obrazu. Chyba: %s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848: Nelze zastavit zachytávání. Chyba: %s\n"
#define MSGTR_TV_TTSupportedLanguages "Podporované jazyky teletextu:\n"
#define MSGTR_TV_TTSelectedLanguage "Vybraný výchozí jazyk teletextu: %s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "Vyhledávání kanálů není bez tuneru možné\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "Nelze připojit zadaný vstup k video dekodéru. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "Nelze připojit zadaný vstup k audio dekodéru. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow: Nelze zvolit video formát. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow: Nelze zvolit audio formát. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow: Nelze získat IMediaControl rozhraní. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow: Nelze spustit graph! Chyba:0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow: Zařízení #%d nenalezeno\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow: Nelze získat název pro zařízení #%d\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow: Používám zařízení #%d: %s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow: Nelze získat kmitočet přímo. Použije se tabulka kanálů z OS.\n"
//following phrase will be printed near the selected audio/video input
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow: Nelze nahrát kmitočtovou tabulku z kstvtune.ax\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow: Špatný parametr zařízení: %s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow: Špatný index zařízení: %d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow: Špatný parametr audio zařízení: %s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow: Špatný index audio zařízení: %d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow: Vzorkovací kmitočet %d zařízení nepodporuje. Měním na první dostupný.\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow: Nastavování jasu/tónu/sytosti/kontrastu zařízení nepodporuje.\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow: Změnu výšky/šířky videa zařízení nepodporuje.\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow: Volbu zdroje pro zachytávání zařízení nepodporuje.\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow: Nelze parsovat strukturu audio formátu.\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow: Nelze parsovat strukturu video formátu.\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow: Nelze nastavit audio režim %d. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow: Nepodporovaný typ média předán do %s\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow: Nelze najít nejbližší kanál v systémové kmitočtové tabulce\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow: Nelze přepnout na nejbližší kanál ze systémové kmitočtové tabulky. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow: Nelze ukončit VideoPort pin s jakýmkoli filtrem v graphu. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow: Nelze sestavit video chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow: Nelze sestavit audio chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow: Nelze sestavit VBI chain of capture graph. Chyba:0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow: Inicializace directshow graph selhala.\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow: Nelze najít zachytávací video zařízení\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow: Nelze najít zachytávací audio zařízení\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow: Nelze získat činný typ média (Chyba:0x%x). Předpokládám shodu s požadovaným.\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "Řetězec se zdá být již eskejpován v url_escape %c%c%c\n"
