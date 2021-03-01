// Last sync on 2004-10-20 with help_mp-en.h 1.148
// Translated by:  Carl Fürstenberg <azatoth AT gmail DOT com>
// Helped by: Jan Knutar <jknutar AT nic DOT fi>
// ========================= MPlayer hjälp ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<spårnr>      spela (S)VCD (Super Video CD) spår (rå enhet, ingen montering)\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<titlenr>     spela DVD titel från enhet istället för ifrån en enkel fil\n"\
                      " -alang/-slang       välj DVD audio/textningsspråk (m.h.a. ett 2-teckens landskod)\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Användning:   mplayer [argument] [url|sökväg/]filnamn\n"\
"\n"\
"Grundläggande argument: (komplett lista återfinns i `man mplayer`)\n"\
" -vo <drv[:enhet]>   välj video-ut drivrutin & enhet ('-vo help' för lista)\n"\
" -ao <drv[:enhet]>   välj audio-ut drivrutin & enhet ('-ao help' för lista)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -ss <tidpos>        sök till given position (sekunder eller hh:mm:ss)\n"\
" -nosound            spela inte upp ljud\n"\
" -fs                 fullskärmsuppspelning (eller -vm, -zoom, detaljer på manualsidan)\n"\
" -x <x> -y <y>       sätt skärmupplösning (för användning med -vm eller -zoom)\n"\
" -sub <fil>          specifiera textningsfil att använda (se också -subfps, -subdelay)\n"\
" -playlist <fil>     specifiera spellistefil\n"\
" -vid x -aid y       välj video (x) och audio (y) ström att spela\n"\
" -fps x -srate y     ändra video (x fps) och audio (y Hz) frekvens\n"\
" -pp <kvalité>       aktivera postredigeringsfilter (detaljer på manualsidan)\n"\
" -framedrop          aktivera reducering av antalet bildrutor (för långsamma maskiner)\n"\
"\n"\
"Grundläggande navigering: (komplett lista återfinns på manualsidan, läs även input.conf)\n"\
" <-  eller  ->       sök bakåt/framåt 10 sekunder\n"\
" upp eller ner       sök bakåt/framåt 1 minut\n"\
" pgup eller pgdown   sök bakåt/framåt 10 minuter\n"\
" < eller >           stega bakåt/framåt i spellistan\n"\
" p eller SPACE       pausa filmen (tryck på valfri tagent för att fortsätta)\n"\
" q eller ESC         stanna spelningen och avsluta programmet\n"\
" + eller -           ställ in audiofördröjning med ± 0.1 sekund\n"\
" o                   växla OSD läge:  ingen / lägesindikator / lägesindikator + tidtagare\n"\
" * eller /           öka eller sänk PCM-volym\n"\
" z eller x           ställ in textningsfördröjning med ± 0.1 sekund\n"\
" r or t              ställ in textningsposition upp/ner, se också '-vf expand'\n"\
"\n"\
" * * * LÄS MANUALEN FÖR FLER DETALJER, MER AVANCERADE ARGUMENT OCH KOMMANDON * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c:

#define MSGTR_Exiting "\nStänger ner...\n"
#define MSGTR_ExitingHow "\nStänger ner... (%s)\n"
#define MSGTR_Exit_quit "Avsluta"
#define MSGTR_Exit_eof "Slut på fil"
#define MSGTR_Exit_error "Oöverkomligt fel"
#define MSGTR_IntBySignal "\nMPlayer var avbruten av signal %d i modul: %s\n"
#define MSGTR_NoHomeDir "Kan inte lokalisera $HOME-katalog.\n"
#define MSGTR_GetpathProblem "get_path(\"config\") problem\n"
#define MSGTR_CreatingCfgFile "Skapar konfigfil: %s\n"
#define MSGTR_CantLoadFont "Kan inte ladda font: %s\n"
#define MSGTR_CantLoadSub "Kan inte ladda vald textning: %s\n"
#define MSGTR_DumpSelectedStreamMissing "dump: FATALT: Vald ström ej tillgänglig!\n"
#define MSGTR_CantOpenDumpfile "Kan inte öppna dumpfil.\n"
#define MSGTR_CoreDumped "Core dumpad ;)\n"
#define MSGTR_FPSnotspecified "FPS ej specifierad i filhuvudet eller är icke godkänd, använd argument -fps.\n"
#define MSGTR_TryForceAudioFmtStr "Försöker att forcera audiocodecfamilj %s...\n"
#define MSGTR_CantFindAudioCodec "Kan inte finna codec för audioformat 0x%X.\n"
#define MSGTR_TryForceVideoFmtStr "Försöker att forcera videocodecfamilj %s...\n"
#define MSGTR_CantFindVideoCodec "Kan inte finna codec för vald -vo och videoformat 0x%X.\n"
#define MSGTR_CannotInitVO "FATALT: Kan inte initiera videodrivrutin.\n"
#define MSGTR_CannotInitAO "Kan inte öppna/initiera audioenhet -> inget ljud.\n"
#define MSGTR_StartPlaying "Påbörjar uppspelning...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"           ***********************************************************\n"\
"           **** Ditt system är för slött för att spela upp detta! ****\n"\
"           ***********************************************************\n\n"\
"Troliga orsaker, problem, samt sätt att fixa det:\n"\
"- Troligast: trasig/buggig _audio_drivrutin\n"\
"  - Försök -ao sdl eller använd OSS-emulatorn i ALSA.\n"\
"  - Experimentera med olika värden för -autosync, 30 är en bra start.\n"\
"- Seg video-ut\n"\
"  - Försök en annan -vo drivrutin (-vo help för en lista) eller försök -framedrop!\n"\
"- Seg CPU\n"\
"  - Försök att inte spela upp allt för stora DVD/DivX på en seg CPU! Testa med -hardframedrop.\n"\
"- Trasig fil\n"\
"  - Försök med olika kombinationer av -nobps -ni -forceidx -mc 0.\n"\
"- Segt media (NFS/SMB mounts, DVD, VCD etc.)\n"\
"  - Försök med -cache 8192.\n"\
"- Använder du -cache till att spela upp en ickeinterleaved AVIfil?\n"\
"  - Försök -nocache.\n"\
"Läs DOCS/HTML/en/video.html för optimeringstips.\n"\
"Om inget av dessa hjälper, läs DOCS/HTML/en/bugreports.html.\n\n"

