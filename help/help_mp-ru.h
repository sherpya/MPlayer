// Translated by: Nick Kurshev <nickols_k@mail.ru>,
// Dmitry Baryshkov <mitya@school.ioffe.ru>
// Reworked by: Sergey Kazorin <translation-team@basealt.ru>

// Synced with help_mp-en.h: r38423


// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<номер трека> играть дорожку (S)VCD (Super Video CD) (указывайте\n                 устройство, не монтируйте его)\n"
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
" -vo <drv[:dev]> выбор драйвера и устройства видеовывода (список см. с «-vo help»)\n"\
" -ao <drv[:dev]> выбор драйвера и устройства аудиовывода (список см. с «-ao help»)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -alang/-slang   выбрать язык аудио/субтитров DVD (двухбуквенный код страны)\n"\
" -ss <время>     переместиться на заданную (секунды или ЧЧ:ММ:СС) позицию\n"\
" -nosound        без звука\n"\
" -fs             параметры полноэкранного проигрывания (или -vm, -zoom, подробности\n                 на man-странице)\n"\
" -x <x> -y <y>   установить разрешение дисплея (использовать с -vm или -zoom)\n"\
" -sub <файл>     указать файл субтитров (см. также -subfps, -subdelay)\n"\
" -playlist <файл> указать список воспроизведения (плейлист)\n"\
" -vid x -aid y   параметры выбора видео (x) и аудио (y) потока для воспроизведения\n"\
" -fps x -srate y параметры изменения кадровой частоты видео (x, кадр/сек) и аудио (y, Гц)\n"\
" -pp <quality>   разрешить фильтр постобработки (подробности на man-странице)\n"\
" -framedrop      включить отбрасывание кадров (для медленных машин)\n"\
"\n"\
"Основные кнопки: (полный список на man-странице, см. также input.conf)\n"\
" <- или ->       перемещение вперёд/назад на 10 секунд\n"\
" up или down     перемещение вперёд/назад на 1 минуту\n"\
" pgup or pgdown  перемещение вперёд/назад на 10 минут\n"\
" < или >         перемещение вперёд/назад в списке воспроизведения\n"\
" p или ПРОБЕЛ    приостановить воспроизведение (любая клавиша - продолжить)\n"\
" q или ESC       остановить воспроизведение и выйти\n"\
" + или -         регулировать задержку звука по +/- 0.1 секунде\n"\
" o               цикличный перебор OSD режимов:  нет / навигация / навигация+таймер\n"\
" * или /         прибавить или убавить PCM громкость\n"\
" z или x         регулировать задержку субтитров по +/- 0.1 секунды\n"\
" r или t         регулировка вертикальной позиции субтитров, см. также -vf expand\n"\
"\n"\
"ПОДРОБНУЮ ИНФОРМАЦИЮ О ДОПОЛНИТЕЛЬНЫХ КЛЮЧАХ И ПАРАМЕТРАХ СМ. В ДОКУМЕНТАЦИИ!\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\nВыход…\n"
#define MSGTR_ExitingHow "\nВыход… (%s)\n"
#define MSGTR_Exit_quit "Выход"
#define MSGTR_Exit_eof "Конец файла"
#define MSGTR_Exit_error "Неустранимая ошибка"
#define MSGTR_IntBySignal "\nMPlayer прерван сигналом %d в модуле: %s \n"
#define MSGTR_NoHomeDir "Не удалось найти ДОМАШНИЙ каталог\n"
#define MSGTR_GetpathProblem "проблемы в get_path(\"config\")\n"
#define MSGTR_CreatingCfgFile "Создание файла конфигурации: %s\n"
#define MSGTR_CantLoadFont "Не удалось загрузить растровый шрифт: %s\n"
#define MSGTR_CantLoadSub "Не удалось загрузить субтитры: %s\n"
#define MSGTR_DumpSelectedStreamMissing "дамп: НЕУСТРАНИМАЯ ОШИБКА: Выбранный поток потерян!\n"
#define MSGTR_CantOpenDumpfile "Не удалось открыть файл дампа!!!\n"
#define MSGTR_CoreDumped "Создан дамп ядра ;)\n"
#define MSGTR_DumpBytesWrittenPercent "дамп: %"PRIu64" байт записано (~%.1f%%)\r"
#define MSGTR_DumpBytesWritten "дамп: %"PRIu64" байт записано\r"
#define MSGTR_DumpBytesWrittenTo "дамп: %"PRIu64" байт записано на «%s».\n"
#define MSGTR_FPSnotspecified "В заголовке не указаны (или недопустимые) кадры/сек! Используйте параметр -fps!\n"
#define MSGTR_TryForceAudioFmtStr "Попытка принудительного использования семейства аудиокодеков %s…\n"
#define MSGTR_CantFindAudioCodec "Не удалось найти кодек для аудиоформата 0x%X!\n"
#define MSGTR_TryForceVideoFmtStr "Попытка принудительного использования семейство видеокодеков %s…\n"
#define MSGTR_CantFindVideoCodec "Не удалось найти кодек для выбранного -vo и видеоформата 0x%X!\n"
#define MSGTR_CannotInitVO "НЕУСТРАНИМАЯ ОШИБКА: Не удалось инициализировать видеодрайвер!\n"
#define MSGTR_CannotInitAO "Не удалось открыть/инициализировать аудиоустройство -> без звука.\n"
#define MSGTR_StartPlaying "Начало воспроизведения…\n"

#define MSGTR_SystemTooSlow "\n\n"\
"      *************************************************************************\n"\
"      **** Система слишком МЕДЛЕННАЯ для воспроизведения этих медиаданных! ****\n"\
"      *************************************************************************\n"\
"Возможные причины, проблемы, обходы: \n"\
"- Наиболее частая: плохой/сырой _аудио_ драйвер\n"\
"  - Попробуйте -ao sdl или используйте эмуляцию OSS на ALSA.\n"\
"  - Поэкспериментируйте с различными значениями -autosync, начиная с 30.\n"\
"- Медленный видео вывод\n"\
"  - Попробуйте другие -vo driver (список: -vo help) или используйте с -framedrop!\n"\
"- Медленный процессор\n"\
"  - Не пытайтесь воспроизводить большие DVD/DivX на медленных процессорах!\n" \
"    Попробуйте использовать некоторые параметры lavdopts, например:\n-vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all.\n"\
"- Битый файл\n"\
"  - Попробуйте различные комбинации параметров: -nobps -ni -forceidx -mc 0\n"\
"- Медленный носитель (смонтированные NFS/SMB, DVD, VCD и тому подобное)\n"\
"  - Используйте -cache 8192.\n"\
"- Используете ли Вы -cache для проигрывания нечередующихся [non-interleaved]\nAVI файлов?\n"\
"  - Используйте -nocache.\n"\
"Ознакомьтесь с DOCS/HTML/ru/video.html для получения рекомендаций по \nподстройке/ускорению.\n"\
"Если ничего не помогает, ознакомьтесь с DOCS/HTML/ru/bugreports.html!\n\n"

#define MSGTR_NoGui "MPlayer был скомпилирован БЕЗ поддержки GUI!\n"
#define MSGTR_GuiNeedsX "Для GUI MPlayer необходимо наличие X11!\n"
#define MSGTR_Playing "\nВоспроизведение %s.\n"
#define MSGTR_NoSound "Аудио: без звука\n"
#define MSGTR_FPSforced "Кадры/сек принудительно установлены в значение %5.3f (время кадра: %5.3f).\n"
#define MSGTR_AvailableVideoOutputDrivers "Доступные драйвера вывода видео:\n"
#define MSGTR_AvailableAudioOutputDrivers "Доступные драйвера вывода звука:\n"
#define MSGTR_AvailableAudioCodecs "Доступные аудиокодеки:\n"
#define MSGTR_AvailableVideoCodecs "Доступные видеокодеки:\n"
#define MSGTR_AvailableAudioFm "Доступные (интегрированные при компиляции) семейства/драйверы аудиокодеков:\n"
#define MSGTR_AvailableVideoFm "Доступные (интегрированные при компиляции) семейства/драйверы видеокодеков:\n"
#define MSGTR_AvailableFsType "Доступные режимы изменения полноэкранного слоя:\n"
#define MSGTR_CannotReadVideoProperties "Видео: Не удалось прочитать свойства.\n"
#define MSGTR_NoStreamFound "Поток не найден.\n"
#define MSGTR_ErrorInitializingVODevice "Ошибка при открытии/инициализации выбранного устройства видеовывода (-vo).\n"
#define MSGTR_ForcedVideoCodec "Принудительно задан видеокодек: %s\n"
#define MSGTR_ForcedAudioCodec "Принудительно задан аудиокодек: %s\n"
#define MSGTR_Video_NoVideo "Видео: нет видео\n"
#define MSGTR_NotInitializeVOPorVO "\nНЕУСТРАНИМАЯ ОШИБКА: Не удалось инициализировать видеофильтры (-vf) или видеовывод (-vo).\n"
#define MSGTR_Paused "=== ПРИОСТАНОВЛЕНО ===" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\nНе удалось загрузить список воспроизведения (плейлист) %s.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- Сбой MPlayer по причине вызова «Недопустимой инструкции».\n"\
"  Это могло произойти в результате ошибки нового кода динамического\nопределения типа ЦП…\n"\
"  Ознакомьтесь с DOCS/HTML/ru/bugreports.html.\n"
#define MSGTR_Exit_SIGILL \
"- Сбой MPlayer по причине вызова «Недопустимой инструкции».\n"\
"  Это часто происходит при запуске программы не на том ЦП, для которого она\n"\
"  была скомпилирована/оптимизирована.\n"\
"  Данную информацию необходимо проверить!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- Сбой MPlayer по причине неверного использования ЦП/сопроцессора/ОЗУ.\n"\
"  Перекомпилируйте MPlayer с параметром --enable-debug, выполните обратную\n трассировку и дизассемблирование«gdb»\n"\
"  Более подробная информация доступна на\nDOCS/HTML/ru/bugreports_what.html#bugreports_crash\n"
#define MSGTR_Exit_SIGCRASH \
"- Сбой MPlayer. Это неожиданная ситуация.\n"\
"  Она могла возникнуть в результате ошибки в коде MPlayer _либо_ в коде\n"\
"  драйвера с вашей стороны, _либо_ в коде текущей версии gcc. Если имеются\n"\
"  основания полагать, что ошибка происходит по вине MPlayer, ознакомьтесь\n"\
"  с DOCS/HTML/ru/bugreports.html и следуйте инструкциям этого документа.\n"\
"  Помощь не может быть и не будет оказана без предоставления информация\nо возможной ошибке.\n"
#define MSGTR_LoadingConfig "Загрузка конфигурационного файла «%s»\n"
#define MSGTR_LoadingProtocolProfile "Загрузка профиля для протокола «%s»\n"
#define MSGTR_LoadingExtensionProfile "Загрузка профиля для расширения «%s»\n"
#define MSGTR_AddedSubtitleFile "СУБТИТРЫ: добавлен файл субтитров (%d): %s\n"
#define MSGTR_RemovedSubtitleFile "СУБТИТРЫ: Удалён файл субтитров (%d): %s\n"
#define MSGTR_RTCDeviceNotOpenable "Не удалось открыть %s: %s (пользователь должен обладать правом чтения этого\nфайла).\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "Ошибка инициализации Linux RTC в ioctl (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "Попробуйте добавить \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" \nв загрузочные сценарии системы.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "Ошибка инициализации Linux RTC в ioctl (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "ПРЕДУПРЕЖДЕНИЕ: getch2_init вызван дважды!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "Не удалось открыть видеофильтр libmenu с этим корневым меню %s.\n"
#define MSGTR_AudioFilterChainPreinitError "Ошибка в цепочке pre-init аудиофильтра!\n"
#define MSGTR_LinuxRTCReadError "Ошибка чтения Linux RTC: %s\n"
#define MSGTR_SoftsleepUnderflow "Предупреждение! Недопустимо низкое значение программной задержки!\n"
#define MSGTR_MasterQuit "Параметр -udp-slave: выход в связи с прекращение работы мастера\n"
#define MSGTR_InvalidIP "Параметр -udp-ip: недопустимый IP-адрес\n"
#define MSGTR_Forking "Создание дочернего процесса…\n"
#define MSGTR_Forked "Дочерний процесс создан…\n"
#define MSGTR_CouldntStartGdb "Не удалось запустить gdb\n"
#define MSGTR_CouldntFork "Не удалось создать дочерний процесс\n"
#define MSGTR_FilenameTooLong "Слишком длинное имя файла, не удалось загрузить файл или конфигурационные файлы,\nпривязанные к каталогу\n"
#define MSGTR_AudioDeviceStuck "Аудио устройство зависло!\n"
#define MSGTR_AudioOutputTruncated "Вывод звука усечен в конце.\n"
#define MSGTR_ASSCannotAddVideoFilter "ASS: не удалось добавить видео фильтр\n"
#define MSGTR_PtsAfterFiltersMissing "pts после фильтров ОТСУТСТВУЕТ\n"
#define MSGTR_CommandLine "Командная строка:"
#define MSGTR_MenuInitFailed "Сбой инициализации меню.\n"
#define MSGTR_InvalidSwitchRatio "Недопустимое соотношение сторон «%f», параметр не был установлен.\n"

