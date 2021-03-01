// Translated by:  Panagiotis Issaris <takis@lumumba.luc.ac.be>
//maintained by:   Philippe De Swert <philippedeswert@scarlet.be>
//synced with help_mp-en.h revision 1.152

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<trackno>   speel VCD (Video CD) track van cdrom in plaats van standaard bestand\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<titelnr>   speel DVD titel/track van device in plaats van gewoon bestand\n"\
                      " -alang/-slang    selecteer DVD audio/ondertitelingstaal (door middel van 2-karakter landcode)\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Gebruik:   mplayer [opties] [url|pad/]bestandsnaam\n"\
"\n"\
"Basis-opties: (volledige lijst in de man-pagina's)\n"\
" -vo <drv[:dev]>  selecteer video uitvoer driver & device (zie '-vo help' voor lijst)\n"\
" -ao <drv[:dev]>  selecteer audio uitvoer driver & device (zie '-ao help' voor lijst)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -ss <timepos>    ga naar opgegeven (seconden of hh:mm:ss) positie\n"\
" -nosound         speel het geluid niet af\n"\
" -fs -vm -zoom    volledig scherm afspeel opties (fullscr,vidmode chg,softw.scale)\n"\
" -x <x> -y <y>    herschaal beeld naar <x> * <y> resolutie [als -vo driver het ondersteunt!]\n"\
" -sub <bestand>   specificeer het te gebruiken ondertitel bestand (zie ook -subfps, -subdelay)\n"\
" -playlist <file> specificeer het te gebruiken playlist bestand\n"\
" -vid x -aid y    opties om te spelen video (x) en audio (y) stream te selecteren\n"\
" -fps x -srate y  opties om video (x fps) en audio (y Hz) tempo te veranderen\n"\
" -pp <kwaliteit>  activeer postprocessing filter (Zie manpage/docs voor meer informatie)\n"\
" -framedrop       activeer frame-dropping (voor trage machines)\n"\
"\n"\
"Toetsen: (Zie manpage voor de volledige lijst, kijk ook naar input.conf)\n"\
" <-  of  ->       ga 10 seconden achterwaarts/voorwaarts\n"\
" omhoog of omlaag ga 1 minuut achterwaarts/voorwaarts\n"\
" PGUP of PGDOWN   ga 10 minuten achterwaarts/voorwaarts\n"\
" < of >           ga naar vorige/volgende item in playlist\n"\
" p of SPACE       pauzeer film (druk eender welke toets om verder te gaan)\n"\
" q of ESC         stop afspelen en sluit programma af\n"\
" + of -           pas audio vertraging aan met +/- 0.1 seconde\n"\
" o                doorloop OSD modes: geen / enkel zoekbalk / zoekbalk en tijd\n"\
" * of /           verhoog of verlaag volume (druk 'm' om master/pcm te selecteren)\n"\
" z of x           pas ondertiteling vertraging aan met +/- 0.1 seconde\n"\
" r or t           verticale positionering van de ondertiteling, zie ook -vf expand!\n"\
"\n"\
" * * * ZIE MANPAGE VOOR DETAILS, OVERIGE (GEAVANCEERDE) OPTIES EN TOETSEN! * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c:

#define MSGTR_Exiting "\nBezig met afsluiten...\n"
#define MSGTR_ExitingHow "\nBezig met afsluiten... (%s)\n"
#define MSGTR_Exit_quit "Stop"
#define MSGTR_Exit_eof "Einde van bestand"
#define MSGTR_Exit_error "Fatale fout"
#define MSGTR_IntBySignal "\nMPlayer onderbroken door signaal %d in module: %s \n"
#define MSGTR_NoHomeDir "Kan HOME dir niet vinden\n"
#define MSGTR_GetpathProblem "get_path(\"config\") probleem\n"
#define MSGTR_CreatingCfgFile "Bezig met het creëren van configuratie bestand: %s\n"
#define MSGTR_CantLoadFont "Kan font niet laden: %s\n"
#define MSGTR_CantLoadSub "Kan ondertitels niet lezen: %s\n"
#define MSGTR_DumpSelectedStreamMissing "dump: FATAL: geselecteerde stream ontbreekt!\n"
#define MSGTR_CantOpenDumpfile "Kan dump bestand niet openen!!!\n"
#define MSGTR_CoreDumped "Core dumped ;)\n"
#define MSGTR_FPSnotspecified "FPS niet gespecificeerd (of foutief) in de header! Gebruik de optie -fps!\n"
#define MSGTR_TryForceAudioFmtStr "Probeer audio codec driver familie %s te forceren...\n"
#define MSGTR_CantFindAudioCodec "Kan codec voor audio format 0x%X niet vinden!\n"
#define MSGTR_TryForceVideoFmtStr "Probeer video codec driver familie %s te forceren...\n"
#define MSGTR_CantFindVideoCodec "Kan codec voor video format 0x%X niet vinden!\n"
#define MSGTR_VOincompCodec "Sorry, geselecteerde video_out device is incompatibel met deze codec.\n"
#define MSGTR_CannotInitVO "FATAL: Kan video driver niet initialiseren!\n"
#define MSGTR_CannotInitAO "Kon audio device niet open/init -> NOSOUND\n"
#define MSGTR_StartPlaying "Start afspelen...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"           ****************************************************\n"\
"           **** Je system is te TRAAG om dit af te spelen! ****\n"\
"           ****************************************************\n"\
"Mogelijke oorzaken, problemen, oplossingen: \n"\
"- Meestal: slechte/buggy _audio_ driver.\n"\
"  - Probeer -ao sdl of gebruik ALSA 0.5 of oss emulatie van ALSA 0.9.\n"\
"  - Experimenteer met verschillende waardes voor -autosync, 30 is een goede startwaarde.\n"\
"- Trage video output\n"\
"  - Probeer een andere -vo driver (voor lijst: -vo help) of probeer met -framedrop!\n"\
"- Trage CPU.\n"\
"  - Probeer geen grote DVD/DivX af te spelen op een trage CPU! Probeer -hardframedrop.\n"\
"- Beschadigd bestand.\n"\
"  - Probeer verschillende combinaties van -nobps -ni -forceidx -mc 0.\n"\
"- Trage media (NFS/SMB mounts, DVD, VCD enz.)\n"\
"  - Probeer met -cache 8192.\n"\
"- Gebruik je -cache om een niet-interleaved bestand af te spelen?\n"\
"  - Probeer met -nocache\n"\
"Lees DOCS/HTML/en/video.html voor tips aangaande het afstellen en versnellen van MPlayer.\n"\
"Als geen van deze oorzaken van toepassingen zijn, lees dan DOCS/HTML/en/bugreports.html!\n\n"

#define MSGTR_NoGui "MPlayer werd gecompileerd ZONDER GUI ondersteuning!\n"
#define MSGTR_GuiNeedsX "MPlayer GUI heeft X11 nodig!\n"
#define MSGTR_Playing "Bezig met het afspelen van %s\n"
#define MSGTR_NoSound "Audio: geen geluid!!!\n"
#define MSGTR_FPSforced "FPS geforceerd om %5.3f te zijn (ftime: %5.3f)\n"
#define MSGTR_AvailableVideoOutputDrivers "Beschikbare video output drivers:\n"
#define MSGTR_AvailableAudioOutputDrivers "Beschikbare audio output drivers:\n"
#define MSGTR_AvailableAudioCodecs "Beschikbare audio codecs:\n"
#define MSGTR_AvailableVideoCodecs "Beschikbare video codecs:\n"
#define MSGTR_AvailableAudioFm "\nBeschikbare (ingecompileerde) audio codec families/drivers:\n"
#define MSGTR_AvailableVideoFm "\nBeschikbare (ingecompileerde) video codec families/drivers:\n"
#define MSGTR_AvailableFsType "Beschikbare fullscreen modi:\n"
#define MSGTR_CannotReadVideoProperties "Video: kan eigenschappen niet lezen\n"
#define MSGTR_NoStreamFound "Geen stream gevonden\n"
#define MSGTR_ErrorInitializingVODevice "Fout bij het openen/initialiseren van het gekozen video_out (-vo) apparaat!\n"
#define MSGTR_ForcedVideoCodec "Gedwongen video codec: %s\n"
#define MSGTR_ForcedAudioCodec "Gedwongen audio codec: %s\n"
#define MSGTR_Video_NoVideo "Video: geen video!!!\n"
#define MSGTR_NotInitializeVOPorVO "\nFATAAL: Kon de video filters (-vf) of de video uitvoer (-vo) niet initialiseren!\n"
#define MSGTR_Paused "------ GEPAUZEERD -------"
#define MSGTR_PlaylistLoadUnable "\nKon de playlist %s niet laden\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer crashte door een 'Illegale Instructie'.\n"\
"  Het kan een bug zijn in onze nieuwe runtime CPU-detectie code...\n"\
"  Lees DOCS/HTML/en/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer crashte door een 'Illegale Instructie'.\n"\
"  Dit gebeurt meestal als je het uitvoert op een andere CPU dan diegene waarvoor het werd\n"\
"  gecompileerd/geoptimaliseerd.\n  Verifieer dit!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer crashte door slecht gebruik van CPU/FPU/RAM.\n"\
"  Hercompileer MPlayer met --enable-debug en genereer een 'gdb' backtrace en\n"\
"  disassembly. Voor details, zie DOCS/HTML/en/bugreports_what.html#bugreports_crash\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer crashte. Dit zou niet mogen gebeuren.\n"\
"  Het kan een bug in de MPlayer code _of_ in uw drivers _of_ in uw gcc\n"\
"  versie. Als je denkt dat het MPlayer's fout is, lees dan DOCS/HTML/en/bugreports.html\n"\
"  en volg de instructies. We kunnen en zullen niet helpen tenzij je deze informatie\n"\
"  meelevert bij het rapporteren van een mogelijke bug.\n"

#define MSGTR_LoadingConfig "Bezig met het laden van de configuratie '%s'\n"
#define MSGTR_AddedSubtitleFile "SUB: bestand met ondertitels toegevoegd (%d): %s\n"
#define MSGTR_RTCDeviceNotOpenable "Kon %s niet openen: %s (moet leesbaar zijn voor deze gebruiker.)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Linux RTC initialisatiefout in ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Probeer \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" toe te voegen in de opstartbestanden van uw systeem.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Linux RTC initialisatiefout in ioctl (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "WAARSCHUWING: getch2_init is twee maal opgeroepen!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Kan de libmenu video filter niet openen met het root menu %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Er is een fout opgetreden bijde pre-initialisatie van de audio filter!\n"
#define MSGTR_LinuxRTCReadError "Linux RTC leesfout: %s\n"
#define MSGTR_SoftsleepUnderflow "Waarschuwing! Softsleep underflow!\n"
#define MSGTR_EdlOutOfMem "Kan niet genoeg geheugen toewijzen voor de EDL data.\n"
#define MSGTR_EdlRecordsNo "Bezig met het lezen van %d EDL acties.\n"
#define MSGTR_EdlQueueEmpty "Er zijn geen EDL acties meer te verwerken.\n"
#define MSGTR_EdlCantOpenForWrite "Kan het EDL bestand [%s] niet openen om te schrijven.\n"
#define MSGTR_EdlNOsh_video "Kan EDl niet gebruiken zonder video, daarom wordt het uitgeschakeld.\n"
#define MSGTR_EdlNOValidLine "Ongeldige EDL lijn: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Slecht geformatteerde EDL lijn [%d] Deze wordt niet gebruikt.\n"
#define MSGTR_EdlBadLineOverlap "Laatste stop positie was [%f]; volgende start positie is "\
"[%f]. Posities moeten in chronologische volgorde zijn en kunnen niet overlappen. Deze worden genegeerd\n"
#define MSGTR_EdlBadLineBadStop "De eindtijd moet voor de starttijd komen.\n"

// mencoder.c:

#define MSGTR_UsingPass3ControlFile "Pass3 control bestand gebruikend: %s\n"
#define MSGTR_MissingFilename "\nOntbrekende bestandsnaam!\n\n"
#define MSGTR_CannotOpenFile_Device "Kan bestand/aparaat niet openen\n"
#define MSGTR_CannotOpenDemuxer "Kan demuxer niet openen\n"
#define MSGTR_NoAudioEncoderSelected "\nGeen audio encoder (-oac) geselecteerd! Kies er een of gebruik -nosound. Probeer -oac help!\n"
#define MSGTR_NoVideoEncoderSelected "\nGeen video encoder (-ovc) geselecteerd! Kies er een. Probeer -ovc help!\n"
#define MSGTR_CannotOpenOutputFile "Kan het uitvoer bestand '%s' niet openen\n"
#define MSGTR_EncoderOpenFailed "Het openen van de encoder is mislukt\n"
#define MSGTR_ForcingOutputFourcc "Uitvoer fourcc gedwongen naar %x [%.4s]\n"
#define MSGTR_DuplicateFrames "\n%d duplicate frame(s)!!!    \n"
#define MSGTR_SkipFrame "\nframe overgeslagen!!!    \n"
#define MSGTR_ErrorWritingFile "%s: fout bij het schrijven van het bestand.\n"
#define MSGTR_RecommendedVideoBitrate "Aangeraden video bitrate voor %s CD: %d\n"
#define MSGTR_VideoStreamResult "\nVideo stream: %8.3f kbit/s  (%d B/s)  grootte: %"PRIu64" bytes  %5.3f secs  %d frames\n"
#define MSGTR_AudioStreamResult "\nAudio stream: %8.3f kbit/s  (%d B/s)  grootte: %"PRIu64" bytes  %5.3f secs\n"
#define MSGTR_OpenedStream "succes: formaat: %d  data: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videocodec: framecopy (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "audiocodec: framecopy (formaat=%x chans=%d rate=%d bits=%d B/s=%d sample-%d)\n"
#define MSGTR_MP3AudioSelected "MP3 audio geselecteerd\n"
#define MSGTR_SettingAudioDelay " AUDIO DELAY wordt gezet op %5.3f\n"
#define MSGTR_LimitingAudioPreload "Audio audio preload wordt beperkt tot 0.4s\n"
#define MSGTR_IncreasingAudioDensity "Audio densiteit wordt opgevoerd tot 4\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Audio preload wordt ingesteld op 0, max pts correctie is 0\n"
#define MSGTR_LameVersion "LAME versie %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Fout: De ingestelde bitrate valt buiten het bereik van deze preset\n"\
"\n"\
"In deze mode moet er een waarde tussen\"8\" en \"320\" gebruikt worden.\n"\
"\n"\
"Voor meer informatie : \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Fout: U hebt geen geldig profiel en/of opties gegeven voor deze preset\n"\
"\n"\
"beschikbare profielen zijn:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - De ABR Mode wordt verondersteld. Om het te,\n"\
"                      gebruiken moet je gewoon een bitrate opgeven. Bevoorbeeld:\n"\
"                      \"preset=185\" activeert deze preset \n"\
"                      en gebruikt 185 als gemiddelde kbps.\n"\
"\n"\
"Enkele voorbeelden:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Voor meer informatie : \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"De presets zijn berekend om de best mogelijke kwaliteit te garanderen.\n"\
"\n"\
"Ze zijn over het algemeen dubbelblind getest om dit te verifieren en dit doel te bereiken\n"\
"\n"\
"Deze worden continu ge-updated om de laatste intwikkelingen op te volgen. En daarom ook de beste \n"\
"resultaten te garanderen die mogelijk is met LAME.\n"\
"\n"\
"Om deze presets te gebruiken:\n"\
"\n"\
"   Voor VBR modes (meestal de hoogste kwaliteit):\n"\
"\n"\
"     \"preset=standard\" Deze preset moet de meeste mensen bekend voorkomen\n"\
"                         voor de meeste muziek en heeft al een vrij hoog kwaliteitsniveau\n"\
"\n"\
"     \"preset=extreme\" Wanneer je over een extreem goed gehoor beschikt en gelijkaardige\n"\
"                             audio uitrusting, zal deze preset U een iets\n"\
"                             betere kwaliteit leveren dan de \"standard\"\n"\
"                             mode.\n"\
"\n"\
"   Voor CBR 320kbps (de hoogst mogelijke kwaliteit met de presets):\n"\
"\n"\
"     \"preset=insane\"  Deze preset zal meestal overkill zijn voor de meeste\n"\
"                             mensen en situaties, maar wanneer je absoluut\n"\
"                             de beste kwaliteit wilt, ongeacht de bestandsgrootte\n"\
"                             is dit uw beste keuze.\n"\
"\n"\
"   Voor ABR modes (hoge kwaliteit voor de opgegeven bitrate, maar niet zo hoog als bij VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  Het gebruik van deze preset zal u gewoonlijk een goed geluidskwaliteit\n"\
"                        voor een bepaalde bitrate. Afhankelijk van de ingegeven\n"\
"                             bitrate zal deze preset de optimale instellingen\n"\
"                             voor deze situatie gebruiken.\n"\
"                             Hoewel deze aanpak werkt, is deze niet zo flexibel\n"\
"                             als VBR,  en zal deze ook niet hetzelfde kwaliteitsniveau\n"\
"                             behalen als VBR met lagere bitrates.\n"\
"\n"\
"De volgende opties zijn ook beschikbaar voor de overeenkomstige profielen:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - Het gebruik van de ABR Mode wordt verondersteld.\n"\
"                      Om het te gebruiken moet je gewoon een bitrate opgeven.\n"\
"                      bv.\"preset=185\" zal deze preset activeren\n"\
"                      en zal 185 gebruiken als gemiddelde kbps.\n"\
"\n"\
"   \"fast\" - Activeert de snelle VBR mode voor een bepaald profiel. Het\n"\
"            nadeel hiervan is dat voor een gegeven bitrate het uiteindelijke\n"\
"            bestand groter zal zijn dan in de normale mode, de kwaliteit\n"\
"            kan ook lager zijn.\n"\
"   Waarschuwing: Met de huidige versie kunnen de \"snelle\" presets hogere bitrates geven\n"\
"            dan de normale, tragere presets.\n"\
"\n"\
"   \"cbr\"  - Wanneer je de ABR mode (zie hoger) gebruikt en 1 van de volgende\n"\
"            bitrates: 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            kan can je de \"cbr\" optie gebruiken om CBR mode encodering\n"\
"            te gebruiken i.p.v de stadaard ABR mode. ABR geeft een hogere\n"\
"            kwaliteit, maar CBR is nuttig in sommige sitauties zoals bij\n"\
"            het streamen van mp3's over het internet.\n"\
"\n"\
"    bevoorbeeld:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Voor ABR mode zijn er een aantal aliases beschikbaar:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_ConfigFileError "Fout in het configuratie bestand of configuratie bestand ontbreekt"
#define MSGTR_ErrorParsingCommandLine "Fout bij het analyseren van de commandoregel"
#define MSGTR_VideoStreamRequired "Een Video stream is verplicht!\n"
#define MSGTR_ForcingInputFPS "input fps zal geinterpreteerd worden als %5.3f \n"
#define MSGTR_DemuxerDoesntSupportNosound "Deze demuxer ondersteunt -nosound nog niet.\n"
#define MSGTR_MemAllocFailed "geheugentoewijzing gefaald\n"
#define MSGTR_NoMatchingFilter "Kon geen geldig(e) filter/ao formaat vinden!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, Problemen met de C compiler?\n"
#define MSGTR_NoLavcAudioCodecName "Audio LAVC, ontbrekende codec naam !\n"
#define MSGTR_LavcAudioCodecNotFound "Audio LAVC, con geen encoder vinden voor deze codec %s\n"
#define MSGTR_CouldntAllocateLavcContext "Audio LAVC, kon geen context toewijzen!\n"
#define MSGTR_CouldntOpenCodec "Kon codec niet openen %s, br=%d\n"

// cfg-mencoder.h:

#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     variabele bitrate methode\n"\
"                0: cbr\n"\
"                1: mt\n"\
"                2: rh(standaard)\n"\
"                3: abr\n"\
"                4: mtrh\n"\
"\n"\
" abr           gemiddelde bitrate\n"\
"\n"\
" cbr           constante bitrate\n"\
"               Dwingt ook CBR mode encodering voor volgende ABR preset modes\n"\
"\n"\
" br=<0-1024>   specificeer bitrate in kBit (enkel voor CBR en ABR)\n"\
"\n"\
" q=<0-9>       kwaliteit (0-hoogste, 9-laagste) (enkel voor VBR)\n"\
"\n"\
" aq=<0-9>      algoritmische kwaliteit (0-beste/traagste, 9-slechtste/snelste)\n"\
"\n"\
" ratio=<1-100> compressieverhouding\n"\
"\n"\
" vol=<0-10>    stel audio input gain in\n"\
"\n"\
" mode=<0-3>    (standaard: auto)\n"\
"                0: stereo\n"\
"                1: joint-stereo\n"\
"                2: dualchannel\n"\
"                3: mono\n"\
"\n"\
" padding=<0-2>\n"\
"                0: neen\n"\
"                1: allemaal\n"\
"                2: pas aan\n"\
"\n"\
" fast          activeer snellere encodering voor de volgende VBR preset modes,\n"\
"               lagere qualiteit en hogere bitrates.\n"\
"\n"\
" preset=<value> voorzien de hoogst mogelijke qualiteitsinstellingen.\n"\
"                 medium: VBR  encodering,  goede qualiteit\n"\
"                 (150-180 kbps bitrate range)\n"\
"                 standard:  VBR encodering, hoge qualiteit\n"\
"                 (170-210 kbps bitrate range)\n"\
"                 extreme: VBR encodering, heel hoge qualiteit\n"\
"                 (200-240 kbps bitrate range)\n"\
"                 insane:  CBR  encodering, hoogste preset qualiteit\n"\
"                 (320 kbps bitrate)\n"\
"                 <8-320>: ABR encodering aan de opgegeven gemiddelde bitrate in kbps.\n\n"

//codec-cfg.c:
#define MSGTR_DuplicateFourcc "gedupliceerde FourCC"
#define MSGTR_TooManyFourccs "teveel FourCCs/formaten..."
#define MSGTR_ParseError "parsing fout"
#define MSGTR_ParseErrorFIDNotNumber "parsing fout (formaat ID is geen nummer?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "parsing fout (formaat ID alias is geen nummer?)"
#define MSGTR_DuplicateFID "gedupliceerde formaat ID"
#define MSGTR_TooManyOut "teveel output formaten..."
#define MSGTR_InvalidCodecName "\ncodec(%s) naam is ongeldig!\n"
#define MSGTR_CodecLacksFourcc "\ncodec(%s) heeft geen FourCC/formaat!\n"
#define MSGTR_CodecLacksDriver "\ncodec(%s) heeft geen aansturingsprogramma!\n"
#define MSGTR_CodecNeedsDLL "\ncodec(%s) heeft een'dll' nodig!\n"
#define MSGTR_CodecNeedsOutfmt "\ncodec(%s) heeft 'outfmt' nodig!\n"
#define MSGTR_CantAllocateComment "Kan geen geheugen toewijzen voor commentaar. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Kan geen geheugen toewijzen voor 'line': %s\n"
#define MSGTR_CantReallocCodecsp "Kan '*codecsp' niet re-alloceren: %s\n"
#define MSGTR_CodecNameNotUnique "De codec naam '%s' is niet uniek."
#define MSGTR_CantStrdupName "Kan strdup -> 'name' niet uitvoeren: %s\n"
#define MSGTR_CantStrdupInfo "Kan strdup -> 'info' niet uitvoeren : %s\n"
#define MSGTR_CantStrdupDriver "Kan strdup -> 'driver' niet uitvoeren : %s\n"
#define MSGTR_CantStrdupDLL "Kan strdup -> 'dll' niet uitvoeren : %s"
#define MSGTR_AudioVideoCodecTotals "%d audio & %d video codecs\n"
#define MSGTR_CodecDefinitionIncorrect "De codec is niet correct gedefinieerd."
#define MSGTR_OutdatedCodecsConf "Dit codecs.conf bestand is te oud en incompatibel met deze versie van MPlayer!"

#define MSGTR_SaveSlotTooOld "Een te oud save slot voor lvl %d gevonden: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "De %s optie kan niet gebruikt worden in het configuratie bestand.\n"
#define MSGTR_InvalidCmdlineOption "De %s optie kan niet gebruikt worden op de commandoregel.\n"
#define MSGTR_InvalidSuboption "Fout: de optie '%s' heeft geen suboptie '%s'.\n"
#define MSGTR_MissingSuboptionParameter "Fout: de suboptie '%s' van '%s' heeft een parameter nodig!\n"
#define MSGTR_MissingOptionParameter "Fout: de optie '%s' heeft een parameter nodig!\n"
#define MSGTR_OptionListHeader "\n Naam                 Type            Min        Max      Globaal  CL    Cfg\n\n"
#define MSGTR_TotalOptions "\nTotaal: %d opties\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM Device '%s' niet gevonden!\n"
#define MSGTR_ErrTrackSelect "Fout bij het selecteren van VCD track!"
#define MSGTR_ReadSTDIN "Lezen van stdin...\n"
#define MSGTR_FileNotFound "Bestand niet gevonden: '%s'\n"

#define MSGTR_SMBInitError "Kon de libsmbclient bibliotheek niet initialiseren: %d\n"
#define MSGTR_SMBFileNotFound "Kon netwerkbestand '%s' niet openen\n"

#define MSGTR_CantOpenDVD "Kon DVD device niet openen: %s (%s)\n"
#define MSGTR_DVDnumTitles "Er zijn %d titels op deze DVD.\n"
#define MSGTR_DVDinvalidTitle "Foutieve DVD titelnummer: %d\n"
#define MSGTR_DVDnumAngles "Er zijn %d gezichtspunten in deze DVD titel.\n"
#define MSGTR_DVDinvalidAngle "Foutief DVD gezichtspuntnummer: %d\n"
#define MSGTR_DVDnoIFO "Kan het IFO bestand voor DVD titel %d niet openen.\n"
#define MSGTR_DVDnoVOBs "Kan titel VOBS niet openen (VTS_%02d_1.VOB).\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "Waarschuwing! Audio stream header %d geherdefinieerd!\n"
#define MSGTR_VideoStreamRedefined "Waarschuwing! Video stream header %d geherdefinieerd!\n"
#define MSGTR_TooManyAudioInBuffer "\nDEMUXER: Te veel (%d in %d bytes) audio packetten in de buffer!\n"
#define MSGTR_TooManyVideoInBuffer "\nDEMUXER: Te veel (%d in %d bytes) video packetten in de buffer!\n"
#define MSGTR_MaybeNI "(misschien speel je een non-interleaved stream/bestand of werkte de codec niet)\n" \
                      "Voor .AVI bestanden probeer je best non-interleaved mode met de optie -ni\n"
#define MSGTR_SwitchToNi "\nSlecht geinterleaved .AVI bestand gedetecteerd - schakel om naar -ni mode!\n"
#define MSGTR_Detected_XXX_FileFormat "%s bestandsformaat gedetecteerd!\n"
#define MSGTR_FormatNotRecognized "============= Sorry, dit bestandsformaat niet herkend/ondersteund ===============\n"\
                                  "=== Als dit een AVI bestand, ASF bestand of MPEG stream is, contacteer dan aub de auteur! ===\n"
#define MSGTR_MissingVideoStream "Geen video stream gevonden!\n"
#define MSGTR_MissingAudioStream "Geen audio stream gevonden...  ->nosound\n"
#define MSGTR_MissingVideoStreamBug "Ontbrekende video stream!? Contacteer de auteur, het zou een bug kunnen zijn :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: bestand bevat de geselecteerde audio- of videostream niet\n"

#define MSGTR_NI_Forced "Geforceerd"
#define MSGTR_NI_Detected "Gedetecteerd"
#define MSGTR_NI_Message "%s NON-INTERLEAVED AVI bestandsformaat!\n"

#define MSGTR_UsingNINI "NON-INTERLEAVED mode voor beschadigd AVI bestandsformaat wordt gebruikt!\n"
#define MSGTR_CouldntDetFNo "Kon het aantal frames niet bepalen (voor absolute verplaatsing)  \n"
#define MSGTR_CantSeekRawAVI "Kan niet in raw .AVI streams verplaatsen! (index nodig, probeer met de -idx optie!)  \n"
#define MSGTR_CantSeekFile "Kan niet verplaatsen in dit bestand!  \n"

#define MSGTR_MOVcomprhdr "MOV: Gecomprimeerde headers (nog) niet ondersteund!\n"
#define MSGTR_MOVvariableFourCC "MOV: Waarschuwing! variabele FOURCC gedetecteerd!?\n"
#define MSGTR_MOVtooManyTrk "MOV: Waarschuwing! te veel tracks!"
#define MSGTR_ErrorOpeningOGGDemuxer "Kan de Ogg demuxer niet openen\n"
#define MSGTR_CannotOpenAudioStream "Kan audio stream niet openen: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Kan ondertitelingstream niet openen: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Kan audio demuxer niet openen: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Openen van de ondertiteling demuxer is mislukt: %s\n"
#define MSGTR_TVInputNotSeekable "TV invoer is niet doorzoekbaar! (Waarschijnlijk zal zoeken de kanalen veranderen ;)\n"
#define MSGTR_ClipInfo "Clip info: \n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: 30fps NTSC data gevonden, framerate wordt aangepast.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: 24fps progressive NTSC data gevonden, framerate wordt aangepast.\n"

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "kon codec niet openen\n"
#define MSGTR_CantCloseCodec "kon codec niet sluiten\n"

#define MSGTR_MissingDLLcodec "FOUT: Kon de nodige DirectShow codec niet openen: %s\n"
#define MSGTR_ACMiniterror "Kon Win32/ACM AUDIO codec niet laden/initialiseren (ontbrekend DLL bestand?)\n"
#define MSGTR_MissingLAVCcodec "Kan codec codec '%s' niet vinden in libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: FATAAL: EOF tijdens het zoeken naar sequentie header\n"
#define MSGTR_CannotReadMpegSequHdr "FATAAL: Kan sequentie header niet lezen!\n"
#define MSGTR_CannotReadMpegSequHdrEx "FATAAL: Kan sequentie header extension niet lezen!\n"
#define MSGTR_BadMpegSequHdr "MPEG: Foutieve sequentie header!\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Foutieve sequentie header extension!\n"

#define MSGTR_ShMemAllocFail "Kan gedeeld geheugen niet toewijzen\n"
#define MSGTR_CantAllocAudioBuf "Kan audio uitvoer buffer niet toewijzen\n"

#define MSGTR_UnknownAudio "Onbekend/ontbrekend audio formaat, gebruik -nosound\n"

#define MSGTR_UsingExternalPP "[PP] Gebruik makend van externe postprocessing filter, max q = %d\n"
#define MSGTR_UsingCodecPP "[PP] Gebruik makend van de codec's interne postprocessing, max q = %d\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Aangevraagde video codec familie [%s] (vfm=%s) niet beschikbaar (activeer het bij het compileren!)\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Aangevraagde audio codec familie [%s] (afm=%s) niet beschikbaar (activeer het bij het compileren!)\n"
#define MSGTR_OpeningVideoDecoder "Bezig met het openen van de video decoder: [%s] %s\n"
#define MSGTR_OpeningAudioDecoder "Bezig met het openen van de audio decoder: [%s] %s\n"
#define MSGTR_VDecoderInitFailed "VDecoder initialisatie mislukt :(\n"
#define MSGTR_ADecoderInitFailed "ADecoder initialisatie mislukt :(\n"
#define MSGTR_ADecoderPreinitFailed "ADecoder preinitialisatie mislukt :(\n"

// LIRC:
#define MSGTR_LIRCopenfailed "Laden van lirc ondersteuning mislukt!\n"
#define MSGTR_LIRCcfgerr "Lezen van LIRC config bestand mislukt %s!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Kon video filter '%s' niet vinden\n"
#define MSGTR_CouldNotOpenVideoFilter "Kon video filter '%s' niet openen\n"
#define MSGTR_OpeningVideoFilter "Bezig met het openen van video filter: "
#define MSGTR_CannotFindColorspace "Kan geen gemeenschappelijke colorspace vinden, zelfs bij gebruik van 'scale' :(\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Kon geen bijpassende kleurenruimte vinden - ik probeer opnieuw met -vf scale...\n"
#define MSGTR_MovieAspectIsSet "Film-Aspect is %.2f:1 - voorscalering naar het correcte film-aspect.\n"
#define MSGTR_MovieAspectUndefined "Movie-Aspect is niet gedefinieerd - geen voorscalering toegepast.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "De binary codecs moeten worden geinstalleerd of bijgewerkt.\nZie http://www.mplayerhq.hu/dload.html\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Kon de \"EWMH fullscreen Event!\" niet versturen\n"

#define MSGTR_InsertingAfVolume "[Mixer] Geen hardware mixing, een volume filter wordt gebruikt.\n"
#define MSGTR_NoVolume "[Mixer] Geen geluidsvolume controle beschikbaar.\n"

// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "Over MPlayer"
#define MSGTR_GUI_Add "Toevoegen"
#define MSGTR_GUI_AspectRatio "Aspect ratio"
#define MSGTR_GUI_Audio "Audio"
#define MSGTR_GUI_AudioDelay "Audio vertraging"
#define MSGTR_GUI_AudioDriverConfiguration "Audio configuratie"
#define MSGTR_GUI_AudioTrack "Laad extern audio bestand"
#define MSGTR_GUI_AudioTracks "Audio track"
#define MSGTR_GUI_AvailableDrivers "Beschikbare drivers:"
#define MSGTR_GUI_AvailableSkins "Skins"
#define MSGTR_GUI_Bass "Bas"
#define MSGTR_GUI_Blur "Blur"
#define MSGTR_GUI_Brightness "Helderheid"
#define MSGTR_GUI_Browse "Browse"
#define MSGTR_GUI_Cache "Cache"
#define MSGTR_GUI_CacheSize "Cache grootte"
#define MSGTR_GUI_Cancel "Annuleer"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Centrum"
#define MSGTR_GUI_Channel1 "Kanaal 1"
#define MSGTR_GUI_Channel2 "Kanaal 2"
#define MSGTR_GUI_Channel3 "Kanaal 3"
#define MSGTR_GUI_Channel4 "Kanaal 4"
#define MSGTR_GUI_Channel5 "Kanaal 5"
#define MSGTR_GUI_Channel6 "Kanaal 6"
#define MSGTR_GUI_ChannelAll "Allemaal"
#define MSGTR_GUI_ChapterN "hoofdstuk %d"
#define MSGTR_GUI_ChapterNN "Hoofdstuk %2d"
#define MSGTR_GUI_Chapters "Hoofdstukken"
#define MSGTR_GUI_Clear "Wis"
#define MSGTR_GUI_CodecFamilyAudio "Audio codec familie"
#define MSGTR_GUI_CodecFamilyVideo "Video codec familie"
#define MSGTR_GUI_Coefficient "Coefficient"
#define MSGTR_GUI_ConfigFileError "Fout in het configuratie bestand of configuratie bestand ontbreekt\n"
#define MSGTR_GUI_Configure "Configureer"
#define MSGTR_GUI_ConfigureDriver "Configureer driver"
#define MSGTR_GUI_Contrast "Contrast"
#define MSGTR_GUI_Cp874 "Thaise karakterset (CP874)"
#define MSGTR_GUI_Cp936 "Vereenvoudigde Chinese karakterset (CP936)"
#define MSGTR_GUI_Cp949 "Koreaanse karakterset (CP949)"
#define MSGTR_GUI_Cp1250 "Slavische/Centraal Europese Windows (CP1250)"
#define MSGTR_GUI_Cp1251 "Cyrillisch Windows (CP1251)"
#define MSGTR_GUI_CpBIG5 "Traditionele Chinese karakterset (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "West Europese talen (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Slavische/Centraal Europese talen (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Esperanto, Galician, Maltees, Turks (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Oude Baltische karakterset (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Cyrillisch (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Arabisch (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Modern Grieks (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Hebreewse karakterset (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Turks (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Baltisch (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Celtisch (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "West Europese talen with Euro (ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Russisch (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Ukraiens, Belarusian (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Japanse karakterset (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_DefaultSetting "standaard driver"
#define MSGTR_GUI_Delay "Vertraging"
#define MSGTR_GUI_Demuxers_Codecs "Codecs & demuxer"
#define MSGTR_GUI_Device "Apparaat"
#define MSGTR_GUI_DeviceCDROM "CD-ROM apparaat"
#define MSGTR_GUI_DeviceDVD "DVD apparaat"
#define MSGTR_GUI_Directory "Pad"
#define MSGTR_GUI_DirectoryTree "Directory tree"
#define MSGTR_GUI_DropSubtitle "Drop ondertitels..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAutomaticAVSync "AutoSync aan/uit"
#define MSGTR_GUI_EnableCache "Cache aan/uit"
#define MSGTR_GUI_EnableDirectRendering "Gebruik directe rendering"
#define MSGTR_GUI_EnableDoubleBuffering "Gebruik dubbele buffering"
#define MSGTR_GUI_EnableEqualizer "Gebruik equalizer"
#define MSGTR_GUI_EnableExtraStereo "Gebruik extra stereo"
#define MSGTR_GUI_EnableFrameDropping "Gebruik frame dropping"
#define MSGTR_GUI_EnableFrameDroppingIntense "Gebruik HARD frame drop(gevaarlijk)"
#define MSGTR_GUI_EnablePlaybar "Activeer playbar"
#define MSGTR_GUI_EnablePostProcessing "Gebruik postprocess"
#define MSGTR_GUI_Encoding "Encodering"
#define MSGTR_GUI_Equalizer "Equalizer"
#define MSGTR_GUI_Error "Fout!"
#define MSGTR_GUI_ErrorFatal "Fatale fout!"
#define MSGTR_GUI_File "Speel bestand"
#define MSGTR_GUI_Files "Bestanden"
#define MSGTR_GUI_Flip "Keer het beeld ondersteboven"
#define MSGTR_GUI_Font "Lettertype"
#define MSGTR_GUI_FrameRate "FPS"
#define MSGTR_GUI_FrontLeft "Links vooraan"
#define MSGTR_GUI_FrontRight "Rechts vooraan"
#define MSGTR_GUI_HideVideoWindow "Toon video venster wanneer inactief"
#define MSGTR_GUI_Hue "Hue"
#define MSGTR_GUI_Lavc "Gebruik LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Auto kwaliteit"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Misc"
#define MSGTR_GUI_Mixer "Mixer"
#define MSGTR_GUI_MixerChannel "Mixer kanaal"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Sorry, je kan geen niet-MPEG bestanden met je DXR3/H+ apparaat aspelen zonder het bestand te herencoderen.\nActiveer lavc in het DXR3/H+ configuratiescherm.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Sorry, niet genoeg geheugen voor tekenbuffer.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "geen bestand geladen"
#define MSGTR_GUI_MSG_NoMediaOpened "geen mediabestand geopend"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Denk eraan, dat sommige functies het afspelen herstarten."
#define MSGTR_GUI_MSG_SkinBitmapConversionError "24 bit naar 32 bit converteerfout (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "bestand niet gevonden (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "PNG lees fout (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Skin niet gevonden (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "16 bits of minder kleurendiepte bitmap niet ondersteund (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[skin] fout in skin configuratie bestand op regel %d: %s"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "font bestand niet gevonden\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "font image bestand niet gevonden\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "onbestaande font identifier (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "onvoldoende geheugen\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "te veel fonts gedeclareerd\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "onbekende boodschap: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "onbekende parameter (%s)\n"
#define MSGTR_GUI_MSG_VideoOutError "Sorry, kon geen GUI compatibele video uitvoer driver vinden.\n"
#define MSGTR_GUI_Mute "Mute"
#define MSGTR_GUI_NetworkStreaming "Netwerk streaming..."
#define MSGTR_GUI_Next "Volgende stream"
#define MSGTR_GUI_NoChapter "geen hoofdstuk"
#define MSGTR_GUI__none_ "(geen)"
#define MSGTR_GUI_NonInterleavedParser "Gebruik non-interleaved AVI parser"
#define MSGTR_GUI_NormalizeSound "Normalizeer het geluid"
#define MSGTR_GUI_Ok "Ok"
#define MSGTR_GUI_Open "Open..."
#define MSGTR_GUI_Original "Origineel"
#define MSGTR_GUI_OsdLevel "OSD level"
#define MSGTR_GUI_OSD_Subtitles "Ondertiteling & OSD"
#define MSGTR_GUI_Outline "Outline"
#define MSGTR_GUI_PanAndScan "Panscan"
#define MSGTR_GUI_Pause "Pauzeer"
#define MSGTR_GUI_Play "Speel"
#define MSGTR_GUI_Playback "Afspelen"
#define MSGTR_GUI_Playlist "AfspeelLijst"
#define MSGTR_GUI_Position "Positie"
#define MSGTR_GUI_PostProcessing "Postprocess"
#define MSGTR_GUI_Preferences "Voorkeuren"
#define MSGTR_GUI_Previous "Vorige stream"
#define MSGTR_GUI_Quit "Afsluiten"
#define MSGTR_GUI_RearLeft "Links achter"
#define MSGTR_GUI_RearRight "Rechts achter"
#define MSGTR_GUI_Remove "Verwijderen"
#define MSGTR_GUI_Saturation "Saturatie"
#define MSGTR_GUI_SaveWindowPositions "Bewaar de positie van het venster"
#define MSGTR_GUI_ScaleMovieDiagonal "Proportioneel met de filmdiagonaal"
#define MSGTR_GUI_ScaleMovieHeight "Proportioneel met de filmhoogte"
#define MSGTR_GUI_ScaleMovieWidth "Proportioneel met de filmbreedte"
#define MSGTR_GUI_ScaleNo "Geen automatische scalering"
#define MSGTR_GUI_SeekingInBrokenMedia "Reconstrueer de index tabel als dat nodig is"
#define MSGTR_GUI_SelectAudioFile "Selecteer extern audio kanaal..."
#define MSGTR_GUI_SelectedFiles "Geselecteerde bestanden"
#define MSGTR_GUI_SelectFile "Selecteer bestand..."
#define MSGTR_GUI_SelectFont "Selecteer lettertype..."
#define MSGTR_GUI_SelectSubtitle "Selecteer ondertiteling..."
#define MSGTR_GUI_SizeDouble "Dubbele grootte"
#define MSGTR_GUI_SizeFullscreen "Volledig scherm"
#define MSGTR_GUI_SizeNormal "Normale grootte"
#define MSGTR_GUI_SizeOSD "OSD schaal"
#define MSGTR_GUI_SizeSubtitles "Tekst schaal"
#define MSGTR_GUI_SkinBrowser "Skin browser"
#define MSGTR_GUI_Sponsored "GUI ontwikkeling gesponsord door UHU Linux"
#define MSGTR_GUI_StartFullscreen "Start op volledige schermgrootte"
#define MSGTR_GUI_Stop "Stop"
#define MSGTR_GUI_Subtitle "Ondertiteling"
#define MSGTR_GUI_SubtitleAllowOverlap "Activeer/deactiveer ondertitel overlapping"
#define MSGTR_GUI_SubtitleAutomaticLoad "Geen automatisch laden van ondertiteling"
#define MSGTR_GUI_SubtitleConvertMpsub "Converteer de gegeven ondertiteling naar MPlayer's ondertitelingsformaat"
#define MSGTR_GUI_SubtitleConvertSrt "Converteer de gegeven ondertiteling naar het tijdsgebaseerde SubViewer(SRT) formaat"
#define MSGTR_GUI_Subtitles "Ondertiteling"
#define MSGTR_GUI_SyncValue "Autosync"
#define MSGTR_GUI_TitleNN "Titel %2d"
#define MSGTR_GUI_Titles "Titels"
#define MSGTR_GUI_TrackN "Track %d"
#define MSGTR_GUI_TurnOffXScreenSaver "Stop XScreenSaver"
#define MSGTR_GUI_URL "Speel URL"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Video"
#define MSGTR_GUI_VideoEncoder "Video encoder"
#define MSGTR_GUI_VideoTracks "Video track"
#define MSGTR_GUI_Warning "Waarschuwing!"

// ======================= VO Video Output drivers ========================

#define MSGTR_VO_GenericError "Deze fout is opgetreden"
#define MSGTR_VO_UnableToAccess "Geen toegang"
#define MSGTR_VO_ExistsButNoDirectory "bestaat al, maar is geen directory."
#define MSGTR_VO_DirExistsButNotWritable "Output directory bestaat reeds, maar is alleen-lezen."
#define MSGTR_VO_CantCreateDirectory "Kan de output directory niet aanmaken."
#define MSGTR_VO_CantCreateFile "Kan het output bestand niet aanmaken."
#define MSGTR_VO_DirectoryCreateSuccess "Output directory succesvol aangemaakt."
#define MSGTR_VO_ValueOutOfRange "Waarde buiten het bereik"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Progressieve JPEG geactiveerd."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Progressieve JPEG gedesactiveerd."
#define MSGTR_VO_JPEG_BaselineJPEG "Baseline JPEG geactiveerd."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Baseline JPEG gedesactiveerd."
#define MSGTR_VO_PNM_ASCIIMode "ASCII mode geactiveerd."
#define MSGTR_VO_PNM_RawMode "Raw mode geactiveerd."

// vo_pnm.c
#define MSGTR_VO_PNM_PPMType "Zal PPM bestanden schrijven."
#define MSGTR_VO_PNM_PGMType "Zal write PGM bestanden schrijven."
#define MSGTR_VO_PNM_PGMYUVType "Zal PGMYUV bestanden schrijven."

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "De interlaced mode vereist een beeldhoogte die deelbaar is door vier."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "De beeldbreedte moet deelbaar zijn door twee."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Kan geen geheugen of \"file handle\" verkrijgen om de \"%s\" te schrijven!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Fout bij het schrijven van het beeld naar de output!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Ongekende subapparaat: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "De volgende interlaced output mode wordt gebruikt, top-field first."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "De volgende interlaced output mode zordt gebruikt, bottom-field first."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "De (standaard) progressive frame mode wordt gebruikt."

// Old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "De pgm video output driver is vervangen door -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "De md5 video output driver is vervangen door -vo md5sum.\n"

// ======================= AO Audio Output drivers ========================

// libao2

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: de alsa9 en alsa1x modules werden verwijderd, gebruik -ao alsa.\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Kan het mixer apparaat %s niet openen : %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup: De mixer van de geluidskaart heeft geen kanaal dat de standaard '%s' gebruikt.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Kan het audio apparaat niet openen %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Can't make filedescriptor blocking: %s\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Kon het audio apparaat niet instellen op %d kanalen.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: het stuurprogramma ondersteunt SNDCTL_DSP_GETOSPACE niet :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Het stuurprogramma van uw geluidskaart ondersteunt select() niet  ***\n Hercompileer MPlayer met #undef HAVE_AUDIO_SELECT in config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\nFatale fout: *** Kan het audio apparaat niet heropenen / resetten *** %s\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Verbonden met de sound server.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Kan geen stream openen.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Stream geopend.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] buffer grootte: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Kon het volume niet op  %d zetten.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] dxr2: %d Hz is niet ondersteund, probeer \"-aop list=resample\"\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound gefaald: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] latency: [server: %0.2fs, net: %0.2fs] (adjust %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] Kon de esd playback stream niet openen: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB het instellen van de audio mixer is mislukt: %s\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d s niet ondersteund, probeer \"-aop list=resample\"\n"

// ao_null.c
// This one desn't even  have any mp_msg nor printf's?? [CHECK]

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] File: %s (%s)\nPCM: Samplerate: %iHz Kanalen: %s Formaat %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Info: de snelste uitlezing wordt bereikt met -vc null -vo null\nPCM: Info: Om WAVE bestanden te schrijven gebruik -ao pcm:waveheader (standaard).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Kon %s niet openen om te schrijven!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Samplerate: %iHz Kanalen: %s Formaat %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] het %s audio stuurprogramma wordt gebruikt.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Niet ondersteund audio formaat: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] De initialisatie van de SDL Audio is gefaald: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Kan het audio apparaat niet openen: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] control.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] init: Samplerate: %iHz Kanalen: %s Formaat %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] afspelen: geen geldige apparaat.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] init: setparams gefaald: %s\nKon de gewenste samplerate niet instellen.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] init: de AL_RATE werd niet geaccepteerd.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] init: getparams gefaald: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] init: de samplerate is nu %f (de gewenste bitrate is %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] init: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] init: Kan het audio kanaal niet openen: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] uninit: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] reset: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume: ...\n"

// ao_plugin.c


// ========================== LIBMPCODECS ===================================

#define MSGTR_SamplesWanted "Samples van dit formaat zijn gewenst om de ondersteuning ervan te verbetern. Gelieve de ontwikkelaars te contacteren."