#define MSGTR_NoGui "MPlayer var kompilerad UTAN GUI-support.\n"
#define MSGTR_GuiNeedsX "MPlayer GUI kräver X11.\n"
#define MSGTR_Playing "Spelar %s.\n"
#define MSGTR_NoSound "Audio: inget ljud\n"
#define MSGTR_FPSforced "FPS forcerad att vara %5.3f  (ftime: %5.3f).\n"
#define MSGTR_AvailableVideoOutputDrivers "Tillgängliga video-ut-drivrutiner:\n"
#define MSGTR_AvailableAudioOutputDrivers "Tillgängliga audio-ut-drivrutiner:\n"
#define MSGTR_AvailableAudioCodecs "Tillgängliga audiocodec:\n"
#define MSGTR_AvailableVideoCodecs "Tillgängliga videocodec:\n"
#define MSGTR_AvailableAudioFm "Tillgängliga (inkompilerade) audiocodec familjer/drivrutiner:\n"
#define MSGTR_AvailableVideoFm "Tillgängliga (inkompilerade) videocodec familjer/drivrutiner:\n"
#define MSGTR_AvailableFsType "Tillgängliga lägen för fullskärmslager:\n"
#define MSGTR_CannotReadVideoProperties "Video: Kan inte läsa inställningar.\n"
#define MSGTR_NoStreamFound "Ingen ström funnen.\n"
#define MSGTR_ErrorInitializingVODevice "Fel vid öppning/initiering av vald video_out-enhet (-vo).\n"
#define MSGTR_ForcedVideoCodec "Forcerad videocodec: %s\n"
#define MSGTR_ForcedAudioCodec "Forcerad audiocodec: %s\n"
#define MSGTR_Video_NoVideo "Video: ingen video\n"
#define MSGTR_NotInitializeVOPorVO "\nFATALT: Kunde inte initiera videofilter (-vf) eller video-ut (-vo).\n"
#define MSGTR_Paused "  =====  PAUSE  =====" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\nOförmögen att ladda spellista %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer krachade av en 'Illegal Instruction'.\n"\
"  Det kan vare en bugg i vår nya \"runtime CPU-detection\" kod...\n"\
"  Var god läs DOCS/HTML/en/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer krashade av en 'Illegal Instruction'.\n"\
"  Detta händer vanligast om du kör koden på en annan CPU än den var\n"\
"  kompilerad/optimerad för\n"\
"  Verifiera detta!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer krashade på grund utav dålig användning av CPU/FPU/RAM.\n"\
"  Omkompilera MPlayer med '--enable-debug' och kör en \"'gdb' backtrace\" och\n"\
"  deassemblera. Detaljer återfinns i DOCS/HTML/en/bugreports_what.html#bugreports_crash.\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer krashade. Detta borde inte inträffa.\n"\
"  Det kan vara en bugg i MPlayers kod, eller i din drivrutin, eller i din\n"\
"  gcc version. Om du tror det är MPlayers fel, var vänlig läs\n"\
"  DOCS/HTML/en/bugreports.html och följ instruktionerna där, Vi kan inte och\n"\
"  kommer inte att hjälpa dig, om du inte kan befodra denna information när \n"\
"  du rapporterar en trolig bugg.\n"
#define MSGTR_LoadingConfig "Laddar konfiguration '%s'\n"
#define MSGTR_AddedSubtitleFile "SUB: lade till textningsfil %d: %s \n"
#define MSGTR_RTCDeviceNotOpenable "Misslyckades att öppna %s: %s (den borde vara läsbar av användaren.)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "'Linux RTC' initieringsfel i 'ioctl' rtc_irqp_set %lu: %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Försök lägg till \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" till ditt systems uppstartningsscript.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "'Linux RTC init' fel i 'ioctl' [rtc_pie_on]: %s\n"
#define MSGTR_Getch2InitializedTwice "VARNING: getch2_init anropad dubbelt!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Kan inte öppna 'libmenu video filter' med rotmeny %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Fel vid förinitiering av audiofilter!\n"
#define MSGTR_LinuxRTCReadError "'Linux RTC' läsfel: %s\n"
#define MSGTR_SoftsleepUnderflow "Varning! Softsleep underflow!\n"
/*
 * FIXME A lot of shorted words, not translating atm
 */

#define MSGTR_EdlOutOfMem "Kan inte allokera tillräckligt med minne för att hålla EDL-data.\n"
#define MSGTR_EdlRecordsNo "Läst %d EDL-funtioner.\n"
#define MSGTR_EdlQueueEmpty "Det är inga EDL-funktioner att ta hand om.\n"
#define MSGTR_EdlCantOpenForWrite "Kan inte öppna EDL-fil [%s] för skrivning.\n"
#define MSGTR_EdlNOsh_video "Kan inte använda EDL utan video, inaktiverar.\n"
#define MSGTR_EdlNOValidLine "Icke godkänd EDL-rad: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Dåligt formaterad EDL-rad [%d]. Kastar bort.\n"
#define MSGTR_EdlBadLineOverlap "Senaste stopposition var [%f] ; nästa start är [%f]. Noteringar måste vara i kronologisk ordning, kan inte lappa över. Kastar bort.\n"
#define MSGTR_EdlBadLineBadStop "Stopptid måste vara efter starttid.\n"


// mencoder.c:

#define MSGTR_UsingPass3ControlFile "Använder pass3-kontrollfil: %s\n"
#define MSGTR_MissingFilename "\nFilnamn saknas.\n\n"
#define MSGTR_CannotOpenFile_Device "Kan inte öppna fil/enhet.\n"
#define MSGTR_CannotOpenDemuxer "Kan inte öppna demuxer.\n"
#define MSGTR_NoAudioEncoderSelected "\nIngen audioencoder (-oac) vald. Välj en (se -oac help) eller använd -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nIngen videoencoder (-ovc) vald. Välj en (se -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Kan inte öppna utfil '%s'.\n"
#define MSGTR_EncoderOpenFailed "Misslyckade att öppna encodern.\n"
#define MSGTR_ForcingOutputFourcc "Forcerar utmatning 'fourcc' till %x [%.4s]\n" // FIXME fourcc?
#define MSGTR_DuplicateFrames "\n%d duplicerad bildruta/or!\n"
#define MSGTR_SkipFrame "\nHoppar över bildruta!\n"
#define MSGTR_ErrorWritingFile "%s: Fel vid skrivning till fil.\n"
#define MSGTR_RecommendedVideoBitrate "Rekommenderad videobitrate för %s CD: %d\n"
#define MSGTR_VideoStreamResult "\nVideostöm: %8.3f kbit/s  (%d B/s)  storlek: %"PRIu64" byte  %5.3f sekunder  %d bildrutor\n"
#define MSGTR_AudioStreamResult "\nAudiostöm: %8.3f kbit/s  (%d B/s)  storlek: %"PRIu64" byte  %5.3f sekunder\n"
#define MSGTR_OpenedStream "klart: format: %d  data: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videocodec: framecopy (%dx%d %dbpp fourcc=%x)\n" // FIXME translate?
#define MSGTR_ACodecFramecopy "audiocodec: framecopy (format=%x chans=%d rate=%d bits=%d B/s=%d sample-%d)\n" // -''-
#define MSGTR_MP3AudioSelected "MP3 audio valt\n"
#define MSGTR_SettingAudioDelay "Sätter AUDIO DELAY till %5.3f\n"
#define MSGTR_LimitingAudioPreload "Begränsar audioförinladdning till 0.4s\n" // preload?
#define MSGTR_IncreasingAudioDensity "Höjer audiodensitet till 4\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Forcerar audioförinladdning till 0, 'max pts correction' till 0\n"
#define MSGTR_LameVersion "LAME version %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Fel: Angiven bitrate är utanför godkänd rymd för detta val\n"\
"\n"\
"Vid användning av detta val så måste du ange ett värde mellan \"8\" och \"320\"\n"\
"\n"\
"För vidare information testa: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Fel: du angav inte en godkänd profil och/eller förinställda val\n"\
"\n"\
"Tillgängliga profiler är:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR-mode är underförstått. För att använda det,,\n"\
"                      helpt enkelt ange en bitrate. För exempel:\n"\
"                      \"preset=185\" aktiverar detta\n"\
"                      förinställda val, och använder 185 som ett genomsnittlig kbps.\n"\
"\n"\
"    Några exempel:\n"\
"\n"\
"       \"-lameopts fast:preset=standard  \"\n"\
" eller \"-lameopts  cbr:preset=192       \"\n"\
" eller \"-lameopts      preset=172       \"\n"\
" eller \"-lameopts      preset=extreme   \"\n"\
"\n"\
"För vidare information, försök: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
    "De förinställda switcharna är designade för att försörja den högsta möjliga kvalité.\n"\
"\n"\
"De har för mestadels blivit utsatta för och instämmnt via rigorösa dubbelblindslystningstester\n"\
"för att verifiera och åstakomma detta mål.\n"\
"\n"\
"Dessa är ideligen uppdaterade för att sammanträffa med de senaste utveckling som\n"\
"förekommer, och som result skulle försörja dig med bortåt den bästa kvalité\n"\
"för stunden möjligt från LAME.\n"\
"\n"\
"För att aktivera dessa förinställda värden:\n"\
"\n"\
"   För VBR-modes (generellt högsta kvalité) \b:\n"\
"\n"\
"     \"preset=standard\" Denna förinställning torde generellt vara transparent\n"\
"                             för de flesta för den mesta musik och har redan\n"\
"                             relativt hög kvalité.\n"\
"\n"\
"     \"preset=extreme\" Om du har extremt god hörsel och liknande utrustning,\n"\
"                             då kommer denna inställning generellt att tillgodose\n"\
"                             något högre kvalité än \"standard\"-inställningen\n"\
"\n"\
"   För 'CBR 320kbps' (högsta möjliga kvalité från förinställningsswitcharna):\n"\
                                                                               "\n"\
"     \"preset=insane\"  Denna förinställning kommer troligen att vara för mycket för de\n"\
"                             flesta och de flesta situationer, men om du måste absolut\n"\
"                             ha den högsta möjliga kvalité med inga invändningar om\n"\
"                             filstorleken så är detta den väg att gå.\n"\
"\n"\
"   För ABR-modes (hög kvalité per given bitrate, men inte så hög som för VBR) \b:\n"\
"\n"\
"     \"preset=<kbps>\"  Användning av denna inställning vill för det mesta ge dig god\n"\
"                             kvalité vid specifik bitrate, Beroende på angiven bitrate,\n"\
"                             denna inställning kommer att anta den mest optimala inställning\n"\
"                             för en optimal situation. Fast detta tillvägagångssätt fungerar,\n"\
"                             så är den inte tillnärmandesvis så flexibelt som VBR, och för det\n"\
"                             mesta så kommer den inte att komma åt samma nivå av kvalité som\n"\
"                             VBR vid högre bitrate.\n"\
"\n"\
"Följande inställningar är även tillgängliga för motsvarande profil:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR-mode är underförstått. För att använda det,\n"\
"                      helt enkelt ange en bitrate. För exempel:\n"\
"                      \"preset=185\" aktiverar denna inställning\n"\
"                      och använder 185 som ett genomsnittlig kbps.\n"\
"\n"\
"   \"fast\" - Aktiverar den nya snabba VBR föe en speciell profil.\n"\
"            Nackdel till snabbhetsswitchen är att oftast kommer\n"\
"            bitrate att vara något högre än vid 'normal'-mode\n"\
"            och kvalitén kan även bil något lägre.\n"\
"   Varning: Med aktuell version kan 'fast'-inställningen resultera i\n"\
"            för hör bitrate i jämförelse med ordinarie inställning.\n"\
"\n"\
"   \"cbr\"  - Om du använder ABR-mode (läs ovanstående) med en signifikant\n"\
"            bitrate, såsom 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            du kan använda \"cbr\"-argument för att forcera CBR-modeskodning\n"\
"            istället för som standard ABR-mode. ABR gör högre kvalité\n"\
"            men CBR kan vara användbar i situationer såsom vid strömmande\n"\
"            av mp3 över internet.\n"\
"\n"\
"    Till exempel:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Ett par alias är tillgängliga för ABR-mode:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_ConfigFileError "konfigurationsfilsfel"
#define MSGTR_ErrorParsingCommandLine "fel vid tolkning av cmdline"
#define MSGTR_VideoStreamRequired "Videoström är obligatoriskt!\n"
#define MSGTR_ForcingInputFPS "'input fps' kommer att bli tolkad som %5.3f istället\n"
#define MSGTR_DemuxerDoesntSupportNosound "Denna demuxer stödjer inte -nosound ännu.\n"
#define MSGTR_MemAllocFailed "minnesallokering misslyckades\n"
#define MSGTR_NoMatchingFilter "Kunde inte finna matchande filter/ao-format!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, kanske trasig C-kompilator?\n"
#define MSGTR_NoLavcAudioCodecName "Audio LAVC, förkommet codecsnamn!\n"
#define MSGTR_LavcAudioCodecNotFound "Audio LAVC, kunde inte finna encoder för codec %s\n"
#define MSGTR_CouldntAllocateLavcContext "Audio LAVC, kunde inte allokera kontext!\n"
#define MSGTR_CouldntOpenCodec "Kunde inte öppna codec %s, br=%d\n"