// --- edit decision lists
#define MSGTR_EdlOutOfMem "Не удалось выделить достаточный объём памяти для хранения данных EDL.\n"
#define MSGTR_EdlOutOfMemFile "Не удалось выделить достаточный объём памяти для хранения данных EDL имени\nфайла [%s].\n"
#define MSGTR_EdlRecordsNo "Чтение %d действий EDL.\n"
#define MSGTR_EdlQueueEmpty "Нет действий EDL, которые необходимо исполнить (очередь пуста).\n"
#define MSGTR_EdlCantOpenForWrite "Не удалось открыть файл EDL [%s] для записи.\n"
#define MSGTR_EdlNOsh_video "Нельзя использовать EDL без видео, отключение.\n"
#define MSGTR_EdlNOValidLine "Неверная строка EDL: %s\n"
#define MSGTR_EdlBadlyFormattedLine "Неверно отформатированная строка EDL [%d] пропущена.\n"
#define MSGTR_EdlBadLineOverlap "Последняя позиция останова была [%f]; следующая стартовая "\
"позиция [%f]. Записи должны располагаться в хронологическом порядке,\nперекрытие недопустимо. Пропущено.\n"
#define MSGTR_EdlBadLineBadStop "Время останова должно располагаться после времени старта.\n"
#define MSGTR_EdloutBadStop "Отмена пропуска EDL, последний запуск > остановки\n"
#define MSGTR_EdloutStartSkip "Начало пропуска EDL, нажмите «i» ещё раз для завершения блока.\n"
#define MSGTR_EdloutEndSkip "Конец пропуска EDL, строка записана.\n"

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
#define MSGTR_OSDDeinterlace "Деинтерлейс: %s"
#define MSGTR_OSDCapturing "Захват: %s"
#define MSGTR_OSDCapturingFailure "Сбой захвата"

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
#define MSGTR_SubSourceDemux "внедрённые"

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
#define MSGTR_UsingPass3ControlFile "Используется следующий файл для контроля 3-го прохода: %s\n"
#define MSGTR_MissingFilename "\nПропущено имя файла.\n\n"
#define MSGTR_CannotOpenFile_Device "Не удалось открыть файл/устройство.\n"
#define MSGTR_CannotOpenDemuxer "Не удалось открыть демультиплексор.\n"
#define MSGTR_NoAudioEncoderSelected "\nКодировщик аудио (-oac) не выбран.\nВыберите кодировщик (см. -oac help) или используйте -nosound.\n"
#define MSGTR_NoVideoEncoderSelected "\nКодировщик видео (-ovc) не выбран. Выберите кодировщик (см. -ovc help).\n"
#define MSGTR_CannotOpenOutputFile "Не удалось открыть файл вывода «%s».\n"
#define MSGTR_EncoderOpenFailed "Не удалось открыть кодировщик.\n"
#define MSGTR_MencoderWrongFormatAVI "\nПРЕДУПРЕЖДЕНИЕ: ВЫХОДНОЙ ФОРМАТ ФАЙЛА _AVI_. См. -of help.\n"
#define MSGTR_MencoderWrongFormatMPG "\nПРЕДУПРЕЖДЕНИЕ: ВЫХОДНОЙ ФОРМАТ ФАЙЛА _MPEG_. См. -of help.\n"
#define MSGTR_MissingOutputFilename "Не указан выходной файл, используйте опцию -o."
#define MSGTR_ForcingOutputFourcc "Выходной fourcc используется принудительно в %x [%.4s]\n"
#define MSGTR_ForcingOutputAudiofmtTag "Принудительно используется тег выходного аудиоформата в 0x%x.\n"
#define MSGTR_DuplicateFrames "\nПовторяющихся кадров: %d!\n"
#define MSGTR_SkipFrame "\nПропуск кадра!\n"
#define MSGTR_ResolutionDoesntMatch "\nНовый видеофайл обладает иным разрешением или цветовым пространством, чем\nпредыдущий.\n"
#define MSGTR_FrameCopyFileMismatch "\nВсе видеофайлы должны обладать одинаковым fps, разрешением и кодеком для\n-ovc copy.\n"
#define MSGTR_AudioCopyFileMismatch "\nВсе файлы должны быть закодированы одинаковым аудиокодеком и форматом для\n-oac copy.\n"
#define MSGTR_NoAudioFileMismatch "\nНедопустимо смешивать файлы, содержащие только видео, с аудио- и видеофайлами.\nПопробуйте -nosound.\n"
#define MSGTR_NoSpeedWithFrameCopy "ПРЕДУПРЕЖДЕНИЕ: не гарантируется корректная работа -speed с -oac copy!\n"\
"Кодирование может быть испорчено!\n"
#define MSGTR_ErrorWritingFile "%s: Ошибка при записи файла.\n"
#define MSGTR_FlushingVideoFrames "\nСброс видео кадров.\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "Фильтры не настроены! Пустой файл?\n"
#define MSGTR_RecommendedVideoBitrate "Рекомендуемый битрейт для %s CD: %d\n"
#define MSGTR_VideoStreamResult "\nПоток видео: %8.3f кбит/с  (%d Б/с)  размер: %"PRIu64" байт(а/ов)  %5.3f сек.  %d кадр(а/ов)\n"
#define MSGTR_AudioStreamResult "\nПоток аудио: %8.3f кбит/с  (%d Б/с)  размер: %"PRIu64" байт(а/ов)  %5.3f сек.\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL SKIP: Начало: %.2f  Конец: %.2f   Текущая: V: %.2f  A: %.2f     \r"
#define MSGTR_OpenedStream "успешно: формат: %d  данные: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "видеокодек: копирование кадров (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "аудиокодек: копирование кадров (формат=%x цепочек=%d скорость=%d битов=%d Б/с=%d\nобразец=%d)\n"
#define MSGTR_MP3AudioSelected "Выбрано MP3 аудио\n"
#define MSGTR_SettingAudioDelay "Установка задержки аудио в %5.3fс\n"
#define MSGTR_SettingVideoDelay "Установка задержки видео в %5.3fс.\n"
#define MSGTR_LimitingAudioPreload "Предварительная загрузка аудио ограничена до 0.4с\n"
#define MSGTR_IncreasingAudioDensity "Увеличение плотности аудио до 4\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Принудительная установка предварительной загрузки аудио в 0, максимальной\nкоррекции pts в 0\n"
#define MSGTR_LameVersion "Версия LAME %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Ошибка: Заданный битрейт вне допустимого значения для данной предустановки.\n"\
"\n"\
"При использовании этого режима необходимо указать значение между \"8\" и \"320\"\n"\
"\n"\
"Для получения дополнительной информации используйте: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Ошибка: Не был указан допустимый профиль и/или параметры предустановки.\n"\
"\n"\
"Доступные профили:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (Режим ABR) - Подразумевается режим ABR. Для использования\n"\
"                       просто укажите битрейт. Например:\n"\
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
"Дополнительная информация доступна с помощью: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Ключи предустановок проработаны с целью обеспечения максимально возможного\nкачества.\n"\
"\n"\
"Проработка и настройка их производилась преимущественно с помощью тщательных\n"\
"тестов двойного прослушивания, позволивших обеспечить необходимую проверку\nи достичь намеченных целей.\n"\
"\n"\
"Ключи предустановок постоянно обновляются, чтобы соответствовать последним\n"\
"разработкам с целью предоставления пользователям самого лучшего возможного\n"\
"на сегодняшний день качества при использовании LAME.\n"\
"\n"\
"Эти предустановки используются следующим образом:\n"\
"\n"\
"   Для VBR режимов (обеспечивают лучшее качество):\n"\
"\n"\
"     \"preset=standard\" Эта предустановка обычно удовлетворяет большинству\n"\
"                             людей при прослушивании самой разной музыки,\n"\
"			      путём обеспечения достаточно высокого уровня\nкачества\n"\
"\n"\
"     \"preset=extreme\" При обладании достаточно тонким слухом и соответствующим\n"\
"                             оборудованием эта предустановка в целом способна\n"\
"                             обеспечивать более хорошее качество, нежели режим\n"\
"                             \"standard\".\n"\
"\n"\
"   Для CBR 320kbps (максимально возможное качество, которое способны\n                             обеспечить ключи предустановок):\n"\
"\n"\
"     \"preset=insane\"  Эта предустановка может показаться чрезмерной для\n"\
"                             большинства людей и ситуаций, но при необходимости\n"\
"                             обеспечить максимально возможное качество\n"\
"                             без каких-либо ограничений по размеру файла - это\n"\
"			      самый подходящий вариант.\n"\
"\n"\
"   Для ABR режимов (высокое качество для заданного битрейта, но не такое\n                             высокое, как VBR):\n"\
"\n"\
"     \"preset=<kbps>\"  Использование этой предустановки обеспечивает хорошее\n"\
"                             качество для заданного битрейта. На основе\n"\
"                             указанного битрейта предустановка определяет\n"\
"                             оптимальные параметры для каждой конкретной.\n"\
"                             ситуации. Однако этот абсолютно рабочий подход\n"\
"                             не способен обеспечить такую же гибкость как VBR\n"\
"                             и такой же уровень качества как VBR на высоких\n                             битрейтах.\n"\
"\n"\
"Также доступны следующие параметры для соответствующих профилей:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (Режим ABR) - Подразумевается режим ABR, для использования которого\n"\
"                       достаточно просто указать битрейт. Например: параметр\n"\
"                       \"preset=185\" активирует эту предустановку, где 185\n"\
"                       будет использоваться в качестве среднего значения\n                              кбит/сек.\n"\
"\n"\
"   \"fast\" - Активирует новый быстрый VBR для конкретного профиля.\n"\
"            Недостатком этого ключа является несколько повышенный по\n"\
"            сравнению с нормальным режимом битрейт, а также иногда\n"\
"            относительно низкое качество.\n"\
"Предупреждение: В текущей версии быстрые предустановки могут привести к слишком\n"\
"                высокому битрейту по сравнению с обычными предустановками.\n"\
"\n"\
"   \"cbr\"  - При использовании режима ABR (см. выше) с таким «кратным»"\
"            битрейтом как 80, 96, 112, 128, 160, 192, 224, 256, 320,\n"\
"            Можно использовать параметр «cbr» для принудительного кодирования\n"\
"            в режиме CBR вместо использования стандартного режима abr. ABR\n"\
"            обеспечивает более высокое качество, но CBR может оказаться полезен\n"\
"            в таких ситуациях как потоковая передача mp3 по сети интернет.\n"\
"\n"\
"    Например:\n"\
"\n"\
"     \"-lameopts fast:preset=standard  \"\n"\
" или \"-lameopts  cbr:preset=192       \"\n"\
" или \"-lameopts      preset=172       \"\n"\
" или \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"Для режима ABR доступно несколько псевдонимов:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"Не удалось задать опции LAME, проверьте битрейт/частоту_дискретизации,\n"\
"на некоторых очень малых значения битрейта (<32) возникает необходимость\n"\
"в меньших значениях частоты\nдискретизации (например, -srate 8000).\n"\
"Если ничего не поможет, попробуйте задать предустановку."
#define MSGTR_ConfigFileError "ошибка в конфигурационном файле"
#define MSGTR_ErrorParsingCommandLine "ошибка разбора командной строки"
#define MSGTR_VideoStreamRequired "Наличие потока видео обязательно!\n"
#define MSGTR_ForcingInputFPS "входные кадры/сек будут заменены на %5.3f\n"
#define MSGTR_DemuxerDoesntSupportNosound "Этот демультиплексор пока не поддерживает -nosound.\n"
#define MSGTR_MemAllocFailed "не удалось выделить память\n"
#define MSGTR_NoMatchingFilter "Не удалось найти соответствующий фильтр/формат_аудиовывода!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, возможно, некорректно работает компилятор C?\n"
#define MSGTR_NoLavcAudioCodecName "Аудио LAVC, пропущено имя кодека!\n"
#define MSGTR_LavcAudioCodecNotFound "Аудио LAVC, не удалось найти кодировщик для кодека %s\n"
#define MSGTR_CouldntAllocateLavcContext "Аудио LAVC, не удалось разместить контекст!\n"
#define MSGTR_CouldntOpenCodec "Не удалось открыть кодек %s, br=%d\n"
#define MSGTR_CantCopyAudioFormat "Аудиоформат 0x%x несовместим с «-oac copy», попробуйте «-oac pcm»\nили используйте «-fafmttag» для его переопределения.\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     метод кодирования с переменным битрейтом\n"\
"                0: cbr (постоянный битрейт)\n"\
"                1: mt (VBR алгоритм Марка Тейлора [Mark Taylor])\n"\
"                2: rh (VBR алгоритм Роберта Гиджимана [Robert Hegemann]\n                   -- по умолчанию)\n"\
"                3: abr (усреднённый битрейт)\n"\
"                4: mtrh (VBR алгоритм Марка Тейлора и Роберта Гиджимана)\n"\
"\n"\
" abr           усреднённый битрейт\n"\
"\n"\
" cbr           постоянный битрейт\n"\
"               Также принудительно использует режим кодирования CBR на\n               последующих режимах предустановок ABR\n"\
"\n"\
" br=<0-1024>   укажите битрейт в кбит (только CBR и ABR)\n"\
"\n"\
" q=<0-9>       качество (0-наилучшее, 9-наихудшее) (только для VBR)\n"\
"\n"\
" aq=<0-9>      качество алгоритма (0-наилучшее/медленно, 9-наихудшее/быстро)\n"\
"\n"\
" ratio=<1-100> коэффициент сжатия\n"\
"\n"\
" vol=<0-10>    установите усиление на входе аудио\n"\
"\n"\
" mode=<0-3>    (по умолчанию: автоопределение)\n"\
"                0: стерео\n"\
"                1: объединённое стерео [joint-stereo]\n"\
"                2: двухканальный\n"\
"                3: моно\n"\
"\n"\
" padding=<0-2>\n"\
"                0: нет\n"\
"                1: полный\n"\
"                2: регулируемый\n"\
"\n"\
" fast          Переключение на быстрое кодирование на последующих режимах\n"\
"               предустановок VBR; качество похуже и завышенные битрейты.\n"\
"\n"\
" preset=<value> Обеспечивает установку максимально возможного качества.\n"\
"                 medium: кодирование VBR, хорошее качество\n"\
"                 (диапазон битрейта 150-180 кбит/с)\n"\
"                 standard: кодирование VBR, высокое качество\n"\
"                 (диапазон битрейта 170-210 кбит/с)\n"\
"                 extreme: кодирование VBR, очень высокое качество\n"\
"                 (диапазон битрейта 200-240 кбит/с)\n"\
"                 insane:  кодирование CBR, наилучшее предустановленное качество\n"\
"                 (битрейт 320 кбит/с)\n"\
"                 <8-320>: кодирование ABR со средним заданным битрейтом.\n\n"

