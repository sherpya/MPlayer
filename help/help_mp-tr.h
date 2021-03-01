// MPlayer Turkish Translation
// Synced with help_mp-en.h r26067
// Translated by: Tuncer Altay, tunceraltay (at) yahoo.com and Kadir T. İri, centurium (at) gmx.net
// Additions by Mehmet KÖSE <mehmetkse@gmail.com>
// ~/Turkey/Ankara/Ankara University/Computer Engineering Department

// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<parçano>    (S)VCD (Süper Video CD) parça numarasını oynatır (sade aygıtı kullan, sisteme takma)\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<başlıkno>   Dosya yerine aygıttan DVD başlığını oynatır.\n"\
                      " -alang/-slang      DVD ses/altyazı dili seçer (2 karakterli ülke kodu ile)\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"Kullanım: mplayer [seçenekler] [adres|yol/]dosya adı\n"\
"\n"\
"Genel seçenekler: (Tüm seçenekler için man sayfalarına bakınız)\n"\
" -vo <sürücü>          video çıkış sürücüsünü seçer ('-vo help' ile listeyi görebilirsiniz)\n"\
" -ao <sürücü>          ses çıkış sürücüsü seçer ('-ao help' ile listeyi görebilirsiniz)\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -ss <zamankon>     Verilen konumu arar (saniye veya hh:mm:ss ;saat:dakika:saniye olarak)\n"\
" -nosound           Sesi çalmaz\n"\
" -fs -vm -zoom      Tam ekran çalıştırma seçenekleri (veya -vm, -zoom, detaylar man sayfalarında)\n"\
" -x <x> -y <y>      Ekran çözünürlüğünü ayarlar (-vm veya -zoom kullanımı için)\n"\
" -sub <dosya>       Kullanılacak altyazı dosyasını seçer ( ayrıca -subfps, -subdelay seçeneklerine bakınız)\n"\
" -playlist <dosya>  Çalma listesi dosyasını seçer\n"\
" -vid x -aid y      Oynatılacak video (x) ve çalınacak ses (y) yayınını(stream) seçer\n"\
" -fps x -srate y    Video (x) biçimini fps olarak ve ses (y) biçimini Hz olarak değiştirir\n"\
" -pp <kalite>       postprocessing filtresini etkinleştirir (ayrıntılar için man sayfalarına bakınız)\n"\
" -framedrop         kare(frame) atlamayı etkinleştirir (yavaş bilgisayarlar için)\n"\
"\n"\
"Başlıca Tuşlar: (tüm liste man sayfasındadır, ayrıca input.conf  dosyasını kontrol ediniz)\n"\
" <-  veya  ->       geri sar/ileri sar (10 saniye )\n"\
" yukarı veya aşağı  geri sar/ileri sar  (1 dakika)\n"\
" pgup veya pgdown   geri sar/ileri sar (10 dakika)\n"\
" < veya >           çalma listesinde önceki/sonraki \n"\
" p veya BOŞLUK      duraklat (devam etmek için herhangi bir tuşa basınız)\n"\
" q veya ESC         durdur ve uygulamadan çık\n"\
" + veya -           ses gecikmesini +/- 0.1 saniye olarak ayarla\n"\
" o                  OSD modunu değiştir:  yok / oynatma imi / oynatma imi + zamanlayıcı\n"\
" * veya /           sesi yükselt veya alçalt\n"\
" z veya x           altyazı gecikmesini +/- 0.1 saniye olarak ayarla\n"\
" r veya t           altyazı konumunu yukarı/aşağı ayarla, -vf seçeneğine de bakınız\n"\
"\n"\
" * * AYRINTILAR, DAHA FAZLA (GELİŞMİŞ) SEÇENEKLER VE TUŞLAR İÇİN MAN SAYFALARINA BAKINIZ * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c:

#define MSGTR_Exiting "\nÇıkılıyor...\n"
#define MSGTR_ExitingHow "\nÇıkılıyor... (%s)\n"
#define MSGTR_Exit_quit "Çık"
#define MSGTR_Exit_eof "Dosya sonu"
#define MSGTR_Exit_error "Önemli Hata"
#define MSGTR_IntBySignal "\nMPlayer %d sinyali tarafından kapatıldı, modül: %s\n"
#define MSGTR_NoHomeDir "Anadizin(HOME) bulunamıyor\n"
#define MSGTR_GetpathProblem "get_path(\"config\") problemi\n"
#define MSGTR_CreatingCfgFile "Ayar dosyası oluşturuluyor: %s\n"
#define MSGTR_CantLoadFont "Yazıtipi açılamıyor: %s\n"
#define MSGTR_CantLoadSub "Altyazı açılamıyor: %s\n"
#define MSGTR_DumpSelectedStreamMissing "döküm: HATA:seçili yayın(stream) hatalı!\n"
#define MSGTR_CantOpenDumpfile "döküm(dump) dosyası açılamıyor!!!\n"
#define MSGTR_CoreDumped "Belleğin dökümü alındı ;)\n"
#define MSGTR_FPSnotspecified "Başlıktaki FPS belirsiz veya geçersiz, -fps seçeneğini kullanınız!\n"
#define MSGTR_TryForceAudioFmtStr "%s ses kodek sürücülerini zorlama deneniyor...\n"
#define MSGTR_CantFindAudioCodec "0x%X! ses biçimi için kodek bulunamıyor\n"
#define MSGTR_TryForceVideoFmtStr "%s video kodek sürücülerini zorlama deneniyor...\n"
#define MSGTR_CantFindVideoCodec "Seçili -vo sürücüsü ve 0x%X video biçimi için kodek bulunamadı!\n"
#define MSGTR_CannotInitVO "HATA: Video sürücüsü başlatılamıyor!\n"
#define MSGTR_CannotInitAO "Ses aygıtı açılamadı/başlatılamadı -> ses yok\n"
#define MSGTR_StartPlaying "Çalma başlatılıyor...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"           ***************************************************\n"\
"           *** Sisteminiz dosyayı oynatmak için çok YAVAŞ! ***\n"\
"           ***************************************************\n\n"\
" Olası nedenler, sorunlar , çözümler:\n"\
"- En yaygın: bozuk/hatalı _ses_ sürücüsü\n"\
"  - \"-ao sdl\" seçeneğini deneyiniz veya ALSA'nın OSS emulasyonunu kullanınız.\n"\
"  - \"-autosync\" seçeneği için farklı değerler deneyiniz, 30 iyi bir başlangıçtır.\n"\
"- Yavaş video çıktısı\n"\
"  - Farklı bir -vo sürücüsü deneyiniz (liste için -vo help) veya -framedrop seçeneğini deneyiniz!\n"\
"- Yavaş işlemci\n"\
"  - Yavaş bir işlemci ile büyük bir DVD/DivX oynatmayınız! Bazı lavdopts seçeneklerini deneyiniz,\n"\
"     örn: -vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all\n"\
"- Bozuk dosya\n"\
"  - \"-nobps -ni -forceidx -mc 0\" seçeneğinin çeşitli bileşenlerini deneyiniz.\n"\
"- Yavaş medya (NFS/SMB bağlamaları, DVD, VCD vb.)\n"\
"  - \"-cache 8192\" seçeneğini deneyiniz.\n"\
"- Boşluksuz AVI dosyası oynatmak için -cache seçeneğini mi kullanıyorsunuz?\n"\
"  - \"-nocache\" seçeneğini deneyiniz.\n"\
"Uyumlama/hızlandırma(tuning/speedup) ipuçları için DOCS/HTML/en/video.html sayfasını okuyunuz.\n"\
"Bunların hiçbirisi size yardım etmiyorsa DOCS/HTML/en/bugreports.html sayfasını okuyunuz.\n\n"

#define MSGTR_NoGui "MPlayer GRAFİK ARABİRİMSİZ seçeneğiyle oluşturulmuş!\n"
#define MSGTR_GuiNeedsX "MPlayer grafik arabirimi X11 gerektirir!\n"
#define MSGTR_Playing "%s oynatılıyor\n"
#define MSGTR_NoSound "Ses: ses yok!\n"
#define MSGTR_FPSforced "FPS  %5.3f olarak zorlandı (ftime: %5.3f)\n"
#define MSGTR_AvailableVideoOutputDrivers "Kullanılabilir video çıktı sürücüleri:\n"
#define MSGTR_AvailableAudioOutputDrivers "Kullanılabilir ses çıktı sürücüleri:\n"
#define MSGTR_AvailableAudioCodecs "Kullanılabilir ses kodekleri:\n"
#define MSGTR_AvailableVideoCodecs "Kullanılabilir video kodekleri:\n"
#define MSGTR_AvailableAudioFm "\nKullanılabilir ses kodek sınıfları/sürücüleri (gömülü):\n"
#define MSGTR_AvailableVideoFm "\nKullanılabilir video kodek sınıfları/sürücüleri (gömülü):\n"
#define MSGTR_AvailableFsType "Kullanılabilir tamekran değiştirme modları:\n"
#define MSGTR_CannotReadVideoProperties "Video: Özellikler okunamıyor\n"
#define MSGTR_NoStreamFound "Yayın(stream) bulunamadı\n"
#define MSGTR_ErrorInitializingVODevice "Video çıkış (-vo) aygıtı açılış/başlatma hatası!\n"
#define MSGTR_ForcedVideoCodec "Video kodeği zorlandı: %s\n"
#define MSGTR_ForcedAudioCodec "Ses kodeği zorlandı: %s\n"
#define MSGTR_Video_NoVideo "Video: video yok!!!\n"
#define MSGTR_NotInitializeVOPorVO "\nHATA: Video filtreleri (-vf) veya video çıkışı (-vo) başlatılamadı!\n"
#define MSGTR_Paused "--------   DURAKLADI   --------"
#define MSGTR_PlaylistLoadUnable "\n%s çalma listesi yüklenemedi.\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- 'Geçersiz Komut' nedeniyle MPlayer çöktü.\n"\
"  Yeni runtime CPU-detection kodlarımızda bir hata olabilir...\n"\
"  Lütfen DOCS/HTML/en/bugreports.html sayfasını okuyunuz.\n"
#define MSGTR_Exit_SIGILL \
"- 'Geçersiz Komut' nedeniyle MPlayer çöktü.\n"\
"  Genellikle uygulamanın derlendiği/geliştirildiği işlemciden farklı bir işlemci üzerinde\n"\
"  MPlayer'ı çalıştırdığınızda bu hata oluşur.\n"\
"  Bunu doğrulayın!\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- Mplayer CPU/FPU/RAM'in hatalı kullanımı nedeniyle çöktü.\n"\
" Mplayer'ı --enable-debug seçeneğiyle yeniden derleyiniz. Bir 'gdb' backtrace ve\n"\
" disassembly oluşturunuz. Ayrıntılar için DOCS/HTML/en/bugreports_what.html#bugreports_crash\n"
#define MSGTR_Exit_SIGCRASH \
"- Mplayer çöktü. Bu durum oluşmamalıydı.\n"\
"  MPlayer kodları, gcc sürümünüz veya sürücülerinizde bir hata olabilir. Mplayer hatası \n"\
"  olduğunu düşünüyorsanız, lütfen DOCS/HTML/en/bugreports.html sayfasını okuyunuz \n"\
"  ve yönergeleri izleyiniz. Muhtemel bir hata bildirirken bu bilgileri sağlayamazsanız \n"\
"  yardım edemeyiz ve etmeyeceğiz.\n"
#define MSGTR_LoadingConfig "'%s' ayarı yükleniyor\n"
#define MSGTR_LoadingProtocolProfile "Protokole bağlı profil yükleniyor '%s'\n"
#define MSGTR_LoadingExtensionProfile "Uzantıya bağlı profil yükleniyor '%s'\n"
#define MSGTR_AddedSubtitleFile "ALTYAZI: (%d) altyazı dosyası eklendi : %s\n"
#define MSGTR_RemovedSubtitleFile "ALTYAZI: (%d) altyazı dosyası kaldırıldı: %s\n"
#define MSGTR_RTCDeviceNotOpenable "%s dosyası açılamadı: %s (kullanıcı için okunabilir olmalı)\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "ioctl'de Linux RTC açılış hatası  (rtc_irqp_set %lu): %s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "\"echo %lu > /proc/sys/dev/rtc/max-user-freq\" seçeneğini sistem açılış betiklerine eklemeyi deneyiniz.\n"
#define MSGTR_LinuxRTCInitErrorPieOn "ioctl'de Linux RTC açılış hatası (rtc_pie_on): %s\n"
#define MSGTR_Getch2InitializedTwice "UYARI:getch2_init iki defa çağırıldı!\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "%s root menüsü ile libmenu video filtresi açılamaz.\n"
#define MSGTR_AudioFilterChainPreinitError "Ses filtresi zincirinde başlama öncesi hata!\n"
#define MSGTR_LinuxRTCReadError "Linux RTC okuma hatası: %s\n"
#define MSGTR_SoftsleepUnderflow "Uyarı! softsleep underflow!\n"

#define MSGTR_EdlOutOfMem "EDl verisini tutmak için yeterli bellek atanamıyor.\n"
#define MSGTR_EdlRecordsNo "%d EDL hareketlerini Oku.\n"
#define MSGTR_EdlQueueEmpty "Dikkat edilecek herhangi EDL hareketi yok.\n"
#define MSGTR_EdlCantOpenForWrite "Yazma esnasında [%s] EDL dosyası açılamaz.\n"
#define MSGTR_EdlNOsh_video "Video olmadan EDL kullanılamaz, devredışı bırakılıyor.\n"
#define MSGTR_EdlNOValidLine "Geçersiz EDL satırı: %s\n"
#define MSGTR_EdlBadlyFormattedLine "[%d] EDL satırı hatalı düzenlenmiş, çıkarılıyor.\n"
#define MSGTR_EdlBadLineOverlap "Son duruş konumu [%f] idi; sonraki başlangıç "\
"[%f]. Girişler kronolojik düzende olamalıdır, bindirme olamaz. İptal ediliyor.\n"
#define MSGTR_EdlBadLineBadStop "Duruş zamanı başlangıç zamanından sonra olmalıdır.\n"
#define MSGTR_EdloutBadStop "EDL atlaması iptal edildi, son başlama > duruş\n"
#define MSGTR_EdloutStartSkip "EDL atlaması başlangıcı, bloğu bitirmek için\n"\
"'i' tuşuna tekrar basın.\n"
#define MSGTR_EdloutEndSkip "EDL atlaması sonu, satır yazıldı.\n"

// mplayer.c OSD

