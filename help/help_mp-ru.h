// Translated by: Nick Kurshev <nickols_k@mail.ru>,
// Dmitry Baryshkov <mitya@school.ioffe.ru>
// Reworked by: Andrew Savchenko aka Bircoph <Bircoph[at]list[dot]ru>

// Synced with help_mp-en.h: r28860


// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<номер трека> играть дорожку (S)VCD (Super Video CD) (указывайте устройство,\n                 не монтируйте его)\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<номер ролика> играть DVD ролик с устройства вместо файла\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Использование:   mplayer [опции] [URL|путь/]имя_файла\n"\
"\n"\
"Базовые опции: (полный список см. на man-странице)\n"\
" -vo <drv[:dev]> выбор драйвера и устройства видеовывода (список см. с '-vo help')\n"\
" -ao <drv[:dev]> выбор драйвера и устройства аудиовывода (список см. с '-ao help')\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -alang/-slang   выбрать язык аудио/субтитров DVD (двухбуквенный код страны)\n"\
" -ss <время>     переместиться на заданную (секунды или ЧЧ:ММ:СС) позицию\n"\
" -nosound        без звука\n"\
" -fs             опции полноэкранного проигрывания (или -vm, -zoom, подробности\n                 на man-странице)\n"\
" -x <x> -y <y>   установить разрешение дисплея (использовать с -vm или -zoom)\n"\
" -sub <файл>     указать файл субтитров (см. также -subfps, -subdelay)\n"\
" -playlist <файл> указать список воспроизведения (плейлист)\n"\
" -vid x -aid y   опции для выбора видео (x) и аудио (y) потока для воспроизведения\n"\
" -fps x -srate y опции для изменения частоты видео (x, кадр/сек) и аудио (y, Гц)\n"\
" -pp <quality>   разрешить фильтр постобработки (подробности на man-странице)\n"\
" -framedrop      включить отбрасывание кадров (для медленных машин)\n"\
"\n"\
"Основные кнопки: (полный список в странице man, также см. input.conf)\n"\
" <- или ->       перемещение вперёд/назад на 10 секунд\n"\
" up или down     перемещение вперёд/назад на  1 минуту\n"\
" pgup or pgdown  перемещение вперёд/назад на 10 минут\n"\
" < или >         перемещение вперёд/назад в списке воспроизведения\n"\
" p или ПРОБЕЛ    приостановить фильм (любая клавиша - продолжить)\n"\
" q или ESC       остановить воспроизведение и выйти\n"\
" + или -         регулировать задержку звука по +/- 0.1 секунде\n"\
" o               цикличный перебор OSD режимов:  нет / навигация / навигация+таймер\n"\
" * или /         прибавить или убавить PCM громкость\n"\
" z или x         регулировать задержку субтитров по +/- 0.1 секунде\n"\
" r или t         регулировка вертикальной позиции субтитров, см. также -vf expand\n"\
"\n"\
" * * * ПОДРОБНЕЕ СМ. ДОКУМЕНТАЦИЮ, О ДОПОЛНИТЕЛЬНЫХ ОПЦИЯХ И КЛЮЧАХ! * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\nВыходим...\n"
#define MSGTR_ExitingHow "\nВыходим... (%s)\n"
#define MSGTR_Exit_quit "Выход"
#define MSGTR_Exit_eof "Конец файла"
#define MSGTR_Exit_error "Фатальная ошибка"
#define MSGTR_IntBySignal "\nMPlayer прерван сигналом %d в модуле: %s \n"
#define MSGTR_NoHomeDir "Не могу найти ДОМАШНИЙ каталог\n"
#define MSGTR_GetpathProblem "проблемы в get_path(\"config\")\n"
#define MSGTR_CreatingCfgFile "Создание файла конфигурации: %s\n"
#define MSGTR_CantLoadFont "Не могу загрузить побитовый шрифт: %s\n"
#define MSGTR_CantLoadSub "Не могу загрузить субтитры: %s\n"
#define MSGTR_DumpSelectedStreamMissing "дамп: ФАТАЛЬНАЯ ОШИБКА: Выбранный поток потерян!\n"
#define MSGTR_CantOpenDumpfile "Не могу открыть файл дампа!!!\n"
#define MSGTR_CoreDumped "Создан дамп ядра ;)\n"
#define MSGTR_FPSnotspecified "В заголовке кадры/сек не указаны (или недопустимые)! Используйте опцию -fps!\n"
#define MSGTR_TryForceAudioFmtStr "Попытка форсировать семейство аудиокодеков %s...\n"
#define MSGTR_CantFindAudioCodec "Не могу найти кодек для аудиоформата 0x%X!\n"
#define MSGTR_TryForceVideoFmtStr "Попытка форсировать семейство видеокодеков %s...\n"
#define MSGTR_CantFindVideoCodec "Не могу найти кодек для выбранного -vo и видеоформата 0x%X!\n"
#define MSGTR_CannotInitVO "ФАТАЛЬНАЯ ОШИБКА: Не могу инициализировать видеодрайвер!\n"
#define MSGTR_CannotInitAO "Не могу открыть/инициализировать аудиоустройство -> без звука.\n"
#define MSGTR_StartPlaying "Начало воспроизведения...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         *****************************************************************\n"\
"         **** Ваша система слишком МЕДЛЕННА чтобы воспроизводить это! ****\n"\
"         *****************************************************************\n"\
"Возможные причины, проблемы, обходы: \n"\
"- Наиболее частая: плохой/сырой _аудио_ драйвер\n"\
"  - Попробуйте -ao sdl или используйте эмуляцию OSS на ALSA.\n"\
"  - Поэкспериментируйте с различными значениями -autosync, начните с 30.\n"\
"- Медленный видео вывод\n"\
"  - Попытайтесь другие -vo driver (список: -vo help) или попытайтесь с -framedrop!\n"\
"- Медленный процессор\n"\
"  - Не пытайтесь воспроизводить большие DVD/DivX на медленных процессорах!\n" \
"    Попробуйте некоторые опции lavdopts, например:\n-vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all.\n"\
"- Битый файл\n"\
"  - Попробуйте различные комбинации: -nobps -ni -forceidx -mc 0\n"\
"- Медленный носитель (смонтированные NFS/SMB, DVD, VCD и т.п.)\n"\
"  - Используйте -cache 8192.\n"\
"- Используете ли Вы -cache для проигрывания нeслоёных [non-interleaved] AVI файлов?\n"\
"  - Используйте -nocache.\n"\
"Читайте DOCS/HTML/ru/video.html для советов по подстройке/ускорению.\n"\
"Если ничего не помогло, тогда читайте DOCS/HTML/ru/bugreports.html!\n\n"

#define MSGTR_NoGui "MPlayer был скомпилирован БЕЗ поддержки GUI!\n"
#define MSGTR_GuiNeedsX "GUI MPlayer необходим X11!\n"
#define MSGTR_Playing "\nВоспроизведение %s.\n"
#define MSGTR_NoSound "Аудио: без звука\n"
#define MSGTR_FPSforced "Кадры/сек форсированы в %5.3f (время кадра: %5.3f).\n"
#define MSGTR_AvailableVideoOutputDrivers "Доступные драйвера вывода видео:\n"
#define MSGTR_AvailableAudioOutputDrivers "Доступные драйвера вывода звука:\n"
#define MSGTR_AvailableAudioCodecs "Доступные аудиокодеки:\n"
#define MSGTR_AvailableVideoCodecs "Доступные видеокодеки:\n"
#define MSGTR_AvailableAudioFm "Доступные (вкомпилированные) семейства/драйверы аудиокодеков:\n"
#define MSGTR_AvailableVideoFm "Доступные (вкомпилированные) семейства/драйверы видеокодеков:\n"
#define MSGTR_AvailableFsType "Доступные режимы изменения полноэкранного слоя:\n"
#define MSGTR_CannotReadVideoProperties "Видео: Не могу прочитать свойства.\n"
#define MSGTR_NoStreamFound "Поток не найден.\n"
#define MSGTR_ErrorInitializingVODevice "Ошибка при открытии/инициализации выбранного устройства видеовывода (-vo).\n"
#define MSGTR_ForcedVideoCodec "Форсирован видеокодек: %s\n"
#define MSGTR_ForcedAudioCodec "Форсирован аудиокодек: %s\n"
#define MSGTR_Video_NoVideo "Видео: нет видео\n"
#define MSGTR_NotInitializeVOPorVO "\nФАТАЛЬНАЯ ОШИБКА: Не могу инициализировать видеофильтры (-vf) или видеовывод (-vo).\n"
#define MSGTR_Paused "=== ПРИОСТАНОВЛЕНО ===" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\nНе могу загрузить список воспроизведения (плейлист) %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- MPlayer рухнул из-за 'Неправильной Инструкции'.\n"\
"  Это может быть ошибкой нашего нового кода динамического определения типа CPU...\n"\
"  Пожалуйста, читайте DOCS/HTML/ru/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- MPlayer рухнул из-за 'Неправильной Инструкции'.\n"\
"  Обычно, это происходит когда Вы его запускаете на CPU, отличном от того, для которого\n"\
"  он был скомпилирован/оптимизирован.\n"\
"  Проверьте это!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- MPlayer рухнул из-за плохого использования CPU/FPU/RAM.\n"\
"  Перекомпилируйте MPlayer с --enable-debug и сделайте 'gdb' backtrace и\n"\
"  дизассемблирование. Для подробностей, см.\nDOCS/HTML/ru/bugreports_what.html#bugreports_crash\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer рухнул. Это не должно происходить.\n"\
"  Это может быть ошибкой в коде MPlayer _или_ в Вашем драйвере, _или_\n"\
"  в Вашей версии gcc. Если Вы думаете, что в этом виноват MPlayer, пожалуйста,\n"\
"  прочтите DOCS/HTML/ru/bugreports.html и следуйте инструкциям оттуда.\n"\
"  Мы не сможем и не будем помогать, пока Вы не предоставите эту информацию,\nсообщая о возможной ошибке.\n"
#define MSGTR_LoadingConfig "Загружаю конфигурационный файл '%s'\n"
#define MSGTR_LoadingProtocolProfile "Загружаю профиль для протокола '%s'\n"
#define MSGTR_LoadingExtensionProfile "Загружаю профиль для расширения '%s'\n"
#define MSGTR_AddedSubtitleFile "СУБТИТРЫ: добавлен файл субтитров (%d): %s\n"
#define MSGTR_RemovedSubtitleFile "СУБТИТРЫ: Удалён файл субтитров (%d): %s\n"
#define MSGTR_RTCDeviceNotOpenable "Не могу открыть %s: %s (пользователь должен обладать правом чтения на этот файл).\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Ошибка инициализации Linux RTC в ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Попробуйте добавить \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" \nв загрузочные скрипты Вашей системы.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Ошибка инициализации Linux RTC в ioctl (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "ПРЕДУПРЕЖДЕНИЕ: getch2_init вызван дважды!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Не могу открыть видеофильтр libmenu с этим корневым меню %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Ошибка в цепочке pre-init аудиофильтра!\n"
#define MSGTR_LinuxRTCReadError "Ошибка чтения Linux RTC: %s\n"
#define MSGTR_SoftsleepUnderflow "Предупреждение! Недопустимо низкое значение программной задержки!\n"

#define MSGTR_EdlOutOfMem "Не могу выделить достаточный объём памяти для хранения данных EDL.\n"
#define MSGTR_EdlRecordsNo "Чтение %d EDL действий.\n"
#define MSGTR_EdlQueueEmpty "Нет действий EDL, которые следует исполнить (очередь пуста).\n"
#define MSGTR_EdlCantOpenForWrite "Не могу открыть файл EDL [%s] для записи.\n"
#define MSGTR_EdlNOsh_video "Нельзя использовать EDL без видео, отключаю.\n"
#define MSGTR_EdlNOValidLine "Неверная строка EDL: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Плохо форматированная строка EDL [%d]. Пропускаю.\n"
#define MSGTR_EdlBadLineOverlap "Последняя позиция останова была [%f]; следующая стартовая "\
"позиция [%f]. Записи должны быть в хронологическом порядке, не могу перекрыть.\nПропускаю.\n"
#define MSGTR_EdlBadLineBadStop "Время останова должно быть после времени старта.\n"
#define MSGTR_EdloutBadStop "EDL пропуск отменён, последний start > stop\n"
#define MSGTR_EdloutStartSkip "Начало EDL пропуска, нажмите 'i' ещё раз для завершения блока.\n"
#define MSGTR_EdloutEndSkip "Конец EDL пропуска, строка записана.\n"