// cfg-mencoder.h:

#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     variabel bitrate metod\n"\
"                0: cbr\n"\
"                1: mt\n"\
"                2: rh(default)\n"\
"                3: abr\n"\
"                4: mtrh\n"\
"\n"\
" abr           medelbitrate\n"\
"\n"\
" cbr           konstant bitrate\n"\
"               Även forcerar CBR-modeskodning på subsequentiellt ABR-inställningsläge.\n"\
"\n"\
" br=<0-1024>   specifierar bitrate i kBit (CBR och ABR endast)\n"\
"\n"\
" q=<0-9>       kvalité (0-högst, 9-lägst) (endast för VBR)\n"\
"\n"\
" aq=<0-9>      algoritmiskt kvalité (0-bäst/segast, 9-sämst/snabbast)\n"\
"\n"\
" ratio=<1-100> kompressionsratio\n"\
"\n"\
" vol=<0-10>    sätt audio-in-ökning\n"\
"\n"\
" mode=<0-3>    (standard: auto)\n"\
"                0: stereo\n"\
"                1: joint-stereo\n"\
"                2: dualchannel\n"\
"                3: mono\n"\
"\n"\
" padding=<0-2>\n"\
"                0: no\n"\
"                1: all\n"\
"                2: adjust\n"\
"\n"\
" fast          Aktivera snabbare kodning på subsequentiellt VBR-inställningsläge,\n"\
"               något lägre kvalité och högre bitrate.\n"\
"\n"\
" preset=<value> Tillhandahåller den högsta tillgängliga kvalitétsinställning.\n"\
"                 medium: VBR  kodning, godkvalité\n"\
"                 (150-180 kbps bitratesrymd)\n"\
"                 standard:  VBR kodning, hög kvalité\n"\
"                 (170-210 kbps bitratesrymd)\n"\
"                 extreme: VBR kodning, mycket hög kvalité\n"\
"                 (200-240 kbps bitratesrymd)\n"\
"                 insane:  CBR  kodning, högsta förinställd kvalité\n"\
"                 (320 kbps bitrate)\n"\
"                 <8-320>: ABR kodning vid i medeltal angiven bitrate (kbps).\n\n"

//codec-cfg.c:
#define MSGTR_DuplicateFourcc "duplicerad FourCC"
#define MSGTR_TooManyFourccs "för många FourCCs/format..."
#define MSGTR_ParseError "tolkningsfel"
#define MSGTR_ParseErrorFIDNotNumber "tolkningsfel (format-ID är inget nummer?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "tolkningsfel (format-ID-alias är inget nummer?)"
#define MSGTR_DuplicateFID "duplicerade format-ID"
#define MSGTR_TooManyOut "för många ut..." //FIXME 'to many out'?
#define MSGTR_InvalidCodecName "\ncodec(%s) namn är icke godkänt!\n"
#define MSGTR_CodecLacksFourcc "\ncodec(%s) har inte FourCC/format!\n"
#define MSGTR_CodecLacksDriver "\ncodec(%s) har ingen drivrutin!\n"
#define MSGTR_CodecNeedsDLL "\ncodec(%s) behöver en 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\ncodec(%s) behöver en 'outfmt'!\n"
#define MSGTR_CantAllocateComment "Kan inte allokera minne flr kommentar. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token() \b: max >= MAX_MR_TOKEN!" //FIXME translate?
#define MSGTR_CantGetMemoryForLine "Kan inte få minne för 'line': %s\n"
#define MSGTR_CantReallocCodecsp "Kan inte realloc '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "Codec namn '%s' är inte unikt."
#define MSGTR_CantStrdupName "Kan inte strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Kan inte strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Kan inte strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Kan inte strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d audio & %d video codecs\n"
#define MSGTR_CodecDefinitionIncorrect "Codec är inte definerad korrekt."
#define MSGTR_OutdatedCodecsConf "Denna codecs.conf är för gammal och inkompatibel med denna MPlayer version!" // release is more like 'släpp', sounds wrong, using version instead

// fifo.c

// m_config.c
#define MSGTR_SaveSlotTooOld "Allt för gammal sparningsslottar funna från nivå %d: %d !!!\n" // FIXME slot?
#define MSGTR_InvalidCfgfileOption "Alternativ %s kan inte användas i en konfigurationsfil\n"
#define MSGTR_InvalidCmdlineOption "Alternativ %s kan inte bli används som ett kommandoradsargument\n"
#define MSGTR_InvalidSuboption "Fel: alternativ '%s' har inga underalternativ '%s'\n" // FIXME suboption?
#define MSGTR_MissingSuboptionParameter "Fel: underalternativ '%s' av '%s' måste ha en parameter!\n"
#define MSGTR_MissingOptionParameter "Fel: alternativ '%s' måste ha en parameter!\n"
#define MSGTR_OptionListHeader "\n Namn                 Typ             Min        Max      Global  CL    Cfg\n\n" // TODO why static tabs?
#define MSGTR_TotalOptions "\nTotalt: %d alternativ\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM-enhet '%s' ej funnet.\n"
#define MSGTR_ErrTrackSelect "Fel vid val av VCD-spår."
#define MSGTR_ReadSTDIN "Läser från stdin...\n"
#define MSGTR_FileNotFound "Fil ej funnen: '%s'\n"

#define MSGTR_SMBInitError "Kan inte initiera libsmbclient-bilioteket: %d\n"
#define MSGTR_SMBFileNotFound "Kunde inte öppna från LAN: '%s'\n"