// codec-cfg.c
#define MSGTR_DuplicateFourcc "повторяющийся FourCC"
#define MSGTR_TooManyFourccs "слишком много FourCC/форматов…"
#define MSGTR_ParseError "ошибка разбора синтаксиса"
#define MSGTR_ParseErrorFIDNotNumber "ошибка разбора синтаксиса (ID формата не является числом?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "ошибка разбора синтаксиса (псевдоним ID формата не является числом?)"
#define MSGTR_DuplicateFID "дублируется ID формата"
#define MSGTR_TooManyOut "слишком много выходных форматов…"
#define MSGTR_InvalidCodecName "\nнедопустимое имя кодека «%s»!\n"
#define MSGTR_CodecLacksFourcc "\nу кодека «%s» отсутствует FourCC/формат!\n"
#define MSGTR_CodecLacksDriver "\nу кодека «%s» отсутствует драйвер!\n"
#define MSGTR_CodecNeedsDLL "\nкодек «%s» требует наличия «dll»!\n"
#define MSGTR_CodecNeedsOutfmt "\nкодек «%s» требует наличия «outfmt»!\n"
#define MSGTR_CantAllocateComment "Не удалось выделить память для комментария. "
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "Не удалось выделить память для строки: %s\n"
#define MSGTR_CantReallocCodecsp "Не удалось выполнить realloc для «*codecsp»: %s\n"
#define MSGTR_CodecNameNotUnique "Имя кодека «%s» не уникально."
#define MSGTR_CantStrdupName "Не удалось выполнить strdup -> 'name': %s\n"
#define MSGTR_CantStrdupInfo "Не удалось выполнить strdup -> 'info': %s\n"
#define MSGTR_CantStrdupDriver "Не удалось выполнить strdup -> 'driver': %s\n"
#define MSGTR_CantStrdupDLL "Не удалось выполнить strdup -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "Аудиокодеков: %d и видеокодеков: %d\n"
#define MSGTR_CodecDefinitionIncorrect "Кодек определён некорректно."
#define MSGTR_OutdatedCodecsConf "Это устаревший codecs.conf, несовместимый с данным релизом MPlayer!"

// fifo.c

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "«--» завершает перечисление параметров, но имя файла в командной строке\nне указано."
#define MSGTR_TheLoopOptionMustBeAnInteger "Параметр loop должен быть целочисленным: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "Неизвестный параметр командной строки: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "Ошибка разбора параметра командной строки: -%s\n"
#define MSGTR_InvalidPlayEntry "Неверный элемент воспроизведения %s\n"
#define MSGTR_NotAnMEncoderOption "-%s не является параметром MEncoder\n"
#define MSGTR_NoFileGiven "Файл не указан\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "Найден слишком старый слот сохранения из lvl %d: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "Параметр %s не может использоваться в конфигурационном файле.\n"
#define MSGTR_InvalidCmdlineOption "Параметр %s не может использоваться в командной строке.\n"
#define MSGTR_InvalidSuboption "Ошибка: у параметра «%s» отсутствует подпараметр «%s».\n"
#define MSGTR_MissingSuboptionParameter "Ошибка: у подпараметра «%s» параметра «%s» должно быть значение!\n"
#define MSGTR_MissingOptionParameter "Ошибка: у параметра «%s» должно быть значение!\n"
#define MSGTR_OptionListHeader "\n Имя                  Тип             Минимум    Максимум Общий   CL    Конф\n\n"
#define MSGTR_TotalOptions "\nВсего параметров: %d\n"
#define MSGTR_ProfileInclusionTooDeep "ПРЕДУПРЕЖДЕНИЕ: Слишком глубокое вложение профиля.\n"
#define MSGTR_NoProfileDefined "Ни один профиль не был определён.\n"
#define MSGTR_AvailableProfiles "Доступные профили:\n"
#define MSGTR_UnknownProfile "Неизвестный профиль «%s».\n"
#define MSGTR_Profile "Профиль %s: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n Имя                  Тип             Минимум    Максимум\n\n"
#define MSGTR_TotalProperties "\nВсего: %d свойств\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "ВНИМАНИЕ: попытка использования DLL кодеков без установки переменной \n         окружения DYLD_BIND_AT_LAUNCH скорее всего приведет к сбою.\n"


// ================================ GUI ================================

#define MSGTR_GUI_235To1 "2.35:1"
#define MSGTR_GUI_AboutMPlayer "О MPlayer"
#define MSGTR_GUI_Add "Добавить"
#define MSGTR_GUI_AspectRatio "Соотношение сторон"
#define MSGTR_GUI_Audio "Аудио"
#define MSGTR_GUI_AudioDelay "Задержка аудио"
#define MSGTR_GUI_AudioDriverConfiguration "Конфигурация аудио драйвера"
#define MSGTR_GUI_AudioTrack "Загрузить внешний аудио файл"
#define MSGTR_GUI_AudioTracks "Аудио дорожка"
#define MSGTR_GUI_AvailableDrivers "Доступные драйверы:"
#define MSGTR_GUI_AvailableSkins "Оболочки"
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
#define MSGTR_GUI_CD "CD"
#define MSGTR_GUI_Center "Центральный"
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
#define MSGTR_GUI_CodecFamilyAudio "Семейство аудиокодеков"
#define MSGTR_GUI_CodecFamilyVideo "Семейство видеокодеков"
#define MSGTR_GUI_CodecsAndLibraries "Кодеки и сторонние библиотеки"
#define MSGTR_GUI_Coefficient "Коэффициент"
#define MSGTR_GUI_ConfigFileError "ошибка в конфигурационном файле\n"
#define MSGTR_GUI_Configure "Настроить"
#define MSGTR_GUI_ConfigureDriver "Настроить драйвер"
#define MSGTR_GUI_Contrast "Контраст"
#define MSGTR_GUI_Contributors "Разработчики кода и документации"
#define MSGTR_GUI_Cp874 "Тайская (CP874)"
#define MSGTR_GUI_Cp932 "Японская (CP932)"
#define MSGTR_GUI_Cp936 "Упрощённая китайская (CP936)"
#define MSGTR_GUI_Cp949 "Корейская (CP949)"
#define MSGTR_GUI_Cp950 "Традиционная китайская (CP950)"
#define MSGTR_GUI_Cp1250 "Центрально-европейская (CP1250)"
#define MSGTR_GUI_Cp1251 "Кириллица (CP1251)"
#define MSGTR_GUI_Cp1252 "Западно-европейская (CP1252)"
#define MSGTR_GUI_Cp1253 "Греческая (CP1253)"
#define MSGTR_GUI_Cp1254 "Турецкая (CP1254)"
#define MSGTR_GUI_Cp1255 "Иврит (CP1255)"
#define MSGTR_GUI_Cp1256 "Арабская (CP1256)"
#define MSGTR_GUI_Cp1257 "Балтийская (CP1257)"
#define MSGTR_GUI_Cp1258 "Вьетнамская (CP1258)"
#define MSGTR_GUI_CpBIG5 "Традиционная Китайская (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Западно-европейская (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Центрально-европейская (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Южно-европейская (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Северо-европейская (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Кириллица (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Арабская (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Греческая (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "Иврит (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Турецкая (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_10 "Нордическая (ISO-8859-10)"
#define MSGTR_GUI_CpISO8859_11 "Тайская (ISO-8859-11)"
#define MSGTR_GUI_CpISO8859_13 "Балтийская (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Кельтская (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Западно-европейская, расширенная (ISO-8859-15)"
#define MSGTR_GUI_CpISO8859_16 "Юго-восточная европейская (ISO-8859-16)"
#define MSGTR_GUI_CpKOI8_R "Русская (KOI8-R)"
#define MSGTR_GUI_CpKOI8_RU "Русская/Украинская/Белорусская (KOI8-RU)"
#define MSGTR_GUI_CpKOI8_U "Украинская (KOI8-U)"
#define MSGTR_GUI_CpShiftJis "Японская (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Юникод"
#define MSGTR_GUI_CpUTF8 "UTF-8"
#define MSGTR_GUI_dB "дБ"
#define MSGTR_GUI_DecimalMark "."
#define MSGTR_GUI_Decrease "Уменьшить"
#define MSGTR_GUI__Default_ "(По умолчанию)"
#define MSGTR_GUI_DefaultSetting "драйвер по умолчанию"
#define MSGTR_GUI_Delay "Задержка"
#define MSGTR_GUI_Demuxer "Демультиплексор"
#define MSGTR_GUI_Demuxers_Codecs "Кодеки и демультиплексор"
#define MSGTR_GUI_Device "Устройство"
#define MSGTR_GUI_DeviceCDROM "CD-ROM"
#define MSGTR_GUI_DeviceDVD "DVD"
#define MSGTR_GUI_Directory "Путь"
#define MSGTR_GUI_DirectoryTree "Дерево каталогов"
#define MSGTR_GUI_DropSubtitle "Убрать субтитры…"
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_Ellipsis "…"
#define MSGTR_GUI_EnableAssSubtitle "SSA/ASS рендеринг субтитров"
#define MSGTR_GUI_EnableAutomaticAVSync "Автосинхронизация вкл/выкл"
#define MSGTR_GUI_EnableCache "Кэш вкл/выкл"
#define MSGTR_GUI_EnableDigitalTV "Включить приём цифрового телевидения"
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
#define MSGTR_GUI_ErrorFatal "Неустранимая ошибка!"
#define MSGTR_GUI_File "Воспроизвести файл"
#define MSGTR_GUI_Files "Файлы"
#define MSGTR_GUI_FilterAudioAll "Все звуковые файлы"
#define MSGTR_GUI_FilterAudioDolbyPCM "Dolby Digital / PCM (*.ac3, *.pcm)"
#define MSGTR_GUI_FilterAudioMatroska "Аудио файлы Matroska (*.mka)"
#define MSGTR_GUI_FilterAudioMPEG "Аудио файлы MPEG (*.aac, *.f4a, *.m4a, *.mp2, *.mp3, *.mpga)"
#define MSGTR_GUI_FilterAudioOgg "Аудио файлы Ogg (*.oga, *.ogg, *.spx)"
#define MSGTR_GUI_FilterAudioVideo "Популярные аудио и видео файлы"
#define MSGTR_GUI_FilterAudioWav "Аудио файлы WAV (*.wav)"
#define MSGTR_GUI_FilterAudioWma "Windows Media audio (*.wma)"
#define MSGTR_GUI_FilterFileAll "Все файлы"
#define MSGTR_GUI_FilterFileMP2TS "Транспортные потоки MPEG-2"
#define MSGTR_GUI_FilterFileMP3 "Файлы MP3"
#define MSGTR_GUI_FilterFileMP4 "Файлы MP4"
#define MSGTR_GUI_FilterFileMPEG "Файлы MPEG"
#define MSGTR_GUI_FilterFilePlaylist "Плейлисты"
#define MSGTR_GUI_FilterFileWav "Файлы WAV"
#define MSGTR_GUI_FilterFontAll "Все шрифты"
#define MSGTR_GUI_FilterFontBitmap "Растровые шрифты (*.desc)"
#define MSGTR_GUI_FilterFontTTF "Шрифты TrueType (*.ttf)"
#define MSGTR_GUI_FilterFontType1 "Шрифты Type1 (*.pfb)"
#define MSGTR_GUI_FilterImageCD "Индексная карта для образа CD"
#define MSGTR_GUI_FilterImageCue "Индексная карта для образа CD/VCD/SVCD"
#define MSGTR_GUI_FilterImageDVD "Образ/копия DVD"
#define MSGTR_GUI_FilterImageVCD "Индексная карта для образа VCD/SVCD"
#define MSGTR_GUI_FilterMediumMatroska "Matroska media"
#define MSGTR_GUI_FilterMediumOgg "Ogg media"
#define MSGTR_GUI_FilterMediumQuickTime "QuickTime media"
#define MSGTR_GUI_FilterMediumRealNetworks "RealNetworks media"
#define MSGTR_GUI_FilterMediumWindows "Windows media"
#define MSGTR_GUI_FilterSubtitleAll "Все субтитры"
#define MSGTR_GUI_FilterSubtitleUTF8 "Файлы субтитров в кодировке UTF-8 (*.utf, *.utf-8, *.utf8)"
#define MSGTR_GUI_FilterVideoAll "Все видео файлы"
#define MSGTR_GUI_FilterVideoAvi "Файлы AVI"
#define MSGTR_GUI_FilterVideoDivX "Файлы DivX"
#define MSGTR_GUI_FilterVideoFlash "Flash Video"
#define MSGTR_GUI_Flip "Отразить изображение вверх ногами"
#define MSGTR_GUI_Font "Шрифт"
#define MSGTR_GUI_FrameRate "Кадр/сек"
#define MSGTR_GUI_Frequency0 "31.25 Гц"
#define MSGTR_GUI_Frequency1 "62.50 Гц"
#define MSGTR_GUI_Frequency2 "125 Гц"
#define MSGTR_GUI_Frequency3 "250 Гц"
#define MSGTR_GUI_Frequency4 "500 Гц"
#define MSGTR_GUI_Frequency5 "1 кГц"
#define MSGTR_GUI_Frequency6 "2 кГц"
#define MSGTR_GUI_Frequency7 "4 кГц"
#define MSGTR_GUI_Frequency8 "8 кГц"
#define MSGTR_GUI_Frequency9 "16 кГц"
#define MSGTR_GUI_FrontLeft "Передний левый"
#define MSGTR_GUI_FrontRight "Передний правый"
#define MSGTR_GUI_HideVideoWindow "Скрывать окно видео после воспроизведения"
#define MSGTR_GUI_Hue "Оттенок"
#define MSGTR_GUI_Image "Изображение"
#define MSGTR_GUI_Increase "Увеличить"
#define MSGTR_GUI_Information "Информация"
#define MSGTR_GUI_Lavc "Использовать LAVC (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Максимальное использование запаса процессорного времени"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Minimize "Свернуть"
#define MSGTR_GUI_Miscellaneous "Разное"
#define MSGTR_GUI_Mixer "Микшер"
#define MSGTR_GUI_MixerChannel "Канал микшера"
#define MSGTR_GUI_MSG_AddingVideoFilter "[GUI] Добавляю видеофильтр: %s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "Глубина цвета дисплея слишком мала!\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "Перетаскивание: Ничего не возвращено!\n"
#define MSGTR_GUI_MSG_DragAndDropOwner "Странный владелец выделеного… QT?"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Проигрывание не-MPEG файлов на устройстве DXR3/H+ без перекодирования\nневозможно. Включите lavc в конфигурации DXR3/H+.\n"
#define MSGTR_GUI_MSG_IconError "Не найден значок «%s» (размер %d) или не поддерживается его формат.\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "[GUI] Загрузка субтитров: %s\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "Недостаточно памяти для буфера прорисовки.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Недостаточно памяти для буфера прорисовки.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Файл не загружен."
#define MSGTR_GUI_MSG_NoMediaOpened "Носитель медиаданных не открыт."
#define MSGTR_GUI_MSG_NoSkinInstalled "Не установлена оболочка «по умолчанию». Оболочка обеспечивает наличие удобных средств для отображения информации и управления проигрывателем во время воспроизведения. В случае отсутствия оболочки обеспечивается лишь упрощённое управление из меню, доступ к которому осуществляется путём щелчка правой клавишей мыши по окну воспроизведения видео.\n\nУстановите оболочку, выберите её в окне просмотра оболочек, либо поместите строку «gui_skin=Noskin» в файл конфигурации для предотвращения вывода этого сообщения в случае отсутствия необходимости в установке оболочки.\n"
#define MSGTR_GUI_MSG_NotAFile0 "Это не похоже на файл…\n"
#define MSGTR_GUI_MSG_NotAFile1 "«%s» не похоже на файл!\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Необходимо перезапустить воспроизведение, чтобы изменения некоторых параметров\nвступили в силу!"
#define MSGTR_GUI_MSG_RemoteDisplay "Удалённый дисплей, отключение общей памяти X.\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "Удаление субтитров.\n"
#define MSGTR_GUI_MSG_RemovingVideoFilter "Удаление видео фильтра «%s».\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "ошибка преобразования 24-бит в 32-бит (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "Растровый файл %s не найден\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "Ошибка чтения PNG в %s\n"
#define MSGTR_GUI_MSG_SkinCfgError "Ошибка обработки конфигурационного файла с оболочкой «%s»\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Оболочка не найдена (%s).\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "Выбранная оболочка «%s» не найдена, попытка загрузки оболочки «по умолчанию»…\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "Глубина битовой матрицы в 16 бит и меньше не поддерживается (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorDefault "Значение по умолчанию (%d) выходит за пределы диапазона\n"
#define MSGTR_GUI_MSG_SkinErrorItem "Данный элемент не поддерживается «%s».\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "Ошибка в файле конфигурации оболочки на строке %d: %s"
#define MSGTR_GUI_MSG_SkinErrorNumphases "Количество фаз не должно равняться нулю.\n"
#define MSGTR_GUI_MSG_SkinErrorSection "Для «%s» не указан раздел.\n"
#define MSGTR_GUI_MSG_SkinErrorWindow "Для «%s» не указано окно.\n"
#define MSGTR_GUI_MSG_SkinFileNotFound "Не найден файл оболочки «%s».\n"
#define MSGTR_GUI_MSG_SkinFileNotReadable "Не удалось прочитать файл оболочки %s.\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "Не удалось найти файл шрифта.\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "Не удалось найти файл образов шрифта.\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "Ну удалось найти шрифт «%s»\n"
#define MSGTR_GUI_MSG_SkinLegacy "Устаревший параметр файла конфигурации оболочки на строке %d: «%s»,\nвместо него необходимо использовать «%s».\n"
#define MSGTR_GUI_MSG_SkinMemoryError "Недостаточно памяти\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "Определено слишком много шрифтов.\n"
#define MSGTR_GUI_MSG_SkinTooManyItems "Определено слишком много элементов.\n"
#define MSGTR_GUI_MSG_SkinUnknownItem "Неизвестный элемент «%s»\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "Неизвестное сообщение: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownName "Неизвестное имя «%s»\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "Неизвестный параметр «%s»\n"
#define MSGTR_GUI_MSG_TooManyWindows "Открыто слишком много окон.\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "Не удалось сохранить параметр «%s».\n"
#define MSGTR_GUI_MSG_VideoOutError "Не удалось найти совместимый с GUI драйвер видео вывода.\n"
#define MSGTR_GUI_MSG_X11Error "Произошла ошибка X11!\n"
#define MSGTR_GUI_MSG_XShapeError "Система не поддерживает расширение XShape.\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "Ошибка расширения разделяемой памяти оконной системы X\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "Система не поддерживает расширение разделяемой памяти оконной системы X.\n"
#define MSGTR_GUI_Mute "Приглушить"
#define MSGTR_GUI_NetworkStreaming "Сетевые потоки…"
#define MSGTR_GUI_Next "След. поток"
#define MSGTR_GUI_NoChannelName "Отсутствует имя канала"
#define MSGTR_GUI_NoChapter "Нет раздела"
#define MSGTR_GUI_None "Нет"
#define MSGTR_GUI__none_ "(нет)"
#define MSGTR_GUI_NonInterleavedParser "Использовать обработчик нечередующегося AVI"
#define MSGTR_GUI_NormalizeSound "Нормализовать звук"
#define MSGTR_GUI_Ok "Да"
#define MSGTR_GUI_Open "Открыть…"
#define MSGTR_GUI_Original "Исходный"
#define MSGTR_GUI_OsdLevel "Уровень OSD"
#define MSGTR_GUI_OsdLevel0 "Только субтитры"
#define MSGTR_GUI_OsdLevel1 "Громкость и навигация"
#define MSGTR_GUI_OsdLevel2 "Громкость, навигация, таймер и проценты"
#define MSGTR_GUI_OsdLevel3 "Громкость, навигация, таймер, проценты и общее время"
#define MSGTR_GUI_OSD_Subtitles "Субтитры и OSD"
#define MSGTR_GUI_Outline "Контур"
#define MSGTR_GUI_PanAndScan "Усечение сторон"
#define MSGTR_GUI_Pause "Пауза"
#define MSGTR_GUI_Play "Играть"
#define MSGTR_GUI_Playback "Воспроизведение"
#define MSGTR_GUI_Playlist "Список воспроизведения"
#define MSGTR_GUI_PlaylistSupport "Разрешить загрузку списков воспроизведения"
#define MSGTR_GUI_Position "Позиция"
#define MSGTR_GUI_PostProcessing "Постобработка"
#define MSGTR_GUI_Preferences "Параметры"
#define MSGTR_GUI_Previous "Предыдущий"
#define MSGTR_GUI_Quit "Выход"
#define MSGTR_GUI_QuitAfterPlaying "Завершить работу после воспроизведения"
#define MSGTR_GUI_RearLeft "Задний левый"
#define MSGTR_GUI_RearRight "Задний правый"
#define MSGTR_GUI_Remove "Удалить"
#define MSGTR_GUI_ReplayGain "ReplayGain"
#define MSGTR_GUI_ReplayGainAdjustment "с дополнительной коррекцией"
#define MSGTR_GUI_Rotation "Вращение"
#define MSGTR_GUI_Rotation180 "180°"
#define MSGTR_GUI_Rotation90CCW "90° против часовой стрелки"
#define MSGTR_GUI_Rotation90CW "90° по часовой стрелке"
#define MSGTR_GUI_Saturation "Насыщенность"
#define MSGTR_GUI_SaveWindowPositions "Сохранять позицию окна"
#define MSGTR_GUI_ScaleMode "Режим масштабирования"
#define MSGTR_GUI_ScaleMovieDiagonal "Пропорционально диагонали фильма"
#define MSGTR_GUI_ScaleMovieHeight "Пропорционально высоте фильма"
#define MSGTR_GUI_ScaleMovieWidth "Пропорционально ширине фильма"
#define MSGTR_GUI_ScaleNo "Не масштабировать"
#define MSGTR_GUI_SeekBackward10min "Перемещение назад на 10 минут"
#define MSGTR_GUI_SeekBackward10sec "Перемещение назад на 10 секунд"
#define MSGTR_GUI_SeekBackward1min "Перемещение назад на 1 минуту"
#define MSGTR_GUI_SeekForward10min "Перемещение вперёд на 10 минут"
#define MSGTR_GUI_SeekForward10sec "Перемещение вперёд на 10 секунд"
#define MSGTR_GUI_SeekForward1min "Перемещение вперёд на 1 минуту"
#define MSGTR_GUI_SeekingInBrokenMedia "Если требуется, перестроить индексную таблицу"
#define MSGTR_GUI_SelectAudioFile "Выбор внешнего аудио канала…"
#define MSGTR_GUI_SelectedFiles "Выбранные файлы"
#define MSGTR_GUI_SelectFile "Выбрать файл…"
#define MSGTR_GUI_SelectFont "Выбор шрифта…"
#define MSGTR_GUI_SelectImage "Выбор изображения"
#define MSGTR_GUI_SelectSubtitle "Выбрать субтитры…"
#define MSGTR_GUI_SizeDouble "Двойной размер"
#define MSGTR_GUI_SizeFullscreen "Полный экран"
#define MSGTR_GUI_SizeHalf "Половинный размер"
#define MSGTR_GUI_SizeNormal "Нормальный размер"
#define MSGTR_GUI_SizeOSD "Масштаб OSD"
#define MSGTR_GUI_SizeSubtitles "Масштаб текста"
#define MSGTR_GUI_SkinBrowser "Просмотр оболочек"
#define MSGTR_GUI_Skins "Оболочки"
#define MSGTR_GUI_Sponsored "Разработка графического интерфейса спонсирована UHU Linux"
#define MSGTR_GUI_StartFullscreen "Запускать в полноэкранном режиме"
#define MSGTR_GUI_Stop "Остановка"
#define MSGTR_GUI_Subtitle "Субтитры"
#define MSGTR_GUI_SubtitleAddMargins "Использовать края"
#define MSGTR_GUI_SubtitleAllowOverlap "Изменить перекрывание субтитров"
#define MSGTR_GUI_SubtitleAutomaticLoad "Выключить автозагрузку субтитров"
#define MSGTR_GUI_SubtitleConvertMpsub "Конвертировать данные субтитры в специальный формат субтитров MPlayer (mpsub)"
#define MSGTR_GUI_SubtitleConvertSrt "Конвертировать данные субтитры в формат SubRip (srt)"
#define MSGTR_GUI_Subtitles "Субтитры"
#define MSGTR_GUI_SyncValue "Автосинхронизация"
#define MSGTR_GUI_TitleN "Запись %d"
#define MSGTR_GUI_TitleNN "Запись %2d"
#define MSGTR_GUI_Titles "Записи"
#define MSGTR_GUI_Top "Верх"
#define MSGTR_GUI_TrackN "Дорожка %d"
#define MSGTR_GUI_Translations "Переводы"
#define MSGTR_GUI_TurnOffXScreenSaver "Останавливать XScreenSaver"
#define MSGTR_GUI_TV "ТВ"
#define MSGTR_GUI_Unknown "Неизвестный"
#define MSGTR_GUI_URL "URL-адрес"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Видео"
#define MSGTR_GUI_VideoEncoder "Видео кодировщик"
#define MSGTR_GUI_VideoTracks "Видео дорожки"
#define MSGTR_GUI_Volume MSGTR_Volume
#define MSGTR_GUI_Warning "Предупреждение!"