// mplayer.c OSD
#define MSGTR_OSDenabled "включено"
#define MSGTR_OSDdisabled "выключено"
#define MSGTR_OSDAudio "Аудио: %s"
#define MSGTR_OSDChannel "Канал: %s"
#define MSGTR_OSDSubDelay "Задержка субтитров: %d мс"
#define MSGTR_OSDSpeed "Скорость: x %6.2f"
#define MSGTR_OSDosd "OSD: %s"
#define MSGTR_OSDChapter "Раздел: (%d) %s"
#define MSGTR_OSDAngle "Угол: %d/%d"

// property values
#define MSGTR_Enabled "включено"
#define MSGTR_EnabledEdl "включено (EDL)"
#define MSGTR_Disabled "выключено"
#define MSGTR_HardFrameDrop "интенсивный"
#define MSGTR_Unknown "неизвестно"
#define MSGTR_Bottom "низ"
#define MSGTR_Center "центр"
#define MSGTR_Top "верх"
#define MSGTR_SubSourceFile "из файла"
#define MSGTR_SubSourceVobsub "vobsub"
#define MSGTR_SubSourceDemux "внедренные"

// OSD bar names
#define MSGTR_Volume "Громкость"
#define MSGTR_Panscan "Усечение сторон"
#define MSGTR_Gamma "Гамма"
#define MSGTR_Brightness "Яркость"
#define MSGTR_Contrast "Контрастность"
#define MSGTR_Saturation "Насыщенность"
#define MSGTR_Hue "Цвет"
#define MSGTR_Balance "Баланс"

// property state
#define MSGTR_LoopStatus "Повторять: %s"
#define MSGTR_MuteStatus "Приглушить: %s"
#define MSGTR_AVDelayStatus "A-V задержка: %s"
#define MSGTR_OnTopStatus "Поверх остальных: %s"
#define MSGTR_RootwinStatus "root-окно: %s"
#define MSGTR_BorderStatus "Рамка: %s"
#define MSGTR_FramedroppingStatus "Пропуск кадров: %s"
#define MSGTR_VSyncStatus "Вертикальная синхронизация: %s"
#define MSGTR_SubSelectStatus "Субтитры: %s"
#define MSGTR_SubSourceStatus "Источник субтитров: %s"
#define MSGTR_SubPosStatus "Позиция субтитров: %s/100"
#define MSGTR_SubAlignStatus "Выравнивание субтитров: %s"
#define MSGTR_SubDelayStatus "Задержка субтитров: %s"
#define MSGTR_SubScale "Масштаб субтитров: %s"
#define MSGTR_SubVisibleStatus "Субтитры: %s"
#define MSGTR_SubForcedOnlyStatus "Форсированы только субтитры: %s"

// mencoder.c
#define MSGTR_UsingPass3ControlFile "Использую следующий файл для контроля 3-го прохода: %s\n"
#define MSGTR_MissingFilename "\nПропущено имя файла.\n\n"
#define MSGTR_CannotOpenFile_Device "Не могу открыть файл/устройство.\n"
#define MSGTR_CannotOpenDemuxer "Не могу открыть демультиплексор.\n"
#define MSGTR_NoAudioEncoderSelected "\nКодировщик аудио (-oac) не выбран.\nВыберете какой-нибудь (см. -oac help) или используйте -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nКодировщик видео (-ovc) не выбран. Выберете какой-нибудь (см. -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Не могу открыть файл вывода '%s'.\n"
#define MSGTR_EncoderOpenFailed "Не могу открыть кодировщик.\n"
#define MSGTR_MencoderWrongFormatAVI "\nПРЕДУПРЕЖДЕНИЕ: ВЫХОДНОЙ ФОРМАТ ФАЙЛА _AVI_. См. -of help.\n"
#define MSGTR_MencoderWrongFormatMPG "\nПРЕДУПРЕЖДЕНИЕ: ВЫХОДНОЙ ФОРМАТ ФАЙЛА _MPEG_. См. -of help.\n"
#define MSGTR_MissingOutputFilename "Не указан выходной файл, используйте опцию -o."
#define MSGTR_ForcingOutputFourcc "Выходной fourcc форсирован в %x [%.4s]\n"
#define MSGTR_ForcingOutputAudiofmtTag "Форсирую тег выходного аудиоформата в 0x%x.\n"
#define MSGTR_DuplicateFrames "\n%d повторяющийся(хся) кадр(а/ов)!\n"
#define MSGTR_SkipFrame "\nПропускаю кадр!\n"
#define MSGTR_ResolutionDoesntMatch "\nНовый видеофайл обладает иным разрешением или цветовым пространством, чем\nпредыдущий.\n"
#define MSGTR_FrameCopyFileMismatch "\nВсе видеофайлы должны обладать одинаковым fps, разрешением и кодеком для\n-ovc copy.\n"
#define MSGTR_AudioCopyFileMismatch "\nВсе файлы должны обладать одинаковым аудиокодеком и форматом для -oac copy.\n"
#define MSGTR_NoAudioFileMismatch "\nНе могу смешивать файлы, содержащие только видео, с аудио- и видеофайлами.\nПопробуйте -nosound.\n"
#define MSGTR_NoSpeedWithFrameCopy "ПРЕДУПРЕЖДЕНИЕ: не гарантируется корректная работа -speed с -oac copy!\n"\
"Ваше кодирование может быть испорчено!\n"
#define MSGTR_ErrorWritingFile "%s: Ошибка при записи файла.\n"
#define MSGTR_FlushingVideoFrames "\nСброс видео кадров.\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "Фильтры не настроены! Пустой файл?\n"
#define MSGTR_RecommendedVideoBitrate "Рекомендуемый битпоток для %s CD: %d\n"
#define MSGTR_VideoStreamResult "\nПоток видео: %8.3f кбит/с  (%d Б/с)  размер: %"PRIu64" байт(а/ов)  %5.3f сек.  %d кадр(а/ов)\n"
#define MSGTR_AudioStreamResult "\nПоток аудио: %8.3f кбит/с  (%d Б/с)  размер: %"PRIu64" байт(а/ов)  %5.3f сек.\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL SKIP: Начало: %.2f  Конец: %.2f   Текущая: V: %.2f  A: %.2f     \r"
#define MSGTR_OpenedStream "успех: формат: %d  данные: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "видеокодек: копирование кадров (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "аудиокодек: копирование кадров (формат=%x цепочек=%d скорость=%d битов=%d Б/с=%d образец=%d)\n"
#define MSGTR_MP3AudioSelected "Выбрано MP3 аудио\n"
#define MSGTR_SettingAudioDelay "Устанавливаю задержку аудио в %5.3f\n"
#define MSGTR_SettingVideoDelay "Устанавливаю задержку видео в %5.3fs.\n"
#define MSGTR_LimitingAudioPreload "Ограничиваю предварительную загрузку аудио до 0.4с\n"
#define MSGTR_IncreasingAudioDensity "Увеличиваю плотность аудио до 4\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Форсирую предварительную загрузку аудио в 0, максимальную коррекцию pts в 0\n"
#define MSGTR_LameVersion "Версия LAME %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Ошибка: Заданный битпоток вне допустимого значения для данной предустановки.\n"\
"\n"\
"При использовании этого режима Вы должны указать значение между \"8\" и \"320\"\n"\
"\n"\
"Для дополнительной информации используйте: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Ошибка: Вы не указали верный профиль и/или опции предустановки.\n"\
"\n"\
"Доступные профили:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (Режим ABR) - Подразумевается режим ABR. Для использования\n"\
"                       просто укажите битпоток. Например:\n"\
"                       \"preset=185\" активирует эту предустановку (preset)\n"\
"                       и использует 185 как среднее значение кбит/сек.\n"\
"\n"\
"    Несколько примеров:\n"\
"\n"\
"     \"-lameopts fast:preset=standard  \"\n"\
" или \"-lameopts  cbr:preset=192       \"\n"\
" или \"-lameopts      preset=172       \"\n"\
" или \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Для дополнительной информации используйте: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Ключи предустановок разработаны с целью предоставления максимально возможного\nкачества.\n"\
"\n"\
"Они были преимущественно разработаны и настроены с помощью тщательных тестов\n"\
"двойного прослушивания для проверки и достижения этой цели.\n"\
"\n"\
"Ключи предустановок постоянно обновляются для соответствия последним разработкам,\n"\
"в результате чего Вы должны получить практически наилучшее качество, \n"\
"возможное на текущий момент при использовании LAME.\n"\
"\n"\
"Чтобы использовать эти предустановки:\n"\
"\n"\
"   Для VBR режимов (обычно лучшее качество):\n"\
"\n"\
"     \"preset=standard\" Обычно этой предустановки должно быть достаточно\n"\
"                             для большинства людей и большинства музыки, и она\n"\
"                             уже предоставляет достаточно высокое качество.\n"\
"\n"\
"     \"preset=extreme\" Если Вы обладаете чрезвычайно хорошим слухом и\n"\
"                             соответствующим оборудованием, эта предустановка,\n"\
"                             как правило, предоставит несколько лучшее качество,\n"\
"                             чем режим \"standard\".\n"\
"\n"\
"   Для CBR 320kbps (максимально возможное качество, получаемое\n                             при использовании ключей предустановок):\n"\
"\n"\
"     \"preset=insane\"  Использование этой установки является перебором для\n"\
"                             большинства людей и большинства ситуаций, но если\n"\
"                             Вам необходимо максимально возможное качество,\n"\
"                             невзирая на размер файла - это способ сделать так.\n"\
"\n"\
"   Для ABR режимов (высокое качество для заданного битпотока,\nно не такое высокое, как VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  Использование этой предустановки обычно даёт хорошее\n"\
"                             качество для заданного битпотока. Основываясь на\n"\
"                             введённом битпотоке, эта предустановка определит\n"\
"                             оптимальные настройки для каждой конкретной ситуации.\n"\
"                             Несмотря на то, что этот подход работает, он далеко\n"\
"                             не такой гибкий как VBR и обычно не достигает\n"\
"                             такого же уровня качества как VBR на высоких битпотоках.\n"\
"\n"\
"Также доступны следующие опции для соответствующих профилей:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (Режим ABR) - Подразумевается режим ABR. Для использования\n"\
"                       просто укажите битпоток. Например:\n"\
"                       \"preset=185\" активирует эту предустановку (preset)\n"\
"                       и использует 185 как среднее значение кбит/сек.\n"\
"\n"\
"   \"fast\" - Включает новый быстрый VBR для конкретного профиля.\n"\
"            Недостатком этого ключа является то, что часто\n"\
"            битпоток будет немного больше, чем в нормальном режиме;\n"\
"            также качество может быть несколько хуже.\n"\
"Предупреждение: В текущей версии быстрые предустановки могут привести к слишком\n"\
"                высокому битпотоку, по сравнению с обычными предустановками.\n"\
"\n"\
"   \"cbr\"  - Если Вы используете режим ABR (см. выше) с таким \"кратным\""\
"            битпотоком как 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            Вы можете использовать опцию \"cbr\" для форсирования кодирования\n"\
"            в режиме CBR вместо стандартного abr режима. ABR предоставляет\n"\
"            более высокое качество, но CBR может быть полезным в таких\n"\
"            ситуациях, как передача потоков mp3 через интернет.\n"\
"\n"\
"    Например:\n"\
"\n"\
"     \"-lameopts fast:preset=standard  \"\n"\
" или \"-lameopts  cbr:preset=192       \"\n"\
" или \"-lameopts      preset=172       \"\n"\
" или \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Несколько псевдонимов доступно для режима ABR:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"Не могу задать опции LAME, проверьте битпоток/частоту_дискретизации,\n"\
"некоторые очень малые битпотоки (<32) нуждаются в меньших частотах\nдискретизации (например, -srate 8000).\n"\
"Если всё другое не поможет, попробуйте предустановку."
#define MSGTR_ConfigFileError "ошибка в конфигурационном файле"
#define MSGTR_ErrorParsingCommandLine "ошибка анализа командной строки"
#define MSGTR_VideoStreamRequired "Наличие потока видео обязательно!\n"
#define MSGTR_ForcingInputFPS "входные кадры/сек будут заменены на %5.3f\n"
#define MSGTR_DemuxerDoesntSupportNosound "Этот демультиплексор пока что не поддерживает -nosound.\n"
#define MSGTR_MemAllocFailed "не могу выделить память\n"
#define MSGTR_NoMatchingFilter "Не могу найти соответствующий фильтр/формат_аудиовывода!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, возможно, сломанный компилятор C?\n"
#define MSGTR_NoLavcAudioCodecName "Аудио LAVC, пропущено имя кодека!\n"
#define MSGTR_LavcAudioCodecNotFound "Аудио LAVC, не могу найти кодировщик для кодека %s\n"
#define MSGTR_CouldntAllocateLavcContext "Аудио LAVC, не могу разместить контекст!\n"
#define MSGTR_CouldntOpenCodec "Не могу открыть кодек %s, br=%d\n"
#define MSGTR_CantCopyAudioFormat "Аудиоформат 0x%x несовместим с '-oac copy', попробуйте '-oac pcm'\nили используйте '-fafmttag' для его переопределения.\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     метод кодирования с переменным битпотоком\n"\
"                0: cbr (постоянный битпоток)\n"\
"                1: mt (VBR алгоритм Марка Тейлора [Mark Taylor])\n"\
"                2: rh (VBR алгоритм Роберта Гиджимана [Robert Hegemann]\n                   -- по умолчанию)\n"\
"                3: abr (усреднённый битпоток)\n"\
"                4: mtrh (VBR алгоритм Марка Тейлора и Роберта Гиджимана)\n"\
"\n"\
" abr           усреднённый битпоток\n"\
"\n"\
" cbr           постоянный битпоток\n"\
"               Также форсирует режим CBR кодирования на последующих режимах\n               предустановок ABR\n"\
"\n"\
" br=<0-1024>   укажите битпоток в кбит (только CBR и ABR)\n"\
"\n"\
" q=<0-9>       качество (0-высшее, 9-наименьшее) (только для VBR)\n"\
"\n"\
" aq=<0-9>      качество алгоритма (0-лучшее/самый медленный, 9-худшее/быстрейший)\n"\
"\n"\
" ratio=<1-100> коэффициент сжатия\n"\
"\n"\
" vol=<0-10>    установите усиление входящего аудио\n"\
"\n"\
" mode=<0-3>    (по-умолчанию: автоопределение)\n"\
"                0: стерео\n"\
"                1: объединённое стерео [joint-stereo]\n"\
"                2: двухканальный\n"\
"                3: моно\n"\
"\n"\
" padding=<0-2>\n"\
"                0: нет\n"\
"                1: все\n"\
"                2: регулируемое\n"\
"\n"\
" fast          Переключение на быстрое кодирование на последующих режимах\n"\
"               предустановок VBR; несколько худшее качество и завышенные битпотоки.\n"\
"\n"\
" preset=<value> Предоставляет установки наибольшего возможного качества.\n"\
"                 medium: VBR кодирование, хорошее качество\n"\
"                 (амплитуда битпотока 150-180 kbps)\n"\
"                 standard: VBR кодирование, высокое качество\n"\
"                 (амплитуда битпотока 170-210 kbps)\n"\
"                 extreme: VBR кодирование, очень высокое качество\n"\
"                 (амплитуда битпотока 200-240 kbps)\n"\
"                 insane:  CBR кодирование, лучшее предустановленное качество\n"\
"                 (битпоток 320 kbps)\n"\
"                 <8-320>: ABR кодирование с заданным в кбит средним битпотоком.\n\n"