#define MSGTR_CantOpenDVD "Kunde inte öppna DVD-enhet: %s (%s)\n"
#define MSGTR_DVDnumTitles "Det är %d titlar på denna DVD.\n"
#define MSGTR_DVDinvalidTitle "Icke godkänt DVD-titelnummer: %d\n"
#define MSGTR_DVDnumAngles "Det är %d vinkar på denna DVD-titel.\n"
#define MSGTR_DVDinvalidAngle "Ej godkänd DVD-vinkelsnummer: %d\n"
#define MSGTR_DVDnoIFO "Kan inte öppna IFO-fil för DVD-titel %d.\n"
#define MSGTR_DVDnoVOBs "Kunde inte öppna titel VOBS (VTS_%02d_1.VOB).\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "VARNING: Audioströmsfilhuvud %d omdefinerad.\n"
#define MSGTR_VideoStreamRedefined "WARNING: Videoströmsfilhuvud %d omdefinerad.\n"
#define MSGTR_TooManyAudioInBuffer "\nAllt för många audiopaket i bufferten: (%d i %d byte).\n"
#define MSGTR_TooManyVideoInBuffer "\nAllt för många videopaket i bufferten: (%d i %d byte).\n"
#define MSGTR_MaybeNI "Kanske försöker du spela upp en icke-interleaved ström/fil, eller så har decodern falierat?\n" \
                      "För AVI-filer, försök med att forcera icke-interleaved-lägen med -ni argumentet.\n" // FIXME non-interleaved
#define MSGTR_SwitchToNi "\nSvårt interleaved AVI-fil detekterad, går över till '-ni'-läge...\n"
#define MSGTR_Detected_XXX_FileFormat "%s filformat detekterat.\n"
#define MSGTR_FormatNotRecognized "================ Tyvärr, detta filformat är inte rekogniserbart/stött ==================\n"\
                                  "=== Om denna fil är en AVi, ASF eller MPEG-ström, var vänlig kontakta upphovsmannen! ===\n" //FIXME author???
#define MSGTR_MissingVideoStream "Ingen videoström funnen.\n"
#define MSGTR_MissingAudioStream "Ingen audioström funnen -> inget ljud.\n"
#define MSGTR_MissingVideoStreamBug "Saknar videoström!? Kontakta upphovsmannen, det kan vara en bugg :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: Fil innehåller ej den valda audio- eller videoströmmen.\n"

#define MSGTR_NI_Forced "Forcerad"
#define MSGTR_NI_Detected "Påvisad" // FIXME right to say?
#define MSGTR_NI_Message "%s 'NON-INTERLEAVED AVI'-filformat.\n"

#define MSGTR_UsingNINI "Använder trasig 'NON-INTERLEAVED AVI'-filformat.\n"
#define MSGTR_CouldntDetFNo "Kunde inte avgöra antalet bildrutor (för absolut sökning).\n"
#define MSGTR_CantSeekRawAVI "Kan inte söka i råa AVI-strömmar. (Index krävs, försök med '-idx'-switchen.)\n"
#define MSGTR_CantSeekFile "Kan inte söka i denna fil.\n"

#define MSGTR_MOVcomprhdr "MOV: filhuvudkomprimeringssupport kräver ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: VARNING: Variabel FOURCC påvisad!?\n"
#define MSGTR_MOVtooManyTrk "MOV: VARNING: allt förmånga spår"
#define MSGTR_ErrorOpeningOGGDemuxer "Oförmögen att öppna oggdemuxern.\n"
#define MSGTR_CannotOpenAudioStream "Kan inte öppna audioström: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Kan inte öppna textningsström: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Misslyckades att öppna audiodemuxern: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Misslyckades att öppna textningsdemuxern: %s\n"
#define MSGTR_TVInputNotSeekable "TV-in är inte sökbar! (Sökning kommer troligen bli för att ändra kanal ;)\n"
#define MSGTR_ClipInfo "Clip-info:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: '30fps NTSC'-innehåll upptäckt, ändrar framerate.\n" // FIXME framerate?
#define MSGTR_EnterTelecineMode "\ndemux_mpg: '24fps progressive NTSC'-innehåll upptäckt, ändrar framerate.\n" // -''-

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "Kunde inte öppna codec.\n"
#define MSGTR_CantCloseCodec "Kunde inte stänga codec\n"

#define MSGTR_MissingDLLcodec "FEL: Kunde inte öppna obligatorisk DirecShow-codec %s.\n"
#define MSGTR_ACMiniterror "Kunde inte ladda/initiera 'Win32/ACM AUDIO'-codec (saknas Dll-fil?).\n"
#define MSGTR_MissingLAVCcodec "Kunde inte finna codec '%s' i libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: FATAL: EOF under sökning efter sequencefilhuvuden\n" // FIXME sequence?
#define MSGTR_CannotReadMpegSequHdr "FATAL: Kunde inte läsa sequencefilhuvud\n"
#define MSGTR_CannotReadMpegSequHdrEx "FATAL: Kunde inte läsa sequencefilhuvudstillägg.\n"
#define MSGTR_BadMpegSequHdr "MPEG: dålig sequencefilhuvud\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: dålig sequencefilhuvudstillägg\n"

#define MSGTR_ShMemAllocFail "Kunde inte allokera delat minne.\n"
#define MSGTR_CantAllocAudioBuf "Kunde inte allokera audio-ut-buffert.\n"

#define MSGTR_UnknownAudio "Okänd/saknad audioformat -> inget ljud\n"

#define MSGTR_UsingExternalPP "[PP] Använder externt postprocesseringsfiler, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Använder codecens postprocessing, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Begärd videocodecfamilj [%s] (vfm=%s) är ej tillgänglig.\nAktivera det vil kompilation.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Begärd audiocodecfamilj [%s] (afm=%s) är ej tillgänglig.\nAktivera det vil kompilation.\n"
#define MSGTR_OpeningVideoDecoder "Öppnar videodecoder: [%s] %s\n"
#define MSGTR_OpeningAudioDecoder "Öppnar audiodecoder: [%s] %s\n"
#define MSGTR_VDecoderInitFailed "VDecoder-initiering misslyckades :(\n" // FIXME VDecoder something special or just a shortcut?
#define MSGTR_ADecoderInitFailed "ADecoder-initiering misslyckades :(\n" // -''-
#define MSGTR_ADecoderPreinitFailed "ADecoder-preinitiering misslyckades :(\n" // -''-