#define MSGTR_GUI_WIN32_AddFile "Добавить файл"
#define MSGTR_GUI_WIN32_AddURL "Добавить URL-адрес"
#define MSGTR_GUI_WIN32_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_WIN32_Close "Закрыть"
#define MSGTR_GUI_WIN32_ConfigFileError MSGTR_GUI_ConfigFileError
#define MSGTR_GUI_WIN32_DebugConsole "Отладочная консоль"
#define MSGTR_GUI_WIN32_Defaults "По умолчанию"
#define MSGTR_GUI_WIN32_DirectoryList "Список каталогов"
#define MSGTR_GUI_WIN32_DisplayInVideoWindow "Отображать в окне видео (только DirectX или Direct3D)"
#define MSGTR_GUI_WIN32_Down "Вниз"
#define MSGTR_GUI_WIN32_Load "Загрузить"
#define MSGTR_GUI_WIN32_OnlineHelp "Интерактивная справка"
#define MSGTR_GUI_WIN32_Priority "Приоритет"
#define MSGTR_GUI_WIN32_PriorityAboveNormal "выше обычного"
#define MSGTR_GUI_WIN32_PriorityBelowNormal "ниже обычного"
#define MSGTR_GUI_WIN32_PriorityHigh "высокий"
#define MSGTR_GUI_WIN32_PriorityLow "низкий"
#define MSGTR_GUI_WIN32_PriorityNormal "обычный"
#define MSGTR_GUI_WIN32_Save "Сохранить"
#define MSGTR_GUI_WIN32_SavePlaylist "Сохранить плейлист"
#define MSGTR_GUI_WIN32_SeekBackwards "Переход назад"
#define MSGTR_GUI_WIN32_SeekForwards "Переход вперёд"
#define MSGTR_GUI_WIN32_SelectPlaylist "Выбрать плейлист"
#define MSGTR_GUI_WIN32_SelectTitleChapter "Выбрать ролик/раздел"
#define MSGTR_GUI_WIN32_ShowHide "Показать/Скрыть"
#define MSGTR_GUI_WIN32_SubtitleOnOff "Показывать субтитры вкл/выкл"
#define MSGTR_GUI_WIN32_Up "Вверх"

// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "Выбранное устройство видеовывода несовместимо с этим кодеком.\n"\
                "Попробуйте добавить фильтр scale к вашему списку фильтров.\n"\
                "Например, -vf spp,scale вместо -vf spp.\n"
#define MSGTR_VO_GenericError "Произошла следующая ошибка"
#define MSGTR_VO_OutputDirectory "Каталог вывода"
#define MSGTR_VO_UnableToAccess "Не удалось получить доступ"
#define MSGTR_VO_ExistsButNoDirectory "уже существует, но не является каталогом."
#define MSGTR_VO_DirExistsButNotWritable "Каталог вывода уже существует, но не доступен для записи."
#define MSGTR_VO_CantCreateDirectory "Не удалось создать каталог вывода."
#define MSGTR_VO_CantCreateFile "Не удалось создать выходной файл."
#define MSGTR_VO_DirectoryCreateSuccess "Каталог вывода успешно создан."
#define MSGTR_VO_ValueOutOfRange "значение вне допустимого диапазона"

// aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] Предупреждение: Не найдено подходящего разрешения!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] Ошибка: Не найден размер, умещающийся в это разрешение!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "Сбой New_Face. Возможно, неверен путь к шрифту.\nПредоставьте следующий файл шрифта (~/.mplayer/subfont.ttf).\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "Сбой New_Memory_Face.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "шрифт субтитров: сбой load_sub_face.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "шрифт субтитров: сбой prepare_charset.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "Не удалось подготовить шрифт субтитров.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "Не удалось подготовить шрифт OSD.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "Не удалось сгенерировать таблицы.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "Сбой FT_Done_FreeType.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_FontconfigNoMatch "Не удалось выбрать шрифт с помощью fontconfig.\nВыполняется выбор шрифта без использования fontconfig…\n"

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

// vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] Поддерживается только 16 бит на пиксел!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] Визуальный ID %lx.\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] Не удалось открыть /dev/3dfx.\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] Ошибка: %d.\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] Не удалось отобразить области памяти 3dfx: %p,%p,%d.\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] Инициализировано: %p.\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] Неизвестное подустройство: %s.\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\nПодпараметры vo_aa библиотеки aalib:\n"
#define MSGTR_VO_AA_AdditionalOptions "Дополнительные параметры, предоставляемые vo_aa:\n" \
"  help        вывести это сообщение\n" \
"  osdcolor    установить цвет OSD\n  subcolor    установить цвет субтитров\n" \
"        параметры цвета:\n           0 : нормальный\n" \
"           1 : тусклый\n           2 : яркий\n           3 : жирный шрифт\n" \
"           4 : инверсный\n           5 : специальный\n\n\n"

// vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] Не удалось загрузить новую палитру SPU!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] Не удалось задать режим воспроизведения!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] Не удалось задать режим подкартинки!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] Не удалось считать стандарт ТВ!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] Автовыбор стандарта ТВ по частоте кадров: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] Не удалось установить стандарт ТВ!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] Установка для NTSC.\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] Установка для PAL/SECAM.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] Установка пропорции 4:3.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] Установка пропорции 16:9.\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] недостаточно памяти\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] Не удалось назначить ключевой цвет!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] Не удалось назначить точный ключевой цвет,\nбудет использовано ближайшее приближение (0x%lx).\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] Деинициализация.\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] Не удалось восстановить стандарт ТВ!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] Включение предварительной буферизации.\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] Используется новый механизм синхронизации.\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] Используется оверлей.\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] Ошибка: Для использования оверлея программа должна быть скомпилирована\nс установленными библиотеками/заголовками X11.\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] Установка стандарта ТВ: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "авторегулировка к частоте кадров фильма (PAL/PAL-60)"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "авторегулировка к частоте кадров фильма (PAL/NTSC)"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "Используйте текущий стандарт."
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "Передан неизвестный стандарт. Используйте текущий стандарт."
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] Ошибка открытия %s для записи, попытка использовать /dev/em8300\nвместо него.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] Ошибка открытия %s для записи, попытка использовать /dev/em8300_mv\nвместо него.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] Также ошибка открытия /dev/em8300 для записи!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] Также ошибка открытия /dev/em8300_mv для записи!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] Открыто: %s.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] Ошибка открытия %s для записи, попытка использовать /dev/em8300_sp\nвместо него.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] Также ошибка открытия /dev/em8300_sp для записи!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] Не удалось открыть дисплей при настройке обходного пути для использования\nоверлея!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] Не удалось инициализировать X11!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] Не удалось установить атрибут оверлея.\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] Не удалось установить экран оверлея!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] Не удалось активировать оверлей!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] Не удалось изменить размер окна оверлея!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] Не удалось установить bcs оверлея!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] Не удалось получить значения Y-смещений оверлея!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] Не удалось получить значения X-смещений оверлея!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] Не удалось получить коррекцию на масштабирование оверлея по X!\nВыход.\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] смещение по Y: %d.\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] смещение по X: %d.\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] коррекция по X: %d.\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] Не удалось установить сигнал mix!\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Прогрессивный JPEG включен."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Прогрессивный JPEG выключен."
#define MSGTR_VO_JPEG_BaselineJPEG "Базовый JPEG включен."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Базовый JPEG выключен."

// vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): размер изменён в %dx%d.\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] деинициализация!\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] ошибка в mga_vid_config ioctl (неверная версия mga_vid.o?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] Не удалось получить значения яркости из модуля ядра!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] Не удалось установить значения яркости из модуля ядра!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] Ширина/высота экрана неизвестны!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] недопустимый выходной формат %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] Версия текущего драйвера mga_vid несовместима с этой версией MPlayer!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] Не удалось открыть: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] Разрешение источника, по крайней мере в одном измерении, больше чем\n1023x1023. Перемасштабируйте программно или используйте -lavdopts lowres=1\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] несоответствие версий драйвера mga_vid ядра (%u) и MPlayer (%u)\n"

// vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] Неизвестное подустройство: %s.\n"

// vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] Предупреждение: уровень сжатия установлен в 0, сжатие отключено!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] Информация: Используйте -vo png:z=<n> для установки уровня сжатия\nот 0 до 9.\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] Информация: (0 = нет сжатия, 1 = быстрое, худшее - 9 лучшее,\nсамое медленное сжатие)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] Ошибка открытия «%s» для записи!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] Ошибка в create_png.\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "Включён режим ASCII."
#define MSGTR_VO_PNM_RawMode "Включён режим без обработки."
#define MSGTR_VO_PNM_PPMType "Будут записаны файлы PPM."
#define MSGTR_VO_PNM_PGMType "Будут записаны файлы PGM."
#define MSGTR_VO_PNM_PGMYUVType "Будут записаны файлы PGMYUV."

// vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] Не удалось получить какой-либо приемлемый режим SDL для вывода.\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] Неподдерживаемый формат изображения (0x%X).\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] Информация: используйте -vm или -zoom для переключения\nв лучшее разрешение.\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] Не удалось установить видеорежим: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] Не удалось создать оверлей YUV: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] Не удалось создать поверхность RGB: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] Используется преобразование глубины/цветового пространства,\nэто замедлит работу (%ibpp -> %ibpp).\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] Неподдерживаемый формат изображения в draw_slice, свяжитесь\nс разработчиками MPlayer!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Сбой blit: %s.\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] Сбой инициализации SDL: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] Используется драйвер: %s.\n"

// vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] Принудительно используемый vid_mode %d (%s) недоступен.\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] Принудительно используемый vid_mode %d (%s) слишком мал.\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode: %d, %dx%d %dbpp.\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Сбой Vga_setmode(%d).\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] Видеорежим линейный и для передачи изображения может быть\nиспользован memcpy.\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] Видеорежим использует аппаратное ускорение и может\nиспользоваться put_image.\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] В случае успешной работы хотелось бы получить обратную связь.\n[VO_SVGA] (отправьте лог, созданный с помощью `mplayer test.avi -v -v -v -v &> svga.log`).\nСпасибо!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] Видеорежим содержит %d стр.\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] Центровка изображения. Начинается в (%d,%d)\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] Использование VIDIX. w=%i h=%i  mw=%i mh=%i\n"

// vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Шаг %d(%d) x %d => %d.\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] Сбой очистки экрана шагом AGP.\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Сбой blit.\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] Невстроенный формат оверлея необходимо преобразовать.\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] Неподдерживаемый входной формат 0x%x.\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] Сбой установки оверлея.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] Сбой включения оверлея.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] Оверлей готов: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] blit текстур готов: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] Сбой выключения оверлея\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] Не удалось открыть %s: %s.\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] Не удалось получить текущую конфигурацию: %s.\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Сбой memmap !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "Нобходимо доработать получение изображения.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] Сбой шага AGP.\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] Сбой установки YUV.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] Сбой шага AGP на проекции Y.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] Сбой шага AGP на проекции U.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] Сбой шага AGP на проекции V.\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] неизвестный формат: 0x%x.\n"

// vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] Не удалось открыть %s: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] Проблема с ioctl FBITGET_FSCREENINFO: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] Проблема с ioctl FBITGET_VSCREENINFO: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] Этот драйвер поддерживает только 3Dfx Banshee, Voodoo3 и Voodoo 5.\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp вывод не поддерживается.\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] Не удалось отобразить области памяти: %s.\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp вывод не поддерживается\n(Это неожиданная ситуация).\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Имеются проблемы с control().\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] Недостаточно памяти для воспроизведения этого фильма.\nПопробуйте меньшее разрешение.\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] Экран %dx%d и %d bpp, входные данные %dx%d и %d bpp, режим %dx%d.\n"

// vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] Неизвестное подустройство: %s.\n"

// vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] Возникла неустранимая ошибка! Продолжение работы невозможно.\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] неизвестное подустройство: '%s'.\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] Слишком мало видеопамяти для данного режима:\n[VO_VESA] Необходимо: %08lX имеется в наличии: %08lX.\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] Необходимо указать возможности монитора. Частота обновления\nне изменена.\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] Ограниченные возможности монитора не соответствуют режиму.\nЧастота обновления не изменена.\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] Обнаружена внутренняя неустранимая ошибка: init вызван до preinit.\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] Опция -flip не поддерживается.\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] Возможная причина: VBE2 BIOS не найден.\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] Найден VESA VBE BIOS, версия %x.%x, ревизия: %x.\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] Видеопамять: %u Кб.\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] Возможности VESA: %s %s %s %s %s.\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! Ниже будет выведена информация OEM !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] Ниже должно быть выведено 5 строк информации OEM.\nВ противном случае, vm86 неисправно.\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] Информация OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] Ревизия OEM: %x.\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] Поставщик OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] Наименование продукта OEM: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] Ревизия продукта OEM: %s.\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] Подсказка: Для работы ТВ-выхода Вам необходимо подключить разъём ТВ\n"\
"[VO_VESA] до загрузки системы, так как VESA BIOS инициализируется только во время проверки POST.\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] Используется режим VESA (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] Не удалось инициализировать программное масштабирование.\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] Невозможно использовать DGA. Принудительно используется режим коммутации\nсегментов. :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] Используется DGA (физические ресурсы: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] Невозможно использовать двойную буферизацию: недостаточно видеопамяти.\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] Не удалось найти DGA, а также перемещаемую рамку окна.\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] Принудительно используется DGA. Выход\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] Не удалось найти допустимый адрес окна.\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] Использование режима коммутации сегментов\n(физические ресурсы: %08lXh, %08lXh).\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] Не удалось выделить временный буфер.\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] Режим не поддерживается -- попробуйте -x 640 -zoom.\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] Изображене действительно отображается на ТВ!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] Не удалось инициализировать видео оверлей Linux.\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] Используется видео оверлей: %s.\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] Не удалось инициализировать драйвер VIDIX.\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] Используется VIDIX.\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] Не удалось найти режим для: %ux%u@%u.\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] Инициализация VESA завершена.\n"

// vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] Эта ветка больше не поддерживается.\n[VESA_LVO] Используйте вместо неё -vo vesa:vidix.\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] Не удалось открыть: «%s»\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] Недопустимый выходной формат: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] Версия текущего драйвера fb_vid несовместима с этой версией MPlayer!\n"

// vo_x11.c

// vo_xv.c
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] Разделяемая память не поддерживается\nВозврат к обычному Xv.\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] Xv не поддерживается этой версией/драйвером X11\n[VO_XV] ******** Попробуйте -vo x11  или  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] Ошибка XvQueryAdaptors.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] Недопустимый параметр port, переопределён параметром port 0.\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] Не удалось захватить порт %i.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] Не удалось найти свободный Xvideo порт - возможно другое приложение\n"\
"[VO_XV] его уже использует. Закройте все видеопрограммы и повторите попытку.\n"\
"[VO_XV] Если это не поможет, посмотрите другие (не-xv) драйверы вывода видео\n[VO_XV] с помощью команды «mplayer -vo help».\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] По видимому, текущая видео карта не имеет поддержки Xvideo.\n"\
"[VO_XV] Запустите «xvinfo» для проверки поддержки Xv и ознакомьтесь с\n"\
"[VO_XV] DOCS/HTML/ru/video.html#xv!\n"\
"[VO_XV] Смотрите другие (не-xv) драйверы в выводе «mplayer -vo help».\n"\
"[VO_XV] Попробуйте -vo x11.\n"
#define MSGTR_VO_XV_ImagedimTooHigh "Размеры исходного изображения слишком велики: %ux%u (максимум %ux%u)\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Для режима c чересстрочной развёрткой необходимо, чтобы высота изображения\nделилась на 4."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Ширина изображения должна делиться на 2."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "Не удалось выделить память или дескриптор файла для записи «%s»!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Ошибка записи изображения в вывод!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Неизвестное подустройство: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Используется режим вывода с чересстрочной развёрткой, верхнее поле первое."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Используется режим вывода с чересстрочной развёрткой, нижнее поле первое."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Используется (по умолчанию) режим прогрессивной развёртки кадров."

// vosub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] Не удалось начать воспроизведение: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] Не удалось остановить воспроизведение: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] Чередующийся UV для YUV410P не поддерживается.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] Был вызван фиктивный vidix_draw_slice().\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] Не поддерживается FourCC для этого драйвера VIDIX:\n%x (%s).\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] Видео разрешение (%dx%d) не поддерживается сервером,\nподдерживается: %dx%d-%dx%d.\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] Сервер видео не поддерживает глубину цвета vidix (%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] Драйвер VIDIX не может увеличить изображение (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] Драйвер VIDIX не может уменьшить изображение (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] Не удалось настроить воспроизведение: %s.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] Не удалось найти работающий драйвер VIDIX.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] Не удалось получить функциональную возможность: %s.\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: Не удалось отправить событие EWMH fullscreen!\n"
#define MSGTR_SelectedVideoMode "XF86VM: Выбран видеорежим %dx%d для размера изображения %dx%d.\n"
#define MSGTR_X11Error "Ошибка X11: %s\n"
#define MSGTR_CouldntFindUTF8Locale "Не удалось найти подходящую локаль UTF-8. Клавиши для отправки символов,\n не входящих в состав Latin-1, не распознаны.\n"
#define MSGTR_CouldntOpenDisplay "vo: не удалось открыть экран X11 (%s)!\n"
#define MSGTR_WindowDestroyed "Окно было уничтожено, выход\n"
#define MSGTR_DPMSnotAvailable "DPMS недоступна?\n"
#define MSGTR_DPMSnotEnabled "Не удалось включить DPMS\n"
#define MSGTR_BadAccessXSelectInput "Ошибка X11: BadAccess во время вызова XSelectInput\n"
#define MSGTR_ButtonPressMaskInUse "Ошибка X11: «ButtonPressMask» вероятно маска указанного окна уже использовалось\nдругим приложением (смотри man-страницу XSelectInput)\n"
#define MSGTR_DiscardMouseControl "Ошибка X11: В MPlayer не работает управление мышью (перенастройка)\n"
#define MSGTR_NoXF86VidModeExtension "Расширение XF86VidMode недоступно.\n"
#define MSGTR_ReturningOriginalMode "Возврат к исходному режиму %dx%d\n"
#define MSGTR_CouldntSetColorkey "[xv common] Не удалось установить colorkey!\n"
#define MSGTR_CouldntGetColorkey "[xv common] Не удалось получить colorkey! Вероятно, на выбранном порту Xv\nотсутствовал оверлей.\n"