//codec-cfg.c
#define MSGTR_DuplicateFourcc "повторяющийся FourCC"
#define MSGTR_TooManyFourccs "слишком много FourCCs/форматов..."
#define MSGTR_ParseError "ошибка разбора синтаксиса"
#define MSGTR_ParseErrorFIDNotNumber "ошибка разбора синтаксиса (ID формата не число?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "ошибка разбора синтаксиса (псевдоним ID формата не число?)"
#define MSGTR_DuplicateFID "повторяющееся ID формата"
#define MSGTR_TooManyOut "слишком много выходных форматов..."
#define MSGTR_InvalidCodecName "\nимя кодека '%s' не верно!\n"
#define MSGTR_CodecLacksFourcc "\nкодек '%s' не имеет FourCC/формат!\n"
#define MSGTR_CodecLacksDriver "\nкодек '%s' не имеет драйвера!\n"
#define MSGTR_CodecNeedsDLL "\nкодеку '%s' необходима 'dll'!\n"
#define MSGTR_CodecNeedsOutfmt "\nкодеку '%s' необходим 'outfmt'!\n"
#define MSGTR_CantAllocateComment "Не могу выделить память для комментария. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Не могу выделить память для строки: %s\n"
#define MSGTR_CantReallocCodecsp "Не могу выполнить realloc для '*codecsp': %s\n"
#define MSGTR_CodecNameNotUnique "Имя кодека '%s' не уникально."
#define MSGTR_CantStrdupName "Не могу выполнить strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Не могу выполнить strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Не могу выполнить strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Не могу выполнить strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d аудио & %d видео кодеков\n"
#define MSGTR_CodecDefinitionIncorrect "Кодек определён некорректно."
#define MSGTR_OutdatedCodecsConf "Этот codecs.conf слишком стар и несовместим с данным релизом MPlayer!"

// fifo.c

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "'--' означает конец опций, но имя файла в командной строке не указано.\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "Опция loop должна быть целочисленной: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "Неизвестная опция командной строки: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "Ошибка анализа опции командной строки: -%s\n"
#define MSGTR_InvalidPlayEntry "Неверный элемент воспроизведения %s\n"
#define MSGTR_NotAnMEncoderOption "-%s не является опцией MEncoder\n"
#define MSGTR_NoFileGiven "Не указан файл\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "Найден слишком старый слот сохранения из lvl %d: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "Опция %s не может использоваться в конфигурационном файле.\n"
#define MSGTR_InvalidCmdlineOption "Опция %s не может использоваться в командной строке.\n"
#define MSGTR_InvalidSuboption "Ошибка: у опции '%s' нет субопции '%s'.\n"
#define MSGTR_MissingSuboptionParameter "Ошибка: у субопции '%s' опции '%s' должен быть параметр!\n"
#define MSGTR_MissingOptionParameter "Ошибка: у опции '%s' должен быть параметр!\n"
#define MSGTR_OptionListHeader "\n Имя                  Тип             Минимум    Максимум Общий   CL    Конф\n\n"
#define MSGTR_TotalOptions "\nВсего: %d опций(я/и)\n"
#define MSGTR_ProfileInclusionTooDeep "ПРЕДУПРЕЖДЕНИЕ: Включение профиля слишком глубоко.\n"
#define MSGTR_NoProfileDefined "Ни один профиль не был определён.\n"
#define MSGTR_AvailableProfiles "Доступные профили:\n"
#define MSGTR_UnknownProfile "Неизвестный профиль '%s'.\n"
#define MSGTR_Profile "Профиль %s: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n Имя                  Тип             Минимум    Максимум\n\n"
#define MSGTR_TotalProperties "\nВсего: %d свойств\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "ВНИМАНИЕ: попытка использования DLL кодеков без установленной переменной \n         окружения DYLD_BIND_AT_LAUNCH. Это скорее всего приведет к краху.\n"


// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "О MPlayer"
#define MSGTR_GUI_Add "Добавить"
#define MSGTR_GUI_AspectRatio "Соотношение сторон"
#define MSGTR_GUI_Audio "Аудио"
#define MSGTR_GUI_AudioDelay "Задержка аудио"
#define MSGTR_GUI_AudioDriverConfiguration "Конфигурация аудио драйвера"
#define MSGTR_GUI_AudioTrack "Загрузить внешний аудио файл"
#define MSGTR_GUI_AudioTracks "Аудио дорожка"
#define MSGTR_GUI_AvailableDrivers "Доступные драйверы:"
#define MSGTR_GUI_AvailableSkins "Шкуры"
#define MSGTR_GUI_Bass "Бас"
#define MSGTR_GUI_Blur "Нерезкость"
#define MSGTR_GUI_Bottom "Низ"
#define MSGTR_GUI_Brightness "Яркость"
#define MSGTR_GUI_Browse "Просмотреть"
#define MSGTR_GUI_Cache "Кэш"
#define MSGTR_GUI_CacheSize "Размер кэша"
#define MSGTR_GUI_Cancel "Отмена"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Центральная"
#define MSGTR_GUI_Channel1 "Канал 1"
#define MSGTR_GUI_Channel2 "Канал 2"
#define MSGTR_GUI_Channel3 "Канал 3"
#define MSGTR_GUI_Channel4 "Канал 4"
#define MSGTR_GUI_Channel5 "Канал 5"
#define MSGTR_GUI_Channel6 "Канал 6"
#define MSGTR_GUI_ChannelAll "Все"
#define MSGTR_GUI_ChapterN "Раздел %d"
#define MSGTR_GUI_ChapterNN "Раздел %2d"
#define MSGTR_GUI_Chapters "Разделы"
#define MSGTR_GUI_Clear "Очистить"
#define MSGTR_GUI_CodecFamilyAudio "Семейство аудио кодеков"
#define MSGTR_GUI_CodecFamilyVideo "Семейство видео кодеков"
#define MSGTR_GUI_CodecsAndLibraries "Кодеки и сторонние библиотеки"
#define MSGTR_GUI_Coefficient "Коэффициент"
#define MSGTR_GUI_ConfigFileError "ошибка в конфигурационном файле\n"
#define MSGTR_GUI_Configure "Настроить"
#define MSGTR_GUI_ConfigureDriver "Настроить драйвер"
#define MSGTR_GUI_Contrast "Контраст"
#define MSGTR_GUI_Contributors "Разработчики кода и документации"
#define MSGTR_GUI_Cp874 "Тайская кодировка (CP874)"
#define MSGTR_GUI_Cp936 "Упрощённая Китайская кодировка (CP936)"
#define MSGTR_GUI_Cp949 "Корейская кодировка (CP949)"
#define MSGTR_GUI_Cp1250 "Славянский/Центрально-европейский Window$ (CP1250)"
#define MSGTR_GUI_Cp1251 "Кириллица Window$ (CP1251)"
#define MSGTR_GUI_Cp1256 "Арабский Windows (CP1256)"
#define MSGTR_GUI_CpBIG5 "Традиционная Китайская кодировка (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Западноевропейские языки (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Славянские/Центрально-европейские языки (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Эсперанто, Галицийский, Мальтийский, Турецкий (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Старая Балтийская кодировка (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Кириллица (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Арабская (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Современная Греческая (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Еврейские кодировки (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Турецкая (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Балтийская (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Кельтская (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Западноевропейские языки с Евро (ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Русская (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Украинская, Белорусская (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Японские кодировки (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Уникод"
#define MSGTR_GUI_DefaultSetting "драйвер по умолчанию"
#define MSGTR_GUI_Delay "Задержка"
#define MSGTR_GUI_Demuxers_Codecs "Кодеки и демультиплексор"
#define MSGTR_GUI_Device "Устройство"
#define MSGTR_GUI_DeviceCDROM "CD-ROM"
#define MSGTR_GUI_DeviceDVD "DVD"
#define MSGTR_GUI_Directory "Путь"
#define MSGTR_GUI_DirectoryTree "Дерево каталогов"
#define MSGTR_GUI_DropSubtitle "Убрать субтитры..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAssSubtitle "SSA/ASS рендеринг субтитров"
#define MSGTR_GUI_EnableAutomaticAVSync "Автосинхронизация вкл/выкл"
#define MSGTR_GUI_EnableCache "Кэш вкл/выкл"
#define MSGTR_GUI_EnableDirectRendering "Включить прямое отображение"
#define MSGTR_GUI_EnableDoubleBuffering "Включить двойную буферизацию"
#define MSGTR_GUI_EnableEqualizer "Включить эквалайзер"
#define MSGTR_GUI_EnableExtraStereo "Включить дополнительное стерео"
#define MSGTR_GUI_EnableFrameDropping "Включить пропуск кадров"
#define MSGTR_GUI_EnableFrameDroppingIntense "Включить ИНТЕНСИВНЫЙ пропуск кадров (опасно)"
#define MSGTR_GUI_EnablePlaybar "Включить полосу воспроизведения"
#define MSGTR_GUI_EnablePostProcessing "Включить постобработку"
#define MSGTR_GUI_EnableSoftwareMixer "Включить программный микшер"
#define MSGTR_GUI_Encoding "Кодировка"
#define MSGTR_GUI_Equalizer "Эквалайзер"
#define MSGTR_GUI_EqualizerConfiguration "Настройка каналов"
#define MSGTR_GUI_Error "Ошибка!"
#define MSGTR_GUI_ErrorFatal "Фатальная ошибка!"
#define MSGTR_GUI_File "Играть файл"
#define MSGTR_GUI_Files "Файлы"
#define MSGTR_GUI_Flip "Отобразить изображение вверх ногами"
#define MSGTR_GUI_Font "Шрифт"
#define MSGTR_GUI_FrameRate "Кадр/сек"
#define MSGTR_GUI_FrontLeft "Передняя Левая"
#define MSGTR_GUI_FrontRight "Передняя Правая"
#define MSGTR_GUI_HideVideoWindow "Показывать окно видео, когда неактивен"
#define MSGTR_GUI_Hue "Цвет"
#define MSGTR_GUI_Lavc "Использовать LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Авто качество"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Разное"
#define MSGTR_GUI_Mixer "Микшер"
#define MSGTR_GUI_MixerChannel "Канал микшера"
#define MSGTR_GUI_MSG_AddingVideoFilter "[GUI] Добавляю видеофильтр: %s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "Извините, глубина цвета слишком мала.\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "D&D: Ничего не возвращено!\n"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Извините, Вы не можете проигрывать не-MPEG файлы на Вашем DXR3/H+ устройстве\nбез перекодирования. Пожалуйста, включите lavc при конфигурации DXR3/H+.\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "[GUI] Загружаю субтитры: %s\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "Извините, недостаточно памяти для буфера прорисовки.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Извините, не хватает памяти для буфера прорисовки.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Файл не загружен."
#define MSGTR_GUI_MSG_NoMediaOpened "Носитель не открыт."
#define MSGTR_GUI_MSG_NotAFile0 "Это не похоже на файл...\n"
#define MSGTR_GUI_MSG_NotAFile1 "Это не похоже на файл: '%s' !\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Пожалуйста, запомните, что Вам нужно перезапустить проигрывание,\nчтобы некоторые изменения вступили в силу!"
#define MSGTR_GUI_MSG_RemoteDisplay "Удалённый дисплей, отключаю XMITSHM.\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "[GUI] Удаляю субтитры.\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "ошибка преобразования 24-бит в 32-бит (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "Файл не найден (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "ошибка чтения PNG (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Скин не найден (%s).\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "Выбранный скин '%s' не найден, пробую 'default'...\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "Глубина битовой матрицы в 16 бит и меньше не поддерживается (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[скин] ошибка в файле конфигурации шкуры на строке %d: %s"
#define MSGTR_GUI_MSG_SkinFileNotFound "[скин] файл '%s' не найден.\n"
#define MSGTR_GUI_MSG_SkinFileNotReadable "[скин] файл ( %s ) не читаем.\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "Файл шрифта не найден.\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "Файл образов шрифта не найден.\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "несуществующий идентификатор шрифта (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "не хватает памяти\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "Объявлено слишком много шрифтов.\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "неизвестное сообщение: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "неизвестный параметр (%s)\n"
#define MSGTR_GUI_MSG_TooManyWindows "Слишком много открытых окон.\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "[cfg] Не могу сохранить опцию '%s'.\n"
#define MSGTR_GUI_MSG_VideoOutError "Извините, не нашёл совместимый с GUI драйвер видео вывода.\n"
#define MSGTR_GUI_MSG_XShapeError "Извините, ваша система не поддерживает расширение XShape.\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "ошибка расширения разделяемой памяти\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "Извините, ваша система не поддерживает расширение разделяемой памяти X'ов.\n"
#define MSGTR_GUI_Mute "Приглушить"
#define MSGTR_GUI_NetworkStreaming "Сетевые потоки..."
#define MSGTR_GUI_Next "След. поток"
#define MSGTR_GUI_NoChapter "Нет раздела"
#define MSGTR_GUI__none_ "(нет)"
#define MSGTR_GUI_NonInterleavedParser "Использовать 'неслоёный' AVI парсер"
#define MSGTR_GUI_NormalizeSound "Нормализовать звук"
#define MSGTR_GUI_Ok "Да"
#define MSGTR_GUI_Open "Открыть..."
#define MSGTR_GUI_Original "Исходный"
#define MSGTR_GUI_OsdLevel "уровень OSD"
#define MSGTR_GUI_OSD_Subtitles "Субтитры и OSD"
#define MSGTR_GUI_Outline "Контуры"
#define MSGTR_GUI_PanAndScan "Усечение сторон"
#define MSGTR_GUI_Pause "Пауза"
#define MSGTR_GUI_Play "Играть"
#define MSGTR_GUI_Playback "Воспроизведение"
#define MSGTR_GUI_Playlist "Плейлист"
#define MSGTR_GUI_Position "Позиция"
#define MSGTR_GUI_PostProcessing "Постобработка"
#define MSGTR_GUI_Preferences "Настройки"
#define MSGTR_GUI_Previous "Пред. поток"
#define MSGTR_GUI_Quit "Выход"
#define MSGTR_GUI_RearLeft "Задняя Левая"
#define MSGTR_GUI_RearRight "Задняя Правая"
#define MSGTR_GUI_Remove "Удалить"
#define MSGTR_GUI_Saturation "Насыщенность"
#define MSGTR_GUI_SaveWindowPositions "Сохранять позицию окна"
#define MSGTR_GUI_ScaleMovieDiagonal "Пропорционально диагонали фильма"
#define MSGTR_GUI_ScaleMovieHeight "Пропорционально высоте фильма"
#define MSGTR_GUI_ScaleMovieWidth "Пропорционально ширине фильма"
#define MSGTR_GUI_ScaleNo "Не масштабировать"
#define MSGTR_GUI_SeekingInBrokenMedia "Если требуется, перестроить индексную таблицу"
#define MSGTR_GUI_SelectAudioFile "Выбор внешнего аудио канала..."
#define MSGTR_GUI_SelectedFiles "Выбранные файлы"
#define MSGTR_GUI_SelectFile "Выбрать файл..."
#define MSGTR_GUI_SelectFont "Выбор шрифта..."
#define MSGTR_GUI_SelectSubtitle "Выбрать субтитры..."
#define MSGTR_GUI_SizeDouble "Двойной размер"
#define MSGTR_GUI_SizeFullscreen "Полный экран"
#define MSGTR_GUI_SizeHalf "Половинный размер"
#define MSGTR_GUI_SizeNormal "Нормальный размер"
#define MSGTR_GUI_SizeOSD "Масштаб OSD"
#define MSGTR_GUI_SizeSubtitles "Масштаб текста"
#define MSGTR_GUI_SkinBrowser "Просмотр шкур"
#define MSGTR_GUI_Skins "Шкуры"
#define MSGTR_GUI_Sponsored "Разработка GUI спонсирована UHU Linux"
#define MSGTR_GUI_StartFullscreen "Стартовать в полноэкранном режиме"
#define MSGTR_GUI_Stop "Останов"
#define MSGTR_GUI_Subtitle "Субтитры"
#define MSGTR_GUI_SubtitleAddMargins "Использ. края"
#define MSGTR_GUI_SubtitleAllowOverlap "Изменить перекрывание субтитров"
#define MSGTR_GUI_SubtitleAutomaticLoad "Выключить автозагрузку субтитров"
#define MSGTR_GUI_SubtitleConvertMpsub "Конвертировать данные субтитры в MPlayer'овский формат субтитров"
#define MSGTR_GUI_SubtitleConvertSrt "Конвертировать данные субтитры в основанный на времени SubViewer (SRT) формат"
#define MSGTR_GUI_Subtitles "Субтитры"
#define MSGTR_GUI_SyncValue "Автосинхронизация"
#define MSGTR_GUI_TitleNN "Ролик %2d"
#define MSGTR_GUI_Titles "Ролики"
#define MSGTR_GUI_Top "Верх"
#define MSGTR_GUI_TrackN "Дорожка %d"
#define MSGTR_GUI_Translations "Переводы"
#define MSGTR_GUI_TurnOffXScreenSaver "Останавливать XScreenSaver"
#define MSGTR_GUI_URL "Играть URL"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Видео"
#define MSGTR_GUI_VideoEncoder "Видео кодировщик"
#define MSGTR_GUI_VideoTracks "Видео дорожка"
#define MSGTR_GUI_Volume MSGTR_Volume
#define MSGTR_GUI_Warning "Предупреждение!"

// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "Извините, выбранное устройство видеовывода не совместимо с этим кодеком.\n"\
                "Попробуйте добавить фильтр scale к вашему списку фильтров.\n"\
                "Например, -vf spp,scale вместо -vf spp.\n"
#define MSGTR_VO_GenericError "Произошла следующая ошибка"
#define MSGTR_VO_UnableToAccess "Не могу получить доступ"
#define MSGTR_VO_ExistsButNoDirectory "уже существует, но не является директорией."
#define MSGTR_VO_DirExistsButNotWritable "Директория вывода уже существует, но не доступна для записи."
#define MSGTR_VO_CantCreateDirectory "Не могу создать директорию вывода."
#define MSGTR_VO_CantCreateFile "Не могу создать выходной файл."
#define MSGTR_VO_DirectoryCreateSuccess "Директория вывода успешно создана."
#define MSGTR_VO_ValueOutOfRange "значение вне допустимого диапазона"

// libvo/aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] Предупреждение: Не найдено подходящего разрешения!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] Ошибка: Не найден размер, умещающийся в это разрешение!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "Сбой New_Face. Возможно неверен путь к шрифту.\nУкажите, пожалуйста, файл шрифта для текста (~/.mplayer/subfont.ttf).\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "Сбой New_Memory_Face.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "шрифт субтитров: сбой load_sub_face.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "шрифт субтитров: сбой prepare_charset.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "Невозможно подготовить шрифт субтитров.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "Невозможно подготовить шрифт OSD.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "Невозможно сгенерировать таблицы.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "Сбой FT_Done_FreeType.\n"

// sub.c
#define MSGTR_VO_SUB_Seekbar "Навигация"
#define MSGTR_VO_SUB_Play "Воспроизведение"
#define MSGTR_VO_SUB_Pause "Пауза"
#define MSGTR_VO_SUB_Stop "Стоп"
#define MSGTR_VO_SUB_Rewind "Назад"
#define MSGTR_VO_SUB_Forward "Вперёд"
#define MSGTR_VO_SUB_Clock "Время"
#define MSGTR_VO_SUB_Contrast "Контраст"
#define MSGTR_VO_SUB_Saturation "Насыщенность"
#define MSGTR_VO_SUB_Volume "Громкость"
#define MSGTR_VO_SUB_Brightness "Яркость"
#define MSGTR_VO_SUB_Hue "Цвет"
#define MSGTR_VO_SUB_Balance "Баланс"

// libvo/vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] Поддерживается только 16bpp!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] Визуальный ID %lx.\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] Не могу открыть /dev/3dfx.\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] Ошибка: %d.\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] Не могу отобразить области памяти 3dfx: %p,%p,%d.\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] Инициализировано: %p.\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] Неизвестное подустройство: %s.\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\nСубопции vo_aa библиотеки aalib:\n"
#define MSGTR_VO_AA_AdditionalOptions "Дополнительные опции, предоставляемые vo_aa:\n" \
"  help        вывести это сообщение\n" \
"  osdcolor    установить цвет OSD\n  subcolor    установить цвет субтитров\n" \
"        параметры цвета:\n           0 : нормальный\n" \
"           1 : тусклый\n           2 : жирный\n           3 : жирный шрифт\n" \
"           4 : инверсный\n           5 : специальный\n\n\n"

// libvo/vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] Не могу загрузить новую палитру SPU!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] Не могу задать режим воспроизведения!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] Не могу задать режим субкартинки!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] Не могу получить режим ТВ!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] Автовыбор режима ТВ по частоте кадров: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] Не могу установить режим ТВ!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] Устанавливаю для NTSC.\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] Устанавливаю для PAL/SECAM.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] Устанавливаю пропорции 4:3.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] Устанавливаю пропорции 16:9.\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] нехватка памяти\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] Не могу назначить ключевой цвет!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] Не могу назначить точный ключевой цвет,\nиспользую ближайшее приближение (0x%lx).\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] Деинициализация.\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] Не могу восстановить режим ТВ!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] Включаю предварительную буферизацию.\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] Использую новый механизм синхронизации.\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] Использую оверлей.\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] Ошибка: Для оверлея необходима компиляция с установленными\nбиблиотеками/заголовками X11.\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] Установлю режим ТВ в: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "авторегулировка к частоте кадров фильма (PAL/PAL-60)"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "авторегулировка к частоте кадров фильма (PAL/NTSC)"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "Использую текущий режим."
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "Предложен неизвестный режим. Использую текущий режим."
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] Ошибка открытия %s для записи, пробую /dev/em8300 взамен.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] Ошибка открытия %s для записи, пробую /dev/em8300_mv взамен.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] Также ошибка открытия /dev/em8300 для записи!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] Также ошибка открытия /dev/em8300_mv для записи!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] Открыто: %s.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] Ошибка открытия %s для записи, пробую /dev/em8300_sp взамен.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] Также ошибка открытия /dev/em8300_sp для записи!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] Не могу открыть дисплей в процессе установки хака оверлея!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] Не могу инициализировать X11!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] Не могу установить атрибут оверлея.\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] Не могу установить экран оверлея!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] Не могу активировать оверлей!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] Не могу изменить размер окна оверлея!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] Не могу установить bcs оверлея!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] Не могу получить значения Y-смещений оверлея!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] Не могу получить значения X-смещений оверлея!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] Не могу получить коррекцию на масштабирование по X оверлея!\nВыхожу.\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] смещение по Y: %d.\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] смещение по X: %d.\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] коррекция по X: %d.\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] Не могу установить сигнал mix!\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Прогрессивный JPEG включен."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Прогрессивный JPEG выключен."
#define MSGTR_VO_JPEG_BaselineJPEG "Базовый JPEG включен."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Базовый JPEG выключен."