#define MSGTR_OSDenabled "etkin"
#define MSGTR_OSDdisabled "etkisiz"
#define MSGTR_OSDAudio "Ses: %s"
#define MSGTR_OSDChannel "Kanal: %s"
#define MSGTR_OSDSubDelay "Altyazı gecikmesi: %d ms"
#define MSGTR_OSDSpeed "Hız: x %6.2f"
#define MSGTR_OSDosd "OSD: %s"
#define MSGTR_OSDChapter "Kısım: (%d) %s"
#define MSGTR_OSDAngle "Açı: %d/%d"

// property values
#define MSGTR_Enabled "etkin"
#define MSGTR_EnabledEdl "etkin(edl)"
#define MSGTR_Disabled "etkisiz"
#define MSGTR_HardFrameDrop "sabit"
#define MSGTR_Unknown "bilinmeyen"
#define MSGTR_Bottom "alt"
#define MSGTR_Center "orta"
#define MSGTR_Top "üst"
#define MSGTR_SubSourceFile "dosya"
#define MSGTR_SubSourceVobsub "vobsub"
#define MSGTR_SubSourceDemux "gömülü"

// osd bar names
#define MSGTR_Volume "Ses"
#define MSGTR_Panscan "Yanaltarama"
#define MSGTR_Gamma "Gama"
#define MSGTR_Brightness "Parlaklık"
#define MSGTR_Contrast " Karşıtlık"
#define MSGTR_Saturation "Doygunluk"
#define MSGTR_Hue "Renk"
#define MSGTR_Balance "Denge"

// property state
#define MSGTR_LoopStatus "Döngü: %s"
#define MSGTR_MuteStatus " Sessiz: %s"
#define MSGTR_AVDelayStatus "A-V gecikmesi: %s"
#define MSGTR_OnTopStatus "Üstte kalma: %s"
#define MSGTR_RootwinStatus "Ana pencere: %s"
#define MSGTR_BorderStatus "Kenar: %s"
#define MSGTR_FramedroppingStatus "Atlanan kare: %s"
#define MSGTR_VSyncStatus "VSync: %s"
#define MSGTR_SubSelectStatus "Altyazılar: %s"
#define MSGTR_SubSourceStatus "Altyazı kaynağı: %s"
#define MSGTR_SubPosStatus "Altyazı konumu: %s/100"
#define MSGTR_SubAlignStatus "Altyazı hizalama: %s"
#define MSGTR_SubDelayStatus "Altyazı gecikmesi: %s"
#define MSGTR_SubScale "Altyazı ölçeği: %s"
#define MSGTR_SubVisibleStatus "Altyazılar: %s"
#define MSGTR_SubForcedOnlyStatus "Sadece zorunlu altyazı:  %s"

// mencoder.c:

#define MSGTR_UsingPass3ControlFile "pass3 kontrol dosyası kullanılıyor: %s\n"
#define MSGTR_MissingFilename "\nDosya adı eksik\n\n"
#define MSGTR_CannotOpenFile_Device "dosya/aygıt açılamadı\n"
#define MSGTR_CannotOpenDemuxer "Ayrıştırıcı açılamadı\n"
#define MSGTR_NoAudioEncoderSelected "\nSeçili Ses kodlayıcı (-oac) yok!  Birini seçin veya -nosound seçeneğini kullanınız. Yardım için: -oac help \n"
#define MSGTR_NoVideoEncoderSelected "\nSeçili Video kodlayıcı (-ovc) yok! Birini seçin, yardım için: -ovc help yazınız\n"
#define MSGTR_CannotOpenOutputFile "Çıktı dosyası açılamadı '%s'\n"
#define MSGTR_EncoderOpenFailed "Kodlayıcı açılamadı\n"
#define MSGTR_MencoderWrongFormatAVI "\nUYARI: ÇIKTI DOSYA BİÇİMİ _AVI_. yardım için: -of help yazınız.\n"
#define MSGTR_MencoderWrongFormatMPG "\nUYARI: ÇIKTI DOSYA BİÇİMİ _MPEG_. yardım için: -of help yazın.\n"
#define MSGTR_MissingOutputFilename "Çıktı dosyası belirtilmedi, Lütfen -o seçeneğine bakınız."
#define MSGTR_ForcingOutputFourcc "FourCC çıktısı %x [%.4s] olarak zorlanıyor\n"
#define MSGTR_ForcingOutputAudiofmtTag "Çıktı ses biçim etiketi 0x%x olarak zorlanıyor.\n"
#define MSGTR_DuplicateFrames "\n%d çift kare!\n"
#define MSGTR_SkipFrame "\nKare atlanıyor!   \n"
#define MSGTR_ResolutionDoesntMatch "\nYeni video dosyası öncekinden farklı renk biçimi veya çözünürlüğe sahip.\n"
#define MSGTR_FrameCopyFileMismatch "\nTüm video dosyaları -ovc kopyalamasına göre aynı fps, çözünürlük ve kodek içermelidir.\n"
#define MSGTR_AudioCopyFileMismatch "\nTüm dosyalar -oac kopyalamasına göre aynı ses kodek ve biçimini içermelidir.\n"
#define MSGTR_NoAudioFileMismatch "\nSes ve video dosyaları ile sadece video dosyaları karıştırılamaz. -nosound deneyiniz.\n"
#define MSGTR_NoSpeedWithFrameCopy "UYARI:-oac kopyalama ile -speed seçeneğinin\n"\
"sorunsuz çalıştığına güvence verilmiyor. Kodlamanız bozuk olabilir!\n"
#define MSGTR_ErrorWritingFile "%s: dosya yazma hatası.\n"
#define MSGTR_FlushingVideoFrames "\nGörüntü kareleri boşaltılıyor.\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "Filtreler ayarlanmadı! Dosya boş mu?\n"
#define MSGTR_RecommendedVideoBitrate "%s CD'si için önerilen video bit oranı: %d\n"
#define MSGTR_VideoStreamResult "\nVideo biçimi: %8.3f kbit/sn  (%d bps)  boyut: %"PRIu64" bayt  %5.3f sn %d kare\n"
#define MSGTR_AudioStreamResult "\nSes biçimi: %8.3f kbit/s  (%d bps)  boyut: %"PRIu64" bayt  %5.3f sn\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL ATLAMASI: Başlama: %.2f  Bitiş: %.2f   Şimdiki: G: %.2f  S: %.2f     \r"
#define MSGTR_OpenedStream "başarım: biçim: %d veri: 0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "videocodec: framecopy (%dx%d %dbpp fourcc=%x)\n"
#define MSGTR_ACodecFramecopy "audiocodec: framecopy (biçim=%x chens=%d rate=%d bits=%d B/s=%d sample-%d)\n"
#define MSGTR_MP3AudioSelected "MP3 ses seçili\n"
#define MSGTR_SettingAudioDelay "Ses gecikmesi %5.3fs olarak ayarlanıyor.\n"
#define MSGTR_SettingVideoDelay "Video gecikmesi %5.3fs olarak ayarlanıyor.\n"
#define MSGTR_LimitingAudioPreload "Ses önyüklemesi 0.4s olarak sınırlanıyor\n"
#define MSGTR_IncreasingAudioDensity "Ses yoğunluğu 4 olarak artırılıyor.\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "Ses önyüklemesi 0 olarak, en yüksek pts düzeltmesi 0 olarak zorlanıyor.\n"
#define MSGTR_LameVersion "LAME sürümü %s (%s)\n\n"
#define MSGTR_InvalidBitrateForLamePreset "Hata: Saptanan bit oranı önayarlama için geçerli aralık dışındadır.\n"\
"\n"\
"Bu modu kullandığınızda \"8\" ve \"320\" arasında bir değer girmelisiniz.\n"\
"\n"\
"Daha fazla bilgi için verilen seçeneği deneyiniz: \"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "Hata: Geçerli bir profil ve/veya önayarlama seçeneklerini girmediniz.\n"\
"\n"\
"Kullanılabilir profiller:\n"\
"\n"\
"   <hızlı>        standard\n"\
"   <hızlı>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR modu ima edilir. Kullanmak için\n"\
"                      sadece bir bit oranı belirleyin. Örneğin:\n"\
"                      \"preset=185\" bu önayarlamayı etkinleştirir\n"\
"                      ve kbps ortalamasını 185 olarak kullanır.\n"\
"\n"\
"     Bazı örnekler:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" veya \"-lameopts  cbr:preset=192       \"\n"\
" veya \"-lameopts      preset=172       \"\n"\
" veya \"-lameopts      preset=extreme   \"\n"\
"\n"\
"Daha fazla bilgi için deneyiniz: \"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"Önayarlama geçişleri en yüksek olası kaliteyi sağlamak içim tasarlanmıştır.\n"\
"\n"\
"Bu hedefi doğrulamak ve başarmak için dinleme testleri körlere benzer titizlikle\n"\
"yapıldı ve önayarlama düğmeleri için çoğunlukla bu durumla karşı karşıya kalındı.\n"\
"\n"\
"Gerçekleşen en son gelişmelerle uyumlu hale getirmek için sürekli olarak bunlar\n"\
"güncellenir. Sonuç olarak önayarlama geçişleri şu andaki LAME ile birlikte hemen hemen\n"\
"en iyi kaliteyle size sağlanmalıdır.\n"\
"\n"\
"Bu önayarlamaları etkinleştirme:\n"\
"\n"\
"   VBR modları için (genellikle en yüksek kalite):\n"\
"\n"\
"     \"preset=standard\" Genellikle bu önayarlama çoğu müzik türünde\n"\
"                             bir çok insan için anlaşılır olmalıdır ve \n"\
"                             oldukça yüksek ses kalitesindedir.\n"\
"\n"\
"     \"preset=extreme\" Son derece iyi duyma yeteneğine ve benzer\n"\
"                             donanıma sahipseniz, bu önayarlama genellikle\n"\
"                             \"standard\" moddan biraz daha yüksek ses \n"\
"                             kalitesi sağlar.\n"\
"\n"\
"   CBR 320kbps için (Önayarlama düğmelerinde mümkün en yüksek kalite):\n"\
"\n"\
"     \"preset=insane\"  Çoğunlukla bu önayarlama pek çok insan ve ortam \n"\
"                             için gereğinden fazla olacaktır. Ancak dosya boyutuna\n"\
"                             aldırmaksızın mutlak en yüksek ses kalitesine sahip \n"\
"                             olmalıysanız, bu tam size göredir.\n"\
"\n"\
"   ABR modu için (verilen bit oranına göre yüksek kalite ama VBR kadar yüksek değil):\n"\
"\n"\
"     \"preset=<kbps>\"  Bu önayarlamanın kullanımı genellikle belirlenen bit \n"\
"                             oranında iyi kalite verecektir. Bu önayarlama \n"\
"                             girilen bit oranına göre özel durumlar için\n"\
"                             en uygun ayarları saptayacaktır. Bu yöntem \n"\
"                             çalışmasına karşın VBR kadar esnek değildir, ve \n"\
"                             daha yüksek bit oranlarında VBR gibi aynı kalite \n"\
"                             seviyesine erişemeyecektir.\n"\
"\n"\
" Aynı zamanda aşağıdaki seçenekler benzer profillerde de kullanılabilir:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr> (ABR Mode) - ABR modu ima edilir. Kullanmak için\n"\
"                      sadece bir bit oranı belirleyiniz. Örneğin:\n"\
"                      \"preset=185\" bu önayarlamayı etkinleştirir\n"\
"                      ve kbps ortalamasını 185 olarak kullanır.\n"\
"\n"\
"   \"fast\" - Özel bir profil için yeni, hızlı VBR'yi etkinleştirir. \n"\
"              Hız düğmesinin sakıncası zamanı bit oranına göre \n"\
"              ayarlamasıdır. Bu durum normal moddan biraz daha \n"\
"              yüksek olacaktır ve aynı zamanda kalite biraz daha düşebilir.\n"\
"   Uyarı: Geçerli sürüm ile hızlı önayarlamalar düzgün önayarlamalara \n"\
"              kıyasla çok yüksek bit oranlarına neden olabilir. \n"\
"\n"\
"   \"cbr\"  - 80, 96, 112, 128, 160, 192, 224, 256, 320 gibi belirgin bit \n"\
"              oranlarıyla ABR modunu (yukarısını okuyun) kullanıyorsanız,\n"\
"              standart abr mod yerine CBR mod kodlamasını zorlamak için\n"\
"              \"cbr\" seçeneğini kullanabilirsiniz. ABR daha yüksek kalite \n"\
"              sağlar ancak önemli olabilen internet üzerinden bir mp3 ile yayın \n"\
"              işlemi (streaming) yaptığınız durumlarda CBR yararlı olabilir.\n"\
"\n"\
"    Örneğin:\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" veya \"-lameopts  cbr:preset=192       \"\n"\
" veya \"-lameopts      preset=172       \"\n"\
" veya \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
" ABR modu için birkaç takma ad kullanılabilir:\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"LAME seçenekleri ayarlanamıyor, bit oranı/örnekleme oranını kontrol ediniz,\n"\
"bazı düşük bit oranları (<32) daha düşük örnekleme oranını gerektiriyor. "\
"(örn: -srate 8000). Başka birşey çalışmazsa bir önayarlama deneyiniz. "
#define MSGTR_ConfigFileError "ayar dosyası hatası"
#define MSGTR_ErrorParsingCommandLine "komutsatırı ayrıştırma hatası"
#define MSGTR_VideoStreamRequired "Video yayını(stream) zorunlu!\n"
#define MSGTR_ForcingInputFPS "girdi fps değeri %5.3f olarak yorumlanacak\n"
#define MSGTR_DemuxerDoesntSupportNosound "Ayrıştırıcı -nosound seçeneğini henüz desteklemiyor.\n"
#define MSGTR_MemAllocFailed "Bellek atanamadı\n"
#define MSGTR_NoMatchingFilter "filter/ao biçimi eşleşmesi bulunamadı!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30, C derleyicisi bozuk  olabilir?\n"
#define MSGTR_NoLavcAudioCodecName "LAVC Sesi, Kodek adı eksik!\n"
#define MSGTR_LavcAudioCodecNotFound "LAVC Sesi,  %s kodek türü için kodlayıcı bulunmadı.\n"
#define MSGTR_CouldntAllocateLavcContext "LAVC Sesi,  içerik atanamadı!\n"
#define MSGTR_CouldntOpenCodec "%s kodeği açılamadı br=%d\n"
#define MSGTR_CantCopyAudioFormat "0x%x ses biçimi -oac kopyalaması ile uyumsuz, lütfen önceki seçenek yerine  '-oac pcm' seçeneğini  deneyiniz veya üzerine yazmak için '-fafmttag' seçeneğini kullanınız.\n"