// LIRC:
#define MSGTR_LIRCopenfailed "Misslyckades med att aktivera LIRC-stöd.\n"
#define MSGTR_LIRCcfgerr "Misslyckades med att läsa LIRC-konfigurationsfil %s.\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Kunde inte finna videofilter '%s'.\n"
#define MSGTR_CouldNotOpenVideoFilter "Kunde inte öppna videofilter '%s'.\n"
#define MSGTR_OpeningVideoFilter "Öppnar videofilter: "
#define MSGTR_CannotFindColorspace "Kunde inte hitta matchande färgrymder, t.o.m. vid insättning av 'scale' :(\n" // FIXME colorspace

// vd.c
#define MSGTR_CouldNotFindColorspace "Kunde inte finna matchande färgrymder - försöker åter med -vf scale...\n" // -''-
#define MSGTR_MovieAspectIsSet "Movie-Aspect är %.2f:1 - prescaling till korrekt film-aspect.\n"
#define MSGTR_MovieAspectUndefined "Film-Aspect är ej definerad - ingen prescaling kommer att äga rum.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Du måste uppgradera/installera de binära codecspaketen.\nGå till http://www.mplayerhq.hu/dload.html\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Kunde inte sända EWMH-fullskärmshändelse!\n"

#define MSGTR_InsertingAfVolume "[Mixer] Ingen hårdvarumixning, lägger till volymfilter.\n"
#define MSGTR_NoVolume "[Mixer] Ingen volymkontroll tillgänglig.\n"


// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "Om MPlayer"
#define MSGTR_GUI_Add "Lägg till"
#define MSGTR_GUI_AspectRatio "Aspect ratio" // FIXME translate?
#define MSGTR_GUI_Audio "Audio"
#define MSGTR_GUI_AudioDelay "Audiofördröjning"
#define MSGTR_GUI_AudioDriverConfiguration "Audiodirvrutinskonfiguration"
#define MSGTR_GUI_AudioTrack "Ladda extern audiofil"
#define MSGTR_GUI_AudioTracks "Audiospår"
#define MSGTR_GUI_AvailableDrivers "Tillgängliga drivrutioner:"
#define MSGTR_GUI_AvailableSkins "Skin"
#define MSGTR_GUI_Bass "Bass"
#define MSGTR_GUI_Blur "Blur"
#define MSGTR_GUI_Brightness "Ljusstyrka"
#define MSGTR_GUI_Browse "Bläddra"
#define MSGTR_GUI_Cache "Cache"
#define MSGTR_GUI_CacheSize "Cachestorlek"
#define MSGTR_GUI_Cancel "Avbryt"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Center"
#define MSGTR_GUI_Channel1 "Kanal 1"
#define MSGTR_GUI_Channel2 "Kanal 2"
#define MSGTR_GUI_Channel3 "Kanal 3"
#define MSGTR_GUI_Channel4 "Kanal 4"
#define MSGTR_GUI_Channel5 "Kanal 5"
#define MSGTR_GUI_Channel6 "Kanal 6"
#define MSGTR_GUI_ChannelAll "Allt"
#define MSGTR_GUI_ChapterN "Kapitel %d"
#define MSGTR_GUI_ChapterNN "Kapitel %2d"
#define MSGTR_GUI_Chapters "Kapitel"
#define MSGTR_GUI_Clear "Rensa"
#define MSGTR_GUI_CodecFamilyAudio "Audiocodecfamilj"
#define MSGTR_GUI_CodecFamilyVideo "Videocodecfamilj"
#define MSGTR_GUI_Coefficient "Koefficient"
#define MSGTR_GUI_ConfigFileError "konfigurationsfilsfel\n"
#define MSGTR_GUI_Configure "Konfiguration"
#define MSGTR_GUI_ConfigureDriver "Konfigurera drivrution"
#define MSGTR_GUI_Contrast "Kontrast"
#define MSGTR_GUI_Cp874 "Thailänsk teckenuppsättning (CP874)"
#define MSGTR_GUI_Cp936 "Enkel Kinesisk teckenuppsättning (CP936)"
#define MSGTR_GUI_Cp949 "Koreansk teckenuppsättning (CP949)"
#define MSGTR_GUI_Cp1250 "Slaviska/Centraleuropeiska Windows (CP1250)"
#define MSGTR_GUI_Cp1251 "Kyrilliska Windown (CP1251)"
#define MSGTR_GUI_CpBIG5 "Traditionell Kinesisk teckenuppsättning (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Västeuropeiska språk (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Slaviska/Centraleuropeiska språk (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Esperanto, Galician, Maltese, Turkiska (ISO-8859-3)" // FIXME Galician, Maltese
#define MSGTR_GUI_CpISO8859_4 "Äldre baltisk teckenuppsättning (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Kyrilliska (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Arabiska (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Modern grekiska (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Hebrew teckenuppsättningar (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Turkiska (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Baltiska (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Celtiska (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Västeuropeiska språk med Euro (ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Ryska (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Ukrainska, Vitrysska (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Japansk teckenuppsättning (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_DefaultSetting "standarddrivrutin"
#define MSGTR_GUI_Delay "Fördröjning"
#define MSGTR_GUI_Demuxers_Codecs "Codecs & demuxer"
#define MSGTR_GUI_Device "Enhet"
#define MSGTR_GUI_DeviceCDROM "CD-ROM-enhet"
#define MSGTR_GUI_DeviceDVD "DVD-enhet"
#define MSGTR_GUI_Directory "Sökväg"
#define MSGTR_GUI_DirectoryTree "Katalogträd"
#define MSGTR_GUI_DropSubtitle "Droppa textning..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAutomaticAVSync "AutoSync på/av"
#define MSGTR_GUI_EnableCache "Cache på/av"
#define MSGTR_GUI_EnableDirectRendering "Aktivera direct rendering"
#define MSGTR_GUI_EnableDoubleBuffering "Aktivera double buffering"
#define MSGTR_GUI_EnableEqualizer "AKtivera equalizer"
#define MSGTR_GUI_EnableExtraStereo "Aktivera extra stereo"
#define MSGTR_GUI_EnableFrameDropping "Aktivera frame dropping"
#define MSGTR_GUI_EnableFrameDroppingIntense "Aktivera HÅRD frame dropping (dangerous)"
#define MSGTR_GUI_EnablePlaybar "Aktivera spelindikator"
#define MSGTR_GUI_EnablePostProcessing "Aktivera postprocessing"
#define MSGTR_GUI_Encoding "Kodning"
#define MSGTR_GUI_Equalizer "Equalizer"
#define MSGTR_GUI_Error "Fel!"
#define MSGTR_GUI_ErrorFatal "Oöverkomligt fel!"
#define MSGTR_GUI_File "Spela fil"
#define MSGTR_GUI_Files "Filer"
#define MSGTR_GUI_Flip "Flippa bilden uppochner"
#define MSGTR_GUI_Font "Font"
#define MSGTR_GUI_FrameRate "FPS"
#define MSGTR_GUI_FrontLeft "Vänster fram"
#define MSGTR_GUI_FrontRight "Höger fram"
#define MSGTR_GUI_HideVideoWindow "Visa videofönster när den är inaktiv"
#define MSGTR_GUI_Hue "Hue"
#define MSGTR_GUI_Lavc "ANvänd LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Autokvalité"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Diverse"
#define MSGTR_GUI_Mixer "Mixer"
#define MSGTR_GUI_MixerChannel "Mixerkanal"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Tyvärr, du kan inte spela icke-MPEG-filer med ditt DXR3/H+-enhet utan omkodning.\nVar god aktivera lavc i 'DXR3/H+'-konfigurationsboxen.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Tyvärr, inte tillräckligt minne för ritbuffert.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Ingen fil laddad"
#define MSGTR_GUI_MSG_NoMediaOpened "Inget media öppnad"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Var god komihåg att du måste starta om uppspelning för att vissa ändringar ska ta effekt!"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "24-bitars till 32-bitars konverteringsfel (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "fil ej funnen (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "PNG läsfel (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Skin ej funnen (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "16-bitar eller lägre bitmappar stödjs inte (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[skin] fel i skinkonfigureringsfil på rad %d: %s"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "Fontfil ej funnen.\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "Fontbildsfil ej funnen.\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "icke-existerande fontidentifkator (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "ej tillräckligt minne\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "Allt för många fonter deklarerade.\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "okänt meddelande: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "okänd parameter (%s)\n"
#define MSGTR_GUI_MSG_VideoOutError "Tyvärr, jag hittade inte en GUI-kompatibel video-ut-drivrutin.\n"
#define MSGTR_GUI_Mute "Dämpa"
#define MSGTR_GUI_NetworkStreaming "Nätverksströmning..."
#define MSGTR_GUI_Next "Nästa ström"
#define MSGTR_GUI_NoChapter "Inget kapitel"
#define MSGTR_GUI__none_ "(ingen)"
#define MSGTR_GUI_NonInterleavedParser "Använd non-interleaved AVI tolk"
#define MSGTR_GUI_NormalizeSound "Normalizera ljud"
#define MSGTR_GUI_Ok "OK"
#define MSGTR_GUI_Open "Öppna..."
#define MSGTR_GUI_Original "Orginal"
#define MSGTR_GUI_OsdLevel "OSD-nivå"
#define MSGTR_GUI_OSD_Subtitles "Textning & OSD"
#define MSGTR_GUI_Outline "Outline"
#define MSGTR_GUI_PanAndScan "Panscan"
#define MSGTR_GUI_Pause "Pausa"
#define MSGTR_GUI_Play "Spela"
#define MSGTR_GUI_Playback "Spelar"
#define MSGTR_GUI_Playlist "Spellista"
#define MSGTR_GUI_Position "Position"
#define MSGTR_GUI_PostProcessing "Postprocessing"
#define MSGTR_GUI_Preferences "Inställningar"
#define MSGTR_GUI_Previous "Föregående ström"
#define MSGTR_GUI_Quit "Avsluta"
#define MSGTR_GUI_RearLeft "Vänster bak"
#define MSGTR_GUI_RearRight "Höger bak"
#define MSGTR_GUI_Remove "Radera"
#define MSGTR_GUI_Saturation "Saturation"
#define MSGTR_GUI_SaveWindowPositions "Spara fönsterposition"
#define MSGTR_GUI_ScaleMovieDiagonal "Propotionellt mot filmdiagonalen"
#define MSGTR_GUI_ScaleMovieHeight "Propotionellt mot filmhöjd"
#define MSGTR_GUI_ScaleMovieWidth "Propotionellt mot filmbredd"
#define MSGTR_GUI_ScaleNo "Ingen autoskalning"
#define MSGTR_GUI_SeekingInBrokenMedia "Återbygg indextabell, om så behövs"
#define MSGTR_GUI_SelectAudioFile "Välj extern audiokanal..."
#define MSGTR_GUI_SelectedFiles "Valda filer"
#define MSGTR_GUI_SelectFile "Välj fil..."
#define MSGTR_GUI_SelectFont "Välj font..."
#define MSGTR_GUI_SelectSubtitle "Välj textning..."
#define MSGTR_GUI_SizeDouble "Dubbel storlek"
#define MSGTR_GUI_SizeFullscreen "Fullskärm"
#define MSGTR_GUI_SizeNormal "Normal storlek"
#define MSGTR_GUI_SizeOSD "OSDskalning"
#define MSGTR_GUI_SizeSubtitles "Textskalning"
#define MSGTR_GUI_SkinBrowser "Skinläsare"
#define MSGTR_GUI_Sponsored "GUI-utveckling sponstrat av UHU Linux"
#define MSGTR_GUI_StartFullscreen "Starta i fullskärm"
#define MSGTR_GUI_Stop "Stopp"
#define MSGTR_GUI_Subtitle "Textning"
#define MSGTR_GUI_SubtitleAllowOverlap "Aktivera textningsöverlappning"
#define MSGTR_GUI_SubtitleAutomaticLoad "Deaktivera automatisk laddning av textning"
#define MSGTR_GUI_SubtitleConvertMpsub "Konvertera given text till MPlayers egna textningsformat"
#define MSGTR_GUI_SubtitleConvertSrt "Konvertera given text till det tidbaserade SubViewer (SRT) formatet"
#define MSGTR_GUI_Subtitles "Textning"
#define MSGTR_GUI_SyncValue "Autosync"
#define MSGTR_GUI_TitleNN "Titel %2d"
#define MSGTR_GUI_Titles "Titlar"
#define MSGTR_GUI_TrackN "Spår %d"
#define MSGTR_GUI_TurnOffXScreenSaver "Stoppa XScreenSaver"
#define MSGTR_GUI_URL "Spela URL"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Video"
#define MSGTR_GUI_VideoEncoder "Videoencoder"
#define MSGTR_GUI_VideoTracks "Videospår"
#define MSGTR_GUI_Warning "Varning!"