// libvo/vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): изменён размер в %dx%d.\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] деинициализация!\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] ошибка в mga_vid_config ioctl (неверная версия mga_vid.o?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] Не могу получить значения люмы из модуля ядра!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] Не могу установить значения люмы из модуля ядра!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] Ширина/высота экрана неизвестны!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] неверный выходной формат %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] Версия вашего драйвера mga_vid несовместима с этой версией MPlayer!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] Не могу открыть: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] Разрешение источника, по крайней мере в одном измерении, больше чем\n1023x1023. Перемасштабируйте программно или используйте -lavdopts lowres=1\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] несоответствие версий драйвера mga_vid ядра (%u) и MPlayer (%u)\n"

// libvo/vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] Неизвестное подустройство: %s.\n"

// libvo/vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] Предупреждение: уровень сжатия установлен в 0, сжатие отключено!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] Информация: Используйте -vo png:z=<n> для установки уровня\nсжатия от 0 до 9.\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] Информация: (0 = нет сжатия, 1 = быстрое, худшее - 9 лучшее,\nсамое медленное сжатие)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] Ошибка открытия '%s' для записи!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] Ошибка в create_png.\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "Режим ASCII включен."
#define MSGTR_VO_PNM_RawMode "'Сырой' режим включен."
#define MSGTR_VO_PNM_PPMType "Будут записаны PPM файлы."
#define MSGTR_VO_PNM_PGMType "Будут записаны PGM файлы."
#define MSGTR_VO_PNM_PGMYUVType "Будут записаны PGMYUV файлы."

// libvo/vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] Не могу получить какой-либо приемлемый режим SDL для вывода.\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] Неподдерживаемый формат изображения (0x%X).\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] Информация: используйте -vm или -zoom для переключения\nв лучшее разрешение.\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] Не могу установить видеорежим: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] Не могу создать оверлей YUV: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] Не могу создать поверхность RGB: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] Использую преобразование глубины/цветового пространства,\nэто замедлит работу (%ibpp -> %ibpp).\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] Неподдерживаемый формат изображения в draw_slice,\nсвяжитесь с разработчиками MPlayer!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Сбой blit: %s.\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] Сбой инициализации SDL: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] Использую драйвер: %s.\n"

// libvo/vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] Форсированный vid_mode %d (%s) не доступен.\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] Форсированный vid_mode %d (%s) слишком мал.\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode: %d, %dx%d %dbpp.\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Сбой Vga_setmode(%d).\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] Видеорежим линейный и для передачи изображения может быть\nиспользован memcpy.\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] Видеорежим обладает аппаратным ускорением и может быть\nиспользован put_image.\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] Если это работает у Вас, мне хотелось бы знать.\n[VO_SVGA] (отправьте лог с помощью `mplayer test.avi -v -v -v -v &> svga.log`).\nСпасибо!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] У видеорежима %d страниц(а/ы).\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] Центрирую изображение. Начинаю в (%d,%d)\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] Использую VIDIX. w=%i h=%i  mw=%i mh=%i\n"

// libvo/vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Шаг %d(%d) x %d => %d.\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] Сбой очистки экрана шагом AGP.\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Сбой blit.\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] Для незаложенных форматов оверлея нужна конверсия.\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] Неподдерживаемый входной формат 0x%x.\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] Сбой установки оверлея.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] Сбой включения оверлея.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] Оверлей готов: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] Готов blit текстур: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] Сбой выключения оверлея\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] Не могу открыть %s: %s.\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] Не могу получить текущую конфигурацию: %s.\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Сбой memmap !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "Нужно доделать получение изображения.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] Сбой шага AGP.\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] Сбой установки YUV.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] Сбой шага AGP на проекции Y.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] Сбой шага AGP на проекции U.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] Сбой шага AGP на проекции V.\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] неизвестный формат: 0x%x.\n"

// libvo/vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] Не могу открыть %s: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] Проблема с ioctl FBITGET_FSCREENINFO: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] Проблема с ioctl FBITGET_VSCREENINFO: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] Этот драйвер поддерживает только 3Dfx Banshee, Voodoo3 и Voodoo 5.\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp вывод не поддерживается.\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] Не могу отобразить области памяти: %s.\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp вывод не поддерживается\n(Этого никогда не должно происходить).\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Ой! Что-то не в порядке с control().\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] Недостаточно памяти для воспроизведения этого фильма.\nПопробуйте меньшее разрешение.\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] Экран %dx%d и %d bpp, входные данные %dx%d и %d bpp, режим %dx%d.\n"

// libvo/vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] Неизвестное подустройство: %s.\n"

// libvo/vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] Возникла фатальная ошибка! Не могу продолжить.\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] неизвестное подустройство: '%s'.\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] У Вас слишком мало видеопамяти для этого режима:\n[VO_VESA] Необходимо: %08lX имеется: %08lX.\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] Вам нужно указать возможности монитора. Не изменяю частоту обновления.\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] Режим не соответствует ограничениям монитора. Не изменяю частоту обновления.\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] Обнаружена внутренняя фатальная ошибка: init вызван до preinit.\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] Опция -flip не поддерживается.\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] Возможная причина: VBE2 BIOS не найден.\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] Найден VESA VBE BIOS, версия %x.%x, ревизия: %x.\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] Видеопамять: %u Кб.\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] Возможности VESA: %s %s %s %s %s.\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! Ниже будет выведена информация OEM !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] Вы должны ниже видеть 5 строк информации OEM.\nЕсли нет, у вас неисправная vm86.\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] Информация OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] Ревизия OEM: %x.\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] Поставщик OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] Наименование продукта OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] Ревизия продукта OEM: %s.\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] Подсказка: Для работы ТВ-выхода Вам необходимо подключить разъём ТВ\n"\
"[VO_VESA] до загрузки, т.к. VESA BIOS инициализируется только в процессе POST.\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] Использую режим VESA (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] Не могу инициализировать программное масштабирование.\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] Не могу использовать DGA. Форсирую режим коммутации сегментов. :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] Использую DGA (физические ресурсы: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] Не могу использовать двойную буферизацию: недостаточно видеопамяти.\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] Не могу найти ни DGA ни переместимого фрейма окна.\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] Вы форсировали DGA. Выхожу\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] Не могу найти допустимый адрес окна.\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] Использую режим коммутации сегментов\n(физические ресурсы: %08lXh, %08lXh).\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] Не могу выделить временный буфер.\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] Извините, неподдерживаемый режим -- попробуйте -x 640 -zoom.\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] О, у Вас действительно есть изображение на ТВ!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] Не могу инициализировать Видеоверлей Linux.\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] Использую видеооверлей: %s.\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] Не могу инициализировать драйвер VIDIX.\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] Использую VIDIX.\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] Не могу найти режим для: %ux%u@%u.\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] Инициализация VESA завершена.\n"

// libvo/vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] Эта ветка больше не поддерживается.\n[VESA_LVO] Используйте -vo vesa:vidix взамен.\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] Не могу открыть: '%s'\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] Неверный выходной формат: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] Версия вашего драйвера fb_vid несовместима с этой версией MPlayer!\n"

// libvo/vo_x11.c

// libvo/vo_xv.c
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] Разделяемая память не поддерживается\nВозврат к обычному Xv.\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] Извините, Xv не поддерживается этой версией/драйвером X11\n[VO_XV] ******** Попробуйте -vo x11  или  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] Ошибка XvQueryAdaptors.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] Неверный параметр port, переопределение параметром port 0.\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] Невозможно захватить порт %i.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] Невозможно найти свободный Xvideo порт - возможно другое приложение\n"\
"[VO_XV] уже его использует. Закройте все программы видео, и попытайтесь снова. Если это\n"\
"[VO_XV] не помогает, смотрите другие (не-xv) драйверы в выводе 'mplayer -vo help'.\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] Похоже, ваша карта не имеет поддержки Xvideo.\n"\
"[VO_XV] Запустите 'xvinfo', чтобы проверить поддержку Xv и прочтите \n"\
"[VO_XV] DOCS/HTML/ru/video.html#xv!\n"\
"[VO_XV] Другие (не-xv) драйверы ищите в выводе 'mplayer -vo help'.\n"\
"[VO_XV] Попробуйте -vo x11.\n"
#define MSGTR_VO_XV_ImagedimTooHigh "Размеры изображения источника слишком велики: %ux%u (максимум %ux%u)\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Для режима c чересстрочной развёрткой необходимо, чтобы высота изображения\nделилась на 4."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Ширина изображения должна делиться на 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Не могу выделить память или файловый описатель для записи \"%s\"!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Ошибка записи изображения в вывод!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Неизвестное подустройство: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Использую режим вывода с чересстрочной развёрткой, верхнее поле первое."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Использую режим вывода с чересстрочной развёрткой, нижнее поле первое."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Использую (по умолчанию) режим построчной развёртки кадров."

// vosub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] Не могу начать воспроизведение: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] Не могу остановить воспроизведение: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] Слоёный UV для YUV410P не поддерживается.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] Был вызван фиктивный vidix_draw_slice().\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] Неподдерживаемый FourCC для этого драйвера VIDIX: %x (%s).\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] У сервера видео разрешение (%dx%d) не поддерживается,\nподдерживаемое: %dx%d-%dx%d.\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] Сервер видео не поддерживает глубину цвета vidix (%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] Драйвер VIDIX не может увеличить изображение (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] Драйвер VIDIX не может уменьшить изображение (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] Не могу настроить воспроизведение: %s.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] Не могу найти работающий драйвер VIDIX.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] Не могу получить возможность: %s.\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Не могу послать событие EWMH fullscreen!\n"
#define MSGTR_SelectedVideoMode "XF86VM: Выбран видеорежим %dx%d для размера изображения %dx%d.\n"