// cfg-mencoder.h:

#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>       değişken bit oranı yöntemi\n"\
"                  0: cbr (sabit bit oranı)\n"\
"                  1: mt (Mark Taylor VBR algoritması)\n"\
"                  2: rh (Robert Hegemann VBR algoritması - varsayılan)\n"\
"                  3: abr (ortalama bit oranı)\n"\
"                  4: mtrh (Mark Taylor Robert Hegemann VBR algoritması)\n"\
"\n"\
" abr             ortalama bit oranı\n"\
"\n"\
" cbr             sabit bit oranı\n"\
"                 Ayrıca sonraki ABR önayarlama modlarında CBR modu kodlaması zorlanır.\n"\
"\n"\
" br=<0-1024>     kBit olarak bit oranını belirleyin (sadece CBR ve ABR)\n"\
"\n"\
" q=<0-9>         kalite (0-en yüksek, 9-en düşük) (sadece VBR için)\n"\
"\n"\
" aq=<0-9>        algoritmik kalite (0-en iyi/en yavaş, 9-en kötü/en hızlı)\n"\
"\n"\
" ratio=<1-100>   sıkıştırma oranı\n"\
"\n"\
" vol=<0-10>      ses giriş kazancını ayarla\n"\
"\n"\
" mode=<0-3>     (varsayılan: otomatik)\n"\
"                  0: stereo\n"\
"                  1: joint-stereo\n"\
"                  2: dualchannel\n"\
"                  3: mono\n"\
"\n"\
" destekleme=<0-2>\n"\
"                  0: yok\n"\
"                  1: hepsi\n"\
"                  2: ayarla\n"\
"\n"\
" fast            VBR önayarlama modlarında daha hızlı kodlamayı başlatır\n"\
"                  biraz daha düşük kalite ve daha yüksek bit oranları.\n"\
"\n"\
" preset=<value> En yüksek olası kalite ayarlarını sağlar.\n"\
"                 medium: VBR kodlaması, iyi kalite\n"\
"                 (150-180 kbps bit oran aralığı\n"\
"                 standard: VBR kodlaması, yüksek kalite\n"\
"                 (170-210 kbps bit oran aralığı)\n"\
"                 extreme: VBR kodlaması, çok yüksek kalite\n"\
"                 (200-240 kbps bit oran aralığı)\n"\
"                 insane:  CBR kodlaması, en yüksek önayarlama kalitesi\n"\
"                 (320 kbps bit oranı)\n"\
"                 <8-320>: ABR kodlaması kbps olarak verilen ortalama bit oranında.\n\n"

//codec-cfg.c:
#define MSGTR_DuplicateFourcc "Birden fazla FourCC"
#define MSGTR_TooManyFourccs "Çok sayıda FourCC/biçimleri..."
#define MSGTR_ParseError "ayrıştırma hatası"
#define MSGTR_ParseErrorFIDNotNumber "ayrıştırma hatası (ID biçimi bir sayı değil?)"
#define MSGTR_ParseErrorFIDAliasNotNumber "ayrıştırma hatası (ID takma adı bir sayı değil?)"
#define MSGTR_DuplicateFID "Birden fazla ID biçimi"
#define MSGTR_TooManyOut "Çok sayıda hata..."
#define MSGTR_InvalidCodecName "\nkodek(%s) adı geçersiz!\n"
#define MSGTR_CodecLacksFourcc "\nkodek(%s) FourCC/biçiminde değil!\n"
#define MSGTR_CodecLacksDriver "\nkodek(%s) bir sürücü içermiyorl!\n"
#define MSGTR_CodecNeedsDLL "\nkodek(%s) için bir 'dll' dosyası gerekiyor!\n"
#define MSGTR_CodecNeedsOutfmt "\nkodek(%s) için bir 'outfmt' gerekiyor!\n"
#define MSGTR_CantAllocateComment "Açıklama için bellek atanamıyor."
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token(): max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "'satır' için belleğe ulaşılamıyor: %s\n"
#define MSGTR_CantReallocCodecsp "'*codecsp': %s yeniden atanamıyor.\n"
#define MSGTR_CodecNameNotUnique "'%s' adı benzersiz değil."
#define MSGTR_CantStrdupName "strdup yapılamıyor -> 'isim': %s\n"
#define MSGTR_CantStrdupInfo "strdup yapılamıyor -> 'bilgi': %s\n"
#define MSGTR_CantStrdupDriver "strdup yapılamıyor -> 'sürücü': %s\n"
#define MSGTR_CantStrdupDLL "strdup yapılamıyor -> 'dll': %s"
#define MSGTR_AudioVideoCodecTotals "%d adet ses & %d adet video kodeği mevcut\n"
#define MSGTR_CodecDefinitionIncorrect "Kodek doğru şekilde tanımlanmamış."
#define MSGTR_OutdatedCodecsConf "Bu codecs.conf dosyası çok eski ve bu Mplayer sürümü ile bağdaşmıyor."

// fifo.c

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "'--' daha fazlaseçenek yok demektir, fakat komut satırında\n"\
" hiçbir dosya adı belirtilmemiş.\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "Döngü seçeneği bir tamsayı olmalıdır: %s\n"
#define MSGTR_UnknownOptionOnCommandLine "Komut satırında bilinmeyen seçenek: -%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "Komut satırındaki seçeneği ayrıştırmada hata: -%s\n"
#define MSGTR_InvalidPlayEntry "Geçersiz çalma girişi %s\n"
#define MSGTR_NotAnMEncoderOption "-%s bir MEncoder seçeneği değil\n"
#define MSGTR_NoFileGiven "Hiçbir dosya verilmemiş\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "Bulunan çok eski kayıt boşluğu (slot) lvl %d: %d !!!\n"
#define MSGTR_InvalidCfgfileOption "%s seçeneği bir ayar dosyasında kullanılamaz. \n"
#define MSGTR_InvalidCmdlineOption "%s seçeneği komut satırında kullanılamaz. \n"
#define MSGTR_InvalidSuboption "Hata: '%s' seçeneği '%s' altseçeneğini içermiyor.\n"
#define MSGTR_MissingSuboptionParameter "Hata: '%s' altseçeneğinin '%s' seçeneği için bir parametresi olmalıdır!\n"
#define MSGTR_MissingOptionParameter "Hata: '%s' seçeneği bir parametre içermelidir! \n"
#define MSGTR_OptionListHeader "\n Ad                   Tür             En az      En çok   Global  CL    Cfg\n\n"
#define MSGTR_TotalOptions "\nToplam: %d seçenekleri\n"
#define MSGTR_ProfileInclusionTooDeep "UYARI: Çok karmaşık profil kalıntısı.\n"
#define MSGTR_NoProfileDefined "Tanımlanmış profil(ler) yok.\n"
#define MSGTR_AvailableProfiles "Kullanılabilir profiller:\n"
#define MSGTR_UnknownProfile "Bilinmeyen profil '%s'.\n"
#define MSGTR_Profile "%s profili: %s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n Ad                   Tür             En az      En çok\n\n"
#define MSGTR_TotalProperties "\nToplam: %d özellik\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "CD-ROM Sürücüsü '%s' bulunamadı!\n"
#define MSGTR_ErrTrackSelect "VCD seçilirken hata!"
#define MSGTR_ReadSTDIN "Standart girdiden okunuyor...\n"
#define MSGTR_FileNotFound "Dosya bulunamadı: '%s'\n"

#define MSGTR_SMBInitError "libsmbclient kütüphanesi başlatılamadı: %d\n"
#define MSGTR_SMBFileNotFound "Yerel ağdan açılamadı: '%s'\n"

#define MSGTR_CantOpenDVD "DVD sürücüsü açılamadı: %s (%s)\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "DVD aygıtı yazma için açılamadı, DVD hızını değiştirme yazma izni gerektirir.\n"
#define MSGTR_DVDrestoreSpeed "DVD hızı kaydediliyor... "
#define MSGTR_DVDlimitSpeed "DVD hızı şu değerle sınırlandırılıyor: %dKB/s... "
#define MSGTR_DVDlimitFail "başarısız\n"
#define MSGTR_DVDlimitOk "başarılı\n"
#define MSGTR_NoDVDSupport "Mplayer DVD desteği olmadan derlendi, çık\n"
#define MSGTR_DVDnumTitles "Bu DVD üzerinde %d tane başlık var.\n"
#define MSGTR_DVDinvalidTitle "Geçersiz DVD başlık numarası: %d\n"
#define MSGTR_DVDinvalidChapterRange "%s özelliği geçersiz bölüm aralığı\n"
#define MSGTR_DVDnumAngles "Bu DVD başlığında %d angle var.\n"
#define MSGTR_DVDinvalidAngle "Yanlış DVD angle numarası: %d\n"
#define MSGTR_DVDnoIFO "DVD Başlığı için IFO dosyası açılamıyor %d.\n"
#define MSGTR_DVDnoVMG "VMG info açılamıyor!\n"
#define MSGTR_DVDnoVOBs "VOBS Başlığı açılamıyor (VTS_%02d_1.VOB).\n"
#define MSGTR_DVDnoMatchingAudio "DVD seslendirme dil eşlemi bulunamadı!\n"
#define MSGTR_DVDaudioChannel "Seçili DVD ses kanalı: %d dil: %c%c\n"
#define MSGTR_DVDaudioStreamInfo "Ses akımı: %d biçim: %s (%s) dil: %s yardım: %d.\n"
#define MSGTR_DVDnumAudioChannels "DVD'de bulunan ses kanalları: %d.\n"
#define MSGTR_DVDnoMatchingSubtitle "DVD altyazı dil eşlemi bulunamadı!\n"
#define MSGTR_DVDsubtitleChannel "Seçili DVD altyazı kanalı: %dil: %c%c\n"
#define MSGTR_DVDsubtitleLanguage "Altyazı ( sid ): %d dil: %s\n"
#define MSGTR_DVDnumSubtitles "DVD'de bulunan altyazılaır: %d\n"


// muxer.c, muxer_*.c:
#define MSGTR_TooManyStreams "Çok sayıda yayın!"
#define MSGTR_RawMuxerOnlyOneStream "Rawaudio muxer sadece bir ses yayınını destekliyor!\n"
#define MSGTR_IgnoringVideoStream "video yayını yoksayılıyor!\n"
#define MSGTR_UnknownStreamType "Uyarı! Bilinmeyen yayın türü: %d\n"
#define MSGTR_WarningLenIsntDivisible "Uyarı! len samplesize ile bölünebilir değil!\n"
#define MSGTR_MuxbufMallocErr "Karıştırıcı tamponu ile bellek atanamıyor!\n"
#define MSGTR_MuxbufReallocErr "Karışıtırıcı tamponu ile bellek tekrar-atanamıyor!\n"
#define MSGTR_WritingHeader "Başlık yazılıyor...\n"
#define MSGTR_WritingTrailer "İndeks yazılıyor...\n"

// demuxer.c, demux_*.c:
#define MSGTR_AudioStreamRedefined "UYARI: Ses yayın başlığı %d tekrar tanımlandı!\n"
#define MSGTR_VideoStreamRedefined "UYARI: Video yayın başlığı %d tekrar tanımlandı!\n"
#define MSGTR_TooManyAudioInBuffer "\nAYRIŞTIRICI: Tamponda birçok (%d - %d baytta) ses paketi var!\n"
#define MSGTR_TooManyVideoInBuffer "\nAYRIŞTIRICI: Tamponda birçok  (%d - %d baytta) video paketi var!\n"
#define MSGTR_MaybeNI "Boşluksuz AVI yayını/dosyası oynatıyor olabilirsiniz veya kodek hatalı?\n" \
                      "AVI dosyaları için -ni seçeneğiyle boşluksuz modu zorlamayı deneyiniz.\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI: CBR-MP3 nBlockAlign başlık hatası etrafına çalışıyor!\n"
#define MSGTR_SwitchToNi "\nHatalı boşluklandırılmış .AVI saptandı  -ni moduna geçiliyor...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI: geçersiz ses akışı ID: %d - gözardı ediliyor (ses yok)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: geçersiz görüntü akışı ID: %d - gözardı ediliyor\n"\
" (varsayılan kullanılıyor)\n"
#define MSGTR_ON2AviFormat "ON2 AVI biçimi"
#define MSGTR_Detected_XXX_FileFormat "%s dosya biçimi bulundu!\n"
#define MSGTR_FormatNotRecognized "============ Üzgünüm, bu dosya biçimi desteklenmiyor  ===========\n"\
                                  "=== Dosya bir AVI,ASF veya MPEG yayın biçimi ise, lütfen geliştiricilere bildiriniz. ===\n"
#define MSGTR_SettingProcessPriority "İşlem önceliği ayarlanıyor: %s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] filefmt:%d  fourcc:0x%X  boyut:%dx%d  fps:%5.3f  ftime:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "Cannot initialize muxer."
#define MSGTR_MissingVideoStream "Video yayını bulunamadı!\n"
#define MSGTR_MissingAudioStream "Ses yayını bulunamadı...  ->  ses yok\n"
#define MSGTR_MissingVideoStreamBug "Eksik video yayını? Lütfen geliştiricilere bildiriniz, bu bir hata olabilir :(\n"

#define MSGTR_DoesntContainSelectedStream "ayrıştırma: Dosya seçili video veya ses yayınını içermiyor.\n"

#define MSGTR_NI_Forced "Zorlandı"
#define MSGTR_NI_Detected "Saptandı"
#define MSGTR_NI_Message "%s BOŞLUKSUZ AVI dosya biçimi.\n"

#define MSGTR_UsingNINI "BOŞLUKSUZ bozuk AVI dosya-biçimi kullanılıyor!\n"
#define MSGTR_CouldntDetFNo "Kare(Frame) numaraları saptanamadı (sadece arama için)  \n"
#define MSGTR_CantSeekRawAVI "Raw AVI yayınında aranamıyor! (İndeks zorunlu, -idx  komutuyla deneyiniz!)  \n"
#define MSGTR_CantSeekFile "Bu dosyada aranamıyor!  \n"