#define MSGTR_InsertingAfVolume "[Микшер] Аппаратное микширование отсутствует, добавляется фильтр громкости.\n"
#define MSGTR_NoVolume "[Микшер] Контроль громкости недоступен.\n"
#define MSGTR_NoBalance "[Микшер] Управление балансом недоступно.\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "Драйвер видеовывода pgm был заменён на -vo pnm:pgmyuv.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "Драйвер видеовывода md5 был заменён на -vo md5sum.\n"
#define MSGTR_VO_GL2_HasBeenRenamed "Драйвер видеовывода gl2 был переименован в -vo gl_tiled,\nно на самом деле вместо него нужно использовать-vo gl.\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "audio_out: модули alsa9 и alsa1x были удалены, используйте вместо них -ao alsa.\n"
#define MSGTR_AO_NoSuchDriver "Отсутствует аудио драйвер «%.*s»\n"
#define MSGTR_AO_FailedInit "Не удалось инициализировать аудио драйвер «%s»\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup: Не удалось открыть устройство микшера %s: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup: У микшера аудиокарты отсутствует канал «%s»,\nиспользуется канал по умолчанию.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup: Не удалось открыть аудиоустройство %s: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup: Не удалось заблокировать дескриптор файла: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] Не удалось установить аудиоустройство %s в вывод %s,\nпопытка использовать %s…\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup: Не удалось установить аудиоустройство\nв %d-канальный режим.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup: драйвер не поддерживает SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  Текущий аудиодрайвер НЕ поддерживает select()  ***\n Перекомпилируйте MPlayer с #undef HAVE_AUDIO_SELECT в config.h !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS] Неустранимая ошибка:\n*** НЕ МОГУ ПОВТОРНО ОТКРЫТЬ / СБРОСИТЬ АУДИОУСТРОЙСТВО (%s) ***\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] Неизвестный/Неподдерживаемый формат OSS: %x.\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Установлено соединение со звуковым сервером.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Не удалось открыть поток.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Поток открыт.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] размер буфера: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] Не удалось установить значение громкости %d.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] dxr2: %d Гц не поддерживается, попробуйте изменить\nчастоту дискретизации.\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] Выполнить esd_open_sound не удалось: %s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] задержка: [сервер: %0.2fs, сеть: %0.2fs] (подстройка %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] не удалось открыть поток воспроизведения esd: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] Сбой установки микшера DVB аудио: %s\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Гц не поддерживается, попробуйте изменить\nчастоту дискретизации.\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Файл: %s (%s)\nPCM: Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Информация: наиболее быстро дамп создаётся с параметрами\n[AO PCM] Информация: -vc null -vo null -ao pcm:fast. Для записи файлов WAVE\n[AO PCM] Информация: используйте параметры -ao pcm:waveheader (по умолчанию).\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Не удалось открыть %s для записи!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] используется аудиодрайвер %s.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Неподдерживаемый аудиоформат: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] Не удалось инициализировать аудио SDL: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Не удалось открыть аудио: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] управление.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] инициализация: Частота дискретизации: %i Гц Каналы: %s Формат %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] воспроизведение: недопустимое устройство.\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] инициализация: ошибка установки параметров: %s\nНе удалось установить требуемую частоту дискретизации.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] инициализация: AL_RATE не принимается на заданном ресурсе.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] инициализация: ошибка получения параметров: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] инициализация: текущая частота дискретизации %f (требуемая частота %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] инициализация: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] инициализация: Не удалось открыть канал аудио: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] деинициализация: …\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] сброс: …\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] пауза аудио: …\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] возобновление аудио: …\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: Сбой SETINFO.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: сбой записи."
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] Не удалось открыть аудиоустройство %s, %s -> нет звука.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] инициализация аудио: данная карта не поддерживает канал %d,\n%s, частоту дискретизации %d Гц.\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] Недопустимый индекс микшера. Возврат к 0.\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] Ошибка открытия микшера: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] Ошибка присоединения %s к микшеру: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] Ошибка регистрации микшера: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] Ошибка загрузки микшера: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] Невозможно найти простой элемент управления «%s»,%i.\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] Ошибка установки левого канала, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] Ошибка установки правого канала, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] подсказка командной строки -ao alsa:\n"\
"[AO_ALSA] Пример: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   Устанавливает четвертое устройство первой карты.\n\n"\
"[AO_ALSA] Параметры:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     Открывает устройство в неблокирующем режиме.\n"\
"[AO_ALSA]   device=<имя-устройства>\n"\
"[AO_ALSA]     Устанавливает устройство (замените , на . и : на =)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] Количество каналов %d не поддерживается.\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] Ошибка открытия в неблокирующем режиме, попытка открыть в режиме блокирования.\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] Ошибка начала воспроизведения: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] Ошибка установки блокирующего режима: %s.\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] Не удалось получить начальные параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] Не удалось установить тип доступа: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] Формат %s не поддерживается оборудованием, попытка использовать\nзначение по умолчанию.\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] Не удалось установить формат: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] Не удалось установить количество каналов: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] Не удалось отключить изменение частоты дискретизации: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] Не удалось установить частоту дискретизации-2: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] Не удалось установить примерное время буфера: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] Не удалось получить время периода: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] Не удалось установить периоды: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] Не удалось установить hw-параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] Не удалось получить размер буфера: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] Не удалось получить sw-параметры: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] Не удалось получить границу: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] Не удалось установить порог запуска: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] Не удалось установить порог остановки: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] Не удалось установить величину тишины: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] ошибка pcm close: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] Не определён обработчик!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] ошибка pcm prepare: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] ошибка pcm pause: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] ошибка pcm drop: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] ошибка pcm resume: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] Ошибка настройки устройства."
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm в ждущем режиме, попытка продолжения.\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] ошибка записи: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] попытка сбросить звуковую карту.\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] Невозможно получить статус pcm: %s\n"

// ao_plugin.c


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "доступные метки в"
#define MSGTR_AF_LADSPA_WarnNoInputs "ПРЕДУПРЕЖДЕНИЕ! У этого подключаемого модуля LADSPA отсутствуют аудиовходы.\n  Входящий сигнал аудио будет потерян."
#define MSGTR_AF_LADSPA_ErrNoOutputs "У этого подключаемого модуля LADSPA отсутствуют выходы аудио."
#define MSGTR_AF_LADSPA_ErrInOutDiff "Число аудиовходов и аудиовыходов у подключаемого модуля LADSPA отличается."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "не удалось загрузить"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Не удалось найти функцию ladspa_descriptor() в указанном файле библиотеки."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Не удалось найти метку в библиотеке подключаемого модуля."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Не указаны подпараметры."
#define MSGTR_AF_LADSPA_ErrNoLibFile "Не указан файл библиотеки."
#define MSGTR_AF_LADSPA_ErrNoLabel "Не указана метка фильтра."
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Недостаточно параметров указано в командной строке."
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: Входной параметр #%d меньше нижней границы %0.4f.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: Входной параметр #%d больше верхней границы %0.4f.\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "неизвестный формат "


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "Не удалось отрыть устройство джойстика %s: %s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "Ошибка чтения устройства джойстика: %s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "Джойстик: Потеряно %d байт(а/ов) данных\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "Джойстик: предупреждение о событии инициализации,\nпотеряна синхронизация с драйвером.\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "Джойстик: предупреждение о неизвестном типе события %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "Невозможно открыть устройство Apple IR: %s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "Слишком много дескрипторов файлов команд,\nне удалось зарегистрировать дескриптор файла %d.\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "Слишком много дескрипторов файлов клавиш,\nне удалось зарегистрировать дескриптор файла %d.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "Команда %s: параметр %d не является целым числом.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "Команда %s: параметр %d не является числом с плавающей точкой.\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "Команда %s: параметр %d не определён.\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "Неизвестный параметр %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "Команда %s требует не менее %d параметров, пока найдено только %d.\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "Ошибка чтения дескриптора %d файла команд: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "Командный буфер дескриптора файла %d полон: пропуск содержимого.\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "Неверная команда для привязки к клавише %s"
#define MSGTR_INPUT_INPUT_ErrSelect "Ошибка вызова select: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "Ошибка в дескрипторе файла %d клавиш ввода\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "Неустранимая ошибка дескриптора файла %d клавиш ввода\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "Слишком много событий одновременного нажатия клавиш\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "Ошибка в дескрипторе файла %d команд\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "Ошибка чтения конфигурационного файла ввода %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "Неизвестная клавиша «%s»\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "Буфер слишком мал для названия этой клавиши: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "Не найдено команды для клавиши %s"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "Буфер слишком мал для команды %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "Не удалось инициализировать джойстик для ввода\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "Не удалось открыть %s: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "Не удалось инициализировать пульт ДУ Apple Remote.\n"

// lirc.c
#define MSGTR_LIRCopenfailed "Не удалось открыть поддержку LIRC.\nНельзя использовать пульт управления.\n"
#define MSGTR_LIRCcfgerr "Не удалось прочитать файл конфигурации LIRC «%s»!\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c
#define MSGTR_TooManyStreams "Слишком много потоков!"
#define MSGTR_RawMuxerOnlyOneStream "Мультиплексор rawaudio поддерживет только один аудиопоток!\n"
#define MSGTR_IgnoringVideoStream "Видеопоток игнорируется!\n"
#define MSGTR_UnknownStreamType "Предупреждение, неизвестный тип потока: %d\n"
#define MSGTR_WarningLenIsntDivisible "Предупреждение: длина не кратна размеру образца!\n"
#define MSGTR_MuxbufMallocErr "Мультиплексор кадрового буфера не может выделить память (malloc)!\n"
#define MSGTR_MuxbufReallocErr "Мультиплексор кадрового буфера не может перераспределить память (realloc)!\n"
#define MSGTR_WritingHeader "Выполняется запись заголовка…\n"
#define MSGTR_WritingTrailer "Выполняется запись индекса…\n"

// demuxer.c, demux_*.c
#define MSGTR_AudioStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заголовок аудио потока %d переопределён!\n"
#define MSGTR_VideoStreamRedefined "ПРЕДУПРЕЖДЕНИЕ: Заголовок видео потока %d переопределён!\n"
#define MSGTR_TooManyAudioInBuffer "\nСлишком много (%d в %d байтах) аудиопакетов в буфере!\n"
#define MSGTR_TooManyVideoInBuffer "\nСлишком много (%d в %d байтах) видеопакетов в буфере!\n"
#define MSGTR_MaybeNI "Либо плохо скомпонованный поток/файл с чередованием аудио и видео,\n"\
		      "либо файл без чередования, либо сбой кодека?\n" \
                      "Для файлов AVI попробуйте принудительно режим без чередования\nс помощью параметра -ni.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: Обход ошибки CBR-MP3 nBlockAlign заголовка!\n"
#define MSGTR_SwitchToNi "\nОбнаружен плохо скомпонованный AVI-файл с чередованием аудио и видео.\nВыполняется переключение в режим -ni…\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: недопустимый ID аудио потока: %d - игнорируется (без звука)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: неверный ID видео потока: %d - игнорируется (используется значение\nпо умолчанию)\n"
#define MSGTR_ON2AviFormat "Формат ON2 AVI"
#define MSGTR_Detected_XXX_FileFormat "Обнаружен %s формат файла!\n"
#define MSGTR_FormatNotRecognized "======= Формат этого файла не распознан/не поддерживается ==========\n"\
                                  "===== Если это AVI, ASF или поток MPEG, свяжитесь с автором! =======\n"
#define MSGTR_SettingProcessPriority "Установка приоритета процесса: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] формат:%d  fourcc:0x%X  размер:%dx%d  кадр/с:%5.3f  ftime:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Не удалось инициализировать мультиплексор."
#define MSGTR_MissingVideoStream "Видеопоток не найден!\n"
#define MSGTR_MissingAudioStream "Аудиопоток не найден -> без звука\n"
#define MSGTR_MissingVideoStreamBug "Видеопоток отсутствует!? свяжитесь с автором, это может оказаться ошибкой :(\n"

#define MSGTR_DoesntContainSelectedStream "demux: в файле отсутствует выбранный аудио или видеопоток\n"

#define MSGTR_NI_Forced "Используется принудительно"
#define MSGTR_NI_Detected "Обнаружен"
#define MSGTR_NI_Message "%s формат AVI-файла с ЧЕРЕДОВАНИЕМ видео и звука!\n"

#define MSGTR_UsingNINI "Использование повреждённого файла формата AVI без чередования аудио и видео!\n"
#define MSGTR_CouldntDetFNo "Не удалось определить число кадров (для абсолютного перемещения).\n"
#define MSGTR_CantSeekRawAVI "Не удалось переместиться в необработанных потоках AVI!\n(требуется индекс, попробуйте с ключом -idx!)\n"
#define MSGTR_CantSeekFile "Не удалось переместиться в этом файле!\n"

#define MSGTR_MOVcomprhdr "MOV: Для поддержки сжатых заголовков необходим zlib!\n"
#define MSGTR_MOVvariableFourCC "MOV: Предупреждение! Обнаружена переменная FOURCC!?\n"
#define MSGTR_MOVtooManyTrk "MOV: Предупреждение! Слишком много дорожек!"
#define MSGTR_ErrorOpeningOGGDemuxer "Не удалось открыть демультиплексор ogg.\n"
#define MSGTR_CannotOpenAudioStream "Не удалось открыть аудиопоток: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Не удалось открыть поток субтитров: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Не удалось открыть демультиплексор аудио: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Не удалось открыть демультиплексор субтитров: %s\n"
#define MSGTR_TVInputNotSeekable "По ТВ входу перемещаться нельзя! (Перемещение может быть доступно\n для смены каналов ;)\n"
#define MSGTR_DemuxerInfoChanged "Информация демультиплексора %s изменена в %s\n"
#define MSGTR_ClipInfo "Информация о клипе:\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg: обнаружено 30000/1001 кадр/с содержимое NTSC,\nпереключение частоты кадров.\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg: обнаружено 24000/1001 кадр/с содержимое NTSC с построчной развёрткой,\nпереключение частоты кадров.\n"

