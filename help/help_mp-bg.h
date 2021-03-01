// Sync'ed with help_mp-en.h 1.167
//
// Преведено от А. Димитров, plazmus@gmail.com
// Всички предложения са добре дошли.

// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<пътечка>   пуска (S)VCD (Super Video CD) пътечка (без монтиране!)\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<номер>     пуска DVD заглавие от устройство, вместо от файл\n"\
                      " -alang/-slang     избор на език за DVD аудиo/субтитри (чрез 2-буквен код)\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Употреба:   mplayer [опции] [url|път/]име_на_файл\n"\
"\n"\
"Основни опции:   (пълният списък е в ръководството - 'man mplayer')\n"\
" -vo <дрв[:устр]>  избор на видео драйвер & устройство ('-vo help' дава списък)\n"\
" -ao <дрв[:устр]>  избор на звуков драйвер & устройство ('-ao help' дава списък)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -ss <позиция>     превъртане до дадена (в секунди или чч:мм:сс) позиция\n"\
" -nosound          изключване на звука\n"\
" -fs               пълноекранно възпроизвеждане (или -vm, -zoom, вж. manpage)\n"\
" -x <x> -y <y>     избор на резолюция (използва се с -vm или -zoom)\n"\
" -sub <файл>       задава файла със субтитри (вижте също -subfps и -subdelay)\n"\
" -playlist <файл>  отваря playlist файл\n"\
" -vid x -aid y     избор на видео (x) и аудио (y) поток за възпроизвеждане\n"\
" -fps x -srate y   смяна на видео (x кадри в секунда) и аудио (y Hz) честотата\n"\
" -pp <качество>    включва филтър за допълнителна обработка на образа\n"\
"                   вижте ръководството и документацията за подробности\n"\
" -framedrop        разрешава прескачането на кадри (при бавни машини)\n"\
"\n"\
"Основни клавиши:   (пълен списък има в ръководството, проверете също input.conf)\n"\
" <-  или  ->       превърта назад/напред с 10 секунди\n"\
" up или down       превърта назад/напред с 1 минута\n"\
" pgup или pgdown   превърта назад/напред с 10 минути\n"\
" < или >           стъпка назад/напред в playlist списъка\n"\
" p или SPACE       пауза (натиснете произволен клавиш за продължение)\n"\
" q или ESC         спиране на възпроизвеждането и изход от програмата\n"\
" + или -           промяна закъснението на звука с +/- 0.1 секунда\n"\
" o                 превключва OSD режима: без/лента за превъртане/лента и таймер\n"\
" * или /           увеличава или намалява силата на звука (PCM)\n"\
" z или x           променя закъснението на субтитрите с +/- 0.1 секунда\n"\
" r или t           премества субтитрите нагоре/надолу, вижте и -vf expand\n"\
"\n"\
" * * * ЗА ПОДРОБНОСТИ, ДОПЪЛНИТЕЛНИ ОПЦИИ И КЛАВИШИ, ВИЖТЕ РЪКОВОДСТВОТО! * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c:

#define MSGTR_Exiting "\nИзлизане от програмата...\n"
#define MSGTR_ExitingHow "\nИзлизане от програмата... (%s)\n"
#define MSGTR_Exit_quit "Изход"
#define MSGTR_Exit_eof "Край на файла"
#define MSGTR_Exit_error "Фатална грешка"
#define MSGTR_IntBySignal "\nMPlayer е прекъснат от сигнал %d в модул: %s\n"
#define MSGTR_NoHomeDir "HOME директорията не може да бъде открита.\n"
#define MSGTR_GetpathProblem "Проблем с функция get_path(\"config\") \n"
#define MSGTR_CreatingCfgFile "Създава се конфигурационен файл: %s\n"
#define MSGTR_CantLoadFont "Не може да се зареди шрифт: %s\n"
#define MSGTR_CantLoadSub "Не могат да бъдат заредени субтитри: %s\n"
#define MSGTR_DumpSelectedStreamMissing "dump: ФАТАЛНО: Избраният поток липсва!\n"
#define MSGTR_CantOpenDumpfile "Не може да се отвори файл за извличане.\n"
#define MSGTR_CoreDumped "Данните извлечени ;)\n"
#define MSGTR_FPSnotspecified "Броя кадри в секунда не е указан или е невалиден, ползвайте опцията -fps .\n"
#define MSGTR_TryForceAudioFmtStr "Опит за ползване на фамилия аудио кодеци %s...\n"
#define MSGTR_CantFindAudioCodec "Не може да бъде намерен кодек за този аудио формат 0x%X.\n"
#define MSGTR_TryForceVideoFmtStr "Опит за ползване на фамилия видео кодеци %s...\n"
#define MSGTR_CantFindVideoCodec "Няма подходящ кодек за указаните -vo и видео формат 0x%X.\n"
#define MSGTR_CannotInitVO "ФАТАЛНО: Видео драйвера не може да бъде инициализиран.\n"
#define MSGTR_CannotInitAO "Аудио устройството не може да бъде отворено/инициализирано -> няма звук.\n"
#define MSGTR_StartPlaying "Започва възпроизвеждането...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"           ************************************************\n"\
"           **** Вашата система е твърде БАВНА за това!  ****\n"\
"           ************************************************\n\n"\
"Възможни причини, проблеми, решения:\n"\
"- Най-вероятно: неработещ/бъгав _аудио_ драйвер\n"\
"  - Опитайте -ao sdl или ползвайте OSS емулацията на ALSA.\n"\
"  - Експериментирайте с различни стойности на -autosync, 30 е добро начало.\n"\
"- Бавно видео извеждане\n"\
"  - Опитайте друг -vo драйвер (-vo help за списък) или пробвайте -framedrop!\n"\
"- Бавен процесор\n"\
"  - Не пускайте голям DVD/DivX филм на бавен процесор! Пробвайте -hardframedrop.\n"\
"- Повреден файл\n"\
"  - Опитайте различни комбинации от  -nobps -ni -forceidx -mc 0.\n"\
"- Бавен източник (NFS/SMB, DVD, VCD и т.н.)\n"\
"  - Опитайте -cache 8192.\n"\
"- Използвате -cache за non-interleaved AVI файл?\n"\
"  - Опитайте -nocache.\n"\
"Прочетете DOCS/HTML/en/video.html за съвети относно настройките.\n"\
"Ако нищо не помага, прочетете DOCS/HTML/en/bugreports.html.\n\n"