#define MSGTR_MOVcomprhdr "MOV: Sıkıştırılmış başlık desteği ZLIB gerektiriyor!\n"
#define MSGTR_MOVvariableFourCC "MOV: UYARI: Değişken FOURCC bulundu!?\n"
#define MSGTR_MOVtooManyTrk "MOV: UYARI: Çok sayıda parça!"
#define MSGTR_ErrorOpeningOGGDemuxer "ogg ayrıştırıcısı açılamadı\n"
#define MSGTR_CannotOpenAudioStream "Ses yayını açılamıyor: %s\n"
#define MSGTR_CannotOpenSubtitlesStream "Altyazı yayını açılamıyor: %s\n"
#define MSGTR_OpeningAudioDemuxerFailed "Ses ayrıştırıcısı açılamadı: %s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "Altyazı ayrıştırıcısı açılamadı: %s\n"
#define MSGTR_TVInputNotSeekable "TV girdisi aranabilir değil! (Muhtemelen arama kanal değiştirmek için yapılacak ;)\n"
#define MSGTR_DemuxerInfoChanged "Çözücü bilgisi %s şuna çevrildi: %s\n"
#define MSGTR_ClipInfo "Klip bilgisi: \n"

#define MSGTR_LeaveTelecineMode "\nBulunan içerik demux_mpg: 30000/1001fps NTSC, kare oranı değiştiriliyor.\n"
#define MSGTR_EnterTelecineMode "\nBulunan içerik demux_mpg: 24000/1001fps progressive NTSC, kare oranı değiştirliyor.\n"

#define MSGTR_CacheFill "\rÖnbellek doyumu: %5.2f%% (%"PRId64" bayt)  "
#define MSGTR_NoBindFound "'%s' tuşu için bağ bulunamadı.\n"
#define MSGTR_FailedToOpen "%s açılamadı\n"

#define MSGTR_VideoID "[%s] Görüntü akışı bulundu, -vid %d\n"
#define MSGTR_AudioID "[%s] ses akışı bulundu, -aid %d\n"
#define MSGTR_SubtitleID "[%s] Altyazı akışı bulundu, -sid %d\n"

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "kodek açılamadı\n"
#define MSGTR_CantCloseCodec "kodek kapatılamadı\n"

#define MSGTR_MissingDLLcodec "HATA: Gerekli %s DirectShow kodeği açılamadı. \n"
#define MSGTR_ACMiniterror "Win32/ACM AUDIO kodeği yüklenemedi (dll dosyası hatalı mı?)\n"
#define MSGTR_MissingLAVCcodec " '%s' kodeği libavcodec içinde bulunamadı...\n"

#define MSGTR_MpegNoSequHdr "MPEG: HATA: Sıra başlığı arama esnasında EOF \n"
#define MSGTR_CannotReadMpegSequHdr "HATA: Sıra başlığı okunamıyor!\n"
#define MSGTR_CannotReadMpegSequHdrEx "HATA: Sıra başlığı eki okunamıyor!\n"
#define MSGTR_BadMpegSequHdr "MPEG: Hatalı sıra başlığı!\n"
#define MSGTR_BadMpegSequHdrEx "MPEG: Hatalı sıra başlığı eki!\n"

#define MSGTR_ShMemAllocFail "Paylaşımlı bellek atanamadı\n"
#define MSGTR_CantAllocAudioBuf "Ses çıkış tamponu atamadı\n"

#define MSGTR_UnknownAudio "Bilinmeyen/hatalı ses biçimi -> ses yok\n"

#define MSGTR_UsingExternalPP "[PP] Harici postprocessing filtresi kullanılıyor, max q = %d\n"
#define MSGTR_UsingCodecPP "[PP] Kodeklerin postprocessing işlemi kullanılıyor, max q = %d\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "İstenilen video kodek sınıfı [%s] (vfm=%s) kullanılabilir değil.\n(derlerken etkinleştirin.\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "İstenilen ses kodek sınıfı [%s] (afm=%s) kullanılabilir değil.\n(derlerken etkinleştirin!)\n"
#define MSGTR_OpeningVideoDecoder "Video dekoderi açılıyor: [%s] %s\n"
#define MSGTR_SelectedVideoCodec "Seçili video kodeği: [%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "Ses dekoderi açılıyor: [%s] %s\n"
#define MSGTR_SelectedAudioCodec "Seçili ses kodeği: [%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "Video Dekoderi açılamadı :(\n"
#define MSGTR_ADecoderInitFailed "Ses Dekoderi açılamadı :(\n"
#define MSGTR_ADecoderPreinitFailed "Ses Dekoderi önaçılışı başarısız :(\n"

// LIRC:
#define MSGTR_LIRCopenfailed "LIRC desteği açılamadı!\n"
#define MSGTR_LIRCcfgerr "%s LIRC ayar dosyası okunamadı!\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "'%s' video filtresi bulunamadı \n"
#define MSGTR_CouldNotOpenVideoFilter "'%s' video filtresi açılamadı \n"
#define MSGTR_OpeningVideoFilter "Video filtresi açılıyor: "
#define MSGTR_CannotFindColorspace "Eşlenen renk biçimi bulunamadı, hatta 'scale' seçeneği eklensede :(\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "Renk biçimi bulunamadı.  -vf scale ile tekrar deneniyor...\n"
#define MSGTR_MovieAspectIsSet "Video-Görünümü: %.2f:1 - doğru video görünümü için önölçekleniyor.\n"
#define MSGTR_MovieAspectUndefined "Video-Görünümü tanımsız - önölçekleme onaylanamadı.\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "İkili kodek paketini yükseltmeniz/yüklemeniz gerekiyor.\n http://www.mplayerhq.hu/dload.html\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11: EWMH tam ekran sonucu gönderilemedi!\n"
#define MSGTR_SelectedVideoMode "XF86VM: Seçili video modu %dx%d:  %dx%d görüntü boyutuna göre. \n"

#define MSGTR_InsertingAfVolume "[Karıştırıcı] Karıştırma donanımı yok, ses filtresi ekleniyor.\n"
#define MSGTR_NoVolume "[Karıştırıcı] Kullanılabilir ses kontrolü yok.\n"
#define MSGTR_NoBalance "[Karıştırıcı] Kullanılabilir denge kontrolü yok.\n"

// ================================ GUI ================================

#define MSGTR_GUI_AboutMPlayer "MPlayer Hakkında"
#define MSGTR_GUI_Add "Ekle"
#define MSGTR_GUI_AspectRatio "Görünüm Oranı"
#define MSGTR_GUI_Audio "Ses"
#define MSGTR_GUI_AudioDelay "Ses Gecikmesi"
#define MSGTR_GUI_AudioDriverConfiguration "Ses sürücüsü ayarları"
#define MSGTR_GUI_AudioTrack "Ek ses dosyası yükle"
#define MSGTR_GUI_AudioTracks "Ses parçası"
#define MSGTR_GUI_AvailableDrivers "Kullanılabilir sürücüler:"
#define MSGTR_GUI_AvailableSkins "Arayüzler"
#define MSGTR_GUI_Bass "Bas"
#define MSGTR_GUI_Blur "Bulanık"
#define MSGTR_GUI_Bottom "Dip"
#define MSGTR_GUI_Brightness "Parlaklık"
#define MSGTR_GUI_Browse "Gözat"
#define MSGTR_GUI_Cache "Önbellek"
#define MSGTR_GUI_CacheSize "Önbellek boyutu"
#define MSGTR_GUI_Cancel "İptal"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_Center "Merkez"
#define MSGTR_GUI_Channel1 "Kanal 1"
#define MSGTR_GUI_Channel2 "Kanal 2"
#define MSGTR_GUI_Channel3 "Kanal 3"
#define MSGTR_GUI_Channel4 "Kanal 4"
#define MSGTR_GUI_Channel5 "Kanal 5"
#define MSGTR_GUI_Channel6 "Kanal 6"
#define MSGTR_GUI_ChannelAll "Hepsi"
#define MSGTR_GUI_ChapterN "Bölüm %d"
#define MSGTR_GUI_ChapterNN "Bölüm %2d"
#define MSGTR_GUI_Chapters "Bölümler"
#define MSGTR_GUI_Clear "Temizle"
#define MSGTR_GUI_CodecFamilyAudio "Ses kodlama sınıfı"
#define MSGTR_GUI_CodecFamilyVideo "Video kodlama sınıfı"
#define MSGTR_GUI_CodecsAndLibraries " Kodekler ve üçüncü parti kütüphaneleri"
#define MSGTR_GUI_Coefficient "Katsayı"
#define MSGTR_GUI_ConfigFileError "ayar dosyası hatası\n"
#define MSGTR_GUI_Configure "Ayarla"
#define MSGTR_GUI_ConfigureDriver "Sürücüyü Ayarla"
#define MSGTR_GUI_Contrast "Karşıtlık"
#define MSGTR_GUI_Contributors " Kod ve belgelemeye katkıda bulunanlar"
#define MSGTR_GUI_Cp874 "Tayca karakterseti (CP874)"
#define MSGTR_GUI_Cp936 "Basit Çince karakterseti (CP936)"
#define MSGTR_GUI_Cp949 "Korece karakterseti (CP949)"
#define MSGTR_GUI_Cp1250 "Slavca/Orta Avrupa Windows (CP1250)"
#define MSGTR_GUI_Cp1251 "Kiril Windows (CP1251)"
#define MSGTR_GUI_Cp1256 "Arapça Windows (CP1256)"
#define MSGTR_GUI_CpBIG5 "Geleneksel Çince karakterseti (BIG5)"
#define MSGTR_GUI_CpISO8859_1 "Batı Avrupa Dilleri (ISO-8859-1)"
#define MSGTR_GUI_CpISO8859_2 "Slavca/Orta Avrupa Dilleri (ISO-8859-2)"
#define MSGTR_GUI_CpISO8859_3 "Esperanto, Galician, Malta dili, Türkçe (ISO-8859-3)"
#define MSGTR_GUI_CpISO8859_4 "Eski Baltık karakterseti (ISO-8859-4)"
#define MSGTR_GUI_CpISO8859_5 "Kiril (ISO-8859-5)"
#define MSGTR_GUI_CpISO8859_6 "Arapça (ISO-8859-6)"
#define MSGTR_GUI_CpISO8859_7 "Modern Yunanca (ISO-8859-7)"
#define MSGTR_GUI_CpISO8859_8 "İbrani karakterseti (ISO-8859-8)"
#define MSGTR_GUI_CpISO8859_9 "Türkçe (ISO-8859-9)"
#define MSGTR_GUI_CpISO8859_13 "Baltık (ISO-8859-13)"
#define MSGTR_GUI_CpISO8859_14 "Seltik (ISO-8859-14)"
#define MSGTR_GUI_CpISO8859_15 "Batı Avrupa Dilleri ve Euro(ISO-8859-15)"
#define MSGTR_GUI_CpKOI8_R "Rusça (KOI8-R)"
#define MSGTR_GUI_CpKOI8_U "Ukraynaca, Beyazrusça (KOI8-U/RU)"
#define MSGTR_GUI_CpShiftJis "Japonca karaktersetleri (SHIFT-JIS)"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_DefaultSetting "varsayılan sürücü"
#define MSGTR_GUI_Delay "Gecikme"
#define MSGTR_GUI_Demuxers_Codecs "Kodekler & Ayrıştırıcı"
#define MSGTR_GUI_Device "Aygıt"
#define MSGTR_GUI_DeviceCDROM "CD-ROM aygıtı"
#define MSGTR_GUI_DeviceDVD "DVD aygıtı"
#define MSGTR_GUI_Directory "Yol"
#define MSGTR_GUI_DirectoryTree "Dizin Ağacı"
#define MSGTR_GUI_DropSubtitle "Altyazı kaldır..."
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAssSubtitle "SSA/ASS altyazısı dönüştürülüyor"
#define MSGTR_GUI_EnableAutomaticAVSync "Otomatik eşzamanlama açık/kapalı"
#define MSGTR_GUI_EnableCache "Önbellek açık/kapalı"
#define MSGTR_GUI_EnableDirectRendering "Doğrudan sağlamayı etkinleştir"
#define MSGTR_GUI_EnableDoubleBuffering "Çift tamponlamayı etkinleştir"
#define MSGTR_GUI_EnableEqualizer "Sentezleyiciyi etkinleştir"
#define MSGTR_GUI_EnableExtraStereo "Ek stereo etkinleştir"
#define MSGTR_GUI_EnableFrameDropping "Kare (frame) atlamayı etkinleştir"
#define MSGTR_GUI_EnableFrameDroppingIntense "Ek kare (frame) atlamayı etkinleştir (tehlikeli)"
#define MSGTR_GUI_EnablePlaybar "Oynatma çubuğunu etkinleştir"
#define MSGTR_GUI_EnablePostProcessing "postprocessing filtresini etkinleştir"
#define MSGTR_GUI_EnableSoftwareMixer "Karıştırıcıyı etkinleştir"
#define MSGTR_GUI_Encoding "Karakterset"
#define MSGTR_GUI_Equalizer "Sentezleyici"
#define MSGTR_GUI_EqualizerConfiguration "Sentezleyici Ayarları"
#define MSGTR_GUI_Error "Hata!"
#define MSGTR_GUI_ErrorFatal "Önemli hata!"
#define MSGTR_GUI_File "Dosya oynat"
#define MSGTR_GUI_Files "Dosyalar"
#define MSGTR_GUI_Flip "Görüntüyü ters çevir"
#define MSGTR_GUI_Font "Yazıtipi"
#define MSGTR_GUI_FrameRate "FPS"
#define MSGTR_GUI_FrontLeft "Ön Sol"
#define MSGTR_GUI_FrontRight "Ön Sağ"
#define MSGTR_GUI_HideVideoWindow "Oynatım durduğunda video penceresini göster"
#define MSGTR_GUI_Hue "Renk"
#define MSGTR_GUI_Lavc "LAVC kullan (FFmpeg)"
#define MSGTR_GUI_MaximumUsageSpareCPU "Otomatik kalite"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "Çeşitli"
#define MSGTR_GUI_Mixer "Karıştırıcı"
#define MSGTR_GUI_MixerChannel "Karıştırıcı kanalı"
#define MSGTR_GUI_MSG_AddingVideoFilter "[GUI] Video filtresi ekleniyor: %s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "Üzgünüm, renk derinliği çok düşük.\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "D&D: Hiçbirşey geri dönmedi!\n"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "Üzgünüm, MPEG olmayan dosyaları DXR3/H+ aygıtınızla oynatamazsınız. \nLütfen DXR3/H+ ayarlarından lavc seçeneğini etkinleştirin.\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "[GUI] Altyazılar yükleniyor: %s\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "Üzgünüm, ilerleme tamponu için yeterli bellek yok.\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "Üzgünüm, tampon için yeterli hafıza yok.\n"
#define MSGTR_GUI_MSG_NoFileLoaded "Dosya yüklenemedi"
#define MSGTR_GUI_MSG_NoMediaOpened "Medya yok"
#define MSGTR_GUI_MSG_NotAFile0 " Bu bir dosya olarak görünmüyor...\n"
#define MSGTR_GUI_MSG_NotAFile1 "Bu bir dosya olarak görünmüyor: %s !\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "Yaptığınız değişiklikleri etkinleştirmek için lütfen Mplayer'ı yeniden başlatmayı unutmayınız."
#define MSGTR_GUI_MSG_RemoteDisplay "uzaktan kumanda göstergesi, XMITSHM devredışı bırakılıyor.\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "[GUI] Altyazılar siliniyor.\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "24 bit --> 32 bit dönüştürme hatası (%s)\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "( %s ) dosyası bulunamadı\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "PNG okuma hatası (%s)\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "Arayüz bulunamadı (%s).\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "Seçili arayüz ( %s ) bulunamadı , 'varsayılan'ı deneyiniz...\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "16 bit veya daha az derinlikteki bitmap desteklenmiyor (%s).\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "[arayüz] Hata : arayüz ayar dosyası %d satırı: %s"
#define MSGTR_GUI_MSG_SkinFileNotFound "[arayüz] ( %s ) dosyası bulunamadı.\n"
#define MSGTR_GUI_MSG_SkinFileNotReadable "[arayüz] ( %s ) dosyası okunabilir değil.\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "yazıtipi dosyası bulunamadı\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "yazıtipi görüntü dosyası bulunamadı\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "Varolmayan yazıtipi tanımlayıcısı (%s)\n"
#define MSGTR_GUI_MSG_SkinMemoryError "yetersiz bellek\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "Çok sayıda yazıtipi tanıtıldı\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "Bilinmeyen ileti: %s\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "bilinmeyen parametre (%s)\n"
#define MSGTR_GUI_MSG_TooManyWindows "Çok sayıda açık pencere var.\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "[cfg] '%s' seçeneği kaydedilemiyor.\n"
#define MSGTR_GUI_MSG_VideoOutError "Üzgünüm, KGA ile uyumlu video çıkış sürücüsü bulunamadı.\n"
#define MSGTR_GUI_MSG_XShapeError "Üzgünüm, sisteminiz Xshape genişletmesini desteklemiyor.\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "paylaşımlı bellek genişletme hatası\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "Üzgünüm, sisteminiz X paylaşımlı bellek genişletmesini desteklemiyor.\n"
#define MSGTR_GUI_Mute "Sessiz"
#define MSGTR_GUI_NetworkStreaming "Ağ Yayın İşlemi..."
#define MSGTR_GUI_Next "Sonraki yayın"
#define MSGTR_GUI_NoChapter "Bölüm yok"
#define MSGTR_GUI__none_ "(yok)"
#define MSGTR_GUI_NonInterleavedParser "Boşluksuz AVI dosyası kullan"
#define MSGTR_GUI_NormalizeSound "Sesi öntanımla"
#define MSGTR_GUI_Ok "Tamam"
#define MSGTR_GUI_Open "Aç..."
#define MSGTR_GUI_Original "Normal"
#define MSGTR_GUI_OsdLevel "OSD durumu"
#define MSGTR_GUI_OSD_Subtitles "Altyazı & OSD"
#define MSGTR_GUI_Outline "Altı çizili"
#define MSGTR_GUI_PanAndScan "Yanaltarama"
#define MSGTR_GUI_Pause "Duraklat"
#define MSGTR_GUI_Play "Oynat"
#define MSGTR_GUI_Playback "Oynatılıyor"
#define MSGTR_GUI_Playlist "Çalma listesi"
#define MSGTR_GUI_Position "Konum"
#define MSGTR_GUI_PostProcessing "Postprocessing Filtre"
#define MSGTR_GUI_Preferences "Tercihler"
#define MSGTR_GUI_Previous "Önceki yayın"
#define MSGTR_GUI_Quit "Çık"
#define MSGTR_GUI_RearLeft "Arka Sol"
#define MSGTR_GUI_RearRight "Arka Sağ"
#define MSGTR_GUI_Remove "Kaldır"
#define MSGTR_GUI_Saturation "Doygunluk"
#define MSGTR_GUI_SaveWindowPositions "Pencere konumunu kaydet"
#define MSGTR_GUI_ScaleMovieDiagonal "Köşegeni orantıla"
#define MSGTR_GUI_ScaleMovieHeight "Yüksekligi orantıla"
#define MSGTR_GUI_ScaleMovieWidth "Genişliği orantıla"
#define MSGTR_GUI_ScaleNo "Otomatik boyutlandırma yok"
#define MSGTR_GUI_SeekingInBrokenMedia "Gerekirse indeks-tablosunu yeniden oluştur"
#define MSGTR_GUI_SelectAudioFile "Ek ses kanalı seç..."
#define MSGTR_GUI_SelectedFiles "Seçili dosyalar"
#define MSGTR_GUI_SelectFile "Dosya Seç..."
#define MSGTR_GUI_SelectFont "Yazıtipi Seç..."
#define MSGTR_GUI_SelectSubtitle "Altyazı Seç..."
#define MSGTR_GUI_SizeDouble "Büyük boyut"
#define MSGTR_GUI_SizeFullscreen "Tam-Ekran"
#define MSGTR_GUI_SizeHalf "Küçük boyut"
#define MSGTR_GUI_SizeNormal "Normal boyut"
#define MSGTR_GUI_SizeOSD "OSD boyutu"
#define MSGTR_GUI_SizeSubtitles "Yazı boyutu"
#define MSGTR_GUI_SkinBrowser "Arayüz seçici"
#define MSGTR_GUI_Skins " Arayüzler"
#define MSGTR_GUI_Sponsored " Görüntü Arabirim'in geliştirilmesine UHU Linux katkıda bulunmuştur"
#define MSGTR_GUI_StartFullscreen "Tam ekran başlat"
#define MSGTR_GUI_Stop "Durdur"
#define MSGTR_GUI_Subtitle "Altyazı"
#define MSGTR_GUI_SubtitleAddMargins "Kenar boşluğu kullanın"
#define MSGTR_GUI_SubtitleAllowOverlap "Altyazı bindirmesini sabitle"
#define MSGTR_GUI_SubtitleAutomaticLoad "Altyazı otomatik yüklemesini kapat"
#define MSGTR_GUI_SubtitleConvertMpsub "Mevcut altyazıyı MPlayer'ın altyazı biçimine çevir"
#define MSGTR_GUI_SubtitleConvertSrt "Mevcut altyazıyı zaman tabanlı SubViewer (SRT) biçimine çevir"
#define MSGTR_GUI_Subtitles "Altyazılar"
#define MSGTR_GUI_SyncValue "Otomatik eşzamanlama"
#define MSGTR_GUI_TitleNN "Başlık %2d"
#define MSGTR_GUI_Titles "Başlıklar"
#define MSGTR_GUI_Top "Tepe"
#define MSGTR_GUI_TrackN "Parça %d"
#define MSGTR_GUI_Translations " Çeviriler"
#define MSGTR_GUI_TurnOffXScreenSaver "Ekran koruyucusunu kapat"
#define MSGTR_GUI_URL "URL oynat"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "Video"
#define MSGTR_GUI_VideoEncoder "Video kodlayıcı"
#define MSGTR_GUI_VideoTracks "Video parçası"
#define MSGTR_GUI_Volume MSGTR_Volume
#define MSGTR_GUI_Warning "Uyarı!"