#define MSGTR_InsertingAfVolume "[Микшер] Нет аппаратного микширования, вставляю фильтр громкости.\n"
#define MSGTR_NoVolume "[Микшер] Контроль громкости не доступен.\n"
#define MSGTR_NoBalance "[Микшер] Управление балансом недоступно.\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "Драйвер видеовывода pgm был заменён -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "Драйвер видеовывода md5 был заменён -vo md5sum.\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "аудиовывод: модули alsa9 и alsa1x были удалены, используйте -ao alsa взамен.\n"
#define MSGTR_AO_NoSuchDriver "Отсутствует аудио драйвер '%.*s'\n"
#define MSGTR_AO_FailedInit "Не могу инициализировать аудио драйвер '%s'\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] инициализация аудио: Не могу открыть устройство микшера %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] инициализация аудио: У микшера аудиокарты отсутствует канал '%s',\nиспользую канал по умолчанию.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] инициализация аудио: Не могу открыть аудиоустройство %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] инициализация аудио: Не могу заблокировать файловый описатель: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] Не могу установить аудиоустройство %s в %s вывод, пробую %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] инициализация аудио: Не могу установить аудиоустройство\nв %d-канальный режим.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] инициализация аудио: драйвер не поддерживает SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Ваш аудиодрайвер НЕ поддерживает select()  ***\n Перекомпилируйте MPlayer с #undef HAVE_AUDIO_SELECT в config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS] Фатальная ошибка:\n*** НЕ МОГУ ПОВТОРНО ОТКРЫТЬ / СБРОСИТЬ АУДИОУСТРОЙСТВО (%s) ***\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] Неизвестный/Неподдерживаемый формат OSS: %x.\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Соединился с звуковым сервером.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Не могу открыть поток.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Поток открыт.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] размер буфера: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Не могу установить громкость в %d.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] dxr2: %d Гц не поддерживается, попробуйте изменить\nчастоту дискретизации.\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] Выполнить esd_open_sound не удалось: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] задержка: [сервер: %0.2fs, сеть: %0.2fs] (подстройка %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] не могу открыть поток воспроизведения esd: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB аудио: не могу установить микшер: %s\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Гц не поддерживается, попробуйте изменить\nчастоту дискретизации.\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Файл: %s (%s)\nPCM: Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Информация: наиболее быстрый дамп достигается с -vc null -vo null\n[AO PCM]: Информация: -ao pcm:fast. Для записи WAVE файлов используйте\n[AO PCM]: Информация: -ao pcm:waveheader (по умолчанию).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Не могу открыть %s для записи!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] использую %s аудиодрайвер.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Неподдерживаемый аудиоформат: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] Не могу инициализировать SDL аудио: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Не могу открыть аудио: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] управление.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] инициализация: Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] воспроизведение: неверное устройство.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] инициализация: ошибка установки параметров: %s\nНе могу установить требуемую частоту дискретизации.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] инициализация: AL_RATE не доступен на заданном ресурсе.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] инициализация: ошибка получения параметров: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] инициализация: частота дискретизации теперь %f (требуемая частота %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] инициализация: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] инициализация: Не могу открыть канал аудио: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] деинициализация: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] сброс: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] пауза аудио: ...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] возобновление аудио: ...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: Выполнить SETINFO не удалось.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: запись не удалась."
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Не могу открыть аудиоустройство %s, %s -> нет звука.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] инициализация аудио: ваша карта не поддерживает канал %d,\n%s, частоту дискретизации %d Гц.\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] Неверный индекс микшера. Возврат к 0.\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] Ошибка открытия микшера: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] Ошибка присоединения %s к микшеру: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] Ошибка регистрации микшера: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] Ошибка загрузки микшера: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] Невозможно найти простой элемент управления '%s',%i.\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] Ошибка установки левого канала, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] Ошибка установки правого канала, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] подсказка командной строки -ao alsa:\n"\
"[AO_ALSA] Пример: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   Устанавливает четвертое устройство первой карты.\n\n"\
"[AO_ALSA] Опции:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     Открывает устройство в неблокирующем режиме.\n"\
"[AO_ALSA]   device=<имя-устройства>\n"\
"[AO_ALSA]     Устанавливает устройство (замените , на . и : на =)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] Количество каналов %d не поддерживается.\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] Ошибка открытия в неблокирующем режиме, попытка открыть в режиме блокирования.\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] Ошибка начала воспроизведения: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] Ошибка установки блокирующего режима: %s.\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] Невозможно получить начальные параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] Невозможно установить тип доступа: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] Формат %s не поддерживается оборудованием, пробуем значение по-умолчанию.\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] Невозможно установить формат: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] Невозможно установить количество каналов: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] Невозможно отключить ресэмплинг: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] Невозможно установить частоту дискретизации-2: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] Невозможно установить примерное время буфера: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] Невозможно получить время периода: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] Невозможно установить периоды: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] Невозможно установить hw-параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] Невозможно получить размер буфера: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] Невозможно получить sw-параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] Невозможно получить границу: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] Невозможно установить порог запуска: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] Невозможно установить порог останова: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] Невозможно установить величину тишины: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] ошибка pcm close: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] Не определен обработчик!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] ошибка pcm prepare: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] ошибка pcm pause: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] ошибка pcm drop: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] ошибка pcm resume: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] Ошибка настройки устройства."
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm в ждущем режиме, попытка продолжить.\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] ошибка записи: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] попытка сбросить звуковую карту.\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] Невозможно получить pcm статус: %s\n"

// ao_plugin.c


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "доступные метки в"
#define MSGTR_AF_LADSPA_WarnNoInputs "ПРЕДУПРЕЖДЕНИЕ! У этого LADSPA плагина отсутствуют аудиовходы.\n  Входящий сигнал аудио будет потерян."
#define MSGTR_AF_LADSPA_ErrNoOutputs "У этого LADSPA плагина отсутствуют выходы аудио."
#define MSGTR_AF_LADSPA_ErrInOutDiff "Число аудиовходов и аудиовыходов у LADSPA плагина отличается."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "не могу загрузить"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Не могу найти функцию ladspa_descriptor() в указанном файле библиотеки."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Не могу найти метку в библиотеке плагина."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Не указаны субопции."
#define MSGTR_AF_LADSPA_ErrNoLibFile "Не указан файл библиотеки."
#define MSGTR_AF_LADSPA_ErrNoLabel "Не указана метка фильтра."
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Недостаточно настроек указано в командной строке."
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: Входной параметр #%d меньше нижней границы %0.4f.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: Входной параметр #%d больше верхней границы %0.4f.\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "неизвестный формат "


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "Не могу отрыть устройство джойстика %s: %s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "Ошибка чтения устройства джойстика: %s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "Джойстик: Потеряно %d байт(а/ов) данных\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "Джойстик: предупреждение о событии инициализации,\nпотеряна синхронизация с драйвером.\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "Джойстик: предупреждение о неизвестном типе события %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "Невозможно открыть устройство Apple IR: %s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "Слишком много описателей файлов команд,\nне могу зарегистрировать файловый описатель %d.\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "Слишком много описателей файлов клавиш,\nне могу зарегистрировать файловый описатель %d.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "Команда %s: аргумент %d не целое число.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "Команда %s: аргумент %d не вещественный.\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "Команда %s: аргумент %d не определён.\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "Неизвестный аргумент %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "Команда %s требует не менее %d аргументов, мы нашли пока только %d.\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "Ошибка чтения описателя %d файла команд: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "Командный буфер файлового описателя %d полон: пропускаю содержимое.\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "Неверная команда для привязки к клавише %s"
#define MSGTR_INPUT_INPUT_ErrSelect "Ошибка вызова select: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "Ошибка в файловом описателе %d клавиш ввода\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "Фатальная ошибка клавиши ввода в файловом описателе %d\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "Слишком много событий одновременного нажатия клавиш\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "Ошибка в описателе %d файла команд\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "Ошибка чтения конфигурационного файла ввода %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "Неизвестная клавиша '%s'\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "Буфер слишком мал для названия этой клавиши: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "Не найдено команды для клавиши %s"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "Буфер слишком мал для команды %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "Не могу инициализировать джойстик ввода\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "Не могу выполнить open %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "Не могу инициализировать Пульт ДУ Apple Remote.\n"

// lirc.c
#define MSGTR_LIRCopenfailed "Неудачное открытие поддержки LIRC.\nВы не сможете использовать Ваш пульт управления.\n"
#define MSGTR_LIRCcfgerr "Неудачная попытка чтения файла конфигурации LIRC '%s'!\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c
#define MSGTR_TooManyStreams "Слишком много потоков!"
#define MSGTR_RawMuxerOnlyOneStream "Мультиплексор rawaudio поддерживает только один аудиопоток!\n"
#define MSGTR_IgnoringVideoStream "Игнорирую видеопоток!\n"
#define MSGTR_UnknownStreamType "Предупреждение, неизвестный тип потока: %d\n"
#define MSGTR_WarningLenIsntDivisible "Предупреждение: длина не кратна размеру образца!\n"
#define MSGTR_MuxbufMallocErr "Мультиплексор фреймбуфера не может выделить память (malloc)!\n"
#define MSGTR_MuxbufReallocErr "Мультиплексор фреймбуфера не может перераспределить память (realloc)!\n"
#define MSGTR_WritingHeader "Запись заголовка...\n"
#define MSGTR_WritingTrailer "Запись индекса...\n"

// demuxer.c, demux_*.c
#define MSGTR_AudioStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заголовок аудио потока %d переопределён!\n"
#define MSGTR_VideoStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заголовок видео потока %d переопределён!\n"
#define MSGTR_TooManyAudioInBuffer "\nСлишком много (%d в %d байтах) аудиопакетов в буфере!\n"
#define MSGTR_TooManyVideoInBuffer "\nСлишком много (%d в %d байтах) видеопакетов в буфере!\n"
#define MSGTR_MaybeNI "Возможно Вы проигрываете 'неслоёный' поток/файл или неудачный кодек?\n" \
                      "Для AVI файлов попробуйте форсировать 'неслоёный' режим опцией -ni.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: Обход ошибки CBR-MP3 nBlockAlign заголовка!\n"
#define MSGTR_SwitchToNi "\nОбнаружен плохо 'слоёный' AVI файл - переключаюсь в -ni режим...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: неверный ID аудио потока: %d - игнорирование (без звука)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: неверный ID видео потока: %d - игнорирование (используется значение по-умолчанию)\n"
#define MSGTR_ON2AviFormat "ON2 AVI формат"
#define MSGTR_Detected_XXX_FileFormat "Обнаружен %s формат файла!\n"
#define MSGTR_FormatNotRecognized "======= Извините, формат этого файла не распознан/не поддерживается ==========\n"\
                                  "===== Если это AVI, ASF или MPEG поток, пожалуйста свяжитесь с автором! ======\n"
#define MSGTR_SettingProcessPriority "Устанавливаю приоритет процесса: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] формат:%d  fourcc:0x%X  размер:%dx%d  fps:%5.3f  ftime:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Невозможно инициализировать мультиплексор."
#define MSGTR_MissingVideoStream "Видеопоток не найден!\n"
#define MSGTR_MissingAudioStream "Аудиопоток не найден -> без звука\n"
#define MSGTR_MissingVideoStreamBug "Видеопоток потерян!? свяжитесь с автором, это может быть ошибкой :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: в файле нет выбранного аудио или видеопотока\n"

#define MSGTR_NI_Forced "Форсирован"
#define MSGTR_NI_Detected "Обнаружен"
#define MSGTR_NI_Message "%s 'НЕСЛОЁНЫЙ' формат AVI файла!\n"

#define MSGTR_UsingNINI "Использование 'НЕСЛОЁНОГО' испорченного формата AVI файла!\n"
#define MSGTR_CouldntDetFNo "Не смог определить число кадров (для абсолютного перемещения).\n"
#define MSGTR_CantSeekRawAVI "Не могу переместиться в сыром потоке AVI!\n(требуется индекс, попробуйте с ключом -idx!)\n"
#define MSGTR_CantSeekFile "Не могу перемещаться в этом файле!\n"

#define MSGTR_MOVcomprhdr "MOV: Для поддержки сжатых заголовков необходим zlib!\n"
#define MSGTR_MOVvariableFourCC "MOV: Предупреждение! Обнаружен переменный FOURCC!?\n"
#define MSGTR_MOVtooManyTrk "MOV: Предупреждение! слишком много треков!"
#define MSGTR_ErrorOpeningOGGDemuxer "Не могу открыть демультиплексор ogg.\n"
#define MSGTR_CannotOpenAudioStream "Не могу открыть аудиопоток: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Не могу открыть поток субтитров: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Не могу открыть демультиплексор аудио: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Не могу открыть демультиплексор субтитров: %s\n"
#define MSGTR_TVInputNotSeekable "По ТВ входу нельзя перемещаться! (Возможно, перемещение будет для смены каналов ;)\n"
#define MSGTR_DemuxerInfoChanged "Информация демультиплексора %s изменена в %s\n"
#define MSGTR_ClipInfo "Информация о клипе:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: обнаружено 30000/1001 кадров/сек NTSC содержимое, переключаю частоту кадров.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: обнаружено 24000/1001 кадра/сек NTSC содержимое с построчной развёрткой,\nпереключаю частоту кадров.\n"

#define MSGTR_CacheFill "\rЗаполнение кэша: %5.2f%% (%"PRId64" байт(а))   "
#define MSGTR_NoBindFound "Не найдена привязка к клавише '%s'.\n"
#define MSGTR_FailedToOpen "Не могу открыть %s.\n"

#define MSGTR_VideoID "[%s] Найден видео поток, -vid %d\n"
#define MSGTR_AudioID "[%s] Найден аудио поток, -aid %d\n"
#define MSGTR_SubtitleID "[%s] Найден поток субтитров, -sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "ФАТАЛЬНАЯ ОШИБКА: размер заголовка более 1 MB (%d)!\nПожалуйста, обратитесь к разработчикам MPlayer и загрузите/отошлите этот файл.\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "Не могу выделить %d байт(а/ов) для заголовка.\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "EOF при чтении заголовка ASF, испорченный/неполный файл?\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR, вероятно, будет работать только с libavformat,\nпопробуйте -demuxer 35, если у Вас есть проблемы\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "Нет звена данных, следующих за заголовком!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: не найден аудио или видео заголовок - испорченный файл?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "Неверная длина в заголовке ASF!\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "URL DRM лицензии: %s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "Файл был обременен DRM шифрованием, он не будет воспроизводиться в MPlayer!\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "** пустой список?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** Предупреждение: Это не расширенный заголовок AVI..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: Построение индекса ODML (%d звеньев супериндекса).\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: Обнаружен плохой (неполный?) файл. Использую традиционный индекс.\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "Не могу прочитать файл индекса %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s не является корректным файлом индекса MPlayer.\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "Не могу выделить память для данных индекса из %s.\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "преждевременный конец индексного файла %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "Загружаю индексный файл: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "Создаю Индекс: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: Создана индексная таблица для %d звеньев!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "Не могу записать файл индекса %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "Сохранён индексный файл: %s\n"