#define MSGTR_NoGui "MPlayer е компилиран без графичен интерфейс.\n"
#define MSGTR_GuiNeedsX "Графичния интерфейс на MPlayer изисква X11.\n"
#define MSGTR_Playing "Възпроизвеждане на %s.\n"
#define MSGTR_NoSound "Аудио: няма звук\n"
#define MSGTR_FPSforced "Наложени са %5.3f кадъра в секунда (ftime: %5.3f).\n"
#define MSGTR_AvailableVideoOutputDrivers "Достъпни видео драйвери:\n"
#define MSGTR_AvailableAudioOutputDrivers "Достъпни аудио драйвери:\n"
#define MSGTR_AvailableAudioCodecs "Достъпни аудио кодеци:\n"
#define MSGTR_AvailableVideoCodecs "Достъпни видео кодеци:\n"
#define MSGTR_AvailableAudioFm "Достъпни (вградени) фамилии аудио кодеци/драйвери:\n"
#define MSGTR_AvailableVideoFm "Достъпни (вградени) фамилии видео кодеци/драйвери:\n"
#define MSGTR_AvailableFsType "Достъпни пълноекранни режими:\n"
#define MSGTR_CannotReadVideoProperties "Видео: Параметрите не могат да бъдат прочетени.\n"
#define MSGTR_NoStreamFound "Не е открит поток.\n"
#define MSGTR_ErrorInitializingVODevice "Грешка при отваряне/инициализиране на избраното видео устройство (-vo).\n"
#define MSGTR_ForcedVideoCodec "Наложен видео кодек: %s\n"
#define MSGTR_ForcedAudioCodec "Наложен аудио кодек: %s\n"
#define MSGTR_Video_NoVideo "Видео: няма видео\n"
#define MSGTR_NotInitializeVOPorVO "\nФАТАЛНО: Видео филтъра (-vf) или изхода (-vo) не могат да бъдат инициализирани.\n"
#define MSGTR_Paused "  =====  ПАУЗА  =====" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\nPlaylist-ът не може да бъде зареден %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer катастрофира заради 'Невалидна инструкция'.\n"\
"  Може да е бъг в кода за динамично установяване на процесора...\n"\
"  Моля прочетете DOCS/HTML/en/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer катастрофира заради 'Невалидна инструкция'.\n"\
"  Това обикновено се случва когато бъде пуснат на процесор, различен от този\n"\
"  за който е компилиран/оптимизиран.\n"\
"  Проверете това!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer катастрофира заради лоша употреба на процесора/копроцесора/паметта.\n"\
"  рекомпилирайте MPlayer с --enable-debug и направете  backtrace и\n"\
"  дизасемблиране с 'gdb'.\nЗа подробности - DOCS/HTML/en/bugreports_what.html#bugreports_crash.\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer катастрофира. Tова не трябва да се случва.\n"\
"  Може да е бъг в кода на MPlayer _или_ във драйверите ви _или_ във\n"\
"  версията на gcc. Ако смятате, че е по вина на MPlayer, прочетете\n"\
"  DOCS/HTML/en/bugreports.html и следвайте инструкциите там. Ние не можем\n"\
"  и няма да помогнем, ако не осигурите тази информация, когато съобщавате за бъг.\n"
#define MSGTR_LoadingConfig "Зарежда се конфигурационен файл '%s'\n"
#define MSGTR_AddedSubtitleFile "SUB: добавен е файл със субтитри (%d): %s\n"
#define MSGTR_RTCDeviceNotOpenable "Грешка при отваряне на %s: %s (необходими са права за четене).\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Linux RTC грешка при инициализация в ioctl (rtc_irqp_set кд%lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Добавете \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" към системните стартови скриптове.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Linux RTC init грешка в ioctl (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "Внимание: Функцията getch2_init е извикана двукратно!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Видео филтъра libmenu не може да бъде отворен без root меню %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Грешка при предварителна инициализация на аудио филтрите!\n"
#define MSGTR_LinuxRTCReadError "Linux RTC грешка при четене: %s\n"
#define MSGTR_SoftsleepUnderflow "Внимание! Softsleep underflow!\n"

#define MSGTR_EdlOutOfMem "Не може да се задели достатъчно памет за EDL данните.\n"
#define MSGTR_EdlRecordsNo "Прочетени са %d EDL действия.\n"
#define MSGTR_EdlQueueEmpty "Няма EDL действия, които да бъдат извършени.\n"
#define MSGTR_EdlCantOpenForWrite "EDL файла [%s] не може да бъде отворен за запис.\n"
#define MSGTR_EdlNOsh_video "EDL не може да се ползва без видео, изключва се.\n"
#define MSGTR_EdlNOValidLine "Невалиден ред в EDL: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Зле форматиран EDL ред [%d] Отхвърля се.\n"
#define MSGTR_EdlBadLineOverlap "Последната позиция за спиране беше [%f]; следващата за пускане е "\
"[%f]. Елементите в списъка трябва да са в хронологичен ред, не могат да се препокриват.\n"
#define MSGTR_EdlBadLineBadStop "Времето за спиране трябва да е след времето за пускане.\n"

// mencoder.c:

#define MSGTR_UsingPass3ControlFile "Използва се файл за контрол на pass3: %s\n"
#define MSGTR_MissingFilename "\nЛипсва име на файл.\n\n"
#define MSGTR_CannotOpenFile_Device "Файла/устройството не може да бъде отворен.\n"
#define MSGTR_CannotOpenDemuxer "Не може да бъде отворен разпределител.\n"
#define MSGTR_NoAudioEncoderSelected "\nНе е избран аудио енкодер (-oac). Изберете или енкодер (вижте -oac help) или опцията -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nНе е избран видео енкодер (-ovc). Изберете си (вижте -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Изходния файл '%s'не може да бъде отворен.\n"
#define MSGTR_EncoderOpenFailed "Енкодерът не може да бъде отворен.\n"
#define MSGTR_ForcingOutputFourcc "Налагане на изходния fourcc код да бъде %x [%.4s]\n"
#define MSGTR_DuplicateFrames "\n%d дублиращи се кадъра!\n"
#define MSGTR_SkipFrame "\nПрескочен кадър!\n"
#define MSGTR_ResolutionDoesntMatch "\nНовият видео файл има различна резолюция или цветови формат от предишния.\n"
#define MSGTR_FrameCopyFileMismatch "\nВсички видео файлове трябва да имат идентични резолюции, кадрови честоти и кодеци за -ovc copy.\n"
#define MSGTR_AudioCopyFileMismatch "\nВсички файлове трябва да имат идентични аудио кодеци и формати за -oac copy.\n"
#define MSGTR_NoSpeedWithFrameCopy "ПРЕДУПРЕЖДЕНИЕ: -speed не работи гарантирано правилно с -oac copy!\n"\
"Кодирането ви може да се окаже повредено!\n"
#define MSGTR_ErrorWritingFile "%s: Грешка при запис на файла.\n"
#define MSGTR_RecommendedVideoBitrate "Препоръчителен битрейт за %s CD: %d\n"
#define MSGTR_VideoStreamResult "\nВидео поток: %8.3f Кбита/с  (%d B/s)  размер: %"PRIu64" байта  %5.3f сек.  %d кадъра\n"
#define MSGTR_AudioStreamResult "\nАудио поток: %8.3f Кбита/с  (%d B/s)  размер: %"PRIu64" байта  %5.3f сек.\n"
#define MSGTR_OpenedStream "успех: формат: %d  данни: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videocodec: framecopy (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "audiocodec: framecopy (format=%x chans=%d rate=%d bits=%d B/s=%d sample-%d)\n"
#define MSGTR_MP3AudioSelected "Избрано е MP3 аудио\n"
#define MSGTR_SettingAudioDelay "АУДИО ЗАКЪСНЕНИЕТО е настроено на %5.3f\n"
#define MSGTR_LimitingAudioPreload "Предварителното аудио зареждане е ограничено на 0.4с\n"
#define MSGTR_IncreasingAudioDensity "Гъстотата на звука е увеличена на 4\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Налагане на нулево предварително аудио зареждане, max pts correction to 0\n"
#define MSGTR_LameVersion "LAME версия %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Грешка: Указаният битрейт е извън допустимите граници за този профил\n"\
"\n"\
"Когато използвате този режим трябва да въведете стойност между \"8\" и \"320\"\n"\
"\n"\
"Допълнителна информация може да получите с: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Грешка: Не сте въвели валиден профил и/или опции с preset\n"\
"\n"\
"Достъпните профили са:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Режим) - Не е нужно изрично да указвате ABR режима.\n"\
"                      За да го ползвате, просто укажете битрейт. Например:\n"\
"                      \"preset=185\" активира този\n"\
"                      профил и ползва средно 185 килобита в секунда.\n"\
"\n"\
"    Няколко примера:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" или \"-lameopts  cbr:preset=192       \"\n"\
" или \"-lameopts      preset=172       \"\n"\
" или \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Допълнителна информация можете да получите с: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Профилите са създадени за да осигуряват най-доброто възможно качество.\n"\
"\n"\
"В по-голямата си част те са били предмет на сериозни тестове\n"\
"за да се осигури и потвърди това качество.\n"\
"\n"\
"Непрекъснато се обновяват, съгласно най-новите разработки\n"\
"и полученият резултат би трябвало да ви осигури най-доброто\n"\
"качество постижимо с LAME.\n"\
"\n"\
"За да активирате тези профили:\n"\
"\n"\
"   За VBR режими (най-високо качество):\n"\
"\n"\
"     \"preset=standard\" Tози профил е подходящ за повеето хора и повечето\n"\
"                             видове музика и притежава доста високо качество.\n"\
"\n"\
"     \"preset=extreme\" Ако имате изключително добър слух и оборудване от\n"\
"                             високо ниво, този профил ще осигури\n"\
"                             малко по-добро качество от \"standard\"\n"\
"                             режима.\n"\
"\n"\
"   За CBR 320Кбита/с (профил с най-високото възможно качество):\n"\
"\n"\
"     \"preset=insane\"  Настройките в този профил са прекалени за повечето\n"\
"                             хора и ситуации, но ако се налага\n"\
"                             да постигнете абсолютно максимално качество\n"\
"                             без значение от размера на файла, това е начина.\n"\
"\n"\
"   За ABR режим (високо качество при зададем битрейт, но не колкото при VBR):\n"\
"\n"\
"     \"preset=<кбита/с>\"  Този профил обикновено дава добро качество за\n"\
"                             зададения битрейт. В зависимост от указания\n"\
"                             битрейт, профилът ще определи оптималните за\n"\
"                             случая настройки.\n"\
"                             Въпреки че този метод върши работа, той не е\n"\
"                             толкова гъвкав, колкото VBR, и обикновено не\n"\
"                             достига качеството на VBR при високи битрейтове.\n"\
"\n"\
"Следните опции са достъпни за съответните профили:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Режим) - Не е нужно изрично да указвате ABR режима.\n"\
"                      За да го ползвате, просто укажете битрейт. Например:\n"\
"                      \"preset=185\" активира този\n"\
"                      профил и ползва средно 185 килобита в секунда.\n"\
"\n"\
"   \"fast\" - Разрешава новия, бърз VBR за определен профил. Недостаък на това\n"\
"            е, че за сметка на скоростта често полученият битрейт е по-висок,\n"\
"            а качеството дори по-ниско в сранение с нормалния режим на работа.\n"\
"   Внимание: С настоящата версия, полученият с бързия режим битрейт, може да се\n"\
"            окаже твърде висок, в сравнение с нормалните профили.\n"\
"\n"\
"   \"cbr\"  - Ако ползвате ABR режим (прочетете по-горе) със значителен\n"\
"            битрейт като 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            може да ползвате опцията \"cbr\" за да наложите кодиране в CBR\n"\
"            режим, вместо стандартния abr mode. ABR осигурява по-високо\n"\
"            качество, но CBR може да е по-подходящ в ситуации, като\n"\
"            предаването на mp3 през интернет поток.\n"\
"\n"\
"    Например:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" или \"-lameopts  cbr:preset=192       \"\n"\
" или \"-lameopts      preset=172       \"\n"\
" или \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Достъпни са някои псевдоними за ABR режим:\n"\
"phone => 16kbps/моно        phon+/lw/mw-eu/sw => 24kbps/моно\n"\
"mw-us => 40kbps/моно        voice => 56kbps/моно\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit "Не могат да се зададат LAME опциите, проверете битрейтовете/честотите на дискретите,"\
"някои много ниски битрейтове (<32) изискват ниски честоти на дискретите (напр. -srate 8000)."\
"Ако нищо друго не помага пробвайте някой preset."
#define MSGTR_ConfigFileError "грешка в конфигурационния файл"
#define MSGTR_ErrorParsingCommandLine "грешка при обработката на командния ред"
#define MSGTR_VideoStreamRequired "Задължително е да има видео поток!\n"
#define MSGTR_ForcingInputFPS "Входящите кадри в секунда ще се интерпретират като %5.3f\n"
#define MSGTR_DemuxerDoesntSupportNosound "Tози разпределител все още не поддържа -nosound .\n"
#define MSGTR_MemAllocFailed "не може да задели памет\n"
#define MSGTR_NoMatchingFilter "Не може да бъде намерен подходящ филтър/изходен аудио формат!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, може би заради C компилатора?\n"
#define MSGTR_NoLavcAudioCodecName "LAVC аудио, Липсва име на кодек!\n"
#define MSGTR_LavcAudioCodecNotFound "Aудио LAVC, не може да се намери енкодер за кодека %s\n"
#define MSGTR_CouldntAllocateLavcContext "Aудио LAVC, не може да задели контекст!\n"
#define MSGTR_CouldntOpenCodec "Не може да отвори кодек %s, br=%d\n"