// ======================= VO Video Output drivers ========================

#define MSGTR_VOincompCodec "Seçili video_çıkış aygıtı bu kodek ile bağdaşmıyor.\n"\
                  "scale filtresi eklemeyi deneyiniz, örn; -vf spp yerine -vf spp,scale.\n"
#define MSGTR_VO_GenericError "Bu hata oluştu"
#define MSGTR_VO_UnableToAccess "Erişilemiyor"
#define MSGTR_VO_ExistsButNoDirectory "zaten var, ancak bir dizin değil."
#define MSGTR_VO_DirExistsButNotWritable "Çıktı dizini zaten var, ancak yazılabilir değil."
#define MSGTR_VO_CantCreateDirectory "Çıktı dizini oluşturulamıyor."
#define MSGTR_VO_CantCreateFile "Çıktı dosyası oluşturulamıyor."
#define MSGTR_VO_DirectoryCreateSuccess "Çıktı dizini başarıyla oluşturuldu."
#define MSGTR_VO_ValueOutOfRange "Değer erişim dışı"

// vo_aa.c

#define MSGTR_VO_AA_HelpHeader "\n\n aalib vo_aa altseçenekleri:\n"
#define MSGTR_VO_AA_AdditionalOptions " vo_aa ek seçenekleri: Additional options vo_aa provides:\n" \
"  help        bu yardımı görüntüler\n" \
"  osdcolor    osd rengini ayarlar\n  subcolor    altyazı rengini ayarlar\n" \
"        renk parametreleri:\n           0 : normal\n" \
"           1 : donuk\n           2 : keskin\n           3 : kalınyazıtipi\n" \
"           4 : ters\n           5 : özel\n\n\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "Aşamalı JPEG etkin."
#define MSGTR_VO_JPEG_NoProgressiveJPEG "Aşamalı JPEG devredışı."
#define MSGTR_VO_JPEG_BaselineJPEG "Anahat JPEG etkin."
#define MSGTR_VO_JPEG_NoBaselineJPEG "Anahat JPEG devredışı."

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "ASCII modu etkin."
#define MSGTR_VO_PNM_RawMode "Raw modu etkin."
#define MSGTR_VO_PNM_PPMType "PPM dosyaları (yazılacak)."
#define MSGTR_VO_PNM_PGMType "PGM dosyaları (yazılacak)."
#define MSGTR_VO_PNM_PGMYUVType "PGMYUV dosyaları (yazılacak)."

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "Örme(interlaced) modu 4'e bölünebilme için görüntü yüksekliğini gerektiriyor."
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "Görüntü genişliği 2 ile bölünür olmalıdır."
#define MSGTR_VO_YUV4MPEG_OutFileOpenError " \"%s\" yazılması için bellek veya dosya kimlikleyiciye ulaşılamıyor!"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "Çıktıya görüntü yazma hatası!"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "Bilinmeyen alt-aygıt: %s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "Örme(interlaced) çıktı modu kullanılıyor, önce üst-alan."
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "Örme(interlaced) çıktı modu kullanılıyor, önce alt-alan."
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "Aşamalı kare modu kullanılıyor (varsayılan)."

// sub.c
#define MSGTR_VO_SUB_Seekbar "Arama çubuğu"
#define MSGTR_VO_SUB_Play "Oynat"
#define MSGTR_VO_SUB_Pause "Duraklat"
#define MSGTR_VO_SUB_Stop "Durdur"
#define MSGTR_VO_SUB_Rewind "Geri Sar"
#define MSGTR_VO_SUB_Forward "İleri Sar"
#define MSGTR_VO_SUB_Clock "Saat"
#define MSGTR_VO_SUB_Contrast "Karşıtlık"
#define MSGTR_VO_SUB_Saturation "Doygunluk"
#define MSGTR_VO_SUB_Volume "Ses"
#define MSGTR_VO_SUB_Brightness "Parlaklık"
#define MSGTR_VO_SUB_Hue "Renk"
#define MSGTR_VO_SUB_Balance "Denge"

// vo_xv.c
#define MSGTR_VO_XV_ImagedimTooHigh "Kaynak görüntü boyutları çok yüksek: %ux%u (maximum is %ux%u)\n"

// Old vo drivers that have been replaced

#define MSGTR_VO_PGM_HasBeenReplaced "pgm video çıktı sürücüsü -vo pnm:pgmyuv ile değiştirildi.\n"
#define MSGTR_VO_MD5_HasBeenReplaced "md5 video çıktı sürücüsü -vo md5sum ile değiştirildi.\n"

// ======================= AO Audio Output drivers ========================

// libao2

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "ses_çıkışı: alsa9 ve alsa1x modülleri silindi, yerine -ao seçeneğini kullanınız.\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] ses_kurulumu: %s karıştırıcı aygıtı açılamıyor: %s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] ses_kurulumu: Ses kartı karıştırıcısı '%s' kanalına sahip değil, varsayılan kullanılıyor.\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] ses_kurulumu: %s ses aygıtı açılamıyor: %s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] ses_kurulumu: Dosya tanımlayıcısı sağlanamıyor, engelleniyor: %s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] %s  ses aygıtı %s çıktısı için ayarlanamıyor, %s deneyiniz...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] ses_kurulumu: %d kanalları için ses aygıtı ayarlanamadı.\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] ses_kurulumu: sürücü SNDCTL_DSP_GETOSPACE özelliğini desteklemiyor :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   *** Ses sürücünüzün select() desteği YOK ***\nconfig.h dosyasında #undef HAVE_AUDIO_SELECT ile Mplayer'ı yeniden derleyiniz !\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\nÖnemli Hata: *** SES AYGITI TEKRAR AÇILAMIYOR *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] Bilinmeyen/desteklenmeyen OSS biçimi: %x.\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] Ses sunucusuna bağlanıldı.\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] Yayın açılamıyor.\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] Yayın açıldı.\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] tampon boyutu: %d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] %d : Ses ayarlanamadı.\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] %d Hz desteklenmiyor, yeniden örneklemeyi deneyiniz.\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_açık_ses başarısız: %s \n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] gecikme: [sunucu: %0.2fs, ağ: %0.2fs] (ayar %0.2fs)\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] ESD playback yayını açılamadı: %s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB ses karıştırıcısı başarısız: %s.\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] %d Hz desteklenmiyor, yeniden örneklemeyi deneyiniz.\n"