// ======================= VO Video Output drivers ========================

#define MSGTR_VOincompCodec "Vald video-ut-enhet är inte kompatibel med denna codec.\n"
#define MSGTR_VO_GenericError "Detta fel har inträffat"
#define MSGTR_VO_UnableToAccess "Kan inte accessa"
#define MSGTR_VO_ExistsButNoDirectory "finns redan, men är inte en katalog."
#define MSGTR_VO_DirExistsButNotWritable "Ut-katalog finns redan, men är inte skrivbar."
#define MSGTR_VO_CantCreateDirectory "Oförmögen att skapa ut-katalog."
#define MSGTR_VO_CantCreateFile "Oförmögen att skapa utfil."
#define MSGTR_VO_DirectoryCreateSuccess "Ut-katalog skapad."
#define MSGTR_VO_ValueOutOfRange "Värden utanför godkänd rymd"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "'Progressive JPEG' aktiverat."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "'Progressive JPEG' deaktiverat."
#define MSGTR_VO_JPEG_BaselineJPEG "'Baseline JPEG' aktiverat."
#define MSGTR_VO_JPEG_NoBaselineJPEG "'Baseline JPEG' deaktiverat."

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "ASCII-mode aktiverat."
#define MSGTR_VO_PNM_RawMode "Rått-mode aktiverat." // FIXME Rått sounds strange
#define MSGTR_VO_PNM_PPMType "Kommer att skriva PPM-filer."
#define MSGTR_VO_PNM_PGMType "Kommer att skriva PGM-filer."
#define MSGTR_VO_PNM_PGMYUVType "Kommer att skriva PGMYUV-filer."

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "'Interlaced'-mode kräver bildhöjd som är delbar med 4." // FIXME interlaced?
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Bildbredd måste vara delbart med 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Kan inte få minnes- eller filhanterare att skriva till \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Fel vid skrivning av bild till ut!" // FIXME output here?
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Okänd subdevice: %s" // FIXME subdevice
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Använder 'interlaced output mode', övre fältet först." // FIXME top-field first? && 'interlaced output mode'
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Använder 'interlaced output mode',nedre fältet först." // FIXME bottom-field first? && 'interlaced output mode'

#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Använder (som standard) progressiv bildramsinställning."

// Old vo drivers that have been replaced

#define MSGTR_VO_PGM_HasBeenReplaced "pgm-video-ut-drivrutinen har blivit utbytt av '-vo pnm:pgmyuv'.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "md5-video-ut-drivrutinen har blivit utbytt av '-vo md5sum'.\n"


// ======================= AO Audio Output drivers ========================

// libao2

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: alsa9- samt alsa1xmodulerna har blivit borttagna, använd -ao istället.\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Kan inte öppna mixernehet %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup: Audiokortsmixer har inte kanal '%s' använder standard.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Kan inte öppna audioenhet %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Kan inte få till 'filedescriptor'sblockning: %s\n" // FIXME filedescriptor
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Misslyckades att sätta audioenhet till %d kanaler.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: dirvrutin hanerar ej SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Din audiodrivrutin hanterar inte select()  ***\n Komplilera om med '#undef HAVE_AUDIO_SELECT' i config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\nFatalt fel: *** CAN INTE BLI ÅTERÖPPNAD / ÅTERSTÄLLER AUDIOENHET *** %s\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n" // FIXME nothing?
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Anslutet till ljudserver.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Oförmögen att öppna en ström.\n" // FIXME 'ström' or 'ljudström'?
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Ström öppnad.\n" // -''-
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] buffertstorlek: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Sättning av volym till %d misslyckades.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] dxr2: %d Hz är ej tillgänglig, försök med \"-aop list=resample\"\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound misslyckades: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] latency: [server: %0.2fs, net: %0.2fs] (adjust %0.2fs)\n" // FIXME translate?
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] misslyckades att öppna uppspelningsström: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB-audio-sättningsmixer misslyckades: %s\n" // set ~= sättning?
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Hz ej tillgänglig, försöker resampla...\n"

// ao_null.c
// This one desn't even have any mp_msg nor printf's?? [CHECK]

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Fil: %s (%s)\nPCM: Samplerate: %iHz Kanaler: %s Format %s\n" // FIXME Samplerate?
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Info: snabbaste dumplning är tillgänglig via -vc null -vo null\nPCM: Info: för att skriva WAVE-filer använd -ao pcm:waveheader (standard).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Misslyckades att öppna %s för skrivning!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Samplerate: %iHz Kanaler: %s Format %s\n" // -''-
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] använder %s som audioenhet.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Icke tillgängligt audioformat: 0x%x.\n" // support?
#define MSGTR_AO_SDL_CantInit "[AO SDL] Initialisering av 'SDL Audio' misslyckades: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Oförmögen att öppna audio: %s\n" // audio what?

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] kontroll.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] init: Samplerate: %iHz Kanaler: %s Format %s\n" // FIXME Samplerate
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] play: icke godkänd enhet.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] init: setparams misslyckades: %s\nKunde inte sätta önskad samplerate.\n" // -''-
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] init: AL_RATE var inte accepterad på given resurs.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] init: getparams misslyckades: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] init: samplerate är nu %f (önskad rate var %f)\n" // -''- also rate?
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] init: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] init: Oförmögen att öppna audiokanal: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] uninit: ...\n" // FIXME translate?
#define MSGTR_AO_SGI_Reset "[AO SGI] reset: ...\n" // -''-
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause: ...\n" // -''-
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume: ...\n" // -''-

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: SETINFO misslyckades.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: skrivning misslyckades."
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Kan inte öppna audioenhet %s, %s  -> inget ljud.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] audio_setup: ditt kort hanterar inte %d kanaler, %s, %d Hz samplerate.\n" // FIXME samplerate

// ao_plugin.c


// ========================== LIBMPCODECS ===================================

// libmpcodecs/ad_dvdpcm.c:
#define MSGTR_SamplesWanted "Fler exempel på detta format behövs för att vidare öka support. Var vänlig kontakta utvecklarna.\n"