// cfg-mencoder.h:

#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     променлив битрейт метод\n"\
"                0: cbr\n"\
"                1: mt\n"\
"                2: rh(подразбира се)\n"\
"                3: abr\n"\
"                4: mtrh\n"\
"\n"\
" abr           среден битрейт\n"\
"\n"\
" cbr           постоянен bitrate\n"\
"               Също така налага CBR кодиране за последователни ABR режими.\n"\
"\n"\
" br=<0-1024>   указва битрейта в КБита (само за CBR и ABR)\n"\
"\n"\
" q=<0-9>       качество (0-максимално, 9-минимално) (само за VBR)\n"\
"\n"\
" aq=<0-9>      качество на алгоритъма (0-най-добро/бавно, 9-най-лошо/бързо)\n"\
"\n"\
" ratio=<1-100> коефициент на компресия\n"\
"\n"\
" vol=<0-10>    усилване на входния звук\n"\
"\n"\
" mode=<0-3>    (по-подразбиране: автоматичен)\n"\
"                0: stereo\n"\
"                1: joint-стерео\n"\
"                2: двуканален\n"\
"                3: моно\n"\
"\n"\
" padding=<0-2>\n"\
"                0: без\n"\
"                1: всички\n"\
"                2: регулирано\n"\
"\n"\
" fast          По-бързо кодиране на последователни VBR режими,\n"\
"               малко по-ниско качество и по-високи битрейтове.\n"\
"\n"\
" preset=<value> Осигурява най-високото възможно качество при зададени настройки.\n"\
"                 medium: VBR  кодиране,  добро качество\n"\
"                 (150-180 КБита/с битрейт)\n"\
"                 standard:  VBR кодиране, високо качество\n"\
"                 (170-210 Кбита/с битрейт)\n"\
"                 extreme: VBR кодиране, много-високо качество\n"\
"                 (200-240 КБита/с битрейт)\n"\
"                 insane:  CBR  кодиране, най-високо качество\n"\
"                 (320 Кбита/с битрейт)\n"\
"                 <8-320>: ABR кодиране със зададен среден битрейт.\n\n"

//codec-cfg.c:
#define MSGTR_DuplicateFourcc "дублиран FourCC код"
#define MSGTR_TooManyFourccs "твърде много FourCC кодoве/формати..."
#define MSGTR_ParseError "грешка при разчитане"
#define MSGTR_ParseErrorFIDNotNumber "грешка при разчитане (ID на формата не е число?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "грешка при разчитане (ID псевдонима на формата не е число?)"
#define MSGTR_DuplicateFID "дублирано ID на формата"
#define MSGTR_TooManyOut "твърде много изходни формати..."
#define MSGTR_InvalidCodecName "\nкодекът(%s) има невалидно име!\n"
#define MSGTR_CodecLacksFourcc "\nкодекът(%s) няма FourCC код/формат!\n"
#define MSGTR_CodecLacksDriver "\nкодекът(%s) няма драйвер!\n"
#define MSGTR_CodecNeedsDLL "\nкодекът(%s) се нуждае от 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\nкодекът(%s) се нуждае от 'outfmt'!\n"
#define MSGTR_CantAllocateComment "Не може да се задели памет за коментар. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Няма достатъчно памет за 'line': %s\n"
#define MSGTR_CantReallocCodecsp "Не може да презадели памет за '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "Името на кодека '%s' не е уникално."
#define MSGTR_CantStrdupName "Не може да се изпълни strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Не може да се изпълни strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Не може да се изпълни strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Не може да се изпълни strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d аудио & %d видео кодека\n"
#define MSGTR_CodecDefinitionIncorrect "Кодекът не е дефиниран коректно."
#define MSGTR_OutdatedCodecsConf "Tози codecs.conf е твърде стар и несъвместим с тази версия на MPlayer!"

// fifo.c

// m_config.c
#define MSGTR_SaveSlotTooOld "Твърде стар save slot е открит в lvl %d: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "Опцията %s не може да се използва в конфигурационен файл.\n"
#define MSGTR_InvalidCmdlineOption "Опцията %s не може да се ползва от командния ред.\n"
#define MSGTR_InvalidSuboption "Грешка: опцията '%s' няма подопция '%s'.\n"
#define MSGTR_MissingSuboptionParameter "Грешка: подопцията '%s' на '%s' изисква параметър!\n"
#define MSGTR_MissingOptionParameter "Грешка: опцията '%s' изисква параметър!\n"
#define MSGTR_OptionListHeader "\n Име                 Вид            Мин        Mакс      Global  CL    Конф\n\n"
#define MSGTR_TotalOptions "\nОбщо: %d опции\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM устройство '%s' не е открито.\n"
#define MSGTR_ErrTrackSelect "Грешка при избор на VCD пътечка."
#define MSGTR_ReadSTDIN "Четене от стандартния вход (stdin)...\n"
#define MSGTR_FileNotFound "Файла не е намерен: '%s'\n"

#define MSGTR_SMBInitError "Библиотеката libsmbclient не може да бъде инициализирана: %d\n"
#define MSGTR_SMBFileNotFound "'%s' не може да бъде отворен през LAN\n"

#define MSGTR_CantOpenDVD "Не може да бъде отворено DVD устройство: %s (%s)\n"
#define MSGTR_DVDnumTitles "Има %d заглавия на това DVD.\n"
#define MSGTR_DVDinvalidTitle "Невалиден номер на DVD заглавие: %d\n"
#define MSGTR_DVDnumAngles "Има %d гледни точки в това DVD заглавие..\n"
#define MSGTR_DVDinvalidAngle "Невалиден номер на гледна точка: %d\n"
#define MSGTR_DVDnoIFO "Не може да бъде отворен IFO файла на това DVD заглавие %d.\n"
#define MSGTR_DVDnoVOBs "Заглавието не може да бъде отворено (VTS_%02d_1.VOB).\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заглавната част на аудио потока %d е редефинирана.\n"
#define MSGTR_VideoStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заглавната част на видео потока %d е редефинирана.\n"
#define MSGTR_TooManyAudioInBuffer "\nTвърде много аудио пакети в буфера: (%d в %d байта).\n"
#define MSGTR_TooManyVideoInBuffer "\nТвърде много видео пакети в буфера: (%d в %d байта).\n"
#define MSGTR_MaybeNI "Може би възпроизвеждате non-interleaved поток/файл или кодекът не се е справил?\n" \
                      "За AVI файлове, опитайте да наложите non-interleaved режим със опцията -ni.\n"