#define MSGTR_CacheFill "\rЗаполнение кэша: %5.2f%% (%"PRId64" байт(а))   "
#define MSGTR_NoBindFound "Не найдена привязка к клавише «%s».\n"
#define MSGTR_FailedToOpen "Не удалось открыть %s.\n"

#define MSGTR_VideoID "[%s] Найден поток видео, -vid %d\n"
#define MSGTR_AudioID "[%s] Найден поток аудио, -aid %d\n"
#define MSGTR_SubtitleID "[%s] Найден поток субтитров, -sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "НЕУСТРАНИМАЯ ОШИБКА: размер заголовка больше 1 MB (%d)!\nОбратитесь к разработчикам MPlayer и загрузите/отправьте этот файл.\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "Не удалось выделить %d байт(а/ов) для заголовка.\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "EOF при чтении заголовка ASF, повреждённый/неполный файл?\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR, вероятно, будет работать только с libavformat,\nпопробуйте -demuxer 35 при наличии проблем\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "Отсутствует блок данных, следующий за заголовком!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: не найдены аудио или видео заголовки — повреждён файл?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "Недопустимая длина в заголовке ASF!\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "URL-адрес лицензии DRM: %s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "Файл был заполнен DRM-шифрованием, воспроизведение в MPlayer невозможно!\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "** пустой список?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** Предупреждение: Это не расширенный заголовок AVI..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: Построение индекса ODML (%d блоков супериндекса).\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: Обнаружен повреждённый (неполный?) файл.\nИспользуется традиционный индекс.\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "Не удалось прочитать файл индекса %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s не является допустимым файлом индекса MPlayer.\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "Не удалось выделить память для данных индекса из %s.\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "преждевременное завершение индексного файла %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "Загружен индексный файл: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "Создание индекса: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: Создана индексная таблица для %d блоков!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "Не удалось записать файл индекса %s: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "Сохранён индексный файл: %s\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_BadID3v2TagSize "Аудио демультиплексор: неверный размер тега ID3v2: больше потока (%u).\n"
#define MSGTR_MPDEMUX_AUDIO_DamagedAppendedID3v2Tag "Аудио демультиплексор: добавлен повреждённый тег ID3v2.\n"
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "Демультиплексор: неизвестный формат %d.\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "ошибка заполнения_буфера: плохой демультиплексор: не vd, ad или sd.\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] сбой инициализации zlib.\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] сбой распаковки с помощью zlib.\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] сбой распаковки с помощью lzo.\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] Дорожка номер %u зашифрована, а расшифровка еще не \n[mkv] реализована. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] Неизвестный тип шифрования содержимого для дорожки %u. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] Дорожка %u сжата неизвестным/неподдерживаемым \n[mkv] алгоритмом (%u). Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] Дорожка %u сжата zlib, но mplayer скомпилирован \n[mkv] без поддержки сжатия zlib. Пропуск дорожки.\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] ID дорожки %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] ID дорожки %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] Неизвестный/неподдерживаемый CodecID (%s) или отсутствуют/повреждены\n[mkv] данные CodecPrivate (дорожка %u).\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] В дорожке FLAC отсутствуют допустимые заголовки.\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] Неизвестный/неподдерживаемый ID аудио кодека «%s» для дорожки %u или\n[mkv] отсутствуют/ошибочны частные данные кодека.\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] Тип субтитров «%s» не поддерживается.\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] Будет воспроизведена дорожка %u.\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] Видео дорожка не найдена/не требуется.\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] Аудио дорожка не найдена/не требуется.\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] Предупреждение: Не найдена длительность блока дорожки субтитров.\n"

// demux_nuv.c

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "Найден плагин: %s (%s).\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "Закрытие плагина: %s.\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "Ожидание начала воспроизведения «%s» плагином XMMS…\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[МЕНЮ] Не найдены элементы в описании меню.\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[МЕНЮ] ошибка синтаксиса в строке: %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[МЕНЮ] Для описаний меню требуется именной атрибут (строка %d).\n"
#define MSGTR_LIBMENU_BadAttrib "[МЕНЮ] неверный атрибут %s=%s в меню «%s» в строке %d\n"
#define MSGTR_LIBMENU_UnknownMenuType "[МЕНЮ] неизвестный тип меню «%s» в строке %d\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[МЕНЮ] Не удалось открыть конфигурационный файл меню: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[МЕНЮ] Конфигурационный файл слишком велик (> %d Kб)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[МЕНЮ] Конфигурационный файл пуст.\n"
#define MSGTR_LIBMENU_MenuNotFound "[МЕНЮ] Меню %s не найдено.\n"
#define MSGTR_LIBMENU_MenuInitFailed "[МЕНЮ] Меню «%s»: Сбой инициализации.\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[МЕНЮ] Неподдерживаемый выходной формат!!!!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[МЕНЮ] Для описаний элементов меню списка требуется имя (строка %d).\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[МЕНЮ] Для меню списка требуется параметр.\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[МЕНЮ] Ошибка waitpid: %s.\n"
#define MSGTR_LIBMENU_SelectError "[МЕНЮ] Ошибка select.\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[МЕНЮ] Ошибка чтения дочернего дескрипотора файла: %s.\n"
#define MSGTR_LIBMENU_ConsoleRun "[МЕНЮ] Запуск консоли: %s …\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[МЕНЮ] Дочерний процесс уже запущен.\n"
#define MSGTR_LIBMENU_ForkFailed "[МЕНЮ] Сбой создания дочернего процесса !!!\n"
#define MSGTR_LIBMENU_WriteError "[МЕНЮ] ошибка записи\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[МЕНЮ] ошибка открытия каталога: %s\n"
#define MSGTR_LIBMENU_ReallocError "[МЕНЮ] ошибка перераспределения памяти: %s\n"
#define MSGTR_LIBMENU_MallocError "[МЕНЮ] ошибка выделения памяти: %s\n"
#define MSGTR_LIBMENU_ReaddirError "[МЕНЮ] ошибка чтения каталога: %s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[МЕНЮ] Не удалось открыть каталог %s.\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[МЕНЮ] Для описаний подменю требуется атрибут «menu».\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] Недопустимое свойство «%s» в элементе меню предпочтений. (строка %d).\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[МЕНЮ] Для описаний элементов меню предпочтений требуется допустимый \nатрибут «property» или «txt» (строка %d).\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[МЕНЮ] Для меню предпочтений требуется параметр.\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[МЕНЮ] Не удалось найти целевой элемент ????\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[МЕНЮ] Не удалось построить команду: %s.\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[МЕНЮ] Для текстового меню требуется имя текстового файла (файл параметров).\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[МЕНЮ] Не удалось открыть %s.\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[МЕНЮ] Предупреждение, слишком длинная строка. Выполняется разбивка.\n"
#define MSGTR_LIBMENU_ParsedLines "[МЕНЮ] Проанализировано %d строк.\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[МЕНЮ] Неизвестная команда: «%s».\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[МЕНЮ] Не удалось открыть меню: «%s».\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "Не удалось открыть кодек.\n"
#define MSGTR_CantCloseCodec "Не удалось закрыть кодек.\n"

#define MSGTR_MissingDLLcodec "ОШИБКА: Не удалось открыть необходимый кодек DirectShow: %s\n"
#define MSGTR_ACMiniterror "Не удалось загрузить/инициализировать аудиокодек Win32/ACM\n(отсутствует файл DLL?)\n"
#define MSGTR_MissingLAVCcodec "Не удалось найти кодек «%s» в libavcodec…\n"

#define MSGTR_MpegNoSequHdr "MPEG: НЕУСТРАНИМАЯ ОШИБКА: КОНЕЦ ФАЙЛА при поиске последовательности заголовков.\n"
#define MSGTR_CannotReadMpegSequHdr "НЕУСТРАНИМАЯ ОШИБКА: Не удалось прочитать последовательность заголовков.\n"
#define MSGTR_CannotReadMpegSequHdrEx "НЕУСТРАНИМАЯ ОШИБКА: Не удалось прочитать расширение последовательности\nзаголовков.\n"
#define MSGTR_BadMpegSequHdr "MPEG: Неверная последовательность заголовков.\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Неверное расширение последовательности заголовков.\n"

#define MSGTR_ShMemAllocFail "Не удалось зарезервировать разделяемую память.\n"
#define MSGTR_CantAllocAudioBuf "Не удалось зарезервировать выходной аудио буфер.\n"

#define MSGTR_UnknownAudio "Неизвестен/потерян аудио формат -> без звука\n"

#define MSGTR_UsingExternalPP "[PP] Используется внешний фильтр постобработки, max q = %d.\n"
#define MSGTR_UsingCodecPP "[PP] Используется постобработка из кодека, max q = %d.\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "Запрошенное семейство видеокодеков [%s] (vfm=%s) недоступно.\nДобавьте его при компиляции.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "Запрошенное семейство аудиокодеков [%s] (afm=%s) недоступно.\nДобавьте его при компиляции.\n"
#define MSGTR_OpeningVideoDecoder "Открытие декодера видео: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Выбран видеокодек: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Открытие декодера аудио: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Выбран аудиокодек: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "Сбой инициализации декодера видео :(\n"
#define MSGTR_ADecoderInitFailed "Сбой инициализации декодера аудио :(\n"
#define MSGTR_ADecoderPreinitFailed "Сбой предварительной инициализации декодера аудио :(\n"

// ad_dvdpcm.c:
#define MSGTR_SamplesWanted "Для улучшения поддержки необходимы образцы этого формата.\nCвяжитесь с разработчиками.\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] Предупреждение! Размер аудиокадра отличается! read=%d  hdr=%d.\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Не удалось найти подходящее цветовое пространство\nпопытка использовать -vf scale…\n"
#define MSGTR_MovieAspectIsSet "Movie-Aspect - %.2f:1 - выполняется предварительное масштабирование\nдля коррекции соотношения сторон видеоролика.\n"
#define MSGTR_MovieAspectUndefined "Movie-Aspect не определён - предварительное масштабирование не применяется.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "Необходимо обновить/установить пакет бинарных кодеков.\nЗайдите на http://www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] Не удалсь выделить изображение для кодека cinepak.\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] Арифметическое среднее QP: %2.4f, Гармоническое среднее QP: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] Сбой DRI.\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] Не удалось предоставить изображение для кодека.\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] Попытка использовать pixfmt=%d.\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] Выбрано высококачественное кодирование (не в режиме реального времени)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] Используется постоянное значение qscale = %f (VBR).\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] Необработанный вывод с FourCC [%x] не поддерживается!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] Необходимый VfW кодек не определён!!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "Не удалось найти видео фильтр «%s».\n"
#define MSGTR_CouldNotOpenVideoFilter "Не удалось открыть видео фильтр «%s».\n"
#define MSGTR_OpeningVideoFilter "Открытие видео фильтра: "
#define MSGTR_CannotFindColorspace "Не удалось найти подходящее цветовое пространство, даже после вставки «scale» :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] Неверная позиция/ширина/высота - урезанная область вне оригинала!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] Область урезания: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d).\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] Неизвестное имя формата: '%s'.\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] Ошибка разбора параметра.\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "Тип компрессора: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "Подтип компрессора: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "Флаги компрессора: %lu, версия %lu, версия ICM: %lu\n"
#define MSGTR_MPCODECS_Flags "Флаги:"
#define MSGTR_MPCODECS_Quality " качество"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "Полный DR невозможен, попытка использовать вместо него SLICES!\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "Почему был получен NULL??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "%s не поддерживается следующим фильтром/видеовыводом :(\n"


// ================================== LIBASS ====================================

// ass_bitmap.c

// ass.c
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): сбой fopen\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): Отказ загрузки субтитров больше 100M\n"

// ass_cache.c

// ass_fontconfig.c

// ass_render.c

// ass_font.c


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "Не удалось задать частоту дискретизации.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "Не удалось задать длительность буферизации.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "Не удалось задать длительность периода.\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "Неверная конфигурация для данного PCM: нет доступных конфигураций.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "Не удалось определить тип доступа.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "Не удалось определить формат образца.\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "Не удалось определить число каналов - возврат к значениям по умолчанию: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "Не удалось установить аппаратные параметры: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "Не удалось использовать период, равный размеру буфера (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "Не удалось установить программные параметры:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "Ошибка открытия аудио: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaStatusError "Ошибка состояния ALSA: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (длительность не менее %.3f мс)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: ошибка подготовки: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "Ошибка чтения/записи ALSA"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "Не удалось задать число каналов: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "Не удалось включить стерео: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "Не удалось открыть «%s»: %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "неподдерживаемый формат\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "Не удалось задать аудиоформат."
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "Не удалось задать частоту дискретизации: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "Не удалось установить триггер: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "Не удалось получить размер блока!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "Размер блока аудио равен нулю, устанавливается значение %d!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "Размер блока аудио слишком мал, устанавливается значение %d!\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "ошибка записи\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\nВнимание! Конец файла\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "не удалось прочитать предзаголовок\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "Недопустимый размер заголовка, прекращение операции.\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "Не удалось прочитать данные заголовка.\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "не удалось прочитать packet_len.\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "Недопустимый размер пакета RTSP, прекращение операции.\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "Не удалось прочитать данные команды.\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "объект заголовка\n"
#define MSGTR_MPDEMUX_MMST_DataObject "объект данных\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "файловый объект, длина пакета = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "потоковый объект, ID потока: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "Слишком много ID, поток пропущен."
#define MSGTR_MPDEMUX_MMST_UnknownObject "неизвестный объект\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "Не удалось прочитать данные медиа.\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "пропущена подпись\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "Задача выполнена. Благодарим за скачивание медиафайла с содержанием проприетарных\nи запатентованных технологий.\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "неизвестная команда %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "ошибка get_media_packet: %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "Соединение установлено\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "Размер stream_chunck слишком мал: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "несоответствие c size_confirm!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "Предупреждение: удаление заголовка ????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "Ошибка разбора заголовка фрагмента\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "Заголовок не был получен в качестве первого фрагмента !!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "Ошибка: Не удалось выделить буфер размером %d байт(а/ов).\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "Ошибка чтения сетевого потока.\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "Ошибка: Слишком малый размер фрагмента.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "Пропускная способность слишком мала, файл не может быть воспроизведён!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "Пропускная способность слишком мала, отключение аудиопотока.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "Пропускная способность слишком мала, отключение видеопотока.\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "Недопустимая длина в заголовке ASF!\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "Ошибка: chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> Перенаправитель ASF\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "недопустимый URL-адрес прокси\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "неизвестный тип потока ASF\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "Не удалось проанализировать ответ HTTP.\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "Ответ сервера %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ПРЕДУПРЕЖДЕНИЕ АНАЛИЗАТОРА HTTP ASF : Pragma %s урезана от %zu байт до %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "ошибка записи сокета: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "Не удалось выполнить разбор заголовка.\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "Поток не найден.\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "неизвестный тип потока ASF\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "Сбой, выход.\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\nОшибка чтения аудио: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "Восстановлено после «cross-run», некоторые кадры могут быть потеряны!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "Неустранимая ошибка, невозможно восстановить!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\nНедостаточно аудиосэмплов!\n"