// ao_null.c
// This one desn't even  have any mp_msg nor printf's?? [CHECK]

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] Dosya File: %s (%s)\nPCM: Örnekleme oranı: %iHz Kanallar: %s Biçim %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] Bilgi: Daha hızlı döküm -vc null -vo null -ao pcm:fast ile sağlanabilir.\n[AO PCM] Bilgi: WAVE dosyalarını yazmak için  -ao pcm:waveheader (default) seçeneğini kullanınız.\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] Yazma için %s açılamadı!\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] Örnekleme oranı: %iHz Kanallar: %s Biçim %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] %s ses sürücüsü kullanılıyor.\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] Desteklenmeyen ses biçimi: 0x%x.\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] SDL Ses başlatılamadı: %s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] Ses açılamıyor: %s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] kontrol.\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] açılış: Örnekleme oranı: %iHz Kanallar: %s Biçim %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] Oynat:geçersiz aygıt .\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] açılış: setparams başarısız: %s\nİstenen örnekleme oranı ayarlanamadı.\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] açılış: Verilen kaynakta AL_RATE onaylanmadı.\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] açılış: getparams başarısız: %s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] açılış: örnekleme oranı şu anda %f  (istenen oran %f)\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] açılış: %s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] açılış: Ses kanalı açılamıyor: %s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] kapanış: ...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] sıfırla: ...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] sesi_duraklat: \n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] sesi_sürdür: \n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc: SETINFO başarısız.\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc: yazılamadı.\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] %s ses aygıtı açılamıyor, %s  -> ses yok.\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] ses_kurulumu: ses kartınız %d kanalını, %s, %d Hz  örnekleme oranını desteklemiyor.\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] Geçersiz karıştırıcı içeriği. Varsayılan olarak 0. yapılıyor\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] Karıştırıcı açma hatası: %s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] Karıştırıcı ekleme %s hatası: %s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] Karıştırıcı kayıt hatası: %s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] Katıştırıcı yükleme hatası: %s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] Basit kontrol bulunamıyor '%s',%i.\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] Sol kanal ayarlamada hata, %s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] Sağ kanal ayarlamada hata, %s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa komut satırı yardımı:\n"\
"[AO_ALSA] Örnek: mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   İlk kart, dördüncü donanım aygıtını ayarlar.\n\n"\
"[AO_ALSA] Seçenekler:\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     Aygıtı bloksuz modda açar.\n"\
"[AO_ALSA]   device=<aygıt-adı>\n"\
"[AO_ALSA]     Aygıtı ayarlar (',' '.' olarak ve ':' '=' değiştirilmeli)\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] %d tane kanal desteklenmiyor.\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] Bloksuz modda açılamadı, bloklu modda açma deneniyor.\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] Çalma açma hatası: %s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] Blok modu ayarlarken hata %s.\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] Başlangıç parametreeri alınamadı: %s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] Erişim türü ayarlanamadı: %s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] Biçim %s donanım tarafından desteklenmiyor, varsayılan deneniyor.\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] Format ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] Kanallar ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] Tekrar örnekleme ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] Örnekleme-oranı-2 ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] Buffer zamanı şu civarda ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] Periyot boyutu alınamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] Periyotlar ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] hw-parameters ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] buffersize alınamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] sw-parameters alınamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] Sınır alınamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] Eşik başlaması ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] Eşik bitişi ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] Sessizlik boyutu ayarlanamıyor: %s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] pcm kapatma hatası: %s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] Hiçbir handler tanımlanmamış!\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] pcm hazırlama hatası: %s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] pcm duraksama hatası: %s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] pcm bırakma hatası: %s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] pcm devam hatası: %s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] Aygıt ayarı hatası."
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm uyku modunda, devam etmeye çalışıyor.\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] Yazma hatası: %s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] Ses kartını sıfırlamaya çalışıyor.\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] pcm durumu alınamıyor: %s\n"

// ao_plugin.c


// ======================= AF Audio Filters ================================

// libaf
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c

#define MSGTR_AF_LADSPA_AvailableLabels "kullanılabilir etiketler"
#define MSGTR_AF_LADSPA_WarnNoInputs "UYARI! Bu LADSPA ekyazılımı(plugin) ses girdileri içermiyor.\n  Gelen ses sinyali kaybedilecek."
#define MSGTR_AF_LADSPA_ErrNoOutputs "Bu LADSPA ekyazılımı(plugin) ses çıktıları içermiyor."
#define MSGTR_AF_LADSPA_ErrInOutDiff "LADSPA ekyazılımı(plugin) ses girdisi ve çıktısının numaraları farklı."
#define MSGTR_AF_LADSPA_ErrFailedToLoad "yüklenemedi"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "Belirlenen kütüphane dosyasında ladspa_descriptor() foksiyonu bulunamadı."
#define MSGTR_AF_LADSPA_ErrLabelNotFound "Ekyazılım(plugin) kütüphanesinde etiket bulunamadı."
#define MSGTR_AF_LADSPA_ErrNoSuboptions "Altseçenek belirlenmedi"
#define MSGTR_AF_LADSPA_ErrNoLibFile "Kütüphane dosyası belirlenmedi"
#define MSGTR_AF_LADSPA_ErrNoLabel "Filtre etiketi belirlenmedi"
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "Komut satırında tanımlı kontroller yeterli değil"
#define MSGTR_AF_LADSPA_ErrControlBelow "%s: #%d girdi denetimi %0.4f alt sınırının altında.\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s: #%d girdi denetimi %0.4f üst sınırının üstünde.\n"

// format.c

#define MSGTR_AF_FORMAT_UnknownFormat "bilinmeyen-biçim"

// ========================== INPUT =========================================

// joystick.c

#define MSGTR_INPUT_JOYSTICK_CantOpen "%s kumanda kolu açılamadı: %s \n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "Kumanda kolu okuma hatası: %s \n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "Kumanda kolu: %d  bayt veri serbest bırakılıyor.\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "Kumanda kolu : açılış işlemi uyarısı: sürücüyle eşleşme kaybedildi.\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "Kumanda kolu uyarısı, bilinmeyen işlem türü %d\n"

// input.c

#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "Çok sayıda komut dosya tanımlayıcısı, %d dosya tanımlayıcı kaydedilemiyor.\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "Çok sayıda tuş dosya tanımlayıcısı, %d dosya tanımlayıcı kaydedilemiyor.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "%s komutu: %d değişkeni bir tamsayı değil.\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "%s komutu: %d değişkeni bir kayannokta değeri değil.\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "%s komutu:%d değişkeni eksik.\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "Bilinmeyen değişken %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "%s komutu en az %d değişkenini gerektiriyor, sadece %d tane bulundu.\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "Komut dosya tanımlayıcısı okuma hatası: %d: %s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "%d dosya tanımlayıcısı komut tamponu dolu: içerik bırakılıyor.\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "%s sekme tuşu için geçersiz komut"
#define MSGTR_INPUT_INPUT_ErrSelect "Seçim hatası: %s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd " Tuş girdisi %d dosya tanımlayıcısında hata\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "%d dosya tanımlayıcısında geçersiz tuş\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "Çok sayıda aynı anda aşağı tuş işlemi\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "%d komut dosya tanımlayıcısında hata\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "%s ayar dosyası girdi okumasında hata: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "Bilinmeyen tuş '%s'\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "Bu tuş adı için tampon çok küçük: %s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "%s tuşu için komut bulunamadı"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "%s komutu için tampon çok küçük\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "Kumanda kolu girdisi açılamıyor\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "%s açılamıyor: %s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "Apple Remote açılamıyor.\n"

// ========================== LIBMPDEMUX ===================================

// url.c

#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "%c%c%c url_atlamasında dizgi zaten atlatılmış görünüyor.\n"

// ai_alsa.c

#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "Örnekleme oranı ayarlanamıyor\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "Tampon zamanı ayarlanamıyor\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "Zaman aralığı ayarlanamıyor\n"

// ai_alsa.c

#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "Bu PCM için bozuk ayar: kullanılabilir ayar yok\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "Erişim türü kullanılabilir değil\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "Örnek biçimi kullanılabilir değil\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "Kanal sıralama kullanılabilir değil - varsayılana dönülüyor: %d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "hw parametreleri yüklenemiyor: %s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "Tampon boyutuna (%u == %lu) eşit zaman aralığı kullanılamıyor.\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "sw parametreleri yüklenemiyor:\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "Ses açılış hatası: %s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!! (en az %.3f ms uzunluğunda)\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun: hazırlama hatası: %s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA okuma/yazma hatası"

// ai_oss.c

#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "Kanal sıralama ayarlanamıyor: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "Stereo ayarlanamıyor: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "'%s' açılamıyor: %s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "Biçim desteklenmiyor.\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "Ses biçimi ayarlanamıyor."
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "Örnekleme oranı ayarlanamıyor: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "Başlatıcı ayarlanamıyor: %d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "Blok boyutuna ulaşılamıyor!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "Ses blok boyutu sıfır, %d olarak ayarlanıyor!\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "Ses blok boyutu çok düşük, %d olarak ayarlanıyor!\n"

// asfheader.c

#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "HATA: Başlık boyutu 1 MB'tan daha büyük (%d)!\n Lütfen MPlayer geliştiricilerine bildiriniz, ve bu dosyayı gönderiniz.\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "Başlık için %d bayt atanamadı\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "asf başlığı okunurken EOF, bozuk/hatalı dosya!\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "Muhtemelen DVR sadece libavformat ile çalışacak, sorun çıkarsa -demuxer 35 seçeneğini deneyiniz.\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "Belirtilen başlık için yığın(chunk) verisi yok!\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF: ses ve video başlıkları bulunamadı - bozuk dosya?\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "ASF başlığında geçersiz uzunluk!\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "DRM Lisansı URL'si: %s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "Bu dosya DRM şifrelemesiyle ipoteklenmiş, MPlayer'da çalınmaz!\n"

// asf_mmst_streaming.c

#define MSGTR_MPDEMUX_MMST_WriteError "Yazma hatası\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\nDikkat! EOF\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "ön-başlık okumamadı\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "Geçersiz başlık boyutu, vazgeçiliyor\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "Başlık verisi okunamadı.\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "packet_len okunamadı\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "Geçersiz rtsp paket boyutu, vazgeçiliyor\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "Komut verisi okunamadı\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "Başlık nesnesi\n"
#define MSGTR_MPDEMUX_MMST_DataObject "Veri nesnesi\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "Dosya nesnesi, paket uzunluğu=%d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "Yayın nesnesi, yayın id: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "Çok sayıda yayın, id atlanıldı."
#define MSGTR_MPDEMUX_MMST_UnknownObject "Bilinmeyen nesne\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "Medya verisi okunamadı\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "Hatalı imza\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "Herşey tamam. Tescilli ve patentli teknoloji içeren bir medya dosyası indirdiğiniz için teşekkür ederiz.\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "bilinmeyen komut %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "medya_paketine_ulaşma hatası : %s\n"
#define MSGTR_MPDEMUX_MMST_Connected "Bağlanıldı\n"

// asf_streaming.c

#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "yayın_yığın boyutu çok düşük: %d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "boyut_doğrulama uyumsuz!: %d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "Uyarı: başlığı atla ????\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "Yığın(chunk) başlığı ayrıştırma hatası\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "Bir başlığı ilk yığın(chunk) olarak alma!!!!\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "Hata: %d bayt tampon atanamıyor\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "Ağ yayını okunurken hata\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "Hata: Yığın(chunk) çok küçük\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "Bant genişliği çok küçük, dosya oynatılamıyor!\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "Bant genişliği çok küçük, ses yayın seçimini kaldırınız.\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "Bant genişliği çok küçük, video yayın seçimini kaldırınız\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "ASF başlığında geçersiz uzunluk!\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "Hata: yığın_boyutu > paket_boyutu\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF yönlendirici\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "Geçersiz vekil sunucu URL'si\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "Bilinmeyen asf yayın türü\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "HTTP yanıtı ayrıştırılamadı\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "Sunucu dönüşü %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP AYRIŞTIMA UYARISI : Pragma %s cuted from %zu bytes to %zu\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Soket yazma hatası: %s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "Başlık ayrıştırılamadı\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "Yayın(stream) bulunamadı\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "Bilinmeyen ASF yayın türü\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF, URL: %s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "Başarısız, çıkılıyor\n"

// audio_in.c

#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\nSes okuma hatası: %s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "Ters-çalışma'dan kurtarıldı, bazı kareler atlanabilir!\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "Önemli Hata, kurtarılamıyor!\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\nSes örnekleri yeterli değil!\n"

// aviheader.c

#define MSGTR_MPDEMUX_AVIHDR_EmptyList "** boş liste?!\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** uyarı: genişletilmiş AVI başlığı değil..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI: ODML: odml indeksi oluşturuluyor (%d süperindeksyığınları)\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI: ODML: Bozuk (eksik?) dosya saptandı. Olağan indeksleme kullanılacak\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "%s indeks dosyası okunamıyor: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s geçerli bir MPlayer indeks dosyası değil\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "%s dosyası indeks verisine göre bellek atanamadı\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "%s indeks dosyası erken sonlandı\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "İndeks dosyası yüklendi: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "İndeks yenileniyor: %3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI: %d yığını için indeks tablosu yenilendi!\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "%s indeks dosyası yazılamadı: %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "İndeks dosyası kaydedildi: %s\n"

// cache2.c


// cdda.c

#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "CDDA aygıtı açılamıyor.\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "CD açılamıyor.\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "%d parça içeren ses CD'si bulundu.\n"

// cddb.c

#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "TOC okunamadı.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "%s aygıtı açılamadı.\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "Geçerli bir URL değil\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "HTTP sorgusu gönderilemedi.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "HTTP yanıtı okunamadı.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "Bulunamadı.\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "Bilinmeyen Hata kodu\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "Önbellek bulunamadı.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "Tüm xmcd dosyaları okunamadı.\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "%s dizini oluşturulamadı.\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "Tüm xmcd dosyaları yazılamadı.\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "Dönen xmcd veritabanı dosyası geçersiz.\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "Beklenmeyen FIXME\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "Yürütülmeyen kod\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "Satır sonu bulunamıyor\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "Ayrıştırma TAMAM, bulunan: %s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "Albüm bulunamadı\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "Sunucu dönüşleri: komut sözdizim hatası\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "Protokol seviyesi alınamadı\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "Sürücüde CD yok\n"

// cue_read.c

#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] Beklenmeyen cue dosya satırı: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] bin dosya adı test edildi: %s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] bin dosyası bulunamadı, vazgeçiliyor.\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] %s bin dosyası kullanılıyor\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] bin dosyası için bilinmeyen mod. Bu olmamalıydı. Durduruluyor.\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue]  %s açılamıyor\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] %s okuma hatası\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] bin dosya boyutuna ulaşım hatası\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "parça %02d: biçim=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] beklenmeyen bin dosya sonu\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] Yüklemenin %d baytı okunamadı\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE yayın_açık, dosya adı=%s, parça =%d, mevcut parçalar: %d -> %d\n"

// network.c

#define MSGTR_MPDEMUX_NW_UnknownAF "Bilinmeyen adres sınıfı %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "Adres çözülüyor %s , %s için...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "%s için ad çözülemedi: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "Bağlantı kuruluyor %s[%s]: %d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "%s ile sunucuya bağlanılamadı\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "Seçim başarısız.\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "Bağlantı zamanaşımı.\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt başarısız: %s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "Bağlantı hatası: %s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "Geçersiz vekil sunucu ayarı... Vekil sunucusuz deneyiniz.\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "AF_INET için uzak host adı çözülemedi. Vekil sunucusuz deneyiniz.\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "HTTP sorgu gönderme hatası: tüm sorgu göderilmedi.\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "Okunamadı.\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_okuma_yanıtı 0 olarak okunuyor (yani EOF)\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "Kimlik Denetimi yapılmadı. Lütfen URL listesi veya benzer biçime göre\n"\
"kullanıcı adınıza/parolanıza ulaşmak için -user ve -passwd seçeneklerini kullanınız:\n"\
"http://username:password@hostname/file\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "%s için kimlik denetimi gereklidir\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "Kimlik denetimi gereklidir.\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "Sağlanan parola yok, boş parola deneyiniz.\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "Sunucu dönüşü %d: %s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "Önbellek boyutu %d KBayt olarak ayarlanıyor.\n"