#define MSGTR_SwitchToNi "\nЗле структуриран AVI файл - превключване към -ni режим...\n"
#define MSGTR_Detected_XXX_FileFormat "%s формат.\n"
#define MSGTR_FormatNotRecognized "============ За съжаление, този формат не се разпознава/поддържа =============\n"\
                                  "=== Ако този файл е AVI, ASF или MPEG поток, моля уведомете автора! ===\n"
#define MSGTR_MissingVideoStream "Не е открит видео поток.\n"
#define MSGTR_MissingAudioStream "Не е открит аудио поток -> няма звук.\n"
#define MSGTR_MissingVideoStreamBug "Липсва видео поток!? Свържете се с автора, може да е бъг :(\n"

#define MSGTR_DoesntContainSelectedStream "разпределител: Файлът не съдържа избрания аудио или видео поток.\n"

#define MSGTR_NI_Forced "Наложен"
#define MSGTR_NI_Detected "Определен"
#define MSGTR_NI_Message "%s NON-INTERLEAVED AVI файл.\n"

#define MSGTR_UsingNINI "Използва се NON-INTERLEAVED AVI формат.\n"
#define MSGTR_CouldntDetFNo "Не може да се определи броя на кадрите (за превъртане).\n"
#define MSGTR_CantSeekRawAVI "Не могат да се превъртат сурови AVI потоци. (Изисква се индекс, опитайте с -idx .)\n"
#define MSGTR_CantSeekFile "Този файл не може да се превърта.\n"

#define MSGTR_MOVcomprhdr "MOV: Поддръжката на компресирани хедъри изисква ZLIB!\n"
#define MSGTR_MOVvariableFourCC "MOV: ВНИМАНИЕ: Открит е променлив FOURCC код!?\n"
#define MSGTR_MOVtooManyTrk "MOV: ВНИМАНИЕ: твърде много пътечки"
#define MSGTR_ErrorOpeningOGGDemuxer "Не може да бъде отворен ogg разпределител.\n"
#define MSGTR_CannotOpenAudioStream "Не може да се отвори звуков поток: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Не могат да бъдат отворени субтитри: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Не може да бъде отворен аудио разпределител: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Не може да бъде отворен разпределител на субтитри: %s\n"
#define MSGTR_TVInputNotSeekable "Телевизията не може да се превърта! (Могат да се превключват евентуално каналите ;)\n"
#define MSGTR_ClipInfo "Информация за клипа:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: 30000/1001fps NTSC съдържание, превключване на кадровата честота.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: 24000/1001fps прогресивен NTSC, превключване на кадровата честота.\n"

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "Не може да бъде отворен кодек.\n"
#define MSGTR_CantCloseCodec "Не може да бъде затворен кодек.\n"

#define MSGTR_MissingDLLcodec "ГРЕШКА: необходимият DirectShow кодек %s не може да бъде отворен.\n"
#define MSGTR_ACMiniterror "Не може да се зареди/инициализира Win32/ACM АУДИО кодек (липсващ DLL файл?).\n"
#define MSGTR_MissingLAVCcodec "Не може да бъде открит кодек '%s' в libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: ФАТАЛНО: Достигнат е края на файла, по-време на търсене за sequence header.\n"
#define MSGTR_CannotReadMpegSequHdr "ФАТАЛНО: Не може да бъде прочетен sequence header.\n"
#define MSGTR_CannotReadMpegSequHdrEx "FATAL: Не може да бъде прочетено разширението на sequence header.\n"
#define MSGTR_BadMpegSequHdr "MPEG: лош sequence header\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: лошо разширение на sequence header\n"

#define MSGTR_ShMemAllocFail "Не може да се задели споделена памет.\n"
#define MSGTR_CantAllocAudioBuf "Не може да се задели аудио буфер.\n"

#define MSGTR_UnknownAudio "Неизвестен/липсващ аудио формат -> няма звук\n"

#define MSGTR_UsingExternalPP "[PP] Използване на външен филтър за допълнителна обработка, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Използване на допълнителна обработка от страна на кодека, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Заявената фамилия видео кодеци [%s] (vfm=%s) не е достъпна.\nРазрешете я по време на компилация.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Заявената фамилия аудио кодеци [%s] (afm=%s) не е достъпна.\nРазрешете я по време на компилация.\n"
#define MSGTR_OpeningVideoDecoder "Отваряне на видео декодер: [%s] %s\n"
#define MSGTR_OpeningAudioDecoder "Отваряне на аудио декодер: [%s] %s\n"
#define MSGTR_VDecoderInitFailed "Инициализацията на VDecoder се провали :(\n"
#define MSGTR_ADecoderInitFailed "Инициализацията на ADecoder се провали :(\n"
#define MSGTR_ADecoderPreinitFailed "Предварителната инициализация на ADecoder се провали :(\n"

// LIRC:
#define MSGTR_LIRCopenfailed "Няма да има LIRC поддръжка.\n"
#define MSGTR_LIRCcfgerr "Конфигурационният файл за LIRC %s не може да бъде прочетен.\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Не може да бъде открит видео филтър '%s'.\n"
#define MSGTR_CouldNotOpenVideoFilter "Не може да бъде отворен видео филтър '%s'.\n"
#define MSGTR_OpeningVideoFilter "Отваряне на видео филтър: "
#define MSGTR_CannotFindColorspace "Не може да бъде открит съответстващ цветови формат, дори с вмъкване на 'scale':(\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Не е открит подходящ цветови формат - повторен опит с -vf scale...\n"
#define MSGTR_MovieAspectIsSet "Пропорциите на филма са %.2f:1 - мащабиране до правилните пропорции .\n"
#define MSGTR_MovieAspectUndefined "Не са дефинирани пропорции - без предварително мащабиране.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Трябва да обновите/инсталирате пакета с двоичните кодеци.\nОтидете на http://www.mplayerhq.hu/dload.html\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Не може да прати EWMH fullscreen Event!\n"

#define MSGTR_InsertingAfVolume "[Смесител] Няма хардуерно смесване, вмъкване на филтър за силата на звука.\n"
#define MSGTR_NoVolume "[Смесител] Не е достъпна настройка на звука.\n"

// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "Относно MPlayer"
#define MSGTR_GUI_Add "Добавяне"
#define MSGTR_GUI_AspectRatio "Съотношение"
#define MSGTR_GUI_Audio "Аудио"
#define MSGTR_GUI_AudioDelay "Закъснение на звука"
#define MSGTR_GUI_AudioDriverConfiguration "Конфигуриране на аудио драйвера"
#define MSGTR_GUI_AudioTrack "Зареждане на външен звуков файл"
#define MSGTR_GUI_AudioTracks "Аудио писта"
#define MSGTR_GUI_AvailableDrivers "Достъпни драйвери:"
#define MSGTR_GUI_AvailableSkins "Скинове"
#define MSGTR_GUI_Bass "Бас"
#define MSGTR_GUI_Blur "Размазване"
#define MSGTR_GUI_Brightness "Светлост"
#define MSGTR_GUI_Browse "Избор"
#define MSGTR_GUI_Cache "Кеширане"
#define MSGTR_GUI_CacheSize "Размер на кеша"
#define MSGTR_GUI_Cancel "Отказ"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Централен"
#define MSGTR_GUI_Channel1 "Канал 1"
#define MSGTR_GUI_Channel2 "Канал 2"
#define MSGTR_GUI_Channel3 "Канал 3"
#define MSGTR_GUI_Channel4 "Канал 4"
#define MSGTR_GUI_Channel5 "Канал 5"
#define MSGTR_GUI_Channel6 "Канал 6"
#define MSGTR_GUI_ChannelAll "Всички"
#define MSGTR_GUI_ChapterN "Раздел %d"
#define MSGTR_GUI_ChapterNN "Раздел %2d"
#define MSGTR_GUI_Chapters "Раздели"
#define MSGTR_GUI_Clear "Изчистване"
#define MSGTR_GUI_CodecFamilyAudio "Фамилия аудио кодеци"
#define MSGTR_GUI_CodecFamilyVideo "Фамилия видео кодеци"
#define MSGTR_GUI_Coefficient "Коефициент"
#define MSGTR_GUI_ConfigFileError "грешка в конфигурационния файл\n"
#define MSGTR_GUI_Configure "Конфигурация"
#define MSGTR_GUI_ConfigureDriver "Конфигуриране на драйвера"
#define MSGTR_GUI_Contrast "Контраст"
#define MSGTR_GUI_Cp874 "Thai charset (CP874)"
#define MSGTR_GUI_Cp936 "Опростен Китайски (CP936)"
#define MSGTR_GUI_Cp949 "Kорейски (CP949)"
#define MSGTR_GUI_Cp1250 "Славянски/Централноевропейски Windows (CP1250)"
#define MSGTR_GUI_Cp1251 "Кирилица Windows (CP1251)"
#define MSGTR_GUI_CpBIG5 "Традиционен Китайски (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Западноевропейски Езици (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Славянски/Централноевропейски Езици (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Есперанто, Галски, Малтийски, Турски (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Стар Балтийски (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Кирилица (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Арабски (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Съвременен Гръцки (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Hebrew charsets (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Турски (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Балтийски (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Келтски (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Западноевропейски Езици със Euro (ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Руски (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Украински, Беларуски (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Японски (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_DefaultSetting "Подразбиращи се за драйвера"
#define MSGTR_GUI_Delay "Закъснение"
#define MSGTR_GUI_Demuxers_Codecs "Кодеци & demuxer"
#define MSGTR_GUI_Device "Устройство"
#define MSGTR_GUI_DeviceCDROM "CD-ROM устройство"
#define MSGTR_GUI_DeviceDVD "DVD устройство"
#define MSGTR_GUI_Directory "Път"
#define MSGTR_GUI_DirectoryTree "Директории"
#define MSGTR_GUI_DropSubtitle "Премахване на субтитри..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAutomaticAVSync "Автоматична синхронизация"
#define MSGTR_GUI_EnableCache "Кеширане"
#define MSGTR_GUI_EnableDirectRendering "Включване на direct rendering"
#define MSGTR_GUI_EnableDoubleBuffering "Двойно буфериране"
#define MSGTR_GUI_EnableEqualizer "Включване на еквалайзера"
#define MSGTR_GUI_EnableExtraStereo "Включване на допълнително стерео"
#define MSGTR_GUI_EnableFrameDropping "Разрешаване на прескачането на кадри"
#define MSGTR_GUI_EnableFrameDroppingIntense "Разрешаване на ИНТЕНЗИВНО прескачане на кадри (опасно)"
#define MSGTR_GUI_EnablePlaybar "Лента за превъртане"
#define MSGTR_GUI_EnablePostProcessing "Разрешаване на допълнителна обработка"
#define MSGTR_GUI_EnableSoftwareMixer "Включва Софтуерен Смесител"
#define MSGTR_GUI_Encoding "Кодировка"
#define MSGTR_GUI_Equalizer "Еквалайзер"
#define MSGTR_GUI_Error "Грешка!"
#define MSGTR_GUI_ErrorFatal "Фатална грешка!"
#define MSGTR_GUI_File "Пускане на файл"
#define MSGTR_GUI_Files "Файлове"
#define MSGTR_GUI_Flip "Преобръщане на образа"
#define MSGTR_GUI_Font "Шрифт"
#define MSGTR_GUI_FrameRate "FPS"
#define MSGTR_GUI_FrontLeft "Преден Ляв"
#define MSGTR_GUI_FrontRight "Преден Десен"
#define MSGTR_GUI_HideVideoWindow "Показване на видео прозореца при неактивност"
#define MSGTR_GUI_Hue "Тон"
#define MSGTR_GUI_Lavc "Използване на LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Автоматичен контрол на качеството"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Разни"
#define MSGTR_GUI_Mixer "Смесител"
#define MSGTR_GUI_MixerChannel "Канал на смесителя"
#define MSGTR_GUI_MSG_AddingVideoFilter "[GUI] Добавяне на видео филтър: %s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "Твърде ниска дълбочина на цветовете.\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "D&D: Не е върнат резултат!\n"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "За съжаление, не можете да възпроизвеждате различни от MPEG\nфайлове с вашето DXR3/H+ устройство без прекодиране.\nМоля разрешете lavc в полето за конфигурация на DXR3/H+ .\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "[GUI] Зареждане на субтитрите: %s\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "Няма достатъчно памет за draw buffer.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "За съжаление, няма достатъчно памет за draw buffer.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Не е зареден файл."
#define MSGTR_GUI_MSG_NoMediaOpened "Няма отворени елементи."
#define MSGTR_GUI_MSG_NotAFile0 "Това не прилича на файл...\n"
#define MSGTR_GUI_MSG_NotAFile1 "Това не прилича на файл: %s !\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Не забравяйте, да рестартирате възпроизвеждането за да влязат в сила някои опции!"
#define MSGTR_GUI_MSG_RemoteDisplay "Отдалечен дисплей, изключване на  XMITSHM.\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "[GUI] Изтриване на субтитрите.\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "Грешка при преобразуване от 24 към 32 бита (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "файлът не е намерен (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "грешка при четене на PNG (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Скинът не е намерен (%s).\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "Избраният скин ( %s ) не е намерен, ще се ползва 'default'...\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "Bitmap с 16 и по-малко бита за цвят не се поддържа (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[skin] грешка в конфигурационния файл на skin-а на ред %d: %s"
#define MSGTR_GUI_MSG_SkinFileNotFound "[skin] файлът ( %s ) не е намерен.\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "Файлът със шрифта не е намерен.\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "Файл с изображението на шрифта не е намерен.\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "несъществуващ идентификатор на шрифт (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "недостатъчно памет\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "Декларирани са твърде много шрифтове.\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "неизвестно съобщение: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "неизвестен параметър (%s)\n"
#define MSGTR_GUI_MSG_TooManyWindows "Твърде много отворени прозорци.\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "Не може да се запамети опцията '%s'.\n"
#define MSGTR_GUI_MSG_VideoOutError "За съжаление, няма съвместим с GUI видео драйвер.\n"
#define MSGTR_GUI_MSG_XShapeError "За съжаление вашата система не поддържа разширението XShape.\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "грешка в разширението за споделена памет\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "За съжаление вашата система не поддържа разширението на X за споделена памет.\n"
#define MSGTR_GUI_Mute "Без звук"
#define MSGTR_GUI_NetworkStreaming "Поток от мрежата..."
#define MSGTR_GUI_Next "Следващ"
#define MSGTR_GUI_NoChapter "Няма раздели"
#define MSGTR_GUI__none_ "(няма)"
#define MSGTR_GUI_NonInterleavedParser "Разчитане на non-interleaved AVI формат"
#define MSGTR_GUI_NormalizeSound "Изравняване на звука"
#define MSGTR_GUI_Ok "OK"
#define MSGTR_GUI_Open "Oтваряне..."
#define MSGTR_GUI_Original "Без промяна"
#define MSGTR_GUI_OsdLevel "OSD степен"
#define MSGTR_GUI_OSD_Subtitles "Субтитри и OSD"
#define MSGTR_GUI_Outline "Удебеляване"
#define MSGTR_GUI_PanAndScan "Panscan"
#define MSGTR_GUI_Pause "Пауза"
#define MSGTR_GUI_Play "Старт"
#define MSGTR_GUI_Playback "Playing"
#define MSGTR_GUI_Playlist "Списък за възпроизвеждане"
#define MSGTR_GUI_Position "Местоположение"
#define MSGTR_GUI_PostProcessing "Допълнителна обработка"
#define MSGTR_GUI_Preferences "Предпочитания"
#define MSGTR_GUI_Previous "Предишен"
#define MSGTR_GUI_Quit "Изход"
#define MSGTR_GUI_RearLeft "Заден Ляв"
#define MSGTR_GUI_RearRight "Заден Десен"
#define MSGTR_GUI_Remove "Премахване"
#define MSGTR_GUI_Saturation "Наситеност"
#define MSGTR_GUI_SaveWindowPositions "Запаметяване на местоположението на прозореца"
#define MSGTR_GUI_ScaleMovieDiagonal "Пропорционално на дължината на диагонала"
#define MSGTR_GUI_ScaleMovieHeight "Пропорционално на височината на филма"
#define MSGTR_GUI_ScaleMovieWidth "Пропорционално на широчината на филма"
#define MSGTR_GUI_ScaleNo "Без автоматично мащабиране"
#define MSGTR_GUI_SeekingInBrokenMedia "Построяване на индексната таблица наново, при необходимост"
#define MSGTR_GUI_SelectAudioFile "Избор на външен аудио канал..."
#define MSGTR_GUI_SelectedFiles "Избрани файлове"
#define MSGTR_GUI_SelectFile "Избор на файл..."
#define MSGTR_GUI_SelectFont "Избор на шрифт..."
#define MSGTR_GUI_SelectSubtitle "Избор на субтитри..."
#define MSGTR_GUI_SizeDouble "Двоен размер"
#define MSGTR_GUI_SizeFullscreen "На цял екран"
#define MSGTR_GUI_SizeHalf "Половин размер"
#define MSGTR_GUI_SizeNormal "Нормален размер"
#define MSGTR_GUI_SizeOSD "Мащаб на OSD"
#define MSGTR_GUI_SizeSubtitles "Мащаб на текста"
#define MSGTR_GUI_SkinBrowser "Избор на Skin"
#define MSGTR_GUI_Sponsored "Разработката на графичния интерфейс се спонсорира от UHU Linux"
#define MSGTR_GUI_StartFullscreen "Стартиране на цял екран"
#define MSGTR_GUI_Stop "Стоп"
#define MSGTR_GUI_Subtitle "Субтитри"
#define MSGTR_GUI_SubtitleAllowOverlap "Препокриване на субтитрите"
#define MSGTR_GUI_SubtitleAutomaticLoad "Изключване на автоматичното зареждане на субтитри"
#define MSGTR_GUI_SubtitleConvertMpsub "Преобразуване на субтитрите в формата на MPlayer"
#define MSGTR_GUI_SubtitleConvertSrt "Преобразуване на субтитрите в SubViewer (SRT) формат"
#define MSGTR_GUI_Subtitles "Език на субтитрите"
#define MSGTR_GUI_SyncValue "Степен на синхронизацията"
#define MSGTR_GUI_TitleNN "Заглавие %2d"
#define MSGTR_GUI_Titles "Заглавия"
#define MSGTR_GUI_TrackN "Писта %d"
#define MSGTR_GUI_TurnOffXScreenSaver "Изключване на XScreenSaver"
#define MSGTR_GUI_URL "Пускане от URL"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Видео"
#define MSGTR_GUI_VideoEncoder "Видео енкодер"
#define MSGTR_GUI_VideoTracks "видео писта"
#define MSGTR_GUI_Warning "Внимание!"