// cache2.c

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "Неизвестное семейство адресов %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "Разрешение %s для %s…\n"
#define MSGTR_MPDEMUX_NW_CantResolv "Не удалось разрешить имя для %s: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "Установка соединения с сервером %s[%s]: %d…\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "Не удалось установить соединение с сервером: %s\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "Сбой Select.\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "истекло время соединения\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "сбой getsockopt: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "ошибка соединения: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "Недопустимые настройки прокси… попытка соединения без прокси.\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "Не удалось разрешить удалённое имя для AF_INET. Попытка соединения без прокси.\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "Ошибка отправки HTTP-запроса: Отправлен не весь запрос.\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "Сбой чтения.\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response прочитано 0 (то еесть EOF).\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "Сбой аутентификации. Используйте параметры -user и -passwd, чтобы ввести логин и\n"\
"пароль для списка URL-адресов, или сформируйте URL-адрес следующим образом:\n"\
"http://логин:пароль@имя_хоста/файл\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "Для %s требуется аутентификация\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "Требуется аутентификация.\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "Пароль не указан, попытка использовать пустой пароль.\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "Ответ сервера %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "Установлен размер кэша %d КБайт(а/ов)\n"

// open.c, stream.c
#define MSGTR_CdDevNotfound "CD-ROM «%s» не найден!\n"
#define MSGTR_ErrTrackSelect "Ошибка выбора дорожки VCD!"
#define MSGTR_ReadSTDIN "Чтение из stdin (со стандартного входа)…\n"
#define MSGTR_FileNotFound "Файл не найден: «%s»\n"

#define MSGTR_SMBInitError "Не удалось инициализировать библиотеку libsmbclient: %d\n"
#define MSGTR_SMBFileNotFound "Не удалось открыть по сети: «%s»\n"

#define MSGTR_CantOpenBluray "Не удалось открыть устройство Blu-ray: %s\n"
#define MSGTR_CantOpenDVD "Не удалось открыть DVD: %s (%s)\n"

#define MSGTR_URLParsingFailed "Не удалось выполнить разбор URL-адреса %s\n"
#define MSGTR_FailedSetStreamOption "Не удалось установить параметр потока %s=%s\n"
#define MSGTR_StreamNeedType "Потоки должны иметь тип!\n"
#define MSGTR_StreamCantHandleURL "Не удалось найти поток для разбора URL-адреса %s\n"
#define MSGTR_StreamNULLFilename "open_output_stream(), имя файла NULL, необходимо сообщить об этой ошибке\n"
#define MSGTR_StreamErrorWritingCapture "Ошибка записи файла захвата: %s\n"
#define MSGTR_StreamSeekFailed "Сбой навигации\n"
#define MSGTR_StreamNotSeekable "Переход по потоку невозможен!\n"
#define MSGTR_StreamCannotSeekBackward "Переход назад в линейных потоках невозможен!\n"

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "Не удалось открыть устройство CDDA.\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "Не удалось открыть диск.\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "Найден аудио CD с %d дорожками.\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "Не удалось прочитать TOC.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "Не удалось открыть устройство %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "недопустимый URL-адрес\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "Не удалось отправить запрос HTTP.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "Не удалось прочитать ответ HTTP.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "Не найдено.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "неизвестный код ошибки\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "Кэш не найден.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "Не все файлы xmcd были прочитаны.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "Не удалось создать каталог %s.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "Не все файлы xmcd были записаны.\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "Возвращён недопустимый файл базы данных xmcd.\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "неожиданное FIXME\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "необработанный код\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "Не удалось найти конец строки.\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "Разбор выполнен успешно, найдено: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "Альбом не найден.\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "Ответ сервера: ошибка синтаксиса команды\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "Не удалось получить уровень протокола.\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "Нет CD в приводе.\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] Неожиданная строка файла cue: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameFound "[bincue] Найдена спецификация файла: %s"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] проверенное имя бинарного файла: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] Не удалось найти бинарный файл - прекращение операции.\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] Использование бинарного файла %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] неизвестный режим для бинарного файла.\nЭто неожиданная ситуация. Отмена.\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] Не удалось открыть %s.\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] Ошибка чтения из %s\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] Ошибка получения размера бинарного файла.\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "дорожка %02d:  формат=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] неожиданный конец бинарного файла\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] Не удалось прочитать %d байт(а/ов) полезной нагрузки.\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "[bincue] открытие потока: имя файла=%s, дорожка=%d, доступные дорожки: %d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "Не удалось открыть привод DVD для записи, изменение скорости DVD требует доступ\nна запись.\n"
#define MSGTR_DVDrestoreSpeed "Восстановление скорости DVD… "
#define MSGTR_DVDlimitSpeed "Ограничение скорости DVD до %dКб/с… "
#define MSGTR_DVDlimitFail "ошибка\n"
#define MSGTR_DVDlimitOk "успешно\n"
#define MSGTR_NoDVDSupport "MPlayer был скомпилирован без поддержки DVD, выход.\n"
#define MSGTR_DVDnumTitles "Число записей на этом DVD: %d.\n"
#define MSGTR_DVDinvalidTitle "Недопустимый номер записи DVD: %d\n"
#define MSGTR_DVDinvalidChapterRange "Недопустимое описание диапазона раздела %s\n"
#define MSGTR_DVDnumAngles "В этой записи DVD %d углов.\n"
#define MSGTR_DVDinvalidAngle "Недопустимый номер угла записи DVD: %d\n"
#define MSGTR_DVDnoIFO "Не удалось открыть IFO-файл для записи DVD %d.\n"
#define MSGTR_DVDnoVMG "Не удалось открыть информацию VMG!\n"
#define MSGTR_DVDnoVOBs "Не удалось открыть VOBS записи (VTS_%02d_1.VOB).\n"
#define MSGTR_DVDnoMatchingAudio "Не найден подходящий язык аудио DVD!\n"
#define MSGTR_DVDaudioChannel "Выбранный аудиоканал DVD: %d язык: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "аудиопоток: %d формат: %s (%s) язык: %s aid: %d.\n"
#define MSGTR_DVDnumAudioChannels "число аудиоканалов на диске: %d.\n"
#define MSGTR_DVDnoMatchingSubtitle "Не найден подходящий язык субтитров DVD!\n"
#define MSGTR_DVDsubtitleChannel "Выбран канал субтитров DVD: %d язык: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "субтитры ( sid ): %d язык: %s\n"
#define MSGTR_DVDnumSubtitles "число субтитров на диске: %d\n"

// stream_bluray.c
#define MSGTR_BlurayNoDevice "Не указано устройство/расположение Blu-ray…\n"
#define MSGTR_BlurayNoTitles "Не удалось найти здесь записи, совместимые с Blu-ray.\n"

// stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] Обнаружены имена радиоканалов.\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] Неверная частота для канала %s\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] Неверный номер канала: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] Неверный номер канала: %d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] Неверное название канала: %s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] Обнаружен параметр радиочастоты.\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Предупреждение: сбой вызова ioctl get tuner : %s. frac установлен в %d.\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s не является радиоустройством!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] сбой вызова ioctl set frequency 0x%x (%.2f): %s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] сбой вызова ioctl get frequency: %s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] сбой вызова ioctl set mute: %s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] сбой вызова ioctl query control: %s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] сбой вызова ioctl get volume: %s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] сбой вызова ioctl set volume: %s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] Не удалось выделить аудио буфер (блок=%d,размер=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] Текущая частота: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] Выбран канал: %d - %s (частота: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] Не удалось изменить канал: не передан список радиоканалов.\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] Не удалось открыть '%s': %s\n"
#define MSGTR_RADIO_WrongFreq "[radio] Неверная частота: %.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] Используется частота: %.2f.\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] сбой вызова audio_in_init\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] сбой вызова audio_in_setup: %s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] Сбой очистки буфера: %s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] Сбой вызова stream_enable_cache: %s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] Неизвестное имя драйвера: %s\n"
#define MSGTR_RADIO_DriverV4L "[radio] Используется радиоинтерфейс V4Lv1.\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] Используется радиоинтерфейс V4Lv2.\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] Используется радиоинтерфейс *BSD BT848.\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c: norm_from_string(%s): Неизвестный параметр нормы, будет устанавлен %s.\n"
#define MSGTR_TV_NoVideoInputPresent "Ошибка: вход Видео отсутствует!\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==============================================================================\n"\
" ВНИМАНИЕ:\n"\
" ЗАПРОШЕН НЕПРОТЕСТИРОВАННЫЙ ИЛИ НЕИЗВЕСТНЫЙ ФОРМАТ ИЗОБРАЖЕНИЯ (0x%x)\n"\
" Это может привести к неверному воспроизведению или аварийному завершению\n"\
" программы! Сообщения об ошибках приниматься не будут! Требуется повторное\n"\
" выполнение с YV12 (пространство цветов по умолчанию) и изучение документации!\n"\
"==============================================================================\n"
#define MSGTR_TV_CannotSetNorm "Ошибка: Невозможно задать норму!\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP: ширина %d высота %d\n"
#define MSGTR_TV_UnableToSetWidth "Невозможно задать запрошенную ширину: %d\n"
#define MSGTR_TV_UnableToSetHeight "Невозможно задать запрошенную высоту: %d\n"
#define MSGTR_TV_NoTuner "На выбранном входе нет тюнера!\n"
#define MSGTR_TV_UnableFindChanlist "Не удалось найти выбранный список каналов! (%s)\n"
#define MSGTR_TV_ChannelFreqParamConflict "Невозможно устанавливать частоту и канал одновременно!\n"
#define MSGTR_TV_ChannelNamesDetected "Обнаружены названия TV каналов.\n"
#define MSGTR_TV_NoFreqForChannel "Не удалось найти частоту для канала %s (%s)\n"
#define MSGTR_TV_SelectedChannel3 "Выбран канал: %s - %s (частота: %.3f)\n"
#define MSGTR_TV_SelectedChannel2 "Выбран канал: %s (частота: %.3f)\n"
#define MSGTR_TV_UnsupportedAudioType "Тип аудио «%s (%x)» не поддерживается!\n"
#define MSGTR_TV_AvailableDrivers "Доступные драйверы:\n"
#define MSGTR_TV_DriverInfo "Выбран драйвер: %s\n название: %s\n автор: %s\n комментарий %s\n"
#define MSGTR_TV_NoSuchDriver "Отсутствует драйвер: %s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "Не удалось определить драйвер TV автоматически.\n"
#define MSGTR_TV_UnknownColorOption "Указан неизвестный параметр цвета (%d)!\n"
#define MSGTR_TV_NoTeletext "Нет телетекста"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848: Сбой %s ioctl. Ошибка: %s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848: Недопустимая скорость передачи аудио. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848: Не удалось открыть устройство bktr. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848: Не удалось открыть устройство tuner. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848: Не удалось открыть устройство dsp. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848: Сбой конфигурирации dsp. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848: Ошибка чтения аудио данных. Ошибка: %s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848: Сбой mmap. Ошибка: %s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848: Сбой выделения памяти для буфера кадра. Ошибка: %s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848: Ошибка установки ширины изображения. Ошибка: %s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848: Не удалось остановить захват. Ошибка: %s\n"
#define MSGTR_TV_TTSupportedLanguages "Поддерживаемые языки телетекста:\n"
#define MSGTR_TV_TTSelectedLanguage "Выбран язык телетекста по умолчанию: %s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "Сканер каналов при отсутствии тюнера недоступен\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "Не удалось присоединить данный вход к видео декодеру. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "Не удалось присоединить данный вход к аудио декодеру. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow: Не удалось выбрать видео формат. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow: Не удалось выбрать аудио формат. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow: Не удалось получить IMediaControl интерфейс. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow: Не удалось запустить граф! Ошибка:0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow: Устройство #%d не найдено\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow: Не удалось получить название устройства #%d\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow: Используется устройство #%d: %s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow: Не удалось получить частоту напрямую. Будет использована встроенная\nтаблица каналов ОС.\n"
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow: Невозможно загрузить таблицу частот с kstvtune.ax\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow: Неверный параметр device: %s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow: Неверный индекс device: %d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow: Неверный параметр adevice: %s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow: Неверный индекс adevice: %d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow: Частота дискретизации %d не поддерживается устройством.\nВосстанавливается первая доступная.\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow: Подстройка яркости/оттенка/насыщенности/контраста\nне поддерживается устройством\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow: Изменение ширины/высоты видео не поддерживается устройством.\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow: Выбор источника захвата не поддерживается устройством\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow: Невозможно разобрать структуру аудио формата.\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow: Невозможно разобрать структуру видео формата.\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow: Невозможно установить аудио режим %d. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow: Неподдерживаемый тип медиа передан в %s\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow: Не удалось найти ближайший канал в системной таблице частот\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow: Не удалось переключиться на ближайший канал в системой таблице частот.\nОшибка:0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow: Не удалось соединить VideoPort pin ни с одним из фильтров графа.\nОшибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow: Не удалось построить цепочку видео графа захвата. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow: Не удалось построить цепочку аудио графа захвата. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow: Не удалось построить цепочку VBI графа захвата. Ошибка:0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow: Ошибка инициализации графа Directshow.\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow: Не удалось найти устройство захвата видео\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow: Не удалось найти устройство захвата аудио\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow: Не удалось получить текущий медиаформат (Ошибка:0x%x).\nПредполагается совпадение с запрошенным форматом.\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "Вероятно, строка уже закодирована в url_escape %c%c%c\n"

// subtitles
#define MSGTR_SUBTITLES_SubRip_UnknownFontColor "SubRip: неизвестный цвет шрифта субтитров: %s\n"