// demux_audio.c

#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "Ses ayrıştırıcısı: bilinmeyen biçim %d.\n"

// demux_demuxers.c

#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "tampon_doyum hatası: hatalı ayrıştırıcı: vd değil, ad veya sd.\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] zlib başlatması başarısız.\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib çıkarması başarısız.\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo çıkarması başarısız.\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] %u numaralı parça şifrelenmiş ve şifre çözümü henüz\n[mkv] içeriğe alnmamış. Parça atlanıyor.\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] %u numaralı parça için bilinmeyen içerik şifrelemesi. Parça atlanıyor.\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] %u numaralı parça bilinmeyen/desteklenmeyen bir algoritmayla sıkıştırılmış\n[mkv] (%u). Parça atlanıyor.\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] %u numaralı parça zlib ile sıkıştırılmış fakat mplayer henüz derlenmemiş zlib sıkıştırma desteğiyle\n[mkv]. Parça atlanıyor.\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] Parça ID %u: %s (%s) \"%s\", %s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] Parça ID %u: %s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] Bilinmeyen/desteklenmeyen CodecID (%s) veya eksik/bozuk CodecPrivate\n[mkv] verisi (parça %u).\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] FLAC parçası geçerli başlıklar içermiyor.\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] Bilinmeyen/desteklenmeyen ses kodeği ID '%s', %u numaralı parça için   veya eksik/hatalı\n[mkv] özel kodek verisi.\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] '%s' altyazı türü desteklenmiyor.\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] %u numaralı görüntü parçası çalınacak.\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] Hiçbir görüntü parçası bulunmadı/istenmedi.\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] Hiçbir ses parçası bulunmadı/istenmedi.\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] Uyarı: Altyazı parçası için hiçbir BlockDuration bulunamadı.\n"

// demux_nuv.c


// demux_xmms.c

#define MSGTR_MPDEMUX_XMMS_FoundPlugin "Ekyazılım(plugin) bulundu: %s (%s).\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "Ekyazılım(plugin) kapatılıyor: %s.\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "XMMS ekyazılımının(plugin) şunu çalmaya başlaması bekleniyor: '%s'...\n"

// ========================== LIBMPMENU ===================================

// common

#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] Menü tanımında giriş bulunamadı.\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] Sözdizim hatası: %d\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] Menü tanımları için bir ad özelliği gerekiyor (satır %d)\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] Hatalı özellik %s=%s  '%s' menüsünde %d satırında\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] Bilinmeyen menü türü '%s'    %d satırında \n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] Menü ayar dosyası açılamıyor: %s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] Ayar dosyası çok büyük (> %d KB)\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] Ayar dosyası boş\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] %s menüsü bulunamadı.\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] Menu '%s': Açılamadı.\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] Desteklenmeyen çıktı biçimi!!!!\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] Liste menüsü giriş tanımları için bir ad gerekiyor(satır %d).\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] Liste menüsü için bir değişken gerekiyor.\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Waitpid hatası: %s.\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Seçim hatası.\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] Alt süreçte okuma hatası: %s.\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] Console işlemi: %s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] Bir alt süreç zaten çalışıyor.\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Çatallama başarısız !!!\n"
#define MSGTR_LIBMENU_WriteError "[MENU] Yazma hatası.\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] Dizin açma hatası: %s.\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] Tekrar atama hatası: %s.\n"
#define MSGTR_LIBMENU_MallocError "[MENU] Bellek atama hatası: %s.\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] Dizin okuma hatası: %s.\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] %s dizini açılamıyor\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] Altmenü tanımı bir 'menü' özelliği gerektiriyor.\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] Tercihler menü girişinde geçersiz özellik '%s' (satır %d).\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] Tercihler menü girişi geçerli bir 'sahiplik' özelliği gerektiriyor. (satır %d).\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] Tercihler menüsü için bir değişken gerekiyor.\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] Hedef nesne bulunmadı ????\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] komut oluşturulamadı: %s.\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] Metin menüsü bir txt dosya adı gerektiriyor. (parametre dosyası).\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] Açılamıyor: %s.\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] Uyarı!, satır çok uzun. Bölünüyor.\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] %d satırları ayrıştırıldı.\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] Bilinmeyen komut: '%s'.\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] Menü açılamadı: '%s'.\n"

// ========================== LIBMPCODECS ===================================

// libmpcodecs/ad_dvdpcm.c:
#define MSGTR_SamplesWanted "Bu şekildeki örnekler için geliştirme desteği gerekiyor. Lütfen geliştiricilerle bağlantı kurunuz.\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] Uyarı! Ses veri boyutu farklılık gösteriyor.! okuma=%d  hdr=%d.\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] cinepak kodeğine görüntü atanamadı.\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] QP'nin aritmetik karşılığı : %2.4f, QP'nin harmonik karşılığı: %2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI hatası.\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] Kodeğine görüntü atanmadı.\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] XVMC hızlı MPEG2.\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] Deneyiniz pixfmt=%d.\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG]  mc_get_buffer sadece XVMC hızlandırmasıyla çalışmalı!!"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] Sadece izin verilen vo_xvmc ile tamponlar atandı.\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] Yüksek kalite kodlama seçildi (gerçek zamanlı değil)!\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] qscale = %f (VBR) sabiti kullanılıyor.\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] fourcc [%x] ile raw çıktısı desteklenmiyor!\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] Gerekli VfW kodeği belirlenmedi!\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] Hatalı konum/genişlik/yükseklik - Mevcut alanın dış bölümleri kırpıldı!\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] Kırpılan alan: X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d).\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] Bilinmeyen biçim adı: '%s'.\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] Değişken ayrıştırma hatası.\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "Sıkıştırıcı türü: %.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "Sıkıştırıcı alttürü: %.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "Sıkıştırıcı etiketleri: %lu, sürüm %lu, ICM sürüm: %lu\n"
#define MSGTR_MPCODECS_Flags "Etiketler:"
#define MSGTR_MPCODECS_Quality " kalite"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "Tam DR olanaksız, yerine SLICES seçeneğini deneyiniz!\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "Neden NULL ulaşıyoruz??\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "%s sonraki filtre/vo tarafından desteklenmiyor :(\n"

// ================================== LIBMPVO ====================================

// mga_template.c

#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] mga_video_ayarı'nda ioctl hatası (yanlış mga_vid.o sürümü?)"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] Kernel modülünden luma değerlerine ulaşılamadı!\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] Kernel modülünden luma değerleri ayarlanamadı!\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] Ekran genişliği/yüksekliği bilinmiyor!\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] Geçersiz çıkış biçimi %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] mga_video sürücünüzün sürümü bu MPlayer sürümüyle bağdaşmıyor!\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] Açılamadı: %s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] kaynak çözünürlüğün en azιndan bir boyutu 1023x1023'ten daha büyükütür. Yazılımla düzteliniz ya da -lavdopts lowres=1 kulanınız\n"

// libvo/vesa_lvo.c

#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] Bu sınıf artık desteklenmiyor.\n[VESA_LVO] Lütfen onun yerine -vo vesa:vidix kullanınız.\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] Açılamadı: '%s'\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] Geçersiz çıktı biçimi: %s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] fb_video sürücünüzün sürümü bu MPlayer sürümüyle bağdaşmıyor!\n"

// libvo/vo_3dfx.c

#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] Sadece 16bpp destekleniyor!"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] Görsel ID %lx.\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] /dev/3dfx açılamadı.\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] Hata: %d.\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] 3dfx bellek alanları eşlenemedi: %p,%p,%d.\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] Başlatıldı: %p.\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] Bilinmeyen altaygıt: %s.\n"

// libvo/aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] Uyarı: Hiçbir uygun yeni çözünürlük bulunamadı!\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] Hata: Çözünürlüğe uygun hiçbir yeni boyut bulunamadı!\n"

// libvo/vo_dxr3.c

#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] Yeni SPU paleti yüklenemedi!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] Oynatma modu ayarlanamadı!\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] Altresim modu ayarlanamadı!\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] TV standardına ulaşılamadı!\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] Kare oranına göre TV standardı otomatik seçildi: "
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] TV standardı ayarlanamadı!\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] NTSC için ayarlanıyor.\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] PAL/SECAM için ayarlanıyor.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] Görünüm oranı 4:3 olarak ayarlanıyor.\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] Görünüm oranı 16:9 olarak ayarlanıyor.\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] yetersiz bellek\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] keycolor atanamadı!\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] Tam keycolor atanamadı, en yakın eşleşme kullanılıyor (0x%lx).\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] Kapatılıyor.\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] TV standardı yenilenemedi!\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] Öntamponlama etkinleştiriliyor.\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] Yeni eşzamanlama motoru kullanılıyor.\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] Bindirme kullanılıyor.\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] Hata: Bindirme yüklü kütüphaneler/başlıklar ile derlemeyi gerektiriyor.\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] TV standardı ayarlanıyor: "
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "video kare oranını (PAL/PAL-60) otomatik ayarla"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "video kare oranını (PAL/NTSC) otomatik ayarla"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "Geçerli standardı kullan."
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "Bilinmeyen standart sağlandı. Geçerli standardı kullan."
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] Yazma esnasında %s açılış hatası, yerine /dev/em8300 deneyiniz.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] Yazma esnasında %s açılış hatası, yerine /dev/em8300_mv deneyiniz.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] Yazma esnasında ayrıca /dev/em8300 açılış hatası!\n.Bırakılıyor.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] Yazma esnasında ayrıca /dev/em8300_mv açılış hatası!\nBırakılıyor.\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] Açıldı: %s.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] Yazma esnasında %s açılış hatası, yerine /dev/em8300_sp deneyiniz.\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] Yazma esnasında ayrıca /dev/em8300_sp açılış hatası!\nBırakılıyor.\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] Bindirme kaba kurulumu esnasında gösterge açılamadı!\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] X11 açılmadı!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] Bindirme özelliği ayarlanamadı.\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] Bindirme ekranı ayarlanamadı!\nÇıkılıyor.\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] Bindirme etkinleştirilemedi!\nÇıkılıyor.\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] Bindirme penceresi tekrar boyutlandırılamadı!\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] bcs bindirmesi ayarlanamadı!\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] Y-sapma değerleri bindirilemedi!\nÇıkılıyor.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] X-sapma değerleri bindirilemedi!\nÇıkılıyor.\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] X ölçek düzeltimi bindirilemedi!\nÇıkılıyor.\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y-sapması: %d.\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X-sapması: %d.\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X-düzeltimi: %d.\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] Sinyal karıştırma ayarlanamadı!\n"

// font_load_ft.c

#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "New_Face başarısız. Yazıtipi yolu yanlış olabilir.\nLütfen yazıtipi dosyasını sağlayın (~/.mplayer/subfont.ttf).\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "New_Memory_Face başarısız..\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "subtitle font: load_sub_face başarısız.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "subtitle font: prepare_charset başarısız.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "Altyazı yazıtipi hazırlanamıyor.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "OSD yazıtipi hazırlanamıyor.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "Tablolar oluşturulamıyor.\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "FT_Done_FreeType başarısız.\n"

// libvo/vo_mga.c

#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect(): %dx%d olarak tekrar boyutlandırılıyor.\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] Kapat!\n"

// libvo/vo_null.c

#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] Bilinmeyen altaygıt: %s.\n"

// libvo/vo_png.c

#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] Uyarı: sıkıştırma seviyesi 0 olarak ayarlanıyor, sıkıştırma devre dışı!\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] Bilgi: Sıkıştırma seviyesini 0'dan 9'a kadar ayarlamak için -vo png:z=<n> kullanınız.\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] Bilgi: (0 = sıkıştırma yok, 1 = en düşük, en hızlı - 9 en iyi, en yavaş sıkıştırma)\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] Yazma esnasında '%s' açılış hatası!\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] png_oluşturma hatası.\n"

// libvo/vo_sdl.c

#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] Çıktı için herhangi uyumlu SDL moduna ulaşılamadı.\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] Desteklenmeyen görüntü biçimi (0x%X).\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] Bilgi: En iyi çözünürlük için lütfen -vm veya -zoom kullanınız.\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] Video modu ayarlanamadı: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] YUV bindirmesi oluşturulamadı: %s.\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] RGB görünümü oluşturulamadı: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] Derinlik/renk biçimi dönüşümü kullanılıyor, Bu durum nesneleri yavaşlatacak (%ibpp -> %ibpp).\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] ilerleme_dilimi'nde desteklenmeyen görüntü biçimi, MPlayer geliştiricilerine bildiriniz!\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] Blit başarısız: %s.\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] SDL açılışı başarısız: %s.\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] Sürücü kullanılıyor: %s.\n"

// libvo/vobsub_vidix.c

#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] playback başlatılamıyor: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX]  playback durdurulamıyor: %s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] YUV410P için boşluklu UV desteklenmiyor.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] Yapay vidix_draw_slice() çağrıldı.\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] Bu VIDIX sürücüsü için FourCC desteklenmiyor: %x (%s).\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] Video sunucusu (%dx%d) çözünürlüğünü desteklemiyor, desteklenen: %dx%d-%dx%d.\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] Video sunucusu vidix (%d) ile renk derinliğini desteklemiyor.\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX sürücüsü ile görüntü yukarı ölçeklenemiyor (%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX sürücüsü ile görüntü aşağı ölçeklenemiyor(%d%d -> %d%d).\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] playback ayarlanamıyor: %s.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] Çalışan VIDIX sürücüsü bulunamadı.\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] Kapasiteye ulaşılamadı: %s.\n"

// libvo/vo_svga.c

#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] Zorlanan video_modu %d (%s) kullanılabilir değil.\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] Zorlanan video_modu %d (%s) çok küçük.\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Video_modu: %d, %dx%d %dbpp.\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode(%d) başarısız.\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] Video modu doğrusal(linear) ve görüntü aktarımı için memcpy kullanılabildi.\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] Video modu donanım hızlandırma içeriyor ve put_image kullanılabildi.\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] Sizde çalışıyorsa bilmek isterim.\n[VO_SVGA] (Lütfen komut çıktısını gönderiniz `mplayer test.avi -v -v -v -v &> svga.log`). Tşk!\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] Video modu %d sayfa(lar) içeriyor.\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] Ortalanan görüntü. (%d,%d) ile başlatılıyor.\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] VIDIX kullanılıyor. w=%i h=%i  mw=%i mh=%i\n"