// ======================= VO Video Output drivers ========================

#define MSGTR_VOincompCodec "Избраното изходно видео устройство е несъвместимо с този кодек.\n"
#define MSGTR_VO_GenericError "Tази грешка е възникнала"
#define MSGTR_VO_UnableToAccess "Достъпът е невъзможен"
#define MSGTR_VO_ExistsButNoDirectory "вече съществува, но не е директория."
#define MSGTR_VO_DirExistsButNotWritable "Директорията съществува, но не е разрешен запис."
#define MSGTR_VO_CantCreateDirectory "Директорията не може да бъде създадена."
#define MSGTR_VO_CantCreateFile "Файлът не може да бъде създаден."
#define MSGTR_VO_DirectoryCreateSuccess "Директорията е успешно създадена."
#define MSGTR_VO_ValueOutOfRange "Стойността е извън допустимите граници"

// vo_aa.c

#define MSGTR_VO_AA_HelpHeader "\n\nТова са подопциите на aalib vo_aa:\n"
#define MSGTR_VO_AA_AdditionalOptions "Допълнителни опции предвидени от vo_aa:\n" \
"  help        показва това съобщение\n" \
"  osdcolor    задава цвят за osd\n  subcolor    задава цвета на субтитрите\n" \
"        параметрите за цвят са:\n           0 : нормален\n" \
"           1 : dim\n           2 : удебелен\n           3 : удебелен шрифт\n" \
"           4 : обърнат\n           5 : специален\n\n\n"


// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Включен е progressive JPEG формат."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Progressive JPEG форматът е изключен."
#define MSGTR_VO_JPEG_BaselineJPEG "Включен е baseline JPEG формат."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Baseline JPEG форматът е изключен."

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "Включен е ASCII режим."
#define MSGTR_VO_PNM_RawMode "Включен е \"суров\" режим."
#define MSGTR_VO_PNM_PPMType "Ще записва в PPM файлове."
#define MSGTR_VO_PNM_PGMType "Ще записва в PGM файлове."
#define MSGTR_VO_PNM_PGMYUVType "Ще записва в PGMYUV файлове."

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Режимът interlaced изисква височината на образа да е кратна на  4."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Широчината на образа трябва да е кратна на 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Не е получена памет или файлов манипулатор за запис \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Грешка при извеждане на изображението!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Неизвестно подустройство: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Използване на interlaced изходен режим, от горе на долу."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Използване на interlaced изходен режим, от долу на горе."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Използва се (подразбиращ се) прогресивен режим"

// Old vo drivers that have been replaced

#define MSGTR_VO_PGM_HasBeenReplaced "pgm видео драйвера е заменен от -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "md5 видео драйвера е заменен от -vo md5sum.\n"

// ======================= AO Audio Output drivers ========================

// libao2

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: модулите alsa9 и alsa1x са отстранени, използвайте -ao alsa .\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Не може да отвори устройство смесител %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup:\nСмесителят на звуковата карта няма канал '%s', използва се подразбиращ се.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Аудио устройство %s не може да бъде отворено: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Не може да бъде създаден файлов дескриптор: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] Аудио устройство %s не може да бъде настроено за %s извеждане, проба с %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Не може да настрои звуковата карта за %d канала.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: драйверът не поддържа SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Вашият аудио драйвер НЕ поддържа функцията select()  ***\n Рекомпилирайте MPlayer с #undef HAVE_AUDIO_SELECT в config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS] Фатална грешка:\n *** НЕ МОЖЕ ДА ПРЕ-ОТВОРИ/РЕСТАРТИРА АУДИО УСТРОЙСТВОТО *** %s\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Установена е връзка със аудио сървъра.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Потокът не може да бъде отворен.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Потокът е отворен.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] размер на буфера: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Силата на звука не може да бъде сменена на %d.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] dxr2: %d Hz не се поддържат, опитайте \"-aop list=resample\"\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound се провали: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] закъснение: [сървър: %0.2fс, мрежа: %0.2fс] (настройка %0.2fс)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] Не може да бъде отворен esd поток за възпроизвеждане: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB audio set mixer се провали: %s\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Hz не се поддържат, опитайте с resample...\n"

// ao_null.c
// This one desn't even  have any mp_msg nor printf's?? [CHECK]

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] File: %s (%s)\nPCM: Честота: %iHz Канали: %s Формат %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Info: най-бързо извличане се постига с -vc null -vo null\nPCM: Info: за да запишете WAVE файлове ползвайте -ao pcm:waveheader (подразбира се).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] %s не може да се отвори за запис!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Честота: %iHz Канали: %s Формат %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] използва се %s аудио драйвер.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Неподдържан аудио формат: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] Инициализацията на SDL Аудио се провали: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Аудиото не може да се отвори: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] контрол.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] init: Честота: %iHz Канали: %s Формат %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] play: невалидно устройство.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] init: setparams се провали: %s\nНе може да се зададе разчитаната честота.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] init: AL_RATE не се възприема от посоченото устройство.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] init: getparams се провали: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] init: честотата на дискретизация е %f (разчитаната честота е %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] init: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] init: Не може да бъде отворен аудио канал: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] uninit: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] reset: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume: ...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: SETINFO се провали.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: провал на записа."
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Не може да бъде отворено устройство %s, %s  -> без звук.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] audio_setup: вашата звукова карта не поддържа %d канал, %s, %d Hz честота.\n"

// ao_plugin.c


// ======================= AF Audio Filters ================================

// af_ladspa.c

#define MSGTR_AF_LADSPA_AvailableLabels "достъпни етикети в"
#define MSGTR_AF_LADSPA_WarnNoInputs "ВНИМАНИЕ! Този LADSPA плъгин не приема аудио.\n  Пристигащият аудио сигнал ще бъде загубен."
#define MSGTR_AF_LADSPA_ErrNoOutputs "Този LADSPA плъгин не извежда звук."
#define MSGTR_AF_LADSPA_ErrInOutDiff "Броя на аудио входовете на този LADSPA плъгин се различава от броя на аудио изходите."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "не може да се зареди"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Функцията ladspa_descriptor() не може да бъде открита в указания библиотечен файл."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Етикета не може да бъде намерен в библиотеката."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Не са указани подопции"
#define MSGTR_AF_LADSPA_ErrNoLibFile "Не е указан файл с библиотека"
#define MSGTR_AF_LADSPA_ErrNoLabel "Не е указан етикет на филтър"
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Не са указани достатъчно контроли от командния ред"
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: Input control #%d е под долната граница от %0.4f.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: Input control #%d е над горната граница от %0.4f.\n"

// ========================== LIBMPCODECS ===================================

#define MSGTR_SamplesWanted "Мостри от този формат са нужни за подобряване на поддръжката. Свържете се с нас!\n"