// open.c, stream.c
#define MSGTR_CdDevNotfound "CD-ROM '%s' не найден!\n"
#define MSGTR_ErrTrackSelect "Ошибка выбора дорожки VCD!"
#define MSGTR_ReadSTDIN "Чтение из stdin (со стандартного входа)...\n"
#define MSGTR_FileNotFound "Файл не найден: '%s'\n"

#define MSGTR_SMBInitError "Не могу инициализировать библиотеку libsmbclient: %d\n"
#define MSGTR_SMBFileNotFound "Не могу открыть по сети: '%s'\n"

#define MSGTR_CantOpenDVD "Не могу открыть DVD: %s (%s)\n"

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "Неизвестное семейство адресов %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "Разрешаю %s для %s...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "Не могу разрешить имя для %s: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "Соединяюсь с сервером %s[%s]: %d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "Не могу соединится с сервером: %s\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "Select не удался.\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "таймаут соединения\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt не удался: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "ошибка соединения: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "Неверные настройки прокси... пробую без прокси.\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "Не могу разрешить удалённое имя для AF_INET. Пробую без прокси.\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "Ошибка отправки HTTP запроса: Послан не весь запрос.\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "Чтение не удалось.\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response прочитано 0 (т.е. EOF).\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "Ошибка аутентификации. Используйте опции -user и -passwd чтобы предоставить ваши\n"\
"логин/пароль для списка URL, или сформируйте URL следующим образом:\n"\
"http://логин:пароль@имя_хоста/файл\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "Для %s требуется аутентификация\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "Требуется аутентификация.\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "Пароль не указан, пробую пустой пароль.\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "Сервер вернул %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "Установлен размер кэша %d КБайт(а/ов)\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "Демультиплексор: неизвестный формат %d.\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "ошибка заполнения_буфера: плохой демультиплексор: не vd, ad или sd.\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] ошибка инициализации zlib.\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] ошибка zlib распаковки.\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] ошибка lzo распаковки.\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] Дорожка номер %u зашифрована, а расшифровка еще не \n[mkv] реализована. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] Неизвестный тип шифрования содержимого для дорожки %u. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] Дорожка %u сжата неизвестным/неподдерживаемым \n[mkv] алгоритмом (%u). Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] Дорожка %u сжата zlib, но mplayer скомпилирован \n[mkv] без поддержки сжатия zlib. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] ID дорожки %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] ID дорожки %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] Неизвестный/неподдерживаемый CodecID (%s) или отсутствующие/плохие\n[mkv] данные CodecPrivate (дорожка %u).\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] Дорожка FLAC не содержит правильных заголовков.\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] Неизвестный/неподдерживаемый ID аудио кодека '%s' для дорожки %u или отсутствующие/неверные\n[mkv] частные данные кодека.\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] Тип субтитров '%s' не поддерживается.\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] Будет воспроизводиться дорожка %u.\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] Не найдена/не требуется видео дорожка.\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] Не найдена/не требуется аудио дорожка.\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] Предупреждение: Не найдена длительность блока трека субтитров.\n"

// demux_nuv.c

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "Найден плагин: %s (%s).\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "Закрываю плагин: %s.\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "Ожидание начала воспроизведения '%s' плагином XMMS...\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[МЕНЮ] Не найдено вхождений в описании меню.\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[МЕНЮ] ошибка синтаксиса в строке: %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[МЕНЮ] Описания меню требуют именного атрибута (строка %d).\n"
#define MSGTR_LIBMENU_BadAttrib "[МЕНЮ] плохой атрибут %s=%s в меню '%s' в строке %d\n"
#define MSGTR_LIBMENU_UnknownMenuType "[МЕНЮ] неизвестный тип меню '%s' в строке %d\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[МЕНЮ] Не могу открыть конфигурационный файл меню: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[МЕНЮ] Конфигурационный файл слишком велик (> %d KB)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[МЕНЮ] Конфигурационный файл пуст.\n"
#define MSGTR_LIBMENU_MenuNotFound "[МЕНЮ] Меню %s не найдено.\n"
#define MSGTR_LIBMENU_MenuInitFailed "[МЕНЮ] Меню '%s': Ошибка инициализации.\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[МЕНЮ] Неподдерживаемый выходной формат!!!!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[МЕНЮ] Описаниям вхождений меню списка нужно имя (строка %d).\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[МЕНЮ] Меню списка необходим аргумент.\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[МЕНЮ] Ошибка вызова waitpid: %s.\n"
#define MSGTR_LIBMENU_SelectError "[МЕНЮ] Ошибка вызова select.\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[МЕНЮ] Ошибка чтения дочернего файлового описателя: %s.\n"
#define MSGTR_LIBMENU_ConsoleRun "[МЕНЮ] Запуск консоли: %s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[МЕНЮ] Дочерний процесс уже запущен.\n"
#define MSGTR_LIBMENU_ForkFailed "[МЕНЮ] Вызов fork не удался !!!\n"
#define MSGTR_LIBMENU_WriteError "[МЕНЮ] ошибка записи\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[МЕНЮ] ошибка открытия каталога: %s\n"
#define MSGTR_LIBMENU_ReallocError "[МЕНЮ] ошибка перераспределения памяти: %s\n"
#define MSGTR_LIBMENU_MallocError "[МЕНЮ] ошибка выделения памяти: %s\n"
#define MSGTR_LIBMENU_ReaddirError "[МЕНЮ] ошибка чтения каталога: %s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[МЕНЮ] Не могу открыть каталог %s.\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[МЕНЮ] Описаниям подменю нужен атрибут 'menu'.\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] Неверное свойство '%s' в элементе меню предпочтений. (строка %d).\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[МЕНЮ] Описаниям элемента меню предпочтений нужен допустимый \nатрибут 'property' или 'txt' (строка %d).\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[МЕНЮ] Меню предпочтений нужен аргумент.\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[МЕНЮ] Не могу найти целевой пункт ????\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[МЕНЮ] Не могу построить команду: %s.\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[МЕНЮ] Текстовому меню нужно имя текстового файла (параметр file).\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[МЕНЮ] Не могу открыть %s.\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[МЕНЮ] Предупреждение, строка слишком длинная. Разбиваю.\n"
#define MSGTR_LIBMENU_ParsedLines "[МЕНЮ] Проанализировано %d строк.\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[МЕНЮ] Неизвестная команда: '%s'.\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[МЕНЮ] Не могу открыть меню: '%s'.\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c
#define MSGTR_CantOpenCodec "Не могу открыть кодек.\n"
#define MSGTR_CantCloseCodec "Не могу закрыть кодек.\n"

#define MSGTR_MissingDLLcodec "ОШИБКА: Не смог открыть требующийся DirectShow кодек: %s\n"
#define MSGTR_ACMiniterror "Не смог загрузить/инициализировать Win32/ACM аудиокодек (потерян DLL файл?)\n"
#define MSGTR_MissingLAVCcodec "Не могу найти кодек '%s' в libavcodec...\n"

#define MSGTR_MpegNoSequHdr "MPEG: ФАТАЛЬНАЯ ОШИБКА: КОНЕЦ ФАЙЛА при поиске последовательности заголовков.\n"
#define MSGTR_CannotReadMpegSequHdr "ФАТАЛЬНАЯ ОШИБКА: Не могу считать последовательность заголовков.\n"
#define MSGTR_CannotReadMpegSequHdrEx "ФАТАЛЬНАЯ ОШИБКА: Не мочу считать расширение последовательности заголовков.\n"
#define MSGTR_BadMpegSequHdr "MPEG: Плохая последовательность заголовков.\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Плохое расширение последовательности заголовков.\n"

#define MSGTR_ShMemAllocFail "Не могу зарезервировать разделяемую память.\n"
#define MSGTR_CantAllocAudioBuf "Не могу зарезервировать выходной аудио буфер.\n"

#define MSGTR_UnknownAudio "Неизвестный/потерянный аудио формат -> без звука\n"

#define MSGTR_UsingExternalPP "[PP] Использую внешний фильтр постобработки, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Использую постобработку из кодека, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Запрошенное семейство видеокодеков [%s] (vfm=%s) не доступно.\nВключите его во время компиляции.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Запрошенное семейство аудиокодеков [%s] (afm=%s) не доступно.\nВключите его во время компиляции.\n"
#define MSGTR_OpeningVideoDecoder "Открываю декодер видео: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Выбран видеокодек: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Открываю декодер аудио: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Выбран аудиокодек: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "Ошибка инициализации Декодера Видео :(\n"
#define MSGTR_ADecoderInitFailed "Ошибка инициализации Декодера Аудио :(\n"
#define MSGTR_ADecoderPreinitFailed "Ошибка предварительной инициализации Декодера Аудио :(\n"

// libmpcodecs/ad_dvdpcm.c
#define MSGTR_SamplesWanted "Для улучшения поддержки необходимы образцы этого формата.\nПожалуйста, свяжитесь с разработчиками.\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] Предупреждение! Размер фрейма аудио отличается! read=%d  hdr=%d.\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Не могу найти подходящее цветовое пространство - попытаюсь с -vf scale...\n"
#define MSGTR_MovieAspectIsSet "Movie-Aspect - %.2f:1 - выполняю предварительное масштабирование\nдля коррекции соотношения сторон фильма.\n"
#define MSGTR_MovieAspectUndefined "Movie-Aspect не определён - предварительное масштабирование не применяется.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Вам нужно обновить/установить пакет бинарных кодеков.\nЗайдите на http://www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] Не могу выделить изображение для кодека cinepak.\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] Арифметическое среднее QP: %2.4f, Гармоническое среднее QP: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] Сбой DRI.\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] Не могу выделить изображения для кодека.\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] XVMC-ускоренный MPEG-2.\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] Пробую pixfmt=%d.\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] Буфер mc_get_buffer должен использоваться только с XVMC ускорением!!"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] Разрешены только буферы, выделенные vo_xvmc.\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] Выбрано высококачественное кодирование (не в реальном времени)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] Использую постоянный qscale = %f (VBR).\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] Сырой вывод с FourCC [%x] не поддерживается!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] Необходимый VfW кодек не определён!!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Не могу найти видео фильтр '%s'.\n"
#define MSGTR_CouldNotOpenVideoFilter "Не могу открыть видео фильтр '%s'.\n"
#define MSGTR_OpeningVideoFilter "Открываю видео фильтр: "
#define MSGTR_CannotFindColorspace "Не могу найти подходящее цветовое пространство, даже вставив 'scale' :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] Плохая позиция/ширина/высота - урезанная область вне оригинала!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] Область урезания: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d).\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] Неизвестное имя формата: '%s'.\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] Ошибка анализа аргумента.\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "Тип компрессора: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "Подтип компрессора: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "Флаги компрессора: %lu, версия %lu, версия ICM: %lu\n"
#define MSGTR_MPCODECS_Flags "Флаги:"
#define MSGTR_MPCODECS_Quality " качество"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "Полный DR невозможен, пробую SLICES взамен!\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "Почему мы получили NULL??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "%s не поддерживается следующим фильтром/видеовыводом :(\n"


// ================================== LIBASS ====================================

// ass_bitmap.c

// ass.c
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): сбой fopen\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): Отклонение загрузки субтитров больше 100M\n"

// ass_cache.c

// ass_fontconfig.c

// ass_render.c
//FIXME glyph