// libvo/vo_tdfxfb.c

#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] %s açılamıyor: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] FBITGET_FSCREENINFO ioctl ile problem: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] FBITGET_VSCREENINFO ioctl ile problem: %s.\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] Bu sürücü sadece 3Dfx Banshee, Voodoo3 and Voodoo 5'i destekliyor.\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] %d bpp çıktısı desteklenmiyor.\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] Bellek alanları eşlenemedi: %s.\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] %d bpp çıktısı desteklenmiyor (Bu asla oluşmamalıydı).\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] Eik! control() ile hatalı bir nesne.\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] Bu video'yu oynatmak için yeterli bellek yok. Düşük bir çözünürlükte deneyiniz.\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] %dx%d görüntüsü (%d bpp için), %dx%d ( %d bpp için), standart %dx%d.\n"

// libvo/vo_tdfx_vid.c

#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] Yürüt %d(%d) x %d => %d.\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] Ekranı temizlemek için AGP yürütülemedi.\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] Blit başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] Doğal olmayan bindirme biçimi dönüşüm gerektiriyor.\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] 0x%x girdi biçimi desteklenmiyor.\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] Bindirme ayarlanamadı.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] Bindirme açılışı başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] Bindirme hazır: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] Doku blit hazır: %d(%d) x %d @ %d => %d(%d) x %d @ %d.\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] Bindirme kapanışı başarısız\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] %s açılamıyor: %s.\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] Geçerli ayarlamaya ulaşılamıyor: %s.\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap başarısız !!!!!\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "İşlenecek görüntüye ulaş.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP yürütme başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] YUV ayarlanamadı.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] Y düzleminde AGP yürütme başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] U düzleminde AGP yürütme başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] V düzleminde AGP yürütme başarısız.\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] bilinmeyen biçim: 0x%x.\n"

// libvo/vo_tga.c

#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] Bilinmeyen altaygıt: %s.\n"

// libvo/vo_vesa.c

#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] Önemli hata oluştu! Devam edilemiyor.\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] bilinmeyen altaygıt: '%s'.\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] Bu mod için çok az video belleğiniz var:\n[VO_VESA] Gereken: %08lX Mevcut: %08lX.\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] Monitörünüzü özelliklerine göre tanımlamalısınız. Tazeleme oranı değiştirilemiyor.\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] Bu mod ile monitör sınırları uyarlanamaz. Tazeleme oranı değiştirilemiyor.\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] Önemli hata bulundu: açılış daha önce önaçılış olarak adlandırılır.\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] -flip seçeneği desteklenmiyor.\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] Olası neden: VBE2 BIOS bulunamadı.\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] VESA VBE BIOS Sürümü bulundu %x.%x Revizyon: %x.\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] Video belleği: %u Kb.\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA Yetenekleri: %s %s %s %s %s.\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] !!! OEM bilgisi aşağıda görüntülenecek !!!\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] Aşağıdaki ilgili 5 OEM satırını görmelisiniz; Göremediyseniz vm86 bozuk olabilir..\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM bilgisi: %s.\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM Revizyonu: %x.\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM satıcısı: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM Ürün adı: %s.\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM Ürün Revizyonu: %s.\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] İpucu: TV-Out çıkışı için TV konnektörüne sahip olmalısınız.\n"\
"[VO_VESA] sistem açılış öncesi sadece POST esnasında VESA BIOS kendiliğinden başlar.\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] VESA modu kullanılıyor (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] Yazılım ölçekleyici açılamadı.\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] DGA kullanılamıyor. Yığın anahtarlama modunu zorlayınız. :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] DGA kullanılıyor (fiziksel kaynaklar: %08lXh, %08lXh)"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] Çifte tamponlama kullanılamıyor: video belleği yetersiz.\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] Ne DGA ne de tekrar yerleştirilebilir pencere bulunamadı.\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] DGA seçimini zorladınız. Çıkılıyor\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] Geçerli pencere adresi bulunamadı.\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] Yığın anahtarlama modu kullanılıyor (fiziksel kaynaklar: %08lXh, %08lXh).\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] Geçici tampon atanamıyor.\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] Üzgünüm, desteklenmeyen mod -- deneyiniz -x 640 -zoom.\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] Gerçekten TV'nizde bir film var!\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] Linux Video Bindirmesi başlatılamıyor.\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] Video bindirme kullanılıyor: %s.\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] VIDIX sürücüsü açılamıyor.\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] VIDIX kullanılıyor.\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] Mod bulunamıyor: %ux%u@%u.\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] VESA açılışı tamamlandı.\n"

// libvo/vo_x11.c


// libvo/vo_xv.c

#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] Bellek paylaşımı desteklenmiyor\nNormale döndürülüyor Xv.\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] Özür, Xv bu X11 versiyonu/sürücüsü tarafından desteklenmiyor\n[VO_XV] ********  -vo x11  veya  -vo sdl ile deneyin  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors başarısız.\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] Geçersiz port parametresi, port 0 ile üstünden geçiyor.\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] Port %i ele geçirilemedi.\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] Boş Xvideo portu bulunamadı - başka bir program halihazırda kullanıyor\n"\
"[VO_XV] olabilir. Tüm görüntü uygulamalarını kapatıp tekrar deeyin. Eğer bu\n"\
"[VO_XV] yardımcı olmazsa, diğer görüntü çıkış sürücüleri (xv-olmayan) için\n"\
" 'mplayer -vo help' komutunu deneyin\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] Ekran kartınız için uygun Xvideo desteği yok gibi görünüyor.\n"\
"[VO_XV] kartınızın Xv desteğini onaylamak için 'xvinfo' komutunu girip \n"\
"[VO_XV] DOCS/HTML/en/video.html#xv! dosyasını okuyun\n"\
"[VO_XV] Diğer görüntü çıkış sürücüleri(xv-olmayan) için 'mplayer -vo help' komutuna bakın.\n"\
"[VO_XV] -vo x11 komutunu deneyin.\n"


// loader/ldt_keeper.c

#define MSGTR_LOADER_DYLD_Warning "WARNING: Attempting to use DLL codecs but environment variable\n         DYLD_BIND_AT_LAUNCH not set. This will likely crash.\n"


// stream/stream_radio.c

#define MSGTR_RADIO_ChannelNamesDetected "[radio] Radyo kanal adları bulundu.\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] %s kanalı için yanlış frekans.\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] %.2f yanlış kanal numarası.\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] %d yanlış kanal numarası.\n"
#define MSGTR_RADIO_WrongChannelName "[radio] %s yanlış kanal adı.\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] Radyo frekans değiştirgesi bulundu.\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] Uyarı: ioctl ayarlayıcsı alınamadı: %s. frac %d olarak ayarlanıyor\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s bir radyo aygıtı değildir!\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl frekansı 0x%x (%.2f) göre ayarlanamamıştır, %s başarısız.\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl frekansı al, %s başarısız.\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl sesi kes, %s başarısız.\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl sorgu kontrolü, %s başarısız\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl sesi al, %s\n başarısız."
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl sesi ayarla, %s\n başarısız."
#define MSGTR_RADIO_AllocateBufferFailed "[radio] ses için ara bellek bulanamadı (blok=%d,arabel=%d): %s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] Mevcut frekans: %.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] Seçilen kanal: %d - %s (frek: %.2f)\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] Kanal değiştirelemez: Kanal listesine kanal verilmedi.\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] '%s': %s açılamadı.\n"
#define MSGTR_RADIO_WrongFreq "[radio] %.2f : Frekansı yanlış.\n"
#define MSGTR_RADIO_UsingFreq "[radio] %.2f: Frekansı kullanılıyor\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init başarılamadı.\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] audio_in_setup %s\n: çağrısı başarılamadı."
#define MSGTR_RADIO_ClearBufferFailed "[radio] %s: Ara belleği temizleme başarılamadı."
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] %s: stream_enable_cache çağrısı başarılamadı."
#define MSGTR_RADIO_DriverUnknownStr "[radio] Bilinmeyen sürücü adı: %s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] V4Lv2 radyo görünümü kullanılıyor.\n"
#define MSGTR_RADIO_DriverV4L "[radio] V4Lv1 radyo görünümü kullanılıyor.\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] *BSD BT848 radyo arayüzü kullanılıyor.\n"

// ================================== LIBASS ====================================

// ass_bitmap.c

// ass.c
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s): fopen başarısız\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s): 100 megabayttan büyük altyazıları yüklemeyi reddediyor\n"

// ass_cache.c

// ass_fontconfig.c

// ass_render.c

// ass_font.c

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c: norm_from_string(%s): Dandik norm parametresi, %s olarak ayarlanıyor.\n"
#define MSGTR_TV_NoVideoInputPresent "Hata: Hiçbir görüntü girişi mevcut değil!\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" UYARI: TEST EDİLMEMİŞ VEYA BİLİNMEYEN ÇIKTI RESİM BİÇİMİ İÇİN İSTEKTE BULUNULDU (0x%x)\n"\
" Bu durum hatalı çalmaya veya programın göçmesine neden olabilir! Hata raporları\n"\
" gözardı edilecek! YV12 ile tekrar denemelisiniz (varsayılan\n"\
" renk uzayı) ve belgeleri okuyun!\n"\
"==================================================================\n"
#define MSGTR_TV_CannotSetNorm "Hata: Norm ayarlanamıyor!\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP: genişlik %d yükseklik %d\n"
#define MSGTR_TV_UnableToSetWidth "İstek yapılan genişlik ayarlanamıyor: %d\n"
#define MSGTR_TV_UnableToSetHeight "İstek yapılan yükseklik ayarlanamıyor: %d\n"
#define MSGTR_TV_NoTuner "Seçilen girişin bir ayarlayıcı yok!\n"
#define MSGTR_TV_UnableFindChanlist "Seçilen kanal listesi bulunamıyor! (%s)\n"
#define MSGTR_TV_ChannelFreqParamConflict "Aynı anda frekans ve kanal ayarlanamaz!\n"
#define MSGTR_TV_ChannelNamesDetected "TV kanal isimleri algılandı.\n"
#define MSGTR_TV_NoFreqForChannel "%s kanalı için frekans bulunamadı (%s)\n"
#define MSGTR_TV_SelectedChannel3 "Seçilen kanal: %s - %s (frekans: %.3f)\n"
#define MSGTR_TV_SelectedChannel2 "Seçien kanal: %s (frekans: %.3f)\n"
#define MSGTR_TV_UnsupportedAudioType "Ses tipi '%s (%x)' desteklenmiyor!\n"
#define MSGTR_TV_AvailableDrivers "Kullanılabilir sürücüler:\n"
#define MSGTR_TV_DriverInfo "Seçilen sürücü: %s\n isim: %s\n yazar: %s\n yorum: %s\n"
#define MSGTR_TV_NoSuchDriver "Böyle bir sürücü yok: %s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "TV sürücü oto-algılama başarısız.\n"
#define MSGTR_TV_UnknownColorOption "Bilinmeyen renk seçeneği (%d) belirtildi!\n"
#define MSGTR_TV_NoTeletext "Teletext yok"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848: %s ioctl çağrısı başarısız. Hata: %s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848: Geçersiz ses oranı. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848: bktr aygıtı açılamıyor. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848: Ayarlayıcı aygıt açılamıyor. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848: dsp aygıtı açılamıyor. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848: dsp ayarı başarısız. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848: Ses verisini okumada hata. Hata: %s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848: mmap başarısız. Hata: %s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848: Kare(frame) buffer ataması başarısız. Hata: %s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848: Resim genişliğini ayarlamada hata. Hata: %s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848: Yakalama durdurulamıyor. Hata: %s\n"
#define MSGTR_TV_TTSupportedLanguages "Desteklenen Teletext dilleri:\n"
#define MSGTR_TV_TTSelectedLanguage "Seçilen Teletext dili: %s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "Kanal tarayıcı ayarlayıcı olmadan kullanılamaz\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "Verilen giriş görüntü çözücüye bağlanamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "Verilen giriş ses çözücüye bağlanamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow: Görüntü biçimi seçilemiyor. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow: Ses biçimi seçilemiyor. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow: IMediaControl arayüzü alınamıyor. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow: Grafik başlatılamıyor! Hata:0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow: Aygıt #%d bulunamadı\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow: Aygıt #%d için isim alınamadı\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow: Aygıt #%d kullanılıyor: %s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow: Frekans doğrudan alınamadı. OS gömülü kanal tablosu kullanılacak.\n"
//following phrase will be printed near the selected audio/video input
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow: kstvtune.ax dosyasından frekans tablosu yüklenemedi\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow: Yanlış aygıt parametresi: %s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow: Yanlış aygıt indeksi: %d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow: Yanlış aygıt(adevice) parametresi: %s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow: Yanlış aygıt(adevice) indeksi: %d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow: Örnekleme-oranı %d aygıt tarafından desteklenmiyor. İlk kullanılabilir olana dönülüyor.\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow: parlaklık/renk/doygunluk/karşıtlık ayarlaması aygıt tarafından desteklenmiyor\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow: Görüntü genişliği/yükseliği değişimi aygıt tarafından desteklenmiyor.\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow: Yakalama kaynağının seçimi aygıt tarafından desteklenmiyor\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow: Ses biçimi yapısı ayrıştırılamadı.\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow: Görüntü biçimi yapısı ayrıştırılamadı.\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow: Ses modu ayarlanamadı: %d. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow: Desteklenmeyen ortam tipi şuraya gönderildi: %s\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow: Sistem frekans tablosunda en yakın kanal bulunamadı.\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow: Sistem frekans tablosundan en yakın kanala geçiş yapılamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow:  VideoPort pini grafikte filtrelerle bitirilemedi. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow: Yakalama grafiğinin görüntü zinciri oluşturulamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow: Yakalama grafiğinin ses zinciri oluşturulamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow: Yakalama grafiğinin VBI zinciri oluşturulamadı. Hata:0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow: Directshow grafik başlatılması hatası.\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow: Görüntü yakalama aygıtı bulunamadı.\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow: Ses yakalama aygıtı bulunamadı.\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow: Asıl ortam tipi alınamadı (Hata:0x%x). İstek yapılana eşit olduğu varsayılıyor.\n"