// ass_font.c


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "Не могу задать частоту дискретизации.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "Не могу задать время буферизации.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "Не могу задать время периода.\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "Некорректная конфигурация для данного PCM: нет доступных конфигураций.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "Тип доступа не доступен.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "Формат образца не доступен.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "Число каналов не доступно - возвращаюсь к умолчанию: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "Не могу установить аппаратные параметры: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "Не могу использовать период, равный размеру буфера (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "Не могу установить программные параметры:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "Ошибка открытия аудио: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (как минимум длительностью %.3f мс)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: ошибка подготовки: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "Ошибка чтения/записи ALSA"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "Не могу задать число каналов: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "Не могу включить стерео: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "Не могу открыть '%s': %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "неподдерживаемый формат\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "Не могу задать аудиоформат."
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "Не могу задать частоту дискретизации: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "Не могу установить триггер: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "Не могу получить размер блока!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "Размер блока аудио нулевой, устанавливаю в %d!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "Размер блока аудио слишком мал, устанавливаю в %d!\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "ошибка записи\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\nТревога! EOF\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "чтение предварительного заголовка не удалось\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "Неверный размер заголовка, безнадёжно.\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "Не могу прочитать данные заголовка.\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "не могу прочитать packet_len.\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "Неверный размер пакета RTSP, безнадёжно.\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "Не могу прочитать управляющие данные.\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "объект заголовка\n"
#define MSGTR_MPDEMUX_MMST_DataObject "объект данных\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "файловый объект, длина пакета = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "потоковый объект, ID потока: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "Слишком много ID, поток пропущен."
#define MSGTR_MPDEMUX_MMST_UnknownObject "неизвестный объект\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "Не могу прочитать медиаданные.\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "пропущена подпись\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "Всё сделано. Спасибо за загрузку медиафайла,\nсодержащего проприетарную и запатентованную технологию =).\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "неизвестная команда %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "ошибка get_media_packet : %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "Соединились\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "Эх, размер stream_chunck слишком мал: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "несоответствие size_confirm!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "Предупреждение: пропуск заголовка ????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "Ошибка разбора заголовка звена\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "Не получил заголовок как первое звено !!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "Ошибка: Не могу выделить буфер в %d байт(а/ов).\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "Ошибка чтения сетевого потока.\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "Ошибка: Звено слишком мало.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "Скорость передачи слишком мала, файл не может быть проигран!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "Скорость передачи слишком мала, отключаю аудиопоток.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "Скорость передачи слишком мала, отключаю видеопоток.\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "Неверная длина в заголовке ASF!\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "Ошибка: chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> Перенаправитель ASF\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "неверный URL прокси\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "неизвестный тип потока ASF\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "Не могу проанализировать ответ HTTP.\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "Сервер вернул %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ПРЕДУПРЕЖДЕНИЕ АНАЛИЗАТОРА HTTP ASF : Pragma %s урезана от %zu байт до %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "ошибка записи сокета: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "Не могу разобрать заголовок.\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "Поток не найден.\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "неизвестный тип потока ASF\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "Сбой, выхожу.\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\nОшибка чтения аудио: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "Восстановление после перескока, некоторые кадры могут быть потеряны!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "Фатальная ошибка, не могу восстановиться!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\nНедостаточная выборка аудио!\n"

// cache2.c

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "Не могу открыть устройство CDDA.\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "Не могу открыть диск.\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "Найден аудио CD с %d дорожками.\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "Не могу прочитать TOC.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "Не могу открыть устройство %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "неверный URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "Не могу отправить HTTP запрос.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "Не могу прочесть HTTP ответ.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "Не найден.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "неизвестный код ошибки\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "Кэш не найден.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "Не все xmcd файлы были прочитаны.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "Не могу создать каталог %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "Не все xmcd файлы были записаны.\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "Возвращён неверный файл базы данных xmcd.\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "неожиданное FIXME\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "необработанный код\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "Невозможно найти конец строки.\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "Анализ успешен, найдено: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "Альбом не найден.\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "Сервер вернул: ошибка синтаксиса команды\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "Не могу получить уровень протокола.\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "Нет CD в приводе.\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] Неожиданная строка файла cue: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] проверенное имя бинарного файла: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] Не могу найти бинарный файл - безнадёжно.\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] Использую бинарный файл %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] неизвестный режим для бинарного файла.\nЭтого не должно происходить. Выхожу.\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] Не могу открыть %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] Ошибка чтения из %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] Ошибка получения размера бинарного файла.\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "дорожка %02d:  формат=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] неожиданный конец бинарного файла\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] Не могу прочитать %d байт(а/ов) полезной нагрузки.\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE открытие_потока, имя файла=%s, дорожка=%d, доступные дорожки: %d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "Невозможно открыть DVD устройство для записи, изменение скорости DVD требует доступ на запись.\n"
#define MSGTR_DVDrestoreSpeed "Восстановление скорости DVD... "
#define MSGTR_DVDlimitSpeed "Ограничение скорости DVD до %dКБ/с... "
#define MSGTR_DVDlimitFail "ошибка\n"
#define MSGTR_DVDlimitOk "успешно\n"
#define MSGTR_NoDVDSupport "MPlayer был скомпилирован без поддержки DVD, выходим.\n"
#define MSGTR_DVDnumTitles "На этом DVD %d роликов.\n"
#define MSGTR_DVDinvalidTitle "Недопустимый номер DVD ролика: %d\n"
#define MSGTR_DVDinvalidChapterRange "Неверное описание диапазона раздела %s\n"
#define MSGTR_DVDnumAngles "В этом DVD ролике %d углов.\n"
#define MSGTR_DVDinvalidAngle "Недопустимый номер DVD угла: %d\n"
#define MSGTR_DVDnoIFO "Не могу открыть IFO файл для DVD ролика %d.\n"
#define MSGTR_DVDnoVMG "Не могу открыть VMG информацию!\n"
#define MSGTR_DVDnoVOBs "Не могу открыть VOBS ролика (VTS_%02d_1.VOB).\n"
#define MSGTR_DVDnoMatchingAudio "Не найден подходящий аудио язык DVD!\n"
#define MSGTR_DVDaudioChannel "Выбранный аудиоканал DVD: %d язык: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "аудиопоток: %d формат: %s (%s) язык: %s aid: %d.\n"
#define MSGTR_DVDnumAudioChannels "число аудиоканалов на диске: %d.\n"
#define MSGTR_DVDnoMatchingSubtitle "Не найден подходящий язык субтитров DVD!\n"
#define MSGTR_DVDsubtitleChannel "Выбранный канал субтитров DVD: %d язык: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "субтитры ( sid ): %d язык: %s\n"
#define MSGTR_DVDnumSubtitles "число субтитров на диске: %d\n"

// stream/stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] Обнаружены имена радиостанций.\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] Неверная частота для станции %s\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] Неверный номер станции: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] Неверный номер станции: %d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] Неверное название станции: %s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] В параметрах обнаружена частота.\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Предупреждение: сбой вызова ioctl get tuner : %s. frac установлен в %d.\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s не является устройством радио!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] сбой вызова ioctl set frequency 0x%x (%.2f): %s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] сбой вызова ioctl get frequency: %s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] сбой вызова ioctl set mute: %s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] сбой вызова ioctl query control: %s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] сбой вызова ioctl get volume: %s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] сбой вызова ioctl set volume: %s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] Невозможно создать аудио буфер (блок=%d,размер=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] Текущая частота: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] Выбрана станция: %d - %s (частота: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] Невозможно изменить станцию: не передан список радиостанций.\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] Невозможно открыть '%s': %s\n"
#define MSGTR_RADIO_WrongFreq "[radio] Неверная частота: %.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] Используется частота: %.2f.\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] сбой вызова audio_in_init\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] сбой вызова audio_in_setup: %s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] Ошибка очистки буфера: %s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] Ошибка вызова stream_enable_cache: %s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] Неизвестный драйвер: %s\n"
#define MSGTR_RADIO_DriverV4L "[radio] Используется V4Lv1 радио интерфейс.\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] Используется V4Lv2 радио интерфейс.\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] Используется *BSD BT848 радио интерфейс.\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c: norm_from_string(%s): Неизвестный параметр norm, устанавливается %s.\n"
#define MSGTR_TV_NoVideoInputPresent "Ошибка: Видео вход отсутствует!\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" ВНИМАНИЕ:\n"\
" ЗАПРОШЕН НЕПРОТЕСТИРОВАННЫЙ ИЛИ НЕИЗВЕСТНЫЙ ФОРМАТ ИЗОБРАЖЕНИЯ (0x%x)\n"\
" Это может привести к неверному воспроизведению или краху программы!\n"\
" Отчеты об ошибках приниматься не будут! Вам следует попытаться еще раз \n"\
" с YV12 (пространство цветов по умолчанию) и прочесть документацию!\n"\
"==================================================================\n"
#define MSGTR_TV_CannotSetNorm "Ошибка: Невозможно установить norm!\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP: ширина %d высота %d\n"
#define MSGTR_TV_UnableToSetWidth "Невозможно установить запрошенную ширину: %d\n"
#define MSGTR_TV_UnableToSetHeight "Невозможно установить запрошенную высоту: %d\n"
#define MSGTR_TV_NoTuner "Выбранный вход не имеет тюнера!\n"
#define MSGTR_TV_UnableFindChanlist "Невозможно найти выбранный список каналов! (%s)\n"
#define MSGTR_TV_ChannelFreqParamConflict "Вы не можете указать частоту и канал одновременно!\n"
#define MSGTR_TV_ChannelNamesDetected "Обнаружены названия TV каналов.\n"
#define MSGTR_TV_NoFreqForChannel "Невозможно найти частоту для канала %s (%s)\n"
#define MSGTR_TV_SelectedChannel3 "Выбран канал: %s - %s (частота: %.3f)\n"
#define MSGTR_TV_SelectedChannel2 "Выбран канал: %s (частота: %.3f)\n"
#define MSGTR_TV_UnsupportedAudioType "Тип аудио '%s (%x)' не поддерживается!\n"
#define MSGTR_TV_AvailableDrivers "Доступные драйверы:\n"
#define MSGTR_TV_DriverInfo "Выбран драйвер: %s\n название: %s\n автор: %s\n комментарий %s\n"
#define MSGTR_TV_NoSuchDriver "Нет такого драйвера: %s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "Автоматически определить TV драйвер не удалось.\n"
#define MSGTR_TV_UnknownColorOption "Указана неизвестная опция цвета (%d)!\n"
#define MSGTR_TV_NoTeletext "Нет телетекста"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848: Сбой %s ioctl. Ошибка: %s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848: Неверная величина аудио потока. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848: Невозможно открыть устройство bktr. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848: Невозможно открыть устройство tuner. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848: Невозможно открыть устройство dsp. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848: Сбой конфигурирования dsp. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848: Ошибка чтения аудио данных. Ошибка: %s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848: Сбой mmap. Ошибка: %s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848: Ошибка выделения памяти для кадрового буфера. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848: Невозможно установить ширину изображения. Ошибка: %s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848: Невозможно остановить захват. Ошибка: %s\n"
#define MSGTR_TV_TTSupportedLanguages "Поддерживаемые языки Телетекста:\n"
#define MSGTR_TV_TTSelectedLanguage "Выбран язык Телетекста по умолчанию: %s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "Сканер каналов недоступен, если нет тюнера\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "Невозможно присоединить данный вход к видео декодеру. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "Невозможно присоединить данный вход к аудио декодеру. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow: Невозможно выбрать видео формат. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow: Невозможно выбрать аудио формат. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow: Невозможно получить IMediaControl интерфейс. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow: Невозможно запустить граф! Ошибка:0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow: Устройство #%d не найдено\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow: Невозможно получить название устройства #%d\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow: Используется устройство #%d: %s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow: Невозможно получить частоту напрямую. Будет использоваться таблица каналов ОС.\n"
//following phrase will be printed near the selected audio/video input
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow: Невозможно загрузить таблицу частот из kstvtune.ax\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow: Неверный параметр device: %s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow: Неверный индекс device: %d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow: Неверный параметр adevice: %s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow: Неверный индекс adevice: %d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow: Частота дискретизации %d не поддерживается устройством. Устанавливается первая доступная.\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow: Подстройка яркости/цветности/насыщенности/контраста не поддерживается устройством\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow: Изменение ширины/высоты видео не поддерживается устройством.\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow: Выбор источника захвата не поддерживается устройством\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow: Невозможно разобрать структуру аудио формата.\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow: Невозможно разобрать структуру видео формата.\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow: Невозможно установить аудио режим %d. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow: Неподдерживаемый медиа тип передан в %s\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow: Невозможно найти ближайший канал в системной таблице частот\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow: Невозможно переключиться на ближайший канал в системой таблице частот. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow: Невозможно подсоединить к VideoPort коннектору ни один фильтр из графа. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow: Невозможно выстроить цепочку для видео в графе. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow: Невозможно выстроить цепочку для аудио в графе. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow: Невозможно выстроить VBI цепочку в графе. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow: Ошибка инициализации Directshow графа.\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow: Невозможно найти устройство захвата видео\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow: Невозможно найти устройство захвата аудио\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow: Невозможно получить текущий медиа формат (Ошибка:0x%x). Предполагается совпадающий с запрошенным.\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "Похоже, что строка уже пропущена в url_escape %c%c%c\n"
