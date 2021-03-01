// Synced with help_mp-en.h rev. 37846
//
// Translated by JRaSH <jrash06@gmail.com>









// ========================= MPlayer help ===========================

#ifdef CONFIG_VCD
#define MSGTR_HelpVCD " vcd://<曲目号>   播放 (S)VCD（超级VCD）曲目（无格式设备，无需装载）\n"
#else
#define MSGTR_HelpVCD
#endif

#ifdef CONFIG_DVDREAD
#define MSGTR_HelpDVD " dvd://<标题号>   从设备而不是从普通文件上播放 DVD 标题\n"
#else
#define MSGTR_HelpDVD
#endif

#define MSGTR_Help \
"用法：            mplayer [选项] [URL|路径/]文件名\n"\
"\n"\
"基本选项：        （完整列表参见手册页）\n"\
" -vo <drv>        选择视频输出驱动程序（查看驱动程序列表用“-vo help”）\n"\
" -ao <drv>        选择音频输出驱动程序（查看驱动程序列表用“-ao help”）\n"\
MSGTR_HelpVCD \
MSGTR_HelpDVD \
" -alang/-slang    选择 DVD 音频/字幕的语言（使用两字符的国家代号）\n"\
" -ss <位置>       定位至给定的（秒数或时:分:秒 - hh:mm:ss）位置\n"\
" -nosound         不播放声音\n"\
" -fs              全屏播放（或用 -vm、-zoom，详见手册相关页面）\n"\
" -x <x> -y <y>    设置显示的分辨率（用以与 -vm 或 -zoom 一起使用）\n"\
" -sub <文件>      指定所使用的字幕文件（另见 -subfps、-subdelay）\n"\
" -playlist <文件> 指定播放列表文件\n"\
" -vid x -aid y    选择播放视频流（x）和音频流（y）\n"\
" -fps x -srate y  改变视频帧率为 x（fps），音频采样率为 y（Hz）\n"\
" -pp <质量>       启用后期处理过滤器（详见于手册相关页面）\n"\
" -framedrop       启用丢帧（用于运行慢的机器）\n"\
"\n"\
"基本控制键：      （完整列表见手册相关页面，也请查阅 input.conf）\n"\
" <-  或  ->       后退/快进 10 秒\n"\
" 上 或 下         后退/快进 1 分钟\n"\
" pgdown 或 pgup   后退/快进 10 分钟\n"\
" < 或 >           跳到播放列表中的前一个/后一个\n"\
" p 或 空格键      暂停影片（按任意键继续）\n"\
" q 或 ESC         停止播放并退出程序\n"\
" + 或 -           音频延迟 +/- 0.1 秒\n"\
" o                循环切换 OSD 模式：无/定位条/定位条加计时器\n"\
" * 或 /           增加或减少 PCM 音量\n"\
" x 或 z           字幕延迟 +/- 0.1 秒\n"\
" r 或 t           字幕位置上移/下移，另见“-vf expand”\n"\
"\n"\
" * * * 参见手册相关页面可获取具体内容，及更多（高级）选项和控制键的信息 * * *\n"\
"\n"

static const char help_text[] = MSGTR_Help;

// ========================= MPlayer messages ===========================

// mplayer.c
#define MSGTR_Exiting "\n正在退出..\n"
#define MSGTR_ExitingHow "\n正在退出...（%s）\n"
#define MSGTR_Exit_quit "退出"
#define MSGTR_Exit_eof "文件末尾"
#define MSGTR_Exit_error "致命错误"
#define MSGTR_IntBySignal "\nMPlayer 被 %d 信号中断（位于 %s 模块）\n"
#define MSGTR_NoHomeDir "无法找到 HOME 目录\n"
#define MSGTR_GetpathProblem "get_path(\"config\") 调用出现问题\n"
#define MSGTR_CreatingCfgFile "创建配置文件：%s\n"
#define MSGTR_CantLoadFont "无法加载位图字体‘%s’\n"
#define MSGTR_CantLoadSub "无法加载字幕‘%s’\n"
#define MSGTR_DumpSelectedStreamMissing "内核导出：致命错误：指定的媒体流不存在！\n"
#define MSGTR_CantOpenDumpfile "无法打开内核导出文件。\n"
#define MSGTR_CoreDumped "内核已导出 :)\n"
#define MSGTR_DumpBytesWrittenPercent "导出：已写入 %"PRIu64" 字节（~%.1f%%）\r"
#define MSGTR_DumpBytesWritten "导出：已写入 %"PRIu64" 字节\r"
#define MSGTR_DumpBytesWrittenTo "导出：已将 %"PRIu64" 字节写入‘%s’中。\n"
#define MSGTR_FPSnotspecified "FPS 在文件头中没有指定或者无效，请使用 -fps 选项。\n"
#define MSGTR_TryForceAudioFmtStr "尝试强制使用音频编解码器驱动类 %s...\n"
#define MSGTR_CantFindAudioCodec "无法找到音频格式 0x%X 的编解码器。\n"
#define MSGTR_TryForceVideoFmtStr "尝试强制使用视频编解码器驱动类 %s...\n"
#define MSGTR_CantFindVideoCodec "无法找到匹配所选 -vo 参数和视频格式 0x%X 的编解码器。\n"
#define MSGTR_CannotInitVO "致命错误：无法初始化视频驱动！\n"
#define MSGTR_CannotInitAO "无法打开/初始化音频设备 -> 禁用声音。\n"
#define MSGTR_StartPlaying "开始播放...\n"

#define MSGTR_SystemTooSlow "\n\n"\
"         ************************************************\n"\
"         ****     你的系统运行太“慢”，无法播放！     ****\n"\
"         ************************************************\n"\
" 可能的原因、问题和变通的办法：\n"\
"- 最常见的原因：损坏的或有漏洞的 _audio_ 驱动\n"\
"  - 试试 -ao sdl 或使用 ALSA  的 OSS 模拟方式。\n"\
"  - 尝试使用不同的 -autosync 的值，不妨从 30 开始。\n"\
"- 视频输出运行慢\n"\
"  - 试试 -vo 用不同的驱动（参见 -vo help 以获取驱动列表）或者试试 -framedrop！\n"\
"- CPU 运行慢\n"\
"  - 不要试图在运行慢的 CPU 上播放大容量的 DVD/DivX！试试 lavdopts 中的一些选项，\n"\
"    例如：-vfm ffmpeg -lavdopts lowres=1:fast:skiploopfilter=all。\n"\
"- 文件损坏\n"\
"  - 试试组合使用 -nobps -ni -forceidx -mc 0 这些选项。\n"\
"- 媒体读取慢（NFS/SMB 挂载、DVD、VCD 等设备）\n"\
"  - 试试 -cache 8192 选项。\n"\
"- 你是否在用 -cache 选项播放一个非交错合并的 AVI 文件？\n"\
"  - 试试 -nocache 选项。\n"\
"阅读 DOCS/zh/video.html 和 DOCS/zh/sound.html，寻找调整/加速的技巧。\n"\
"如果这些一个都帮不了你，请阅读 DOCS/zh/bugreports.html。\n\n"

#define MSGTR_NoGui "MPlayer 编译时没有包含图形界面的支持。\n"
#define MSGTR_GuiNeedsX "MPlayer 的图形界面需要 X11。\n"
#define MSGTR_Playing "\n正在播放 %s。\n"
#define MSGTR_NoSound "音频：没有音轨\n"
#define MSGTR_FPSforced "FPS 强制设为 %5.3f（ftime：%5.3f）。\n"
#define MSGTR_AvailableVideoOutputDrivers "可用的视频输出驱动：\n"
#define MSGTR_AvailableAudioOutputDrivers "可用的音频输出驱动：\n"
#define MSGTR_AvailableAudioCodecs "可用的音频编解码器：\n"
#define MSGTR_AvailableVideoCodecs "可用的视频编解码器：\n"
#define MSGTR_AvailableAudioFm "\n可用的（编译时已包含的）音频编解码器类/驱动：\n"
#define MSGTR_AvailableVideoFm "\n可用的（编译时已包含的）视频编解码器类/驱动：\n"
#define MSGTR_AvailableFsType "可用的全屏图层变换模式：\n"
#define MSGTR_CannotReadVideoProperties "视频：无法读取视频属性\n"
#define MSGTR_NoStreamFound "未找到媒体流。\n"
#define MSGTR_ErrorInitializingVODevice "打开/初始化所选的（-vo）视频输出设备出错。\n"
#define MSGTR_ForcedVideoCodec "强制使用视频编解码器：%s\n"
#define MSGTR_ForcedAudioCodec "强制使用音频编解码器：%s\n"
#define MSGTR_Video_NoVideo "视频：没有视频流\n"
#define MSGTR_NotInitializeVOPorVO "\n致命错误：无法初始化视频过滤器（-vf）或视频输出（-vf）。\n"
#define MSGTR_Paused "  =====  暂停  =====" // no more than 23 characters (status line for audio files)
#define MSGTR_PlaylistLoadUnable "\n无法装载播放列表 %s\n"
#define MSGTR_Exit_SIGILL_RTCpuSel \
"- “非法指令”导致 MPlayer 崩溃。\n"\
"  这可能是我们新的实时 CPU 侦测代码中的一个缺陷...\n"\
"  请阅读 DOCS/zh/bugreports.html。\n"
#define MSGTR_Exit_SIGILL \
"- “非法指令”导致 MPlayer 崩溃。\n"\
"  这通常是在你运行 MPlayer 的 CPU 与其\n"\
"  编译/优化时所针对的 CPU 不同时发生。\n"\
"  检查是否如此！\n"
#define MSGTR_Exit_SIGSEGV_SIGFPE \
"- 不合理使用 CPU/FPU/RAM 导致 MPlayer 崩溃。\n"\
"  使用 --enable-debug 重新编译 MPlayer 并建立“gdb”反跟踪和\n"\
"  反汇编输出。具体细节见 DOCS/zh/bugreports.html#crash。\n"
#define MSGTR_Exit_SIGCRASH \
"- MPlayer 崩溃了。这不应该发生。\n"\
"  这可能是 MPlayer 或者 你的驱动中 或者 你的 gcc 版本中的一个缺陷。\n"\
"  如你觉得这是 MPlayer 的问题，请阅读 DOCS/zh/bugreports.html\n"\
"  并遵循其中的指示去做。除非你在报告一个潜在的缺陷时候提供我们\n"\
"  所需要的信息，否则我们不能也不会帮助你。\n"
#define MSGTR_LoadingConfig "正在加载配置 '%s'\n"
#define MSGTR_LoadingProtocolProfile "加载协议相关的配置集‘%s’\n"
#define MSGTR_LoadingExtensionProfile "加载扩展组件相关的配置集‘%s’\n"
#define MSGTR_AddedSubtitleFile "字幕：添加字幕文件（%d）：%s\n"
#define MSGTR_RemovedSubtitleFile "字幕：移除字幕文件（%d）： %s\n"
#define MSGTR_RTCDeviceNotOpenable "未能打开 %s：%s（用户应当有权限读取该设备。）\n"
#define MSGTR_LinuxRTCInitErrorIrqpSet "ioctl (rtc_irqp_set %lu) 中出现 Linux RTC 初始化错误：%s\n"
#define MSGTR_IncreaseRTCMaxUserFreq "尝试添加 \"echo %lu > /proc/sys/dev/rtc/max-user-freq\" 到你的系统启动脚本中。\n"
#define MSGTR_LinuxRTCInitErrorPieOn "ioctl (rtc_pie_on) 中出现 Linux RTC 初始置错误：%s\n"
#define MSGTR_Getch2InitializedTwice "警告：getch2_init 被两次调用！\n"
#define MSGTR_CantOpenLibmenuFilterWithThisRootMenu "无法用根菜单 %s 打开 libmenu 视频过滤器。\n"
#define MSGTR_AudioFilterChainPreinitError "音频过滤器链预初始化错误！\n"
#define MSGTR_LinuxRTCReadError "Linux RTC 读取错误：%s\n"
#define MSGTR_SoftsleepUnderflow "警告！Softsleep 数值下溢！\n"
#define MSGTR_MasterQuit "选项 -udp-slave：因主动方中止而中止\n"
#define MSGTR_InvalidIP "选项 -udp-ip：无效的 IP 地址\n"
#define MSGTR_Forking "正在创建子进程...\n"
#define MSGTR_Forked "已经创建子进程...\n"
#define MSGTR_CouldntStartGdb "无法启动 gdb\n"
#define MSGTR_CouldntFork "无法创建子进程\n"
#define MSGTR_FilenameTooLong "文件名太长，无法加载用于特定文件或目录的配置文件\n"
#define MSGTR_AudioDeviceStuck "音频设备已堵塞！\n"
#define MSGTR_AudioOutputTruncated "音频输入信号在尾端截断。\n"
#define MSGTR_ASSCannotAddVideoFilter "ASS：无法添加视频过滤器\n"
#define MSGTR_PtsAfterFiltersMissing "缺失过滤器后的 PTS\n"
#define MSGTR_CommandLine "命令行："
#define MSGTR_MenuInitFailed "菜单初始化失败。\n"
#define MSGTR_InvalidSwitchRatio "无效的宽高比‘%f’，宽高比将保持不变。\n"

// --- edit decision lists
#define MSGTR_EdlOutOfMem "无法分配足够的内存以存放 EDL 数据。\n"
#define MSGTR_EdlOutOfMemFile "无法分配足够的内存以存放 EDL 文件名 [%s]。\n"
#define MSGTR_EdlRecordsNo "读取 %d 个 EDL 行为。\n"
#define MSGTR_EdlQueueEmpty "没有 EDL 行为要处理。\n"
#define MSGTR_EdlCantOpenForWrite "无法打开并写入 EDL 文件 [%s]。\n"
#define MSGTR_EdlNOsh_video "无视频流时无法使用 EDL，禁用该功能。\n"
#define MSGTR_EdlNOValidLine "无效的一行 EDL 文本：%s\n"
#define MSGTR_EdlBadlyFormattedLine "格式错误的一行 EDL 文本 [%d]，丢弃该行。\n"
#define MSGTR_EdlBadLineOverlap "上一个终止位置是 [%f]；下一个起始位置是 [%f]。\n"\
"每一项内容必须按时间顺序排列，不能重叠。 丢弃该行。\n"
#define MSGTR_EdlBadLineBadStop "终止时间必须位于起始时间之后。\n"
#define MSGTR_EdloutBadStop "取消 EDL 跳跃，上一个开始位置 > 停止位置\n"
#define MSGTR_EdloutStartSkip "开始 EDL 跳跃，再按键‘i’以指定跳过内容的结尾。\n"
#define MSGTR_EdloutEndSkip "结束 EDL 跳跃，文本行已写入。\n"

// mplayer.c OSD
#define MSGTR_OSDenabled "已启用"
#define MSGTR_OSDdisabled "已停用"
#define MSGTR_OSDAudio "音频：%s"
#define MSGTR_OSDChannel "频道：%s"
#define MSGTR_OSDSubDelay "字幕延迟：%d 毫秒"
#define MSGTR_OSDSpeed "播放速度：x %6.2f"
#define MSGTR_OSDosd "OSD：%s"
#define MSGTR_OSDChapter "章节：（%d）%s"
#define MSGTR_OSDAngle "视角：%d/%d"
#define MSGTR_OSDDeinterlace "解除隔行扫描：%s"
#define MSGTR_OSDCapturing "抓取：%s"
#define MSGTR_OSDCapturingFailure "抓取失败"

// property values
#define MSGTR_Enabled "已启用"
#define MSGTR_EnabledEdl "已启用（EDL）"
#define MSGTR_Disabled "已停用"
#define MSGTR_HardFrameDrop "强制丢帧"
#define MSGTR_Unknown "未知"
#define MSGTR_Bottom "底部"
#define MSGTR_Center "中部"
#define MSGTR_Top "顶部"
#define MSGTR_SubSourceFile "字幕文件"
#define MSGTR_SubSourceVobsub "VOBSUB"
#define MSGTR_SubSourceDemux "附带字幕"

// OSD bar names
#define MSGTR_Volume "音量"
#define MSGTR_Panscan "全景模式"
#define MSGTR_Gamma "伽玛值"
#define MSGTR_Brightness "亮度"
#define MSGTR_Contrast "对比度"
#define MSGTR_Saturation "饱和度"
#define MSGTR_Hue "色调"
#define MSGTR_Balance "均衡"

// property state
#define MSGTR_LoopStatus "循环模式：%s"
#define MSGTR_MuteStatus "静音：%s"
#define MSGTR_AVDelayStatus "A-V 延迟: %s"
#define MSGTR_OnTopStatus "保持在前面：%s"
#define MSGTR_RootwinStatus "根窗口：%s"
#define MSGTR_BorderStatus "边框：%s"
#define MSGTR_FramedroppingStatus "丢帧：%s"
#define MSGTR_VSyncStatus "垂直刷新同步：%s"
#define MSGTR_SubSelectStatus "字幕：%s"
#define MSGTR_SubSourceStatus "字幕来源：%s"
#define MSGTR_SubPosStatus "字幕位置：%s/100"
#define MSGTR_SubAlignStatus "字幕对齐方式：%s"
#define MSGTR_SubDelayStatus "字幕延迟：%s"
#define MSGTR_SubScale "字幕缩放：%s"
#define MSGTR_SubVisibleStatus "显示字幕：%s"
#define MSGTR_SubForcedOnlyStatus "只用强制使用的字幕：%s"

// mencoder.c
#define MSGTR_UsingPass3ControlFile "使用第三阶段的控制文件：%s\n"
#define MSGTR_MissingFilename "\n没有指定文件名。\n\n"
#define MSGTR_CannotOpenFile_Device "无法打开文件/设备。\n"
#define MSGTR_CannotOpenDemuxer "无法打开流分离器。\n"
#define MSGTR_NoAudioEncoderSelected "\n没有选择音频编码器（-oac）。请选择一个编码器（参见 -oac help）或者使用 -nosound。\n"
#define MSGTR_NoVideoEncoderSelected "\n没有选择视频编码器（-ovc）。请选择一个编码器（参见 -ovc help）。\n"
#define MSGTR_CannotOpenOutputFile "无法打开输出文件‘%s’。\n"
#define MSGTR_EncoderOpenFailed "未能打开编码器。\n"
#define MSGTR_MencoderWrongFormatAVI "\n警告：输出文件格式是 _AVI_。参见 -of help。\n"
#define MSGTR_MencoderWrongFormatMPG "\n警告：输出文件格式是 _MPEG_。参见 -of help。\n"
#define MSGTR_MissingOutputFilename "没有指定输出文件，请参见 -o 选项。"
#define MSGTR_ForcingOutputFourcc "强制输出内容的 FourCC 为 %x [%.4s]。\n"
#define MSGTR_ForcingOutputAudiofmtTag "强制输出音频的格式标签为 0x%x。\n"
#define MSGTR_DuplicateFrames "\n%d 个重复的帧！\n"
#define MSGTR_SkipFrame "\n跳帧中！\n"
#define MSGTR_ResolutionDoesntMatch "\n新的视频文件和前一个文件的分辨率或色彩空间不同。\n"
#define MSGTR_FrameCopyFileMismatch "\n所有的视频文件必须使用同样的帧率、分辨率和编解码器才能使用 -ovc copy。\n"
#define MSGTR_AudioCopyFileMismatch "\n所有的音频文件必须使用同样的音频编解码器和格式才能使用 -oac copy。\n"
#define MSGTR_NoAudioFileMismatch "\n无法把只有视频流的文件与含有音频和视频的文件混在一起使用。请尝试使用 -nosound。\n"
#define MSGTR_NoSpeedWithFrameCopy "警告：无法保证 -speed 能和 -oac copy 一起正常工作！\n"\
"你的编码过程可能会有错误！\n"
#define MSGTR_ErrorWritingFile "%s：写入文件出错。\n"
#define MSGTR_FlushingVideoFrames "\n立即输出视频帧缓冲中的内容。\n"
#define MSGTR_FiltersHaveNotBeenConfiguredEmptyFile "过滤器尚未配置！文件为空？\n"
#define MSGTR_RecommendedVideoBitrate "%s CD 的推荐视频比特率为：%d\n"
#define MSGTR_VideoStreamResult "\n视频流：%8.3f kbit/s （%d B/s） 大小：%"PRIu64" 字节  %5.3f 秒  %d 帧\n"
#define MSGTR_AudioStreamResult "\n音频流：%8.3f kbit/s （%d B/s） 大小：%"PRIu64" 字节  %5.3f 秒\n"
#define MSGTR_EdlSkipStartEndCurrent "EDL 跳跃：开始位置：%.2f  结束位置：%.2f   当前位置：V：%.2f  A：%.2f     \r"
#define MSGTR_OpenedStream "成功：格式：%d  数据：0x%X - 0x%x\n"
#define MSGTR_VCodecFramecopy "视频编解码器：复制帧（%dx%d %dbpp fourcc=%x）\n"
#define MSGTR_ACodecFramecopy "音频编解码器：复制帧（format=%x chans=%d rate=%d bits=%d B/s=%d sample-%d）\n"
#define MSGTR_MP3AudioSelected "已选择 MP3音频。\n"
#define MSGTR_SettingAudioDelay "设置音频延迟为 %5.3fs。\n"
#define MSGTR_SettingVideoDelay "设置视频延迟为 %5.3fs。\n"
#define MSGTR_LimitingAudioPreload "限制音频预加载长度为 0.4s。\n"
#define MSGTR_IncreasingAudioDensity "增加音频密度至 4。\n"
#define MSGTR_ZeroingAudioPreloadAndMaxPtsCorrection "强制音频预加载长度为 0，最大 PTS 校验长度为 0。\n"
#define MSGTR_LameVersion "LAME 版本 %s（%s）\n\n"
#define MSGTR_InvalidBitrateForLamePreset "错误：指定的比特率超出该预设配置的有效范围。\n"\
"\n"\
"当使用该模式时，你必须输入一个\"8\"到\"320\"之间的数值。\n"\
"\n"\
"获取更多信息，请尝试：\"-lameopts preset=help\"\n"
#define MSGTR_InvalidLamePresetOptions "错误：你没有给 preset 指定一个有效的配置集和/或选项。\n"\
"\n"\
"可用的配置集包括:\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr>（ABR 模式） - 默认使用的是 ABR 模式。\n"\
"                      要使用该模式，只要指定一个比特率就行了。\n"\
"                      例如：“preset=185”启用该预设配置，\n"\
"                      使用 185 作为平均比特率。\n"\
"\n"\
"    一些例子：\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" or \"-lameopts  cbr:preset=192       \"\n"\
" or \"-lameopts      preset=172       \"\n"\
" or \"-lameopts      preset=extreme   \"\n"\
"\n"\
"要获取更多信息，请尝试使用：\"-lameopts preset=help\"\n"
#define MSGTR_LamePresetsLongInfo "\n"\
"预设配置开关经过设计以提供最好的编码质量。\n"\
"\n"\
"大多数配置已经经过严格的双盲聆听的测试和调整，以验证并达到这个目标。\n"\
"\n"\
\
"这些配置经过不断升级以跟上最近的开发成果，所以应该能给你提供目前 LAME 所能提供的\n"\
"将近最好的质量。\n"\
"\n"\
\
"要启用这些预设配置：\n"\
"\n"\
"   使用 VBR 模式（通常质量最高）：\n"\
"\n"\
"     “preset=standard” 该预设配置在处理大多数音乐上，通常大多数人应该是感\n"\
"                         觉不到差异的，其质量已经相当高。\n" \
"\n"\
\
"     “preset=extreme”  如果你有极好的听力和相当的设备，该预设配置一般会比\n"\
"                        “standard”模式提供更高一点的质量。\n"\
"\n"\
\
\
"   使用 CBR 320kbps（预设配置开关选项里的最高质量）：\n"\
"\n"\
"     “preset=insane”   对于大多数人在大多数情况下，该选项通常有些过度。但是\n"\
"                         如果你一定要有最高质量并且完全不关心文件大小，那这正\n"\
"                         是适合你的。\n"\
"\n"\
\
"   使用 ABR 模式（给定比特率下提供较高质量，但不及 VBR 质量高）：\n"\
"\n"\
"     \"preset=<kbps>\"  使用该预设配置通常会在一个指定的比特率下提错良好的质量。\n"\
"                        根据输入的比特率，预设配置将判断该情形下的最优设置。虽\n"\
"                        然该方法行之有效，但并没有 VBR 模式那么灵活，并且通常\n"\
"                        在高比特率下达不到 VBR 所具有的同等质量。\n"\
"\n"\
\
\
\
"以下选项在相应的配置集中也可使用：\n"\
"\n"\
"   <fast>        standard\n"\
"   <fast>        extreme\n"\
"                 insane\n"\
"   <cbr>（ABR Mode）- 默认使用的是 ABR 模式。要使用该模式，只要\n"\
"                      指定一个比特率就行了。例如：“preset=185”\n"\
"                      启用该预设配置，使用 185 作为平均比特率。\n"\
"\n"\
\
"   “fast” - 在特定的配置集中启用新的高速 VBR 模式。速度开关的坏处是\n"\
"              比特率往往比普通模式下稍高，并且质量也会稍低一点。\n"\
\
\
"      警告：在当前版本下，高速预设配置可能产生比一般模式高太多的比特率。\n"\
"\n"\
\
"   “cbr”  - 如果你使用 ABR 模式（见上）时指定了一个比特率, 如 80、\n"\
"              96、112、128、160、192、224、256、320，你可以使用\n"\
"              “cbr”选项强制以 CBR 模式编码代替标准 ABR 模式。ABR\n"\
"              固然提供更高的质量，但是 CBR 在某些情况下可能会相当有\n"\
"              用，比如当在因特网上传送 MP3 流可能十分重要时。\n"\
"\n"\
\
"    举例：\n"\
"\n"\
"    \"-lameopts fast:preset=standard  \"\n"\
" 或 \"-lameopts  cbr:preset=192       \"\n"\
" 或 \"-lameopts      preset=172       \"\n"\
" 或 \"-lameopts      preset=extreme   \"\n"\
"\n"\
"\n"\
"ABR 模式下有一些可用的别名：\n"\
"phone => 16kbps/mono        phon+/lw/mw-eu/sw => 24kbps/mono\n"\
"mw-us => 40kbps/mono        voice => 56kbps/mono\n"\
"fm/radio/tape => 112kbps    hifi => 160kbps\n"\
"cd => 192kbps               studio => 256kbps"
#define MSGTR_LameCantInit \
"无法设定 LAME 选项，请检查比特率/采样率，一些非常低的比特率（<32）需要低采样率\n"\
"（如 -srate 8000）。\n"\
"如果其它方法都不行，请尝试使用预设配置。"
#define MSGTR_ConfigFileError "配置文件错误"
#define MSGTR_ErrorParsingCommandLine "解析命令行错误"
#define MSGTR_VideoStreamRequired "必须有视频流！\n"
#define MSGTR_ForcingInputFPS "输入帧率将视为 %5.3f。\n"
#define MSGTR_DemuxerDoesntSupportNosound "该流分离器还不支持 -nosound。\n"
#define MSGTR_MemAllocFailed "内存分配失败。\n"
#define MSGTR_NoMatchingFilter "没找到匹配的过滤器/音频输出格式!\n"
#define MSGTR_MP3WaveFormatSizeNot30 "sizeof(MPEGLAYER3WAVEFORMAT)==%d!=30，C 编译器可能已损坏？\n"
#define MSGTR_NoLavcAudioCodecName "音频 LAVC，没有指定编解码器名称！\n"
#define MSGTR_LavcAudioCodecNotFound "音频 LAVC，无法找到编解码器 %s 对应的编码器。\n"
#define MSGTR_CouldntAllocateLavcContext "音频 LAVC，无法分配运行环境！\n"
#define MSGTR_CouldntOpenCodec "无法打开编解码器 %s，br=%d。\n"
#define MSGTR_CantCopyAudioFormat "音频格式 0x%x 与‘-oac copy’不兼容，请尝试换成‘-oac pcm’或者用‘-fafmttag’屏蔽该选项。\n"

// cfg-mencoder.h
#define MSGTR_MEncoderMP3LameHelp "\n\n"\
" vbr=<0-4>     可变比特率方式\n"\
"                0：cbr（恒定比特率）\n"\
"                1：mt（Mark Taylor VBR 算法）\n"\
"                2：rh（Robert Hegemann VBR 算法 - 默认值）\n"\
"                3：abr（平均比特率）\n"\
"                4：mtrh（Mark Taylor Robert Hegemann VBR 算法）\n"\
"\n"\
" abr           平均比特率\n"\
"\n"\
" cbr           恒定比特率\n"\
"               同时在后面指定的 ABR 预设模式中强制以 CBR 模式编码。\n"\
"\n"\
" br=<0-1024>   以 kBit 为单位设置比特率（仅用于 CBR 和 ABR）\n"\
"\n"\
" q=<0-9>       编码质量（0-最高，9-最低）（仅用于 VBR）\n"\
"\n"\
" aq=<0-9>      算法质量（0-最好/运行最慢，9-最差/运行最快）\n"\
"\n"\
" ratio=<1-100> 压缩率\n"\
"\n"\
" vol=<0-10>    设置音频输入增益\n"\
"\n"\
" mode=<0-3>    （默认值：自动）\n"\
"                0：立体声\n"\
"                1：联合立体声\n"\
"                2：双声道\n"\
"                3：单声道\n"\
"\n"\
" padding=<0-2>\n"\
"                0：无\n"\
"                1：所有\n"\
"                2：调整\n"\
"\n"\
" fast          在后面指定 VBR 预设模式中打开更快的编码方式，\n"\
"               质量较低而比特率较高。\n"\
"\n"\
" preset=<value> 提供尽可能最高质量的设置。\n"\
"                 medium：VBR 编码，质量好\n"\
"                 （比特率范围 150-180 kbps）\n"\
"                 standard：VBR 编码，质量高\n"\
"                 （比特率范围 170-210 kbps）\n"\
"                 extreme：VBR 编码，质量非常高\n"\
"                 （比特率范围 200-240 kbps）\n"\
"                 insane：CBR 编码，质量最高\n"\
"                 （比特率 320 kbps）\n"\
"                 <8-320>：以给定比特率为平均比特率的 ABR 编码方式。\n\n"

// codec-cfg.c
#define MSGTR_DuplicateFourcc "FourCC 代码重复"
#define MSGTR_TooManyFourccs "FourCC/格式代码过多……"
#define MSGTR_ParseError "解析错误"
#define MSGTR_ParseErrorFIDNotNumber "解析错误（格式代码 ID 不是一个数字？）"
#define MSGTR_ParseErrorFIDAliasNotNumber "解析错误（格式代码 ID 别名不是一个数字？）"
#define MSGTR_DuplicateFID "格式代码 ID 重复"
#define MSGTR_TooManyOut "输出过多……"
#define MSGTR_InvalidCodecName "\n编解码器（%s）的名称无效!\n"
#define MSGTR_CodecLacksFourcc "\n编解码器（%s）没有 FourCC/格式代码！\n"
#define MSGTR_CodecLacksDriver "\n编解码器（%s）没有驱动程序！\n"
#define MSGTR_CodecNeedsDLL "\n编解码器（%s）缺少一个‘dll’！\n"
#define MSGTR_CodecNeedsOutfmt "\n编解码器（%s）缺少一个‘outfmt’!\n"
#define MSGTR_CantAllocateComment "无法为注释文本分配内存。"
#define MSGTR_GetTokenMaxNotLessThanMAX_NR_TOKEN "get_token()：max >= MAX_MR_TOKEN!"
#define MSGTR_CantGetMemoryForLine "无法获取内存提供给‘line’：%s\n"
#define MSGTR_CantReallocCodecsp "无法重新分配‘*codecsp’：%s\n"
#define MSGTR_CodecNameNotUnique "编解码器名‘%s’重复。"
#define MSGTR_CantStrdupName "无法执行 strdup ->‘name’的复制：%s\n"
#define MSGTR_CantStrdupInfo "无法执行 strdup ->‘info’的复制：%s\n"
#define MSGTR_CantStrdupDriver "无法执行 strdup ->‘driver’的复制：%s\n"
#define MSGTR_CantStrdupDLL "无法执行 strdup ->‘dll’的复制：%s"
#define MSGTR_AudioVideoCodecTotals "%d 个音频和 %d 个视频编解码器\n"
#define MSGTR_CodecDefinitionIncorrect "编解码器没有正确定义。"
#define MSGTR_OutdatedCodecsConf "此 codecs.conf 太旧，与当前发布的 MPlayer 版本不兼容！"

// fifo.c

// parser-mecmd.c, parser-mpcmd.c
#define MSGTR_NoFileGivenOnCommandLine "‘--’表示不再给出更多选项，但命令行上没有给出文件名。\n"
#define MSGTR_TheLoopOptionMustBeAnInteger "这个循环选项必须是整数：%s\n"
#define MSGTR_UnknownOptionOnCommandLine "命令行上有未知选项：-%s\n"
#define MSGTR_ErrorParsingOptionOnCommandLine "解析命令行选项出错：-%s\n"
#define MSGTR_InvalidPlayEntry "无效的播放条目 %s\n"
#define MSGTR_NotAnMEncoderOption "-%s 不是 MEncoder 的选项\n"
#define MSGTR_NoFileGiven "没有给出文件\n"

// m_config.c
#define MSGTR_SaveSlotTooOld "第 %d 级里找到的保存位置太旧：%d！！！\n"
#define MSGTR_InvalidCfgfileOption "选项 %s 无法在配置文件里使用。\n"
#define MSGTR_InvalidCmdlineOption "选项 %s 无法在命令行里使用。\n"
#define MSGTR_InvalidSuboption "错误:选项‘%s’没有子选项‘%s’。\n"
#define MSGTR_MissingSuboptionParameter "错误：子选项‘%s’（属于选项‘%s’）必须要有一个参数！\n"
#define MSGTR_MissingOptionParameter "错误：选项‘%s’必须要有一个参数！\n"
#define MSGTR_OptionListHeader "\n 名字                 类型            最小       最大     全局  命令行 配置文件\n\n"
#define MSGTR_TotalOptions "\n总共：%d 个选项\n"
#define MSGTR_ProfileInclusionTooDeep "警告：配置集相互套用的层次太深。\n"
#define MSGTR_NoProfileDefined "未定义配置集。\n"
#define MSGTR_AvailableProfiles "可用的配置集：\n"
#define MSGTR_UnknownProfile "未知配置集‘%s’。\n"
#define MSGTR_Profile "配置集 %s：%s\n"

// m_property.c
#define MSGTR_PropertyListHeader "\n 名称                 类型            最小        最大\n\n"
#define MSGTR_TotalProperties "\n总计：%d 个属性\n"

// loader/ldt_keeper.c
#define MSGTR_LOADER_DYLD_Warning "警告：尝试使用 DLL 编解码器，但是环境变量\n         DYLD_BIND_AT_LAUNCH 未设定。 这很可能造成程序崩溃。\n"


// ================================ GUI ================================

#define MSGTR_GUI_235To1 "2.35:1"
#define MSGTR_GUI_AboutMPlayer "关于 MPlayer"
#define MSGTR_GUI_Add "添加"
#define MSGTR_GUI_AspectRatio "宽高比"
#define MSGTR_GUI_Audio "音频"
#define MSGTR_GUI_AudioDelay "音频延迟"
#define MSGTR_GUI_AudioDriverConfiguration "音频驱动配置"
#define MSGTR_GUI_AudioTrack "音频文件"
#define MSGTR_GUI_AudioTracks "音频通道"
#define MSGTR_GUI_AvailableDrivers "可用驱动："
#define MSGTR_GUI_AvailableSkins "可用界面外观"
#define MSGTR_GUI_Bass "低音炮"
#define MSGTR_GUI_Blur "模糊"
#define MSGTR_GUI_Bottom "底部"
#define MSGTR_GUI_Brightness "亮度"
#define MSGTR_GUI_Browse "浏览"
#define MSGTR_GUI_Cache "缓冲"
#define MSGTR_GUI_CacheSize "缓存大小"
#define MSGTR_GUI_Cancel "取消"
#define MSGTR_GUI_CantLoadFont MSGTR_CantLoadFont
#define MSGTR_GUI_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_CD "CD"
#define MSGTR_GUI_Center "中置"
#define MSGTR_GUI_Channel1 "声道 1"
#define MSGTR_GUI_Channel2 "声道 2"
#define MSGTR_GUI_Channel3 "声道 3"
#define MSGTR_GUI_Channel4 "声道 4"
#define MSGTR_GUI_Channel5 "声道 5"
#define MSGTR_GUI_Channel6 "声道 6"
#define MSGTR_GUI_ChannelAll "所有声道"
#define MSGTR_GUI_ChapterN "章节 %d"
#define MSGTR_GUI_ChapterNN "章节 %2d"
#define MSGTR_GUI_Chapters "所有章节"
#define MSGTR_GUI_Clear "清空"
#define MSGTR_GUI_CodecFamilyAudio "音频编解码器类"
#define MSGTR_GUI_CodecFamilyVideo "视频编解码器类"
#define MSGTR_GUI_Codecs "编解码器"
#define MSGTR_GUI_CodecsAndLibraries "编解码器和第三方程序库"
#define MSGTR_GUI_Coefficient "参数"
#define MSGTR_GUI_ConfigFileError "配置文件错误\n"
#define MSGTR_GUI_Configure "配置"
#define MSGTR_GUI_ConfigureDriver "配置驱动程序"
#define MSGTR_GUI_Contrast "对比度"
#define MSGTR_GUI_Contributors "代码和文档贡献者"
#define MSGTR_GUI_Cp874 "泰文（CP874）"
#define MSGTR_GUI_Cp932 "日文（CP932）"
#define MSGTR_GUI_Cp936 "简体中文（CP936）"
#define MSGTR_GUI_Cp949 "朝鲜文（CP949）"
#define MSGTR_GUI_Cp950 "繁体中文（CP950）"
#define MSGTR_GUI_Cp1250 "中欧（CP1250）"
#define MSGTR_GUI_Cp1251 "西里尔文（CP1251）"
#define MSGTR_GUI_Cp1252 "西欧（CP1252）"
#define MSGTR_GUI_Cp1253 "希腊文（CP1253）"
#define MSGTR_GUI_Cp1254 "土尔其文（CP1254）"
#define MSGTR_GUI_Cp1255 "希伯来文（CP1255）"
#define MSGTR_GUI_Cp1256 "阿拉伯文（CP1256）"
#define MSGTR_GUI_Cp1257 "波罗的海文（CP1257）"
#define MSGTR_GUI_Cp1258 "越南文（CP1258）"
#define MSGTR_GUI_CpBIG5 "繁体中文（BIG5）"
#define MSGTR_GUI_CpISO8859_1 "西欧（ISO-8859-1）"
#define MSGTR_GUI_CpISO8859_2 "中欧（ISO-8859-2）"
#define MSGTR_GUI_CpISO8859_3 "南欧（ISO-8859-3）"
#define MSGTR_GUI_CpISO8859_4 "北欧（ISO-8859-4）"
#define MSGTR_GUI_CpISO8859_5 "西里尔文（ISO-8859-5）"
#define MSGTR_GUI_CpISO8859_6 "阿拉伯文（ISO-8859-6）"
#define MSGTR_GUI_CpISO8859_7 "希腊文（ISO-8859-7）"
#define MSGTR_GUI_CpISO8859_8 "希伯来文（ISO-8859-8）"
#define MSGTR_GUI_CpISO8859_9 "土尔其文（ISO-8859-9）"
#define MSGTR_GUI_CpISO8859_10 "北欧五国（ISO-8859-10）"
#define MSGTR_GUI_CpISO8859_11 "泰文（ISO-8859-11）"
#define MSGTR_GUI_CpISO8859_13 "波罗的海文（ISO-8859-13）"
#define MSGTR_GUI_CpISO8859_14 "凯尔特文（ISO-8859-14）"
#define MSGTR_GUI_CpISO8859_15 "西欧，扩展字符集（ISO-8859-15）"
#define MSGTR_GUI_CpISO8859_16 "东南欧（ISO-8859-16）"
#define MSGTR_GUI_CpKOI8_R "俄罗斯（KOI8-R）"
#define MSGTR_GUI_CpKOI8_RU "俄罗斯/乌克兰/白俄罗斯（KOI8-RU）"
#define MSGTR_GUI_CpKOI8_U "乌克兰（KOI8-U）"
#define MSGTR_GUI_CpShiftJis "日文（SHIFT-JIS）"
#define MSGTR_GUI_CpUnicode "Unicode"
#define MSGTR_GUI_CpUTF8 "UTF-8"
#define MSGTR_GUI_DecimalMark "."
#define MSGTR_GUI__Default_ "（默认设置）"
#define MSGTR_GUI_DefaultSetting "默认配置"
#define MSGTR_GUI_Delay "延迟"
#define MSGTR_GUI_Demuxer "流分离器"
#define MSGTR_GUI_Demuxers_Codecs "流分离器 / 编解码器"
#define MSGTR_GUI_Device "设备"
#define MSGTR_GUI_DeviceCDROM "CD-ROM 设备"
#define MSGTR_GUI_DeviceDVD "DVD 设备"
#define MSGTR_GUI_Directory "目录"
#define MSGTR_GUI_DirectoryTree "目录树"
#define MSGTR_GUI_DropSubtitle "去除字幕"
#define MSGTR_GUI_DVD "DVD"
#define MSGTR_GUI_EnableAssSubtitle "启用 ssa/ass 字幕渲染"
#define MSGTR_GUI_EnableAutomaticAVSync "启用自动音视频同步功能"
#define MSGTR_GUI_EnableCache "缓存开启 / 关闭"
#define MSGTR_GUI_EnableDigitalTV "启用数字电视接收功能"
#define MSGTR_GUI_EnableDirectRendering "启用直接渲染"
#define MSGTR_GUI_EnableDoubleBuffering "启用双重缓冲"
#define MSGTR_GUI_EnableEqualizer "启用均衡器"
#define MSGTR_GUI_EnableExtraStereo "启用立体声加强"
#define MSGTR_GUI_EnableFrameDropping "启用丢帧"
#define MSGTR_GUI_EnableFrameDroppingIntense "启用密集丢帧模式（图像可能失真）"
#define MSGTR_GUI_EnablePlaybar "启用播放条"
#define MSGTR_GUI_EnablePostProcessing "启用后期处理"
#define MSGTR_GUI_EnableSoftwareMixer "启用软件混音器"
#define MSGTR_GUI_Encoding "编码"
#define MSGTR_GUI_Equalizer "均衡器"
#define MSGTR_GUI_EqualizerConfiguration "均衡器配置"
#define MSGTR_GUI_Error "错误"
#define MSGTR_GUI_ErrorFatal "致命错误"
#define MSGTR_GUI_File "文件"
#define MSGTR_GUI_Files "所有文件"
#define MSGTR_GUI_FilterAudioAll "所有音频文件"
#define MSGTR_GUI_FilterAudioDolbyPCM "Dolby 数字音频文件 / PCM 音频文件（*.ac3, *.pcm）"
#define MSGTR_GUI_FilterAudioMatroska "Matroska 音频文件（*.mka）"
#define MSGTR_GUI_FilterAudioMPEG "MPEG 音频文件（*.aac, *.f4a, *.m4a, *.mp2, *.mp3, *.mpga）"
#define MSGTR_GUI_FilterAudioOgg "Ogg 音频文件（*.oga, *.ogg, *.spx）"
#define MSGTR_GUI_FilterAudioWav "WAV 音频文件（*.wav）"
#define MSGTR_GUI_FilterAudioWma "Windows Media 音频文件（*.wma）"
#define MSGTR_GUI_FilterFileAll "所有文件"
#define MSGTR_GUI_FilterFileMP2TS "MPEG-2 传输流"
#define MSGTR_GUI_FilterFileMP3 "MP3 文件"
#define MSGTR_GUI_FilterFileMP4 "MP4 文件"
#define MSGTR_GUI_FilterFileMPEG "MPEG 文件"
#define MSGTR_GUI_FilterFilePlaylist "播放列表文件"
#define MSGTR_GUI_FilterFileWav "WAV 文件"
#define MSGTR_GUI_FilterFontAll "所有字体"
#define MSGTR_GUI_FilterFontBitmap "位图字体文件（*.desc）"
#define MSGTR_GUI_FilterFontTTF "TrueType 字体文件（*.ttf）"
#define MSGTR_GUI_FilterFontType1 "Type1 字体文件（*.pfb）"
#define MSGTR_GUI_FilterImageCD "CD 镜像索引表"
#define MSGTR_GUI_FilterImageCue "CD/VCD/SVCD 镜像索引表"
#define MSGTR_GUI_FilterImageDVD "DVD 镜像/拷贝"
#define MSGTR_GUI_FilterImageVCD "VCD/SVCD 镜像索引表"
#define MSGTR_GUI_FilterMediumMatroska "Matroska 媒体文件"
#define MSGTR_GUI_FilterMediumOgg "Ogg 媒体文件"
#define MSGTR_GUI_FilterMediumQuickTime "QuickTime 媒体文件"
#define MSGTR_GUI_FilterMediumRealNetworks "RealNetworks 媒体文件"
#define MSGTR_GUI_FilterMediumWindows "Windows 媒体文件"
#define MSGTR_GUI_FilterSubtitleAll "所有字幕文件"
#define MSGTR_GUI_FilterSubtitleUTF8 "UTF-8 编码的字幕（*.utf, *.utf-8, *.utf8）"
#define MSGTR_GUI_FilterVideoAll "所有视频文件"
#define MSGTR_GUI_FilterVideoAvi "AVI 文件"
#define MSGTR_GUI_FilterVideoDivX "DivX 文件"
#define MSGTR_GUI_FilterVideoFlash "Flash 视频文件"
#define MSGTR_GUI_Flip "上下翻转图像"
#define MSGTR_GUI_Font "字体"
#define MSGTR_GUI_FrameRate "帧率（fps）"
#define MSGTR_GUI_Frequency0 "31.25 Hz"
#define MSGTR_GUI_Frequency1 "62.50 Hz"
#define MSGTR_GUI_Frequency2 "125 Hz"
#define MSGTR_GUI_Frequency3 "250 Hz"
#define MSGTR_GUI_Frequency4 "500 Hz"
#define MSGTR_GUI_Frequency5 "1 kHz"
#define MSGTR_GUI_Frequency6 "2 kHz"
#define MSGTR_GUI_Frequency7 "4 kHz"
#define MSGTR_GUI_Frequency8 "8 kHz"
#define MSGTR_GUI_Frequency9 "16 kHz"
#define MSGTR_GUI_FrontLeft "前左"
#define MSGTR_GUI_FrontRight "前右"
#define MSGTR_GUI_HideVideoWindow "播放后隐藏视频窗口"
#define MSGTR_GUI_Hue "色调"
#define MSGTR_GUI_Image "镜像"
#define MSGTR_GUI_Information "信息"
#define MSGTR_GUI_Lavc "lavc（实时 mpeg1 编码）"
#define MSGTR_GUI_MaximumUsageSpareCPU "空闲 CPU 时间的最大使用量"
#define MSGTR_GUI_MemAllocFailed MSGTR_MemAllocFailed
#define MSGTR_GUI_Miscellaneous "杂项"
#define MSGTR_GUI_Mixer "混音器"
#define MSGTR_GUI_MixerChannel "混音器声道"
#define MSGTR_GUI_MSG_AddingVideoFilter "添加视频滤镜：%s\n"
#define MSGTR_GUI_MSG_ColorDepthTooLow "显示器支持的色彩深度太低！\n"
#define MSGTR_GUI_MSG_DragAndDropNothing "拖曳操作：未返回任何东西！\n"
#define MSGTR_GUI_MSG_DXR3NeedsLavc "无法不重新编码而使用 DXR3/H+ 设备播放非 MPEG 的文件。\n请在 DXR3/H+ 的配置对话框中启用 lavc。\n"
#define MSGTR_GUI_MSG_IconError "未找到图标‘%s’（大小为 %d）或格式不支持。\n"
#define MSGTR_GUI_MSG_LoadingSubtitle "正在加载字幕文件‘%s’。\n"
#define MSGTR_GUI_MSG_LocaleEncoding "不支持 G_FILENAME_ENCODING=@locale 或 G_BROKEN_FILENAMES，\n必须在 G_FILENAME_ENCODING 中明确指定编码方式。\n"
#define MSGTR_GUI_MSG_MemoryErrorImage "没有足够内存以用于渲染图像！\n"
#define MSGTR_GUI_MSG_MemoryErrorWindow "没有足够的内存用于渲染窗口。\n"
#define MSGTR_GUI_MSG_NoFileLoaded "未载入文件"
#define MSGTR_GUI_MSG_NoMediaOpened "未打开媒体内容"
#define MSGTR_GUI_MSG_NotAFile0 "这不像是一个文件！\n"
#define MSGTR_GUI_MSG_NotAFile1 "‘%s’不像是一个文件！\n"
#define MSGTR_GUI_MSG_PlaybackNeedsRestart "有些选项只有重新播放后才能生效。"
#define MSGTR_GUI_MSG_RemoteDisplay "远程显示界面，禁用 X 共享内存模式。\n"
#define MSGTR_GUI_MSG_RemovingSubtitle "移除字幕中。\n"
#define MSGTR_GUI_MSG_RemovingVideoFilter "移除视频滤镜‘%s’。\n"
#define MSGTR_GUI_MSG_SkinBitmapConversionError "%s 中的数据执行 24 比特至 32 比特转换出错\n"
#define MSGTR_GUI_MSG_SkinBitmapNotFound "未找到位图文件 %s\n"
#define MSGTR_GUI_MSG_SkinBitmapPngReadError "%s 中的 PNG 数据读取错误\n"
#define MSGTR_GUI_MSG_SkinCfgError "处理配置文件中的界面外观‘%s’时出错\n"
#define MSGTR_GUI_MSG_SkinCfgNotFound "未找到界面外观‘%s’。\n"
#define MSGTR_GUI_MSG_SkinCfgSelectedNotFound "未找到选定的界面外观‘%s’，尝试使用‘default’参数...\n"
#define MSGTR_GUI_MSG_SkinErrorBitmap16Bit "位图 %s 的颜色精度小于等于 16 比特，因而无法支持。\n"
#define MSGTR_GUI_MSG_SkinErrorDefault "默认值（%d）超出范围\n"
#define MSGTR_GUI_MSG_SkinErrorItem "该项目‘%s’不支持。\n"
#define MSGTR_GUI_MSG_SkinErrorMessage "界面外观配置文件错误，行 %d：%s"
#define MSGTR_GUI_MSG_SkinErrorNumphases "步骤数不可为零。\n"
#define MSGTR_GUI_MSG_SkinErrorSection "没有指定‘%s’的区域。\n"
#define MSGTR_GUI_MSG_SkinErrorWindow "没有指定‘%s’的窗口。\n"
#define MSGTR_GUI_MSG_SkinFileNotFound "未找到界面外观文件 %s。\n"
#define MSGTR_GUI_MSG_SkinFileNotReadable "无法读取界面外观文件 %s。\n"
#define MSGTR_GUI_MSG_SkinFontFileNotFound "未找到字体描述文件。\n"
#define MSGTR_GUI_MSG_SkinFontImageNotFound "未找到字体图像文件。\n"
#define MSGTR_GUI_MSG_SkinFontNotFound "字体标识符（%s）不存在\n"
#define MSGTR_GUI_MSG_SkinLegacy "界面外观配置文件，行 %d：‘%s’已过时，应使用‘%s’。\n"
#define MSGTR_GUI_MSG_SkinMemoryError "内存不足\n"
#define MSGTR_GUI_MSG_SkinTooManyFonts "定义的字体过多。\n"
#define MSGTR_GUI_MSG_SkinTooManyItems "定义项过多。\n"
#define MSGTR_GUI_MSG_SkinUnknownItem "未知项目‘%s’\n"
#define MSGTR_GUI_MSG_SkinUnknownMessage "未知信息‘%s’\n"
#define MSGTR_GUI_MSG_SkinUnknownName "未知名称‘%s’\n"
#define MSGTR_GUI_MSG_SkinUnknownParameter "未知参数（%s）\n"
#define MSGTR_GUI_MSG_TooManyWindows "已打开太多的窗口！\n"
#define MSGTR_GUI_MSG_UnableToSaveOption "[cfg] 无法保存‘%s’选项。\n"
#define MSGTR_GUI_MSG_VideoOutError "未找到支持图形用户界面的视频输出驱动。\n"
#define MSGTR_GUI_MSG_X11Error "发生 X11 错误！\n"
#define MSGTR_GUI_MSG_XShapeError "该系统不支持 XShape 扩展组件！\n"
#define MSGTR_GUI_MSG_XSharedMemoryError "某 X 共享内存扩展组件出错！\n"
#define MSGTR_GUI_MSG_XSharedMemoryUnavailable "X 共享内存模式不可用！\n"
#define MSGTR_GUI_Mute "静音"
#define MSGTR_GUI_NetworkStreaming "网络媒体流传输"
#define MSGTR_GUI_Next "下一个"
#define MSGTR_GUI_NoChannelName "无频道名"
#define MSGTR_GUI_NoChapter "无章节"
#define MSGTR_GUI_None "（无）"
#define MSGTR_GUI__none_ "（无）"
#define MSGTR_GUI_NonInterleavedParser "使用非交错格式的文件解析器"
#define MSGTR_GUI_NormalizeSound "音量规格化"
#define MSGTR_GUI_Ok "确定"
#define MSGTR_GUI_Open "打开"
#define MSGTR_GUI_Original "原始的"
#define MSGTR_GUI_OsdLevel "OSD 模式"
#define MSGTR_GUI_OsdLevel0 "仅显示字幕"
#define MSGTR_GUI_OsdLevel1 "显示音量和定位条"
#define MSGTR_GUI_OsdLevel2 "显示音量、定位条、计时器和进度百分比"
#define MSGTR_GUI_OsdLevel3 "显示音量、定位条、计时器、进度百分比和时间总长"
#define MSGTR_GUI_OSD_Subtitles "OSD 菜单 / 字幕"
#define MSGTR_GUI_Outline "轮廓"
#define MSGTR_GUI_PanAndScan "全景模式"
#define MSGTR_GUI_Pause "暂停"
#define MSGTR_GUI_Play "播放"
#define MSGTR_GUI_Playback "播放控制"
#define MSGTR_GUI_Playlist "播放列表"
#define MSGTR_GUI_Position "位置"
#define MSGTR_GUI_PostProcessing "后期处理"
#define MSGTR_GUI_Preferences "首选项"
#define MSGTR_GUI_Previous "上一个"
#define MSGTR_GUI_Quit "退出"
#define MSGTR_GUI_QuitAfterPlaying "播放后退出"
#define MSGTR_GUI_RearLeft "后左"
#define MSGTR_GUI_RearRight "后右"
#define MSGTR_GUI_Remove "删除"
#define MSGTR_GUI_Rotation "旋转"
#define MSGTR_GUI_Rotation180 "180°"
#define MSGTR_GUI_Rotation90CCW "逆时针 90°"
#define MSGTR_GUI_Rotation90CW "顺时针 90°"
#define MSGTR_GUI_Saturation "饱和度"
#define MSGTR_GUI_SaveWindowPositions "保存窗口位置"
#define MSGTR_GUI_ScaleMode "缩放模式"
#define MSGTR_GUI_ScaleMovieDiagonal "与影片对角线相称"
#define MSGTR_GUI_ScaleMovieHeight "与影片高度相称"
#define MSGTR_GUI_ScaleMovieWidth "与影片宽度相称"
#define MSGTR_GUI_ScaleNo "不自动缩放"
#define MSGTR_GUI_SeekingInBrokenMedia "允许在损坏的媒体文件中定位"
#define MSGTR_GUI_SelectAudioFile "选择音频文件"
#define MSGTR_GUI_SelectedFiles "所选文件"
#define MSGTR_GUI_SelectFile "选择文件"
#define MSGTR_GUI_SelectFont "选择字体"
#define MSGTR_GUI_SelectImage "选择镜像"
#define MSGTR_GUI_SelectSubtitle "选择字幕"
#define MSGTR_GUI_SizeDouble "双倍尺寸"
#define MSGTR_GUI_SizeFullscreen "全屏"
#define MSGTR_GUI_SizeHalf "一半尺寸"
#define MSGTR_GUI_SizeNormal "正常尺寸"
#define MSGTR_GUI_SizeOSD "尺寸（OSD）"
#define MSGTR_GUI_SizeSubtitles "尺寸（字幕）"
#define MSGTR_GUI_SkinBrowser "播放器外观浏览器"
#define MSGTR_GUI_Skins "界面外观"
#define MSGTR_GUI_Sponsored "图形用户界面的开发已由 UHU-Linux 赞助"
#define MSGTR_GUI_StartFullscreen "以全屏方式启动"
#define MSGTR_GUI_Stop "停止"
#define MSGTR_GUI_Subtitle "字幕"
#define MSGTR_GUI_SubtitleAddMargins "为字幕增加留边"
#define MSGTR_GUI_SubtitleAllowOverlap "允许字幕重叠模式"
#define MSGTR_GUI_SubtitleAutomaticLoad "自动装载字幕"
#define MSGTR_GUI_SubtitleConvertMpsub "将所给的字幕转换为 MPlayer 字幕格式（mpsub）"
#define MSGTR_GUI_SubtitleConvertSrt "将所给的字幕转换为 SubRip 字幕格式（srt）"
#define MSGTR_GUI_Subtitles "字幕"
#define MSGTR_GUI_SyncValue "同步值"
#define MSGTR_GUI_TitleN "标题 %d"
#define MSGTR_GUI_TitleNN "标题 %2d"
#define MSGTR_GUI_Titles "所有标题"
#define MSGTR_GUI_Top "顶部"
#define MSGTR_GUI_TrackN "通道 %d"
#define MSGTR_GUI_Translations "翻译"
#define MSGTR_GUI_TurnOffXScreenSaver "关闭 XScreenSaver 屏保"
#define MSGTR_GUI_TV "电视"
#define MSGTR_GUI_Unknown "未知"
#define MSGTR_GUI_URL "网络链接"
#define MSGTR_GUI_VCD "VCD"
#define MSGTR_GUI_Video "视频"
#define MSGTR_GUI_VideoEncoder "视频编解码器"
#define MSGTR_GUI_VideoTracks "视频通道"
#define MSGTR_GUI_Volume MSGTR_Volume
#define MSGTR_GUI_Warning "警告"

#define MSGTR_GUI_WIN32_AddFile "添加文件"
#define MSGTR_GUI_WIN32_AddURL "添加网络地址"
#define MSGTR_GUI_WIN32_CantLoadSub MSGTR_CantLoadSub
#define MSGTR_GUI_WIN32_Close "关闭"
#define MSGTR_GUI_WIN32_ConfigFileError MSGTR_GUI_ConfigFileError
#define MSGTR_GUI_WIN32_DebugConsole "调试控制终端"
#define MSGTR_GUI_WIN32_Defaults "默认值"
#define MSGTR_GUI_WIN32_DirectoryList "目录列表"
#define MSGTR_GUI_WIN32_DisplayInVideoWindow "在视频窗口中显示（仅用于 DirectX 或 Direct3D）"
#define MSGTR_GUI_WIN32_Down "向下"
#define MSGTR_GUI_WIN32_Load "加载"
#define MSGTR_GUI_WIN32_OnlineHelp "线上帮助内容"
#define MSGTR_GUI_WIN32_Priority "进程优先级"
#define MSGTR_GUI_WIN32_PriorityAboveNormal "高于标准"
#define MSGTR_GUI_WIN32_PriorityBelowNormal "低于标准"
#define MSGTR_GUI_WIN32_PriorityHigh "高"
#define MSGTR_GUI_WIN32_PriorityLow "低"
#define MSGTR_GUI_WIN32_PriorityNormal "标准"
#define MSGTR_GUI_WIN32_Save "保存"
#define MSGTR_GUI_WIN32_SavePlaylist "保存播放列表"
#define MSGTR_GUI_WIN32_SeekBackwards "向后定位"
#define MSGTR_GUI_WIN32_SeekForwards "向前定位"
#define MSGTR_GUI_WIN32_SelectPlaylist "选择播放列表"
#define MSGTR_GUI_WIN32_SelectTitleChapter "选择标题/章节"
#define MSGTR_GUI_WIN32_ShowHide "显示/隐藏"
#define MSGTR_GUI_WIN32_SubtitleOnOff "显示字幕开/关"
#define MSGTR_GUI_WIN32_Up "向上"

// ======================= video output drivers ========================

#define MSGTR_VOincompCodec "所选的视频输出设备与该编解码器不兼容。\n"\
                "请尝试在滤镜列表尾部添加缩放滤镜，\n"\
                "例如，用 -vf spp,scale 代替 -vf spp。\n"
#define MSGTR_VO_GenericError "该错误已经发生"
#define MSGTR_VO_OutputDirectory "输出目录"
#define MSGTR_VO_UnableToAccess "无法访问"
#define MSGTR_VO_ExistsButNoDirectory "已经存在，但不是一个目录。"
#define MSGTR_VO_DirExistsButNotWritable "输出目录已经存在，但是不可写。"
#define MSGTR_VO_CantCreateDirectory "无法创建输出目录。"
#define MSGTR_VO_CantCreateFile "无法创建输出文件。"
#define MSGTR_VO_DirectoryCreateSuccess "输出目录创建成功。"
#define MSGTR_VO_ValueOutOfRange "值超出范围"

// aspect.c
#define MSGTR_LIBVO_ASPECT_NoSuitableNewResFound "[ASPECT] 警告：无法找到新的合适的分辨率！\n"
#define MSGTR_LIBVO_ASPECT_NoNewSizeFoundThatFitsIntoRes "[ASPECT] 错误：无法找到适合分辨率的新尺寸!\n"

// font_load_ft.c
#define MSGTR_LIBVO_FONT_LOAD_FT_NewFaceFailed "调用 New_Face 失败。可能字体文件的路径不对。\n请提供文本字体文件（~/.mplayer/subfont.tt）。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_NewMemoryFaceFailed "调用 New_Memory_Face 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFaceFailed "字幕字体：调用 load_sub_face 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_SubFontCharsetFailed "字幕字体：调用 prepare_charset 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareSubtitleFont "无法设置字幕字体。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotPrepareOSDFont "无法设置 OSD 字体。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_CannotGenerateTables "无法生成映射表。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_DoneFreeTypeFailed "调用 FT_Done_FreeType 失败。\n"
#define MSGTR_LIBVO_FONT_LOAD_FT_FontconfigNoMatch "Fontconfig 选取字体失败。请尝试不使用...\n"

// sub.c
#define MSGTR_VO_SUB_Seekbar "定位条"
#define MSGTR_VO_SUB_Play "播放"
#define MSGTR_VO_SUB_Pause "暂停"
#define MSGTR_VO_SUB_Stop "停止"
#define MSGTR_VO_SUB_Rewind "倒回"
#define MSGTR_VO_SUB_Forward "快进"
#define MSGTR_VO_SUB_Clock "计时"
#define MSGTR_VO_SUB_Contrast "对比度"
#define MSGTR_VO_SUB_Saturation "饱和度"
#define MSGTR_VO_SUB_Volume "音量"
#define MSGTR_VO_SUB_Brightness "亮度"
#define MSGTR_VO_SUB_Hue "色调"
#define MSGTR_VO_SUB_Balance "均衡"

// vo_3dfx.c
#define MSGTR_LIBVO_3DFX_Only16BppSupported "[VO_3DFX] 只支持 16bpp！"
#define MSGTR_LIBVO_3DFX_VisualIdIs "[VO_3DFX] 可视化 ID 是 %lx。\n"
#define MSGTR_LIBVO_3DFX_UnableToOpenDevice "[VO_3DFX] 无法打开 /dev/3dfx。\n"
#define MSGTR_LIBVO_3DFX_Error "[VO_3DFX] 错误：%d。\n"
#define MSGTR_LIBVO_3DFX_CouldntMapMemoryArea "[VO_3DFX] 未能映射 3dfx 内存区域：%p,%p,%d。\n"
#define MSGTR_LIBVO_3DFX_DisplayInitialized "[VO_3DFX] 初始化完毕：%p。\n"
#define MSGTR_LIBVO_3DFX_UnknownSubdevice "[VO_3DFX] 未知子设备：%s。\n"

// vo_aa.c
#define MSGTR_VO_AA_HelpHeader "\n\n以下是 aalib vo_aa 的子选项：\n"
#define MSGTR_VO_AA_AdditionalOptions "vo_aa 提供的附加选项：\n" \
"  help        显示此帮助信息\n" \
"  osdcolor    设定 OSD 颜色\n  subcolor    设定字幕颜色\n" \
"        颜色参数有：\n           0 ：标准\n" \
"           1 ：模糊\n           2 ：加粗\n           3 ：字体加粗\n" \
"           4 ：反色\n           5 ：特殊\n\n\n"

// vo_dxr3.c
#define MSGTR_LIBVO_DXR3_UnableToLoadNewSPUPalette "[VO_DXR3] 无法载入新的 SPU 调色板！\n"
#define MSGTR_LIBVO_DXR3_UnableToSetPlaymode "[VO_DXR3] 无法设置播放模式！\n"
#define MSGTR_LIBVO_DXR3_UnableToSetSubpictureMode "[VO_DXR3] 无法设置字幕模式！\n"
#define MSGTR_LIBVO_DXR3_UnableToGetTVNorm "[VO_DXR3] 无法获取电视制式！\n"
#define MSGTR_LIBVO_DXR3_AutoSelectedTVNormByFrameRate "[VO_DXR3] 根据帧速率自动选择电视制式："
#define MSGTR_LIBVO_DXR3_UnableToSetTVNorm "[VO_DXR3] 无法设置电视制式！\n"
#define MSGTR_LIBVO_DXR3_SettingUpForNTSC "[VO_DXR3] 设置为 NTSC 模式。\n"
#define MSGTR_LIBVO_DXR3_SettingUpForPALSECAM "[VO_DXR3] 设置为 PAL/SECAM 模式。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo43 "[VO_DXR3] 宽高比设为 4:3。\n"
#define MSGTR_LIBVO_DXR3_SettingAspectRatioTo169 "[VO_DXR3] 宽高比设为 16:9。\n"
#define MSGTR_LIBVO_DXR3_OutOfMemory "[VO_DXR3] 内存耗尽\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateKeycolor "[VO_DXR3] 无法分配关键色！\n"
#define MSGTR_LIBVO_DXR3_UnableToAllocateExactKeycolor "[VO_DXR3] 无法精确分配关键色，使用最接近的匹配（0x%lx）。\n"
#define MSGTR_LIBVO_DXR3_Uninitializing "[VO_DXR3] 正在终止。\n"
#define MSGTR_LIBVO_DXR3_FailedRestoringTVNorm "[VO_DXR3] 未能恢复电视制式！\n"
#define MSGTR_LIBVO_DXR3_EnablingPrebuffering "[VO_DXR3] 启用预缓冲模式。\n"
#define MSGTR_LIBVO_DXR3_UsingNewSyncEngine "[VO_DXR3] 使用新的同步引擎。\n"
#define MSGTR_LIBVO_DXR3_UsingOverlay "[VO_DXR3] 使用覆盖模式。\n"
#define MSGTR_LIBVO_DXR3_ErrorYouNeedToCompileMplayerWithX11 "[VO_DXR3] 错误：覆盖需要在安装 X11 库和头文件的条件下编译。\n"
#define MSGTR_LIBVO_DXR3_WillSetTVNormTo "[VO_DXR3] 将电视制式设置为："
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALPAL60 "自动调节画面的帧率（PAL/PAL-60）"
#define MSGTR_LIBVO_DXR3_AutoAdjustToMovieFrameRatePALNTSC "自动调节画面的帧率（PAL/NTSC）"
#define MSGTR_LIBVO_DXR3_UseCurrentNorm "使用当前制式。"
#define MSGTR_LIBVO_DXR3_UseUnknownNormSuppliedCurrentNorm "未知制式，使用当前制式。"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTrying "[VO_DXR3] 打开 %s 以写入错误，尝试 /dev/em8300。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingMV "[VO_DXR3] 打开 %s 以写入错误，尝试 /dev/em8300_mv。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWell "[VO_DXR3] 打开 /dev/em8300 以写入同样错误！\n跳出。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellMV "[VO_DXR3] 打开 /dev/em8300_mv 以写入同样错误！\n跳出。\n"
#define MSGTR_LIBVO_DXR3_Opened "[VO_DXR3] 打开：%s。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingTryingSP "[VO_DXR3] 打开 %s 写入错误，尝试 /dev/em8300_sp。\n"
#define MSGTR_LIBVO_DXR3_ErrorOpeningForWritingAsWellSP "[VO_DXR3] 打开 /dev/em8300_sp 以写入同样错误！\n跳出。\n"
#define MSGTR_LIBVO_DXR3_UnableToOpenDisplayDuringHackSetup "[VO_DXR3] 设置覆盖模式破解方式时无法打开显示设备！\n"
#define MSGTR_LIBVO_DXR3_UnableToInitX11 "[VO_DXR3] 无法初始化 X11！\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayAttribute "[VO_DXR3] 未能设置覆盖模式属性。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayScreen "[VO_DXR3] 未能设置覆盖屏幕！\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedEnablingOverlay "[VO_DXR3] 未能启用覆盖模式！\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedSettingOverlayBcs "[VO_DXR3] 未能设置覆盖模式 bcs！\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayYOffsetValues "[VO_DXR3] 未能获取覆盖模式的 Y-偏移量！\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXOffsetValues "[VO_DXR3] 未能获取覆盖模式的 X-偏移量！\n退出。\n"
#define MSGTR_LIBVO_DXR3_FailedGettingOverlayXScaleCorrection "[VO_DXR3] 未能获取覆盖模式的 X-比例校正！\n退出。\n"
#define MSGTR_LIBVO_DXR3_YOffset "[VO_DXR3] Y-偏移量：%d。\n"
#define MSGTR_LIBVO_DXR3_XOffset "[VO_DXR3] X-偏移量：%d。\n"
#define MSGTR_LIBVO_DXR3_XCorrection "[VO_DXR3] X-比例校正：%d。\n"
#define MSGTR_LIBVO_DXR3_FailedResizingOverlayWindow "[VO_DXR3] 未能设置覆盖窗口大小！\n"
#define MSGTR_LIBVO_DXR3_FailedSetSignalMix "[VO_DXR3] 未能设置信号混合模式！\n"

// vo_jpeg.c
#define MSGTR_VO_JPEG_ProgressiveJPEG "启用渐显 JPEG。"
#define MSGTR_VO_JPEG_NoProgressiveJPEG "停用渐显 JPEG。"
#define MSGTR_VO_JPEG_BaselineJPEG "启用基本 JPEG。"
#define MSGTR_VO_JPEG_NoBaselineJPEG "停用基本 JPEG。"

// vo_mga.c
#define MSGTR_LIBVO_MGA_AspectResized "[VO_MGA] aspect()：改变大小为 %dx%d。\n"
#define MSGTR_LIBVO_MGA_Uninit "[VO] 终止！\n"

// mga_template.c
#define MSGTR_LIBVO_MGA_ErrorInConfigIoctl "[MGA] mga_vid_config ioctl 错误（mga_vid.o 版本错误？）"
#define MSGTR_LIBVO_MGA_CouldNotGetLumaValuesFromTheKernelModule "[MGA] 无法在内核模块中获得亮度值！\n"
#define MSGTR_LIBVO_MGA_CouldNotSetLumaValuesFromTheKernelModule "[MGA] 无法在内核模块中设置亮度值！\n"
#define MSGTR_LIBVO_MGA_ScreenWidthHeightUnknown "[MGA] 屏幕宽度/高度未知！\n"
#define MSGTR_LIBVO_MGA_InvalidOutputFormat "[MGA] 无效的输出格式 %0X\n"
#define MSGTR_LIBVO_MGA_IncompatibleDriverVersion "[MGA] 你的 mga_vid 驱动版本与该 MPlayer 的版本不兼容！\n"
#define MSGTR_LIBVO_MGA_CouldntOpen "[MGA] 无法打开：%s\n"
#define MSGTR_LIBVO_MGA_ResolutionTooHigh "[MGA] 源分辨率至少有一维超过了 1023x1023。\n[MGA] 需用软件或用 -lavdopts lowres=1 重新缩放。\n"
#define MSGTR_LIBVO_MGA_mgavidVersionMismatch "[MGA] 内核（%u）与 MPlayer（%u）的 mga_vid 驱动版本号不匹配。\n"

// vo_null.c
#define MSGTR_LIBVO_NULL_UnknownSubdevice "[VO_NULL] 未知子设备：%s。\n"

// vo_png.c
#define MSGTR_LIBVO_PNG_Warning1 "[VO_PNG] 警告：压缩级别设置为 0，禁用压缩！\n"
#define MSGTR_LIBVO_PNG_Warning2 "[VO_PNG] 信息：使用 -vo png:z=<n> 设置 0 到 9 之间的压缩级别。\n"
#define MSGTR_LIBVO_PNG_Warning3 "[VO_PNG] 信息:（0 = 不压缩，1 = 最快，压缩率最低 - 9 最好，最慢的压缩）\n"
#define MSGTR_LIBVO_PNG_ErrorOpeningForWriting "\n[VO_PNG] 打开‘%s’以写入错误！\n"
#define MSGTR_LIBVO_PNG_ErrorInCreatePng "[VO_PNG] create_png 错误。\n"

// vo_pnm.c
#define MSGTR_VO_PNM_ASCIIMode "启用 ASCII 模式。"
#define MSGTR_VO_PNM_RawMode "启用原生模式。"
#define MSGTR_VO_PNM_PPMType "将要写入 PPM 文件。"
#define MSGTR_VO_PNM_PGMType "将要写入 PGM 文件。"
#define MSGTR_VO_PNM_PGMYUVType "将要写入 PGMYUV 文件。"

// vo_sdl.c
#define MSGTR_LIBVO_SDL_CouldntGetAnyAcceptableSDLModeForOutput "[VO_SDL] 无法获得可接受的 SDL 模式以用于输出。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormat "[VO_SDL] 不支持的图像格式（0x%X）。\n"
#define MSGTR_LIBVO_SDL_InfoPleaseUseVmOrZoom "[VO_SDL] 信息 - 请使用 -vm 或 -zoom 切换到最佳分辨率。\n"
#define MSGTR_LIBVO_SDL_FailedToSetVideoMode "[VO_SDL] 设置视频模式失败：%s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateAYUVOverlay "[VO_SDL] 未能创建 YUV 覆盖模式：%s。\n"
#define MSGTR_LIBVO_SDL_CouldntCreateARGBSurface "[VO_SDL] 未能创建 RGB 表面图层：%s。\n"
#define MSGTR_LIBVO_SDL_UsingDepthColorspaceConversion "[VO_SDL] 使用深度/颜色空间转换，这会减慢速度（%ibpp -> %ibpp）。\n"
#define MSGTR_LIBVO_SDL_UnsupportedImageFormatInDrawslice "[VO_SDL] draw_slice 不支持的图像格式，请联系 MPlayer 的开发者！\n"
#define MSGTR_LIBVO_SDL_BlitFailed "[VO_SDL] 位块传输失败：%s。\n"
#define MSGTR_LIBVO_SDL_InitializationFailed "[VO_SDL] 初始化 SDL 模式失败: %s。\n"
#define MSGTR_LIBVO_SDL_UsingDriver "[VO_SDL] 使用驱动：%s。\n"

// vo_svga.c
#define MSGTR_LIBVO_SVGA_ForcedVidmodeNotAvailable "[VO_SVGA] 强制使用的 vid_mode %d（%s）不可用。\n"
#define MSGTR_LIBVO_SVGA_ForcedVidmodeTooSmall "[VO_SVGA] 强制使用的 vid_mode %d（%s）太小。\n"
#define MSGTR_LIBVO_SVGA_Vidmode "[VO_SVGA] Vid_mode：%d，%dx%d %dbpp。\n"
#define MSGTR_LIBVO_SVGA_VgasetmodeFailed "[VO_SVGA] Vga_setmode（%d）失败。\n"
#define MSGTR_LIBVO_SVGA_VideoModeIsLinearAndMemcpyCouldBeUsed "[VO_SVGA] 线性的视频模式，可以使用 memcpy 传输图像。\n"
#define MSGTR_LIBVO_SVGA_VideoModeHasHardwareAcceleration "[VO_SVGA] 硬件加速的视频模式，可以使用 put_image。\n"
#define MSGTR_LIBVO_SVGA_IfItWorksForYouIWouldLikeToKnow "[VO_SVGA] 如果工作正常请告诉我。\n[VO_SVGA]（使用 `mplayer test.avi -v -v -v -v &> svga.log` 生成日志文件并发送）。谢谢！\n"
#define MSGTR_LIBVO_SVGA_VideoModeHas "[VO_SVGA] 视频模式占用 %d 内存页。\n"
#define MSGTR_LIBVO_SVGA_CenteringImageStartAt "[VO_SVGA] 居中图像。超始位置（%d，%d）\n"
#define MSGTR_LIBVO_SVGA_UsingVidix "[VO_SVGA] 使用 VIDIX。w=%i h=%i  mw=%i mh=%i\n"

// vo_tdfx_vid.c
#define MSGTR_LIBVO_TDFXVID_Move "[VO_TDXVID] 移动 %d(%d) x %d => %d。\n"
#define MSGTR_LIBVO_TDFXVID_AGPMoveFailedToClearTheScreen "[VO_TDFXVID] AGP 移动操作未能清空屏幕。\n"
#define MSGTR_LIBVO_TDFXVID_BlitFailed "[VO_TDFXVID] 位块传输失败。\n"
#define MSGTR_LIBVO_TDFXVID_NonNativeOverlayFormatNeedConversion "[VO_TDFXVID] 非原生支持的覆盖格式需要转换。\n"
#define MSGTR_LIBVO_TDFXVID_UnsupportedInputFormat "[VO_TDFXVID] 不支持的输入格式 0x%x。\n"
#define MSGTR_LIBVO_TDFXVID_OverlaySetupFailed "[VO_TDFXVID] 覆盖模式设置失败。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOnFailed "[VO_TDFXVID] 覆盖模式打开失败。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayReady "[VO_TDFXVID] 覆盖模式就绪：%d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_TextureBlitReady "[VO_TDFXVID] 纹理位块传输就绪：%d(%d) x %d @ %d => %d(%d) x %d @ %d。\n"
#define MSGTR_LIBVO_TDFXVID_OverlayOffFailed "[VO_TDFXVID] 覆盖模式关闭失败\n"
#define MSGTR_LIBVO_TDFXVID_CantOpen "[VO_TDFXVID] 无法打开 %s：%s。\n"
#define MSGTR_LIBVO_TDFXVID_CantGetCurrentCfg "[VO_TDFXVID] 无法获得当前配置：%s。\n"
#define MSGTR_LIBVO_TDFXVID_MemmapFailed "[VO_TDFXVID] Memmap 失败！！！\n"
#define MSGTR_LIBVO_TDFXVID_GetImageTodo "获得图像 todo。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailed "[VO_TDFXVID] AGP 移动操作失败。\n"
#define MSGTR_LIBVO_TDFXVID_SetYuvFailed "[VO_TDFXVID] 设置 YUV 失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnYPlane "[VO_TDFXVID] Y 平面 AGP 移动操作失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnUPlane "[VO_TDFXVID] U 平面 AGP 移动操作失败。\n"
#define MSGTR_LIBVO_TDFXVID_AgpMoveFailedOnVPlane "[VO_TDFXVID] V 平面 AGP 移动操作失败。\n"
#define MSGTR_LIBVO_TDFXVID_UnknownFormat "[VO_TDFXVID] 未知格式：0x%x。\n"

// vo_tdfxfb.c
#define MSGTR_LIBVO_TDFXFB_CantOpen "[VO_TDFXFB] 无法打开 %s：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetFscreenInfo "[VO_TDFXFB] FBITGET_FSCREENINFO ioctl 故障：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ProblemWithFbitgetVscreenInfo "[VO_TDFXFB] FBITGET_VSCREENINFO ioctl 故障：%s。\n"
#define MSGTR_LIBVO_TDFXFB_ThisDriverOnlySupports "[VO_TDFXFB] 该驱动仅支持 3Dfx Banshee、Voodoo3 和 Voodoo 5。\n"
#define MSGTR_LIBVO_TDFXFB_OutputIsNotSupported "[VO_TDFXFB] 不支持 %d bpp 输出。\n"
#define MSGTR_LIBVO_TDFXFB_CouldntMapMemoryAreas "[VO_TDFXFB] 无法映射内存区域：%s。\n"
#define MSGTR_LIBVO_TDFXFB_BppOutputIsNotSupported "[VO_TDFXFB] 不支持 %d bpp 输出（这种情况不应出现）。\n"
#define MSGTR_LIBVO_TDFXFB_SomethingIsWrongWithControl "[VO_TDFXFB] 呃！control() 有点问题。\n"
#define MSGTR_LIBVO_TDFXFB_NotEnoughVideoMemoryToPlay "[VO_TDFXFB] 没有足够的显存播放此影片，请尝试使用较低的分辨率。\n"
#define MSGTR_LIBVO_TDFXFB_ScreenIs "[VO_TDFXFB] 屏幕尺寸为 %dx%d 色深 %d bpp，输入尺寸 %dx%d 色深 %d bpp，标准化尺寸 %dx%d。\n"

// vo_tga.c
#define MSGTR_LIBVO_TGA_UnknownSubdevice "[VO_TGA] 未知子设备：%s。\n"

// vo_vesa.c
#define MSGTR_LIBVO_VESA_FatalErrorOccurred "[VO_VESA] 发生致命错误！无法继续。\n"
#define MSGTR_LIBVO_VESA_UnknownSubdevice "[VO_VESA] 未知子设备：‘%s’。\n"
#define MSGTR_LIBVO_VESA_YouHaveTooLittleVideoMemory "[VO_VESA] 显存太小无法支持该模式：\n[VO_VESA] 需要：%08lX 现有：%08lX。\n"
#define MSGTR_LIBVO_VESA_YouHaveToSpecifyTheCapabilitiesOfTheMonitor "[VO_VESA] 需要指定显示器的性能。未改变刷新频率。\n"
#define MSGTR_LIBVO_VESA_UnableToFitTheMode "[VO_VESA] 模式超出显示器的限制。未改变刷新频率。\n"
#define MSGTR_LIBVO_VESA_DetectedInternalFatalError "[VO_VESA] 检测到内部致命错误：init 在 preinit 前被调用。\n"
#define MSGTR_LIBVO_VESA_SwitchFlipIsNotSupported "[VO_VESA] 不支持 -flip 命令。\n"
#define MSGTR_LIBVO_VESA_PossibleReasonNoVbe2BiosFound "[VO_VESA] 可能的原因：未找到 VBE2 BIOS。\n"
#define MSGTR_LIBVO_VESA_FoundVesaVbeBiosVersion "[VO_VESA] 找到 VESA VBE BIOS 版本 %x.%x 修订版本：%x。\n"
#define MSGTR_LIBVO_VESA_VideoMemory "[VO_VESA] 显存：%u Kb。\n"
#define MSGTR_LIBVO_VESA_Capabilites "[VO_VESA] VESA 性能：%s %s %s %s %s。\n"
#define MSGTR_LIBVO_VESA_BelowWillBePrintedOemInfo "[VO_VESA] ！！！下面将显示 OEM 信息！！！\n"
#define MSGTR_LIBVO_VESA_YouShouldSee5OemRelatedLines "[VO_VESA] 应该看到 5 行 OEM 的相关内容；否则说明 vm86 有问题。\n"
#define MSGTR_LIBVO_VESA_OemInfo "[VO_VESA] OEM 信息：%s。\n"
#define MSGTR_LIBVO_VESA_OemRevision "[VO_VESA] OEM 版本：%x。\n"
#define MSGTR_LIBVO_VESA_OemVendor "[VO_VESA] OEM 销售商：%s。\n"
#define MSGTR_LIBVO_VESA_OemProductName "[VO_VESA] OEM 产品名：%s。\n"
#define MSGTR_LIBVO_VESA_OemProductRev "[VO_VESA] OEM 产品版本：%s。\n"
#define MSGTR_LIBVO_VESA_Hint "[VO_VESA] 提示：为使用电视输出你应当在启动前插入电视接头。\n"\
"[VO_VESA] 因为 VESA BIOS 只在自检的时候执行初始化。\n"
#define MSGTR_LIBVO_VESA_UsingVesaMode "[VO_VESA] 使用 VESA 模式 (%u) = %x [%ux%u@%u]\n"
#define MSGTR_LIBVO_VESA_CantInitializeSwscaler "[VO_VESA] 无法初始化软件缩放功能。\n"
#define MSGTR_LIBVO_VESA_CantUseDga "[VO_VESA] 无法使用 DGA。强制使用组交换模式。 :(\n"
#define MSGTR_LIBVO_VESA_UsingDga "[VO_VESA] 使用 DGA（物理资源: %08lXh, %08lXh）"
#define MSGTR_LIBVO_VESA_CantUseDoubleBuffering "[VO_VESA] 无法使用双重缓冲：显存不足。\n"
#define MSGTR_LIBVO_VESA_CantFindNeitherDga "[VO_VESA] 无法找到 DGA 或是可再分配的视频窗口框架。\n"
#define MSGTR_LIBVO_VESA_YouveForcedDga "[VO_VESA] 已强制使用 DGA。正在退出\n"
#define MSGTR_LIBVO_VESA_CantFindValidWindowAddress "[VO_VESA] 无法找到有效的视频窗口地址。\n"
#define MSGTR_LIBVO_VESA_UsingBankSwitchingMode "[VO_VESA] 使用组交换模式（物理资源：%08lXh, %08lXh）。\n"
#define MSGTR_LIBVO_VESA_CantAllocateTemporaryBuffer "[VO_VESA] 无法分配临时缓冲。\n"
#define MSGTR_LIBVO_VESA_SorryUnsupportedMode "[VO_VESA] 抱歉，不支持该模式——请尝试 -x 640 -zoom。\n"
#define MSGTR_LIBVO_VESA_OhYouReallyHavePictureOnTv "[VO_VESA] 噢，你的电视上有图像了！\n"
#define MSGTR_LIBVO_VESA_CantInitialozeLinuxVideoOverlay "[VO_VESA] 无法初始化 Linux 视频覆盖模式。\n"
#define MSGTR_LIBVO_VESA_UsingVideoOverlay "[VO_VESA] 使用视频覆盖模式：%s。\n"
#define MSGTR_LIBVO_VESA_CantInitializeVidixDriver "[VO_VESA] 无法初始化 VIDIX 驱动。\n"
#define MSGTR_LIBVO_VESA_UsingVidix "[VO_VESA] 正在使用 VIDIX。\n"
#define MSGTR_LIBVO_VESA_CantFindModeFor "[VO_VESA] 无法找到适合的模式用于：%ux%u@%u。\n"
#define MSGTR_LIBVO_VESA_InitializationComplete "[VO_VESA] VESA 初始化完成。\n"

// vesa_lvo.c
#define MSGTR_LIBVO_VESA_ThisBranchIsNoLongerSupported "[VESA_LVO] 这个分支版本已经不再维护。\n[VESA_LVO] 请使用 -vo vesa:vidix。\n"
#define MSGTR_LIBVO_VESA_CouldntOpen "[VESA_LVO] 无法打开：‘%s’\n"
#define MSGTR_LIBVO_VESA_InvalidOutputFormat "[VESA_LVI] 无效的输出格式：%s(%0X)\n"
#define MSGTR_LIBVO_VESA_IncompatibleDriverVersion "[VESA_LVO] fb_vid 驱动版本与该版本 MPlayer 不兼容！\n"

// vo_x11.c

// vo_xv.c
#define MSGTR_LIBVO_XV_SharedMemoryNotSupported "[VO_XV] 不支持共享内存模式\n回复到正常 Xv 模式。\n"
#define MSGTR_LIBVO_XV_XvNotSupportedByX11 "[VO_XV] 对不起，该 X11 版本/驱动不支持 Xv\n[VO_XV] ******** 请尝试使用 -vo x11 或  -vo sdl  *********\n"
#define MSGTR_LIBVO_XV_XvQueryAdaptorsFailed  "[VO_XV] XvQueryAdaptors 失败。\n"
#define MSGTR_LIBVO_XV_InvalidPortParameter "[VO_XV] 无效的端口参数，改用端口 0。\n"
#define MSGTR_LIBVO_XV_CouldNotGrabPort "[VO_XV] 无法绑定端口 %i。\n"
#define MSGTR_LIBVO_XV_CouldNotFindFreePort "[VO_XV] 无法找到空闲的 Xvideo 端口 - 或许另一进程已经正在使用\n"\
"[VO_XV] 该端口。关闭所有视频应用程序并再次尝试。如果那样做仍然无效，则\n"\
"[VO_XV] 参见‘mplayer -vo help’获取其它（非 xv）视频输出驱动的信息。\n"
#define MSGTR_LIBVO_XV_NoXvideoSupport "[VO_XV] 好像没有可用的 Xvideo 能够支持你的显卡。\n"\
"[VO_XV] 运行‘xvinfo’检查其是否支持 Xv 并阅读\n"\
"[VO_XV] DOCS/HTML/en/video.html#xv！\n"\
"[VO_XV] 参见‘mplayer -vo help’获取其它（非 xv）视频输出驱动的信息。\n"\
"[VO_XV] 试试 -vo x11。\n"
#define MSGTR_VO_XV_ImagedimTooHigh "源图像尺寸过大：%ux%u（最大值限制为 %ux%u）\n"

// vo_yuv4mpeg.c
#define MSGTR_VO_YUV4MPEG_InterlacedHeightDivisibleBy4 "隔行扫描模式要求图像高度能被 4 整除。"
#define MSGTR_VO_YUV4MPEG_WidthDivisibleBy2 "图像宽度必须能被 2 整除。"
#define MSGTR_VO_YUV4MPEG_OutFileOpenError "无法获取内存或文件句柄以写入“%s”！"
#define MSGTR_VO_YUV4MPEG_OutFileWriteError "图像写入输出错误！"
#define MSGTR_VO_YUV4MPEG_UnknownSubDev "未知子设备：%s"
#define MSGTR_VO_YUV4MPEG_InterlacedTFFMode "使用隔行扫描输出模式，上半扫描场优先方式。"
#define MSGTR_VO_YUV4MPEG_InterlacedBFFMode "使用隔行扫描输出模式，下半扫描场优先方式。"
#define MSGTR_VO_YUV4MPEG_ProgressiveMode "使用（默认的）逐行扫描帧模式。"

// vobsub_vidix.c
#define MSGTR_LIBVO_SUB_VIDIX_CantStartPlayback "[VO_SUB_VIDIX] 无法开始播放：%s\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantStopPlayback "[VO_SUB_VIDIX] 无法停止播放：%s\n"
#define MSGTR_LIBVO_SUB_VIDIX_InterleavedUvForYuv410pNotSupported "[VO_SUB_VIDIX] 不支持 YUV410P 下的交错 UV 模式。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DummyVidixdrawsliceWasCalled "[VO_SUB_VIDIX] 调用伪 vidix_draw_slice()。\n"
#define MSGTR_LIBVO_SUB_VIDIX_UnsupportedFourccForThisVidixDriver "[VO_SUB_VIDIX] 该 VIDIX 驱动不支持的 FourCC 代码：%x (%s)。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedResolution "[VO_SUB_VIDIX] 视频服务器使用了不支持分辨率（%dx%d），支持的分辨率：%dx%d-%dx%d。\n"
#define MSGTR_LIBVO_SUB_VIDIX_VideoServerHasUnsupportedColorDepth "[VO_SUB_VIDIX] 视频服务器使用了 VIDIX 不支持的色彩深度（%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantUpscaleImage "[VO_SUB_VIDIX] VIDIX 驱动无法放大图像（%d%d -> %d%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_DriverCantDownscaleImage "[VO_SUB_VIDIX] VIDIX 驱动无法缩小图像（%d%d -> %d%d）。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CantConfigurePlayback "[VO_SUB_VIDIX] 无法配置播放功能：%s。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntFindWorkingVidixDriver "[VO_SUB_VIDIX] 无法找到可用的 VIDIX 驱动。\n"
#define MSGTR_LIBVO_SUB_VIDIX_CouldntGetCapability "[VO_SUB_VIDIX] 无法获得性能信息：%s。\n"

// x11_common.c
#define MSGTR_EwmhFullscreenStateFailed "\nX11：无法发送 EWMH 全屏事件消息！\n"
#define MSGTR_SelectedVideoMode "XF86VM：已选择视频模式 %dx%d（图像大小 %dx%d）。\n"
#define MSGTR_X11Error "X11 错误：%s\n"
#define MSGTR_CouldntFindUTF8Locale "无法找到 UTF-8 的地区设定，\n超出 Latin-1 范围的字符将无法处理。\n"
#define MSGTR_CouldntOpenDisplay "vo：无法打开 X11 显示终端（%s）！\n"
#define MSGTR_WindowDestroyed "窗口已销毁，退出中\n"
#define MSGTR_DPMSnotAvailable "DPMS 不可用？\n"
#define MSGTR_DPMSnotEnabled "无法启用 DPMS\n"
#define MSGTR_BadAccessXSelectInput "X11 错误：XSelectInput 调用时出现 BadAccess 异常\n"
#define MSGTR_ButtonPressMaskInUse "X11 错误：指定窗口的‘ButtonPressMask’蒙板可能已被其它应用程序使用（参见 man XSelectInput）\n"
#define MSGTR_DiscardMouseControl "X11 错误：MPlayer 失去鼠标的操控（重新配置中）\n"
#define MSGTR_NoXF86VidModeExtension "XF86VidMode 扩展组件不可用。\n"
#define MSGTR_ReturningOriginalMode "重设为原来的模式 %dx%d\n"
#define MSGTR_CouldntSetColorkey "[xv common] 无法设置 colorkey!\n"
#define MSGTR_CouldntGetColorkey "[xv common] 无法获得 colorkey！可能所选的 Xv 端口没有采用覆盖模式。\n"

#define MSGTR_InsertingAfVolume "[混音器] 没有硬件混音支持，插入音量过滤器。\n"
#define MSGTR_NoVolume "[混音器] 没有可用的音量控制器。\n"
#define MSGTR_NoBalance "[混音器] 没有可用的均衡控制器。\n"

// old vo drivers that have been replaced
#define MSGTR_VO_PGM_HasBeenReplaced "PGM 视频输出驱动已替换为 -vo pnm:pgmyuv。\n"
#define MSGTR_VO_MD5_HasBeenReplaced "MD5 视频输出驱动已替换为 -vo md5sum。\n"
#define MSGTR_VO_GL2_HasBeenRenamed "g12 视频输出驱动已更名为 -vo gl_tiled，但实际上应使用 -vo gl。\n"


// ======================= audio output drivers ========================

// audio_out.c
#define MSGTR_AO_ALSA9_1x_Removed "音频输出：alsa9 和 alsa1x 模块已不支持，请用 -ao alsa 代替。\n"
#define MSGTR_AO_NoSuchDriver "无此音频驱动‘%.*s’\n"
#define MSGTR_AO_FailedInit "未能初始化音频驱动‘%s’\n"

// ao_oss.c
#define MSGTR_AO_OSS_CantOpenMixer "[AO OSS] audio_setup：无法打开混音设备 %s：%s\n"
#define MSGTR_AO_OSS_ChanNotFound "[AO OSS] audio_setup：声卡混音器没有‘%s’声道，使用默认声道。\n"
#define MSGTR_AO_OSS_CantOpenDev "[AO OSS] audio_setup：无法打开音频设备 %s：%s\n"
#define MSGTR_AO_OSS_CantMakeFd "[AO OSS] audio_setup：无法禁止对于文件描述符的访问：%s\n"
#define MSGTR_AO_OSS_CantSet "[AO OSS] 无法设定音频设备 %s 至 %s 的输出，尝试使用 %s...\n"
#define MSGTR_AO_OSS_CantSetChans "[AO OSS] audio_setup：设置音频设备为 %d 声道失败。\n"
#define MSGTR_AO_OSS_CantUseGetospace "[AO OSS] audio_setup：驱动不支持 SNDCTL_DSP_GETOSPACE :-(\n"
#define MSGTR_AO_OSS_CantUseSelect "[AO OSS]\n   ***  你的音频驱动不支持 select()  ***\n 需在 config.h 中配置 #undef HAVE_AUDIO_SELECT 后重新编译 MPlayer！\n\n"
#define MSGTR_AO_OSS_CantReopen "[AO OSS]\n致命错误：*** 无法重新打开或重新启动音频设备 *** %s\n"
#define MSGTR_AO_OSS_UnknownUnsupportedFormat "[AO OSS] 未知/不支持的 OSS 格式：%x。\n"

// ao_arts.c
#define MSGTR_AO_ARTS_CantInit "[AO ARTS] %s\n"
#define MSGTR_AO_ARTS_ServerConnect "[AO ARTS] 已连接到声音服务设备。\n"
#define MSGTR_AO_ARTS_CantOpenStream "[AO ARTS] 无法打开音频流。\n"
#define MSGTR_AO_ARTS_StreamOpen "[AO ARTS] 音频流已打开。\n"
#define MSGTR_AO_ARTS_BufferSize "[AO ARTS] 缓冲大小：%d\n"

// ao_dxr2.c
#define MSGTR_AO_DXR2_SetVolFailed "[AO DXR2] 未能设定音量为 %d。\n"
#define MSGTR_AO_DXR2_UnsupSamplerate "[AO DXR2] 不支持 %d Hz，尝试对音频重新采样。\n"

// ao_esd.c
#define MSGTR_AO_ESD_CantOpenSound "[AO ESD] esd_open_sound 失败：%s\n"
#define MSGTR_AO_ESD_LatencyInfo "[AO ESD] 延迟：[服务器：%0.2fs，网络：%0.2fs]（调整值 %0.2fs）\n"
#define MSGTR_AO_ESD_CantOpenPBStream "[AO ESD] 打开 ESD 播放流失败：%s\n"

// ao_mpegpes.c
#define MSGTR_AO_MPEGPES_CantSetMixer "[AO MPEGPES] DVB 音频组件混音器错误：%s。\n"
#define MSGTR_AO_MPEGPES_UnsupSamplerate "[AO MPEGPES] 不支持 %d Hz，尝试对音频重新采样。\n"

// ao_pcm.c
#define MSGTR_AO_PCM_FileInfo "[AO PCM] 文件: %s (%s)\nPCM：采样率：%iHz 声道：%s 格式 %s\n"
#define MSGTR_AO_PCM_HintInfo "[AO PCM] 信息：使用 -benchmark -vc null -vo null -ao pcm:fast 可以更快地转储数据\n[AO PCM] 信息：要写入 WAVE 文件，使用 -ao pcm:waveheader（默认模式）。\n"
#define MSGTR_AO_PCM_CantOpenOutputFile "[AO PCM] 打开并写入 %s 失败！\n"

// ao_sdl.c
#define MSGTR_AO_SDL_INFO "[AO SDL] 采样率：%iHz 声道：%s 格式 %s\n"
#define MSGTR_AO_SDL_DriverInfo "[AO SDL] 使用 %s 音频驱动。\n"
#define MSGTR_AO_SDL_UnsupportedAudioFmt "[AO SDL] 不支持的音频格式：0x%x。\n"
#define MSGTR_AO_SDL_CantInit "[AO SDL] SDL 音频初始化失败：%s\n"
#define MSGTR_AO_SDL_CantOpenAudio "[AO SDL] 无法打开音频：%s\n"

// ao_sgi.c
#define MSGTR_AO_SGI_INFO "[AO SGI] 控制。\n"
#define MSGTR_AO_SGI_InitInfo "[AO SGI] 初始化：采样率：%iHz 声道：%s 格式 %s\n"
#define MSGTR_AO_SGI_InvalidDevice "[AO SGI] 播放：无效的设备。\n"
#define MSGTR_AO_SGI_CantSetParms_Samplerate "[AO SGI] 初始化：setparams 失败：%s\n无法设置为所需的采样率。\n"
#define MSGTR_AO_SGI_CantSetAlRate "[AO SGI] 初始化：AL_RATE 不适用于给定的音频来源。\n"
#define MSGTR_AO_SGI_CantGetParms "[AO SGI] 初始化：getparams 失败：%s\n"
#define MSGTR_AO_SGI_SampleRateInfo "[AO SGI] 初始化：当前的采样率为 %f（所需的速率为 %f）\n"
#define MSGTR_AO_SGI_InitConfigError "[AO SGI] 初始化：%s\n"
#define MSGTR_AO_SGI_InitOpenAudioFailed "[AO SGI] 初始化：无法打开音频声道：%s\n"
#define MSGTR_AO_SGI_Uninit "[AO SGI] 终止：...\n"
#define MSGTR_AO_SGI_Reset "[AO SGI] 重启：...\n"
#define MSGTR_AO_SGI_PauseInfo "[AO SGI] audio_pause：...\n"
#define MSGTR_AO_SGI_ResumeInfo "[AO SGI] audio_resume：...\n"

// ao_sun.c
#define MSGTR_AO_SUN_RtscSetinfoFailed "[AO SUN] rtsc：SETINFO 失败。\n"
#define MSGTR_AO_SUN_RtscWriteFailed "[AO SUN] rtsc：写入失败。\n"
#define MSGTR_AO_SUN_CantOpenAudioDev "[AO SUN] 无法打开音频设备 %s，%s  -> nosound。\n"
#define MSGTR_AO_SUN_UnsupSampleRate "[AO SUN] 音频设置：你的声卡不支持 %d 声道，%s，%d Hz 采样率。\n"

// ao_alsa.c
#define MSGTR_AO_ALSA_InvalidMixerIndexDefaultingToZero "[AO_ALSA] 无效的混音器索引号。取默认值 0。\n"
#define MSGTR_AO_ALSA_MixerOpenError "[AO_ALSA] 打开混音器错误：%s\n"
#define MSGTR_AO_ALSA_MixerAttachError "[AO_ALSA] 混音器接入 %s 错误：%s\n"
#define MSGTR_AO_ALSA_MixerRegisterError "[AO_ALSA] 混音器登记错误：%s\n"
#define MSGTR_AO_ALSA_MixerLoadError "[AO_ALSA] 混音器装载错误：%s\n"
#define MSGTR_AO_ALSA_UnableToFindSimpleControl "[AO_ALSA] 无法找到简单控制器‘%s’，%i。\n"
#define MSGTR_AO_ALSA_ErrorSettingLeftChannel "[AO_ALSA] 设置左声道错误，%s\n"
#define MSGTR_AO_ALSA_ErrorSettingRightChannel "[AO_ALSA] 设置右声道错误，%s\n"
#define MSGTR_AO_ALSA_CommandlineHelp "\n[AO_ALSA] -ao alsa 命令行帮助：\n"\
"[AO_ALSA] 示例：mplayer -ao alsa:device=hw=0.3\n"\
"[AO_ALSA]   设置第一块声卡为第四硬件设备。\n\n"\
"[AO_ALSA] 选项：\n"\
"[AO_ALSA]   noblock\n"\
"[AO_ALSA]     以非阻塞模式打开设备。\n"\
"[AO_ALSA]   device=<设备名>\n"\
"[AO_ALSA]     设置设备（将字符中的‘,’替换为‘.’而‘:’替换为‘=’）\n"
#define MSGTR_AO_ALSA_ChannelsNotSupported "[AO_ALSA] 不支持 %d 声道。\n"
#define MSGTR_AO_ALSA_OpenInNonblockModeFailed "[AO_ALSA] 以非阻塞模式打开失败，尝试以阻塞模式打开。\n"
#define MSGTR_AO_ALSA_PlaybackOpenError "[AO_ALSA] 打开播放模式错误：%s\n"
#define MSGTR_AO_ALSA_ErrorSetBlockMode "[AL_ALSA] 设置阻塞模式 %s 错误。\n"
#define MSGTR_AO_ALSA_UnableToGetInitialParameters "[AO_ALSA] 无法得到初始参数：%s\n"
#define MSGTR_AO_ALSA_UnableToSetAccessType "[AO_ALSA] 无法设置访问类型：%s\n"
#define MSGTR_AO_ALSA_FormatNotSupportedByHardware "[AO_ALSA] 硬件不支持的格式 %s，尝试默认格式。\n"
#define MSGTR_AO_ALSA_UnableToSetFormat "[AO_ALSA] 无法设置格式：%s\n"
#define MSGTR_AO_ALSA_UnableToSetChannels "[AO_ALSA] 无法设置声道：%s\n"
#define MSGTR_AO_ALSA_UnableToDisableResampling "[AO_ALSA] 无法禁用重抽样模式：%s\n"
#define MSGTR_AO_ALSA_UnableToSetSamplerate2 "[AO_ALSA] 无法设置采样率-2：%s\n"
#define MSGTR_AO_ALSA_UnableToSetBufferTimeNear "[AO_ALSA] 无法设置缓冲时间为约：%s\n"
#define MSGTR_AO_ALSA_UnableToGetPeriodSize "[AO ALSA] 无法获得周期大小：%s\n"
#define MSGTR_AO_ALSA_UnableToSetPeriods "[AO_ALSA] 无法设置周期：%s\n"
#define MSGTR_AO_ALSA_UnableToSetHwParameters "[AO_ALSA] 无法设置 hw-parameters：%s\n"
#define MSGTR_AO_ALSA_UnableToGetBufferSize "[AO_ALSA] 无法获得缓冲大小：%s\n"
#define MSGTR_AO_ALSA_UnableToGetSwParameters "[AO_ALSA] 无法获得 sw-parameters：%s\n"
#define MSGTR_AO_ALSA_UnableToGetBoundary "[AO_ALSA] 无法获得边界信息：%s\n"
#define MSGTR_AO_ALSA_UnableToSetStartThreshold "[AO_ALSA] 无法设置开始阈值：%s\n"
#define MSGTR_AO_ALSA_UnableToSetStopThreshold "[AO_ALSA] 无法设置停止阈值：%s\n"
#define MSGTR_AO_ALSA_UnableToSetSilenceSize "[AO_ALSA] 无法设置静音大小：%s\n"
#define MSGTR_AO_ALSA_PcmCloseError "[AO_ALSA] pcm 关闭错误：%s\n"
#define MSGTR_AO_ALSA_NoHandlerDefined "[AO_ALSA] 未定义处理设备！\n"
#define MSGTR_AO_ALSA_PcmPrepareError "[AO_ALSA] pcm 准备错误：%s\n"
#define MSGTR_AO_ALSA_PcmPauseError "[AO_ALSA] pcm 暂停错误：%s\n"
#define MSGTR_AO_ALSA_PcmDropError "[AO_ALSA] pcm 丢弃错误：%s\n"
#define MSGTR_AO_ALSA_PcmResumeError "[AO_ALSA] pcm 继续播放错误：%s\n"
#define MSGTR_AO_ALSA_DeviceConfigurationError "[AO_ALSA] 设备配置错误。"
#define MSGTR_AO_ALSA_PcmInSuspendModeTryingResume "[AO_ALSA] Pcm 处于挂起模式，尝试继续播放。\n"
#define MSGTR_AO_ALSA_WriteError "[AO_ALSA] 写入错误：%s\n"
#define MSGTR_AO_ALSA_TryingToResetSoundcard "[AO_ALSA] 尝试重启声卡。\n"
#define MSGTR_AO_ALSA_CannotGetPcmStatus "[AO_ALSA] 无法获得 pcm 状态：%s\n"

// ao_plugin.c


// ======================= audio filters ================================

// af_scaletempo.c
#define MSGTR_AF_ValueOutOfRange MSGTR_VO_ValueOutOfRange

// af_ladspa.c
#define MSGTR_AF_LADSPA_AvailableLabels "可用标签"
#define MSGTR_AF_LADSPA_WarnNoInputs "警告！该 LADSPA 插件没有音频输入。\n输入的音频信号将会丢失。"
#define MSGTR_AF_LADSPA_ErrNoOutputs "该 LADSPA 插件没有音频输出。"
#define MSGTR_AF_LADSPA_ErrInOutDiff "LADSPA 插件的音频输入和音频输出的数目不相等。"
#define MSGTR_AF_LADSPA_ErrFailedToLoad "装载失败"
#define MSGTR_AF_LADSPA_ErrNoDescriptor "指定的库文件中未能找到 ladspa_descriptor() 函数。"
#define MSGTR_AF_LADSPA_ErrLabelNotFound "未能在插件库里找到标签。"
#define MSGTR_AF_LADSPA_ErrNoSuboptions "未指定子选项。"
#define MSGTR_AF_LADSPA_ErrNoLibFile "未指定库文件。"
#define MSGTR_AF_LADSPA_ErrNoLabel "未指定过滤器标签。"
#define MSGTR_AF_LADSPA_ErrNotEnoughControls "命令行中指定的控件不足。"
#define MSGTR_AF_LADSPA_ErrControlBelow "%s：输入控件 #%d 低于下限 %0.4f。\n"
#define MSGTR_AF_LADSPA_ErrControlAbove "%s：输入控件 #%d 高于上限 %0.4f。\n"

// format.c
#define MSGTR_AF_FORMAT_UnknownFormat "未知格式"


// ========================== INPUT =========================================

// joystick.c
#define MSGTR_INPUT_JOYSTICK_CantOpen "无法打开操纵杆设备 %s：%s\n"
#define MSGTR_INPUT_JOYSTICK_ErrReading "读取操纵杆设备时出错：%s\n"
#define MSGTR_INPUT_JOYSTICK_LoosingBytes "操纵杆：丢失了 %d 字节的数据\n"
#define MSGTR_INPUT_JOYSTICK_WarnLostSync "操纵杆：初始化警告事件，与驱动程序失去同步。\n"
#define MSGTR_INPUT_JOYSTICK_WarnUnknownEvent "操作杆未知警告事件类型 %d\n"

// appleir.c
#define MSGTR_INPUT_APPLE_IR_CantOpen "无法打开 Apple IR 设备：%s\n"

// input.c
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyCmdFds "命令文件描述符太多，无法注册文件描述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrCantRegister2ManyKeyFds "按键文件描述符太多，无法注册文件描述符 %d。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeInt "命令 %s：参数 %d 不是整数。\n"
#define MSGTR_INPUT_INPUT_ErrArgMustBeFloat "命令 %s：参数 %d 不是浮点数。\n"
#define MSGTR_INPUT_INPUT_ErrUnterminatedArg "命令 %s：参数 %d 无结束符。\n"
#define MSGTR_INPUT_INPUT_ErrUnknownArg "未知参数 %d\n"
#define MSGTR_INPUT_INPUT_Err2FewArgs "命令 %s 需要至少 %d 个参数，然而只发现了 %d 个。\n"
#define MSGTR_INPUT_INPUT_ErrReadingCmdFd "读取命令文件描述符 %d 时发生错误：%s\n"
#define MSGTR_INPUT_INPUT_ErrCmdBufferFullDroppingContent "文件描述符 %d 的命令缓冲已满：丢弃文件内容。\n"
#define MSGTR_INPUT_INPUT_ErrInvalidCommandForKey "绑定按键 %s 的命令无效"
#define MSGTR_INPUT_INPUT_ErrSelect "选定错误：%s\n"
#define MSGTR_INPUT_INPUT_ErrOnKeyInFd "按键输入文件描述符 %d 错误\n"
#define MSGTR_INPUT_INPUT_ErrDeadKeyOnFd "按键输入文件描述符 %d 出现无效按键\n"
#define MSGTR_INPUT_INPUT_Err2ManyKeyDowns "同时有过多的按键事件发生\n"
#define MSGTR_INPUT_INPUT_ErrOnCmdFd "命令文件描述符 %d 错误\n"
#define MSGTR_INPUT_INPUT_ErrReadingInputConfig "读取输入配置文件 %s 时发生错误: %s\n"
#define MSGTR_INPUT_INPUT_ErrUnknownKey "未知键‘%s’\n"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForKeyName "此按键名的缓冲太小：%s\n"
#define MSGTR_INPUT_INPUT_ErrNoCmdForKey "未找到按键 %s 的命令"
#define MSGTR_INPUT_INPUT_ErrBuffer2SmallForCmd "命令 %s 的缓冲过小\n"
#define MSGTR_INPUT_INPUT_ErrCantInitJoystick "无法初始化输入操纵杆\n"
#define MSGTR_INPUT_INPUT_ErrCantOpenFile "无法打开 %s：%s\n"
#define MSGTR_INPUT_INPUT_ErrCantInitAppleRemote "无法初始化 Apple Remote 遥控器。\n"

// lirc.c
#define MSGTR_LIRCopenfailed "启用 LIRC 支持失败。将无法使用你的遥控器。\n"
#define MSGTR_LIRCcfgerr "读取 LIRC 配置文件 %s 失败。\n"


// ========================== LIBMPDEMUX ===================================

// muxer.c, muxer_*.c
#define MSGTR_TooManyStreams "媒体流太多！"
#define MSGTR_RawMuxerOnlyOneStream "原生音频流合并器只支持一个音频流！\n"
#define MSGTR_IgnoringVideoStream "忽略视频流！\n"
#define MSGTR_UnknownStreamType "警告，未知的媒体流类型：%d\n"
#define MSGTR_WarningLenIsntDivisible "警告，音频长度无法被采样率整除！\n"
#define MSGTR_MuxbufMallocErr "流合并器帧缓冲无法分配内存！\n"
#define MSGTR_MuxbufReallocErr "合路器帧缓冲无法重新分配内存！\n"
#define MSGTR_WritingHeader "正在写入帧头...\n"
#define MSGTR_WritingTrailer "正在写入索引...\n"

// demuxer.c, demux_*.c
#define MSGTR_AudioStreamRedefined "警告：重复定义了音频流头部 %d。\n"
#define MSGTR_VideoStreamRedefined "警告: 重复定义了视频流头部 %d。\n"
#define MSGTR_TooManyAudioInBuffer "\n缓冲中音频包太多（%d 个包存在于 %d 字节中）。\n"
#define MSGTR_TooManyVideoInBuffer "\n缓冲中视频包太多（%d 个包存在于 %d 字节中）。\n"
#define MSGTR_MaybeNI "可能播放了一个非交错合并或合并异常的媒体流/文件，或者是编解码器运行失败？\n" \
                      "修复该文件或尝试用 -ni 选项（可能造成内存用量巨大）。\n"
#define MSGTR_WorkAroundBlockAlignHeaderBug "AVI：避开 CBR-MP3 nBlockAlign 头部缺陷！\n"
#define MSGTR_SwitchToNi "\n检测到未正确交错合并的 AVI 文件 - 切换到 -ni 模式...\n"
#define MSGTR_InvalidAudioStreamNosound "AVI：无效的音频流 ID: %d - 忽略 (nosound)\n"
#define MSGTR_InvalidAudioStreamUsingDefault "AVI: 无效的视频流 ID：%d - 忽略（使用默认值）\n"
#define MSGTR_ON2AviFormat "ON2 AVI 格式"
#define MSGTR_Detected_XXX_FileFormat "检测到文件格式 %s。\n"
#define MSGTR_FormatNotRecognized "============= 抱歉, 此文件格式无法辨认或支持 ===============\n"\
                                  "===     如果此文件是 AVI、ASF 或 MPEG 媒体流，请联系开发者！   ===\n"
#define MSGTR_SettingProcessPriority "设置进程优先级：%s\n"
#define MSGTR_FilefmtFourccSizeFpsFtime "[V] 文件格式:%d  fourcc代码:0x%X  大小:%dx%d  帧速:%5.3f  每帧时长:=%6.4f\n"
#define MSGTR_CannotInitializeMuxer "无法初始化流合并器。"
#define MSGTR_MissingVideoStream "未找到视频流。\n"
#define MSGTR_MissingAudioStream "未找到音频流...  -> 没有声音。\n"
#define MSGTR_MissingVideoStreamBug "没有视频流！？请联系开发者，这可能是软件的缺陷 :(\n"

#define MSGTR_DoesntContainSelectedStream "流分离：文件中没有所选择的音频或视频流。\n"

#define MSGTR_NI_Forced "强制使用"
#define MSGTR_NI_Detected "检测到"
#define MSGTR_NI_Message "%s 非交错合并的 AVI 文件模式！\n"

#define MSGTR_UsingNINI "使用非交错合并的无效的 AVI 文件格式。\n"
#define MSGTR_CouldntDetFNo "无法确定（绝对定位所用的）帧数。\n"
#define MSGTR_CantSeekRawAVI "无法在原生的 AVI 流中定位。(需要索引信息，请尝试使用 -idx 选项。)\n"
#define MSGTR_CantSeekFile "无法在此文件中定位。\n"

#define MSGTR_MOVcomprhdr "MOV：需要 ZLIB 以支持经过压缩的文件头！\n"
#define MSGTR_MOVvariableFourCC "MOV：警告：检测到可变的 FourCC 代码！？\n"
#define MSGTR_MOVtooManyTrk "MOV：警告：轨道太多。"
#define MSGTR_ErrorOpeningOGGDemuxer "无法打开 Ogg 流分离器。\n"
#define MSGTR_CannotOpenAudioStream "无法打开音频流：%s\n"
#define MSGTR_CannotOpenSubtitlesStream "无法打开字幕流：%s\n"
#define MSGTR_OpeningAudioDemuxerFailed "未能打开音频分离器：%s\n"
#define MSGTR_OpeningSubtitlesDemuxerFailed "未能打开字幕流分离器：%s\n"
#define MSGTR_TVInputNotSeekable "电视信号输入无法定位！（定位功能可能应该用于更换频道）\n"
#define MSGTR_DemuxerInfoChanged "流分离器信息 %s 已变为 %s\n"
#define MSGTR_ClipInfo "媒体剪辑信息：\n"

#define MSGTR_LeaveTelecineMode "\ndemux_mpg：检测到 30000/1001fps 的 NTSC 内容，切换帧率。\n"
#define MSGTR_EnterTelecineMode "\ndemux_mpg：检测到 24000/1001fps 的逐行扫描 NTSC 内容，切换帧率。\n"

#define MSGTR_CacheFill "\r缓冲已填充：%5.2f%%（%"PRId64" 字节）"
#define MSGTR_NoBindFound "按键‘%s’未绑定功能。\n"
#define MSGTR_FailedToOpen "未能打开 %s。\n"

#define MSGTR_VideoID "[%s] 找到视频流，-vid %d\n"
#define MSGTR_AudioID "[%s] 找到音频流，-aid %d\n"
#define MSGTR_SubtitleID "[%s] 找到字幕流，-sid %d\n"

// asfheader.c
#define MSGTR_MPDEMUX_ASFHDR_HeaderSizeOver1MB "致命错误：文件头部大小超过 1 MB（%d）！\n请联系 MPlayer 的作者, 并且发送或上传此文件。\n"
#define MSGTR_MPDEMUX_ASFHDR_HeaderMallocFailed "无法为文件头部分配 %d 字节的存放空间。\n"
#define MSGTR_MPDEMUX_ASFHDR_EOFWhileReadingHeader "读 ASF 头部时遇到文件结尾，文件损坏或不完整？\n"
#define MSGTR_MPDEMUX_ASFHDR_DVRWantsLibavformat "DVR 可能只能与 libavformat 一同使用，如果有问题请尝试 -demuxer 35。\n"
#define MSGTR_MPDEMUX_ASFHDR_NoDataChunkAfterHeader "文件头部之后没有数据段！\n"
#define MSGTR_MPDEMUX_ASFHDR_AudioVideoHeaderNotFound "ASF：未找到音频或视频头部 - 文件损坏？\n"
#define MSGTR_MPDEMUX_ASFHDR_InvalidLengthInASFHeader "ASF 头部长度无效！\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMLicenseURL "DRM 许可证 URL 地址：%s\n"
#define MSGTR_MPDEMUX_ASFHDR_DRMProtected "该文件经过了 DRM 加密保护，无法在 Mplayer 中播放！\n"

// aviheader.c
#define MSGTR_MPDEMUX_AVIHDR_EmptyList "**空列表？！\n"
#define MSGTR_MPDEMUX_AVIHDR_WarnNotExtendedAVIHdr "** 警告：这不是扩展格式的 AVI 头部..\n"
#define MSGTR_MPDEMUX_AVIHDR_BuildingODMLidx "AVI：ODML：构造 ODML 索引 (%d superindexchunks)。\n"
#define MSGTR_MPDEMUX_AVIHDR_BrokenODMLfile "AVI：ODML：检测到损坏的（不完整的？）文件。将使用传统的索引模式。\n"
#define MSGTR_MPDEMUX_AVIHDR_CantReadIdxFile "无法读索引文件 %s：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_NotValidMPidxFile "%s 不是有效的 MPlayer 索引文件。\n"
#define MSGTR_MPDEMUX_AVIHDR_FailedMallocForIdxFile "无法为来自 %s 的索引数据分配内存。\n"
#define MSGTR_MPDEMUX_AVIHDR_PrematureEOF "过早结束的索引文件 %s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileLoaded "已加载索引文件：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_GeneratingIdx "正在生成索引：%3lu %s     \r"
#define MSGTR_MPDEMUX_AVIHDR_IdxGeneratedForHowManyChunks "AVI：为 %d 段数据生成索引表！\n"
#define MSGTR_MPDEMUX_AVIHDR_Failed2WriteIdxFile "无法写入索引文件 %s：%s\n"
#define MSGTR_MPDEMUX_AVIHDR_IdxFileSaved "已保存索引文件：%s\n"

// demux_audio.c
#define MSGTR_MPDEMUX_AUDIO_BadID3v2TagSize "音频分离器：ID3v2 标签大小异常：大于流的长度（%u）。\n"
#define MSGTR_MPDEMUX_AUDIO_DamagedAppendedID3v2Tag "音频分离器：检测到末尾中的 ID3v2 标签已损坏。\n"
#define MSGTR_MPDEMUX_AUDIO_UnknownFormat "音频流分解器：未知格式 %d。\n"

// demux_demuxers.c
#define MSGTR_MPDEMUX_DEMUXERS_FillBufferError "fill_buffer 错误：流分解器错误：非 vd、ad 或 sd。\n"

// demux_mkv.c
#define MSGTR_MPDEMUX_MKV_ZlibInitializationFailed "[mkv] zlib 初始化失败。\n"
#define MSGTR_MPDEMUX_MKV_ZlibDecompressionFailed "[mkv] zlib 解压失败。\n"
#define MSGTR_MPDEMUX_MKV_LzoDecompressionFailed "[mkv] lzo 解压失败。\n"
#define MSGTR_MPDEMUX_MKV_TrackEncrypted "[mkv] 轨道 %u 已加密，但解密还未实现。跳过该轨道。\n"
#define MSGTR_MPDEMUX_MKV_UnknownContentEncoding "[mkv] 轨道 %u 内容的编码类型未知。跳过该轨道。\n"
#define MSGTR_MPDEMUX_MKV_UnknownCompression "[mkv] 轨道 %u 使用了未知/不支持的压缩算法（%u）。\n[mkv] 跳过该轨道。\n"
#define MSGTR_MPDEMUX_MKV_ZlibCompressionUnsupported "[mkv] 轨道 %u 已经 zlib 压缩，但 mplayer 未以支持 zlib 压缩\n[mkv] 选项编译。跳过该轨道。\n"
#define MSGTR_MPDEMUX_MKV_TrackIDName "[mkv] 轨道 ID %u：%s (%s) \"%s\"，%s\n"
#define MSGTR_MPDEMUX_MKV_TrackID "[mkv] 轨道 ID %u：%s (%s), %s\n"
#define MSGTR_MPDEMUX_MKV_UnknownCodecID "[mkv] 未知的/不支持的编解码器 ID（%s），也可能是编解码器私有数据\n[mkv] 缺少/损坏（轨道 %u）。\n"
#define MSGTR_MPDEMUX_MKV_FlacTrackDoesNotContainValidHeaders "[mkv] FLAC 轨道未含有效的头部信息。\n"
#define MSGTR_MPDEMUX_MKV_UnknownAudioCodec "[mkv] 未知/不支持的音频编解码器 ID‘%s’（轨道 %u），也可能是编解码器\n[mkv] 私有数据缺少/损坏。\n"
#define MSGTR_MPDEMUX_MKV_SubtitleTypeNotSupported "[mkv] 未支持字幕类型‘%s’。\n"
#define MSGTR_MPDEMUX_MKV_WillPlayVideoTrack "[mkv] 将播放视频轨道 %u。\n"
#define MSGTR_MPDEMUX_MKV_NoVideoTrackFound "[mkv] 未找到/不需要视频轨道。\n"
#define MSGTR_MPDEMUX_MKV_NoAudioTrackFound "[mkv] 未找到/不需要音频轨道。\n"
#define MSGTR_MPDEMUX_MKV_NoBlockDurationForSubtitleTrackFound "[mkv] 警告：所找到的字幕轨道没有字幕段持续时间信息。\n"

// demux_nuv.c

// demux_xmms.c
#define MSGTR_MPDEMUX_XMMS_FoundPlugin "找到插件：%s（%s）。\n"
#define MSGTR_MPDEMUX_XMMS_ClosingPlugin "关闭插件：%s。\n"
#define MSGTR_MPDEMUX_XMMS_WaitForStart "等待 XMMS 插件开始播放‘%s’...\n"


// ========================== LIBMENU ===================================

// common
#define MSGTR_LIBMENU_NoEntryFoundInTheMenuDefinition "[MENU] 菜单定义中未找到菜单项。\n"

// libmenu/menu.c
#define MSGTR_LIBMENU_SyntaxErrorAtLine "[MENU] 语法错误：第 %d 行\n"
#define MSGTR_LIBMENU_MenuDefinitionsNeedANameAttrib "[MENU] 菜单定义需要名称属性（第 %d 行）。\n"
#define MSGTR_LIBMENU_BadAttrib "[MENU] 无效属性 %s=%s，菜单‘%s’的第 %d 行\n"
#define MSGTR_LIBMENU_UnknownMenuType "[MENU] 未知菜单类型‘%s’（第 %d 行）\n"
#define MSGTR_LIBMENU_CantOpenConfigFile "[MENU] 无法打开菜单配置文件：%s\n"
#define MSGTR_LIBMENU_ConfigFileIsTooBig "[MENU] 配置文件过大（> %d KB）\n"
#define MSGTR_LIBMENU_ConfigFileIsEmpty "[MENU] 配置文件为空。\n"
#define MSGTR_LIBMENU_MenuNotFound "[MENU] 未找到菜单 %s。\n"
#define MSGTR_LIBMENU_MenuInitFailed "[MENU] 菜单‘%s’：初始化失败。\n"
#define MSGTR_LIBMENU_UnsupportedOutformat "[MENU] 不支持该输出格式！\n"

// libmenu/menu_cmdlist.c
#define MSGTR_LIBMENU_ListMenuEntryDefinitionsNeedAName "[MENU] 定义列表菜单项需要给出名称（行 %d）。\n"
#define MSGTR_LIBMENU_ListMenuNeedsAnArgument "[MENU] 列表菜单需要给出参数。\n"

// libmenu/menu_console.c
#define MSGTR_LIBMENU_WaitPidError "[MENU] Waitpid 错误：%s。\n"
#define MSGTR_LIBMENU_SelectError "[MENU] Select 错误。\n"
#define MSGTR_LIBMENU_ReadErrorOnChildFD "[MENU] 子进程的文件描述符读取错误：%s。\n"
#define MSGTR_LIBMENU_ConsoleRun "[MENU] 终端运行：%s ...\n"
#define MSGTR_LIBMENU_AChildIsAlreadyRunning "[MENU] 子进程已经运行。\n"
#define MSGTR_LIBMENU_ForkFailed "[MENU] Fork 失败！！！\n"
#define MSGTR_LIBMENU_WriteError "[MENU] write 错误\n"

// libmenu/menu_filesel.c
#define MSGTR_LIBMENU_OpendirError "[MENU] opendir 错误：%s\n"
#define MSGTR_LIBMENU_ReallocError "[MENU] realloc 错误：%s\n"
#define MSGTR_LIBMENU_MallocError "[MENU] 内存分配错误：%s\n"
#define MSGTR_LIBMENU_ReaddirError "[MENU] readdir 错误：%s\n"
#define MSGTR_LIBMENU_CantOpenDirectory "[MENU] 无法打开目录 %s。\n"

// libmenu/menu_param.c
#define MSGTR_LIBMENU_SubmenuDefinitionNeedAMenuAttribut "[MENU] 定义子菜单需给出‘menu’属性。\n"
#define MSGTR_LIBMENU_InvalidProperty "[MENU] 首选项菜单中存在无效的属性值‘%s’。（行 %d）。\n"
#define MSGTR_LIBMENU_PrefMenuEntryDefinitionsNeed "[MENU] 定义首选项菜单条目需给出一个有效的‘property’或‘txt’属性（行 %d）。\n"
#define MSGTR_LIBMENU_PrefMenuNeedsAnArgument "[MENU] 首选项菜单需要给出参数。\n"

// libmenu/menu_pt.c
#define MSGTR_LIBMENU_CantfindTheTargetItem "[MENU] 无法找到目标项目？？\n"
#define MSGTR_LIBMENU_FailedToBuildCommand "[MENU] 未能生成菜单命令：%s。\n"

// libmenu/menu_txt.c
#define MSGTR_LIBMENU_MenuTxtNeedATxtFileName "[MENU] 文本菜单需要给出文本文件名（参数文件）。\n"
#define MSGTR_LIBMENU_MenuTxtCantOpen "[MENU] 无法打开 %s。\n"
#define MSGTR_LIBMENU_WarningTooLongLineSplitting "[MENU] 警告，行过长。将其分割。\n"
#define MSGTR_LIBMENU_ParsedLines "[MENU] 已解析行 %d。\n"

// libmenu/vf_menu.c
#define MSGTR_LIBMENU_UnknownMenuCommand "[MENU] 未知命令：‘%s’。\n"
#define MSGTR_LIBMENU_FailedToOpenMenu "[MENU] 未能打开菜单：‘%s’。\n"


// ========================== LIBMPCODECS ===================================

// dec_video.c & dec_audio.c:
#define MSGTR_CantOpenCodec "无法打开解码器。\n"
#define MSGTR_CantCloseCodec "无法关闭解码器。\n"

#define MSGTR_MissingDLLcodec "错误：无法打开所需的 DirectShow 编解码器：%s\n"
#define MSGTR_ACMiniterror "无法加载/初始化 Win32/ACM 音频解码器（缺少 DLL 文件？）。\n"
#define MSGTR_MissingLAVCcodec "无法在 libavcodec 中找到解码器‘%s’...\n"

#define MSGTR_MpegNoSequHdr "MPEG：致命错误：搜索 MPEG 序列头部时遇到文件结尾符。\n"
#define MSGTR_CannotReadMpegSequHdr "致命错误：无法读取 MPEG 序列头部。\n"
#define MSGTR_CannotReadMpegSequHdrEx "致命错误：无法读取 MPEG 序列头部扩展数据。\n"
#define MSGTR_BadMpegSequHdr "MPEG：无效的 MPEG 序列头部。\n"
#define MSGTR_BadMpegSequHdrEx "MPEG：无效的 MPEG 序列头部扩展数据。\n"

#define MSGTR_ShMemAllocFail "无法分配共享内存。\n"
#define MSGTR_CantAllocAudioBuf "无法分配音频输出缓冲。\n"

#define MSGTR_UnknownAudio "音频格式未知或缺失 -> 无声音输出\n"

#define MSGTR_UsingExternalPP "[PP] 使用外部的后期处理过滤器，max q = %d。\n"
#define MSGTR_UsingCodecPP "[PP] 使用编解码器的后期处理过滤器，max q = %d。\n"
#define MSGTR_VideoCodecFamilyNotAvailableStr "所请求使用的视频编解码器类 [%s] (vfm=%s) 不可用。\n需在编译时启用该模块。\n"
#define MSGTR_AudioCodecFamilyNotAvailableStr "所请求使用的音频编解码器类 [%s] (afm=%s) 不可用。\n需在编译时启用该模块。\n"
#define MSGTR_OpeningVideoDecoder "正打开视频解码器：[%s] %s\n"
#define MSGTR_SelectedVideoCodec "所选视频编解码器为：[%s] vfm: %s (%s)\n"
#define MSGTR_OpeningAudioDecoder "正打开音频解码器：[%s] %s\n"
#define MSGTR_SelectedAudioCodec "所选音频编解码器为：[%s] afm: %s (%s)\n"
#define MSGTR_VDecoderInitFailed "VDecoder 初始化失败 :(\n"
#define MSGTR_ADecoderInitFailed "ADecoder 初始化失败 :(\n"
#define MSGTR_ADecoderPreinitFailed "ADecoder 预先初始化失败 :(\n"

// libmpcodecs/ad_dvdpcm.c:
#define MSGTR_SamplesWanted "需要该格式的编码样本以优化相关技术支持。有意请联系开发者。\n"

// libmpcodecs/ad_libdv.c
#define MSGTR_MPCODECS_AudioFramesizeDiffers "[AD_LIBDV] 警告！音频帧大小不一致！read=%d  hdr=%d。\n"

// vd.c
#define MSGTR_CouldNotFindColorspace "无法找到匹配的色彩空间 - 使用 -vf scale 重试...\n"
#define MSGTR_MovieAspectIsSet "电影宽高比为 %.2f:1 - 预先放大到正确的电影宽高比。\n"
#define MSGTR_MovieAspectUndefined "电影宽高比未定义 - 没有使用预先放大功能。\n"

// vd_dshow.c, vd_dmo.c
#define MSGTR_DownloadCodecPackage "你需要升级/安装已编译编解码器包。\n请访问 http://www.mplayerhq.hu/dload.html\n"

// libmpcodecs/vd_dmo.c vd_dshow.c vd_vfw.c
#define MSGTR_MPCODECS_CouldntAllocateImageForCinepakCodec "[VD_DMO] 无法为 cinepak 编解码器分配进程映像。\n"

// libmpcodecs/vd_ffmpeg.c
#define MSGTR_MPCODECS_ArithmeticMeanOfQP "[VD_FFMPEG] QP 的算术平均值：%2.4f，QP 的调和平均值：%2.4f\n"
#define MSGTR_MPCODECS_DRIFailure "[VD_FFMPEG] DRI 失败。\n"
#define MSGTR_MPCODECS_CouldntAllocateImageForCodec "[VD_FFMPEG] 无法为编解码器分配进程镜像。\n"
#define MSGTR_MPCODECS_XVMCAcceleratedMPEG2 "[VD_FFMPEG] 经 XVMC 加速的 MPEG-2。\n"
#define MSGTR_MPCODECS_TryingPixfmt "[VD_FFMPEG] 尝试 pixfmt=%d。\n"
#define MSGTR_MPCODECS_McGetBufferShouldWorkOnlyWithXVMC "[VD_FFMPEG] Mc_get_buffer 只能用于 XVMC 加速！！"
#define MSGTR_MPCODECS_OnlyBuffersAllocatedByVoXvmcAllowed "[VD_FFMPEG] 只允许 vo_xvmc 分配的缓冲。\n"

// libmpcodecs/ve_lavc.c
#define MSGTR_MPCODECS_HighQualityEncodingSelected "[VE_LAVC] 已选择高品质编码（非实时编码）！\n"
#define MSGTR_MPCODECS_UsingConstantQscale "[VE_LAVC] 使用恒定的 qscale = %f（VBR）。\n"

// libmpcodecs/ve_raw.c
#define MSGTR_MPCODECS_OutputWithFourccNotSupported "[VE_RAW] 不支持 FourCC [%x] 的原生输出！\n"
#define MSGTR_MPCODECS_NoVfwCodecSpecified "[VE_RAW] 未指定必须的 VfW 编解码器！！\n"

// vf.c
#define MSGTR_CouldNotFindVideoFilter "无法找到视频滤镜‘%s’。\n"
#define MSGTR_CouldNotOpenVideoFilter "无法打开视频滤镜‘%s’。\n"
#define MSGTR_OpeningVideoFilter "打开视频滤镜："
#define MSGTR_CannotFindColorspace "即使通过插值方式，也无法找到匹配的色彩空间 :(\n"

// libmpcodecs/vf_crop.c
#define MSGTR_MPCODECS_CropBadPositionWidthHeight "[CROP] 无效的位置/宽度/高度 - 裁剪区域在原始图像范围外！\n"

// libmpcodecs/vf_cropdetect.c
#define MSGTR_MPCODECS_CropArea "[CROP] 裁剪区域：X: %d..%d  Y: %d..%d  (-vf crop=%d:%d:%d:%d)。\n"

// libmpcodecs/vf_format.c, vf_palette.c, vf_noformat.c
#define MSGTR_MPCODECS_UnknownFormatName "[VF_FORMAT] 未知格式名：‘%s’。\n"

// libmpcodecs/vf_framestep.c vf_noformat.c vf_palette.c vf_tile.c
#define MSGTR_MPCODECS_ErrorParsingArgument "[VF_FRAMESTEP] 解析参数错误。\n"

// libmpcodecs/ve_vfw.c
#define MSGTR_MPCODECS_CompressorType "压缩器类型：%.4lx\n"
#define MSGTR_MPCODECS_CompressorSubtype "压缩器子类型：%.4lx\n"
#define MSGTR_MPCODECS_CompressorFlags "压缩器标记：%lu，版本：%lu，ICM 版本：%lu\n"
#define MSGTR_MPCODECS_Flags "标记："
#define MSGTR_MPCODECS_Quality " 质量"

// libmpcodecs/vf_expand.c
#define MSGTR_MPCODECS_FullDRNotPossible "无法使用完全 DR 模式，尝试使用 SLICES！\n"
#define MSGTR_MPCODECS_FunWhydowegetNULL "为什么返回的是 NULL？？\n"

// libmpcodecs/vf_test.c, vf_yuy2.c, vf_yvu9.c
#define MSGTR_MPCODECS_WarnNextFilterDoesntSupport "下一个滤镜/视频输出不支持 %s :(\n"


// ================================== LIBASS ====================================

// ass_bitmap.c

// ass.c
#define MSGTR_LIBASS_FopenFailed "[ass] ass_read_file(%s)：fopen 失败\n"
#define MSGTR_LIBASS_RefusingToLoadSubtitlesLargerThan100M "[ass] ass_read_file(%s)：拒绝装入大于 100M 的字幕\n"

// ass_cache.c

// ass_fontconfig.c

// ass_render.c

// ass_font.c


// ================================== stream ====================================

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_CannotSetSamplerate "无法设置采样率。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetBufferTime "无法设置缓冲时间。\n"
#define MSGTR_MPDEMUX_AIALSA_CannotSetPeriodTime "无法设置采样间隔时间。\n"

// ai_alsa.c
#define MSGTR_MPDEMUX_AIALSA_PcmBrokenConfig "该 PCM 的配置文件已损坏：无可用配置。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableAccessType "无可用访问类型。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableSampleFmt "无可用采样格式。\n"
#define MSGTR_MPDEMUX_AIALSA_UnavailableChanCount "无可用声道数值 - 使用默认值：%d\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallHWParams "无法设置硬件参数：%s"
#define MSGTR_MPDEMUX_AIALSA_PeriodEqualsBufferSize "无法使用长度等于缓冲大小的采样间隔 (%u == %lu)\n"
#define MSGTR_MPDEMUX_AIALSA_CannotInstallSWParams "无法设置软件参数：\n"
#define MSGTR_MPDEMUX_AIALSA_ErrorOpeningAudio "打开音频出错：%s\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaStatusError "ALSA 状态错误：%s"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUN "ALSA xrun!!!（至少 %.3f ms 长）\n"
#define MSGTR_MPDEMUX_AIALSA_AlsaXRUNPrepareError "ALSA xrun：准备出错：%s"
#define MSGTR_MPDEMUX_AIALSA_AlsaReadWriteError "ALSA 读/写错误"

// ai_oss.c
#define MSGTR_MPDEMUX_AIOSS_Unable2SetChanCount "无法设置声道数：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetStereo "无法设置立体声：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2Open "无法打开‘%s’：%s\n"
#define MSGTR_MPDEMUX_AIOSS_UnsupportedFmt "格式不支持\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetAudioFmt "无法设置音频格式。"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetSamplerate "无法设置采样率：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2SetTrigger "无法设置触发器：%d\n"
#define MSGTR_MPDEMUX_AIOSS_Unable2GetBlockSize "无法获取数据块的大小！\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSizeZero "音频数据块的大小为零，设为 %d！\n"
#define MSGTR_MPDEMUX_AIOSS_AudioBlockSize2Low "音频数据块的大小太小，设为 %d！\n"

// asf_mmst_streaming.c
#define MSGTR_MPDEMUX_MMST_WriteError "写入出错\n"
#define MSGTR_MPDEMUX_MMST_EOFAlert "\n提示！到达文件末端\n"
#define MSGTR_MPDEMUX_MMST_PreHeaderReadFailed "预头部数据取失败\n"
#define MSGTR_MPDEMUX_MMST_InvalidHeaderSize "头部数据大小无效，放弃操作。\n"
#define MSGTR_MPDEMUX_MMST_HeaderDataReadFailed "读取头部数据失败。\n"
#define MSGTR_MPDEMUX_MMST_packet_lenReadFailed "读取 packet_len 失败。\n"
#define MSGTR_MPDEMUX_MMST_InvalidRTSPPacketSize "RTSP 包大小无效，放弃操作。\n"
#define MSGTR_MPDEMUX_MMST_CmdDataReadFailed "读取命令数据失败。\n"
#define MSGTR_MPDEMUX_MMST_HeaderObject "头部数据对象\n"
#define MSGTR_MPDEMUX_MMST_DataObject "数据对象\n"
#define MSGTR_MPDEMUX_MMST_FileObjectPacketLen "文件对象，包长 = %d (%d)\n"
#define MSGTR_MPDEMUX_MMST_StreamObjectStreamID "流对象，流 ID: %d\n"
#define MSGTR_MPDEMUX_MMST_2ManyStreamID "ID 过多，已跳过该数据流。"
#define MSGTR_MPDEMUX_MMST_UnknownObject "未知对象\n"
#define MSGTR_MPDEMUX_MMST_MediaDataReadFailed "读取媒体数据失败\n"
#define MSGTR_MPDEMUX_MMST_MissingSignature "签名数据不存在\n"
#define MSGTR_MPDEMUX_MMST_PatentedTechnologyJoke "所有操作结束。感谢下载含有知识产权及受专利保护技术的媒体文件。\n"
#define MSGTR_MPDEMUX_MMST_UnknownCmd "未知命令 %02x\n"
#define MSGTR_MPDEMUX_MMST_GetMediaPacketErr "get_media_packet 出错：%s\n"
#define MSGTR_MPDEMUX_MMST_Connected "已连接\n"

// asf_streaming.c
#define MSGTR_MPDEMUX_ASF_StreamChunkSize2Small "噢，stream_chunck 的大小过小：%d\n"
#define MSGTR_MPDEMUX_ASF_SizeConfirmMismatch "size_confirm 不匹配：%d %d\n"
#define MSGTR_MPDEMUX_ASF_WarnDropHeader "警告：丢弃头部数据？？\n"
#define MSGTR_MPDEMUX_ASF_ErrorParsingChunkHeader "解析数据区块头部时出错\n"
#define MSGTR_MPDEMUX_ASF_NoHeaderAtFirstChunk "未获得作为第一个区块的头部数据！！\n"
#define MSGTR_MPDEMUX_ASF_BufferMallocFailed "错误：无法分配长度为 %d 字节的缓冲。\n"
#define MSGTR_MPDEMUX_ASF_ErrReadingNetworkStream "读取网络流时出错。\n"
#define MSGTR_MPDEMUX_ASF_ErrChunk2Small "错误：区块太小。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallCannotPlay "带宽太小，无法播放文件！\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedAudio "带宽太小，取消选定的音频流。\n"
#define MSGTR_MPDEMUX_ASF_Bandwidth2SmallDeselectedVideo "带宽太小，取消选定的视频流。\n"
#define MSGTR_MPDEMUX_ASF_InvalidLenInHeader "ASF 头部数据长度无效！\n"
#define MSGTR_MPDEMUX_ASF_ErrChunkBiggerThanPacket "错误：chunk_size > packet_size\n"
#define MSGTR_MPDEMUX_ASF_ASFRedirector "=====> ASF 重定向器\n"
#define MSGTR_MPDEMUX_ASF_InvalidProxyURL "代理 URL 无效\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamType "未知的 ASF 流类型\n"
#define MSGTR_MPDEMUX_ASF_Failed2ParseHTTPResponse "解析 HTTP 响应失败。\n"
#define MSGTR_MPDEMUX_ASF_ServerReturn "服务器返回 %d:%s\n"
#define MSGTR_MPDEMUX_ASF_ASFHTTPParseWarnCuttedPragma "ASF HTTP 解析警告：指令 %s 被从 %zu 字节截断至 %zu 字节处\n"
#define MSGTR_MPDEMUX_ASF_SocketWriteError "Socket 写入出错：%s\n"
#define MSGTR_MPDEMUX_ASF_HeaderParseFailed "解析头部数据失败。\n"
#define MSGTR_MPDEMUX_ASF_NoStreamFound "未找到数据流。\n"
#define MSGTR_MPDEMUX_ASF_UnknownASFStreamingType "未知 ASF 流类型\n"
#define MSGTR_MPDEMUX_ASF_InfoStreamASFURL "STREAM_ASF，URL：%s\n"
#define MSGTR_MPDEMUX_ASF_StreamingFailed "操作失败，退出中。\n"

// audio_in.c
#define MSGTR_MPDEMUX_AUDIOIN_ErrReadingAudio "\n读取音频出错：%s\n"
#define MSGTR_MPDEMUX_AUDIOIN_XRUNSomeFramesMayBeLeftOut "已从交叉干扰状态恢复, 某些帧可能已丢失！\n"
#define MSGTR_MPDEMUX_AUDIOIN_ErrFatalCannotRecover "致命错误，无法恢复！\n"
#define MSGTR_MPDEMUX_AUDIOIN_NotEnoughSamples "\n音频采样数据不足！\n"

// cache2.c

// network.c
#define MSGTR_MPDEMUX_NW_UnknownAF "未知网络地址类 %d\n"
#define MSGTR_MPDEMUX_NW_ResolvingHostForAF "正在解析 %s 用于 %s...\n"
#define MSGTR_MPDEMUX_NW_CantResolv "无解析网络名用于 %s：%s\n"
#define MSGTR_MPDEMUX_NW_ConnectingToServer "正在连接到服务器 %s[%s]：%d...\n"
#define MSGTR_MPDEMUX_NW_CantConnect2Server "使用 %s 连接服务器失败\n"
#define MSGTR_MPDEMUX_NW_SelectFailed "select 调用失败。\n"
#define MSGTR_MPDEMUX_NW_ConnTimeout "连接超时\n"
#define MSGTR_MPDEMUX_NW_GetSockOptFailed "getsockopt 失败：%s\n"
#define MSGTR_MPDEMUX_NW_ConnectError "连接错误：%s\n"
#define MSGTR_MPDEMUX_NW_InvalidProxySettingTryingWithout "代理设置无效... 尝试不使用代理。\n"
#define MSGTR_MPDEMUX_NW_CantResolvTryingWithoutProxy "无法解析 AF_INET 的远程主机名。尝试不使用代理。\n"
#define MSGTR_MPDEMUX_NW_ErrSendingHTTPRequest "发送 HTTP 请求时出错：未完全发出所有请求。\n"
#define MSGTR_MPDEMUX_NW_ReadFailed "读取失败。\n"
#define MSGTR_MPDEMUX_NW_Read0CouldBeEOF "http_read_response 读到 0（例如：文件末端）。\n"
#define MSGTR_MPDEMUX_NW_AuthFailed "验证失败。请使用 -user 和 -passwd 选项来指定你的\n"\
"用户名/密码，以提供给一组 URL 使用，或者使用如下的 URL 格式：\n"\
"http://username:password@hostname/file\n"
#define MSGTR_MPDEMUX_NW_AuthRequiredFor "%s 需要验证\n"
#define MSGTR_MPDEMUX_NW_AuthRequired "需要验证。\n"
#define MSGTR_MPDEMUX_NW_NoPasswdProvidedTryingBlank "未提供密码，尝试使用空密码。\n"
#define MSGTR_MPDEMUX_NW_ErrServerReturned "服务器返回 %d：%s\n"
#define MSGTR_MPDEMUX_NW_CacheSizeSetTo "缓存大小设为 %d 千字节\n"

// open.c, stream.c:
#define MSGTR_CdDevNotfound "未找到 CD-ROM 设备‘%s’。\n"
#define MSGTR_ErrTrackSelect "选择 VCD 轨道出错。"
#define MSGTR_ReadSTDIN "正在从标准输入中读取...\n"
#define MSGTR_FileNotFound "未找到文件：‘%s’\n"

#define MSGTR_SMBInitError "无法初始化 libsmbclient 库：%d\n"
#define MSGTR_SMBFileNotFound "无法通过局域网打开：‘%s’\n"

#define MSGTR_CantOpenBluray "无法打开蓝光设备：%s\n"
#define MSGTR_CantOpenDVD "无法打开 DVD 设备：%s（%s）\n"

#define MSGTR_URLParsingFailed "URL %s 解析失败\n"
#define MSGTR_FailedSetStreamOption "未能设置媒体流选项 %s=%s\n"
#define MSGTR_StreamNeedType "媒体流需有类型！\n"
#define MSGTR_StreamCantHandleURL "未找到媒体流以处理 URL %s\n"
#define MSGTR_StreamNULLFilename "open_output_stream()，文件名为 NULL，请报告该缺陷\n"
#define MSGTR_StreamErrorWritingCapture "写入流捕获文件错误：%s\n"
#define MSGTR_StreamSeekFailed "流定位失败\n"
#define MSGTR_StreamNotSeekable "该流无法定位！\n"
#define MSGTR_StreamCannotSeekBackward "无法在线性媒体流中向后定位！\n"

// stream_cdda.c
#define MSGTR_MPDEMUX_CDDA_CantOpenCDDADevice "无法打开 CDDA 设备。\n"
#define MSGTR_MPDEMUX_CDDA_CantOpenDisc "无法打开光盘。\n"
#define MSGTR_MPDEMUX_CDDA_AudioCDFoundWithNTracks "发现音频 CD，共 %d 音轨。\n"

// stream_cddb.c
#define MSGTR_MPDEMUX_CDDB_FailedToReadTOC "读取 TOC 失败。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToOpenDevice "打开 %s 设备失败。\n"
#define MSGTR_MPDEMUX_CDDB_NotAValidURL "不是有效的 URL\n"
#define MSGTR_MPDEMUX_CDDB_FailedToSendHTTPRequest "发送 HTTP 请求失败。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToReadHTTPResponse "读取 HTTP 响应失败。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorNOTFOUND "未找到。\n"
#define MSGTR_MPDEMUX_CDDB_HTTPErrorUnknown "未知错误代码\n"
#define MSGTR_MPDEMUX_CDDB_NoCacheFound "未找到缓存。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenRead "没有读出所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_FailedToCreateDirectory "创建目录 %s 失败。\n"
#define MSGTR_MPDEMUX_CDDB_NotAllXMCDFileHasBeenWritten "没有写入所有的 xmcd 文件。\n"
#define MSGTR_MPDEMUX_CDDB_InvalidXMCDDatabaseReturned "返回的 xmcd 数据库文件无效。\n"
#define MSGTR_MPDEMUX_CDDB_UnexpectedFIXME "意常情况。请修复\n"
#define MSGTR_MPDEMUX_CDDB_UnhandledCode "未处理的代码\n"
#define MSGTR_MPDEMUX_CDDB_UnableToFindEOL "无法找到行末尾。\n"
#define MSGTR_MPDEMUX_CDDB_ParseOKFoundAlbumTitle "解析成功，找到：%s\n"
#define MSGTR_MPDEMUX_CDDB_AlbumNotFound "未找到专辑。\n"
#define MSGTR_MPDEMUX_CDDB_ServerReturnsCommandSyntaxErr "服务器返回内容：命令语法错误\n"
#define MSGTR_MPDEMUX_CDDB_FailedToGetProtocolLevel "获得协议级别失败。\n"
#define MSGTR_MPDEMUX_CDDB_NoCDInDrive "驱动器里没有 CD。\n"

// stream_cue.c
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedCuefileLine "[bincue] cue 文件中有内容异常的一行：%s"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameFound "[bincue] 未找到文件描述内容：%s"
#define MSGTR_MPDEMUX_CUEREAD_BinFilenameTested "[bincue] 已测试 bin 文件名：%s\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotFindBinFile "[bincue] 无法找到 bin 文件 - 正在放弃尝试。\n"
#define MSGTR_MPDEMUX_CUEREAD_UsingBinFile "[bincue] 正在使用 bin 文件 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_UnknownModeForBinfile "[bincue] 未知 bin 文件模式。不应出现该现象。正在中断操作。\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotOpenCueFile "[bincue] 无法打开 %s。\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrReadingFromCueFile "[bincue] 读取 %s 出错\n"
#define MSGTR_MPDEMUX_CUEREAD_ErrGettingBinFileSize "[bincue] 获取 bin 文件大小时出错。\n"
#define MSGTR_MPDEMUX_CUEREAD_InfoTrackFormat "音轨 %02d： 格式=%d  %02d:%02d:%02d\n"
#define MSGTR_MPDEMUX_CUEREAD_UnexpectedBinFileEOF "[bincue] bin 文件遇到不应出现的结尾\n"
#define MSGTR_MPDEMUX_CUEREAD_CannotReadNBytesOfPayload "[bincue] 无法读取 %d 字节的负载数据。\n"
#define MSGTR_MPDEMUX_CUEREAD_CueStreamInfo_FilenameTrackTracksavail "CUE stream_open, 文件名=%s, 音轨=%d, 可用音轨：%d -> %d\n"

// stream_dvd.c
#define MSGTR_DVDspeedCantOpen "无法以写入方式打开 DVD 设备，改变 DVD 速度需有写入的权限。\n"
#define MSGTR_DVDrestoreSpeed "恢复 DVD 速度... "
#define MSGTR_DVDlimitSpeed "限制 DVD 速度为 %dKB/s... "
#define MSGTR_DVDlimitFail "失败。\n"
#define MSGTR_DVDlimitOk "成功。\n"
#define MSGTR_NoDVDSupport "MPlayer 编译时未包括对 DVD 的支持，正在退出。\n"
#define MSGTR_DVDnumTitles "该 DVD 有 %d 个标题。\n"
#define MSGTR_DVDinvalidTitle "DVD 标题编号无效：%d\n"
#define MSGTR_DVDinvalidChapterRange "章节范围的说明数据无效 %s\n"
#define MSGTR_DVDnumAngles "该 DVD 标题有 %d 个视角。\n"
#define MSGTR_DVDinvalidAngle "DVD 视角编号无效：%d\n"
#define MSGTR_DVDnoIFO "无法打开 DVD 标题 %d 的 IFO 文件。\n"
#define MSGTR_DVDnoVMG "无法打开 VMG 信息！\n"
#define MSGTR_DVDnoVOBs "无法打开标题的 VOBS 文件（VTS_%02d_1.VOB）。\n"
#define MSGTR_DVDnoMatchingAudio "未找到匹配语言的 DVD 音频！\n"
#define MSGTR_DVDaudioChannel "已选择 DVD 音频的频道：%d 语言：%c%c\n"
#define MSGTR_DVDaudioStreamInfo "音频流：%d 格式：%s (%s) 语言：%s 音频号：%d。\n"
#define MSGTR_DVDnumAudioChannels "光盘的音频频道数：%d。\n"
#define MSGTR_DVDnoMatchingSubtitle "未找到匹配语言的 DVD 字幕！\n"
#define MSGTR_DVDsubtitleChannel "已选 DVD 字幕频道：%d 语言：%c%c\n"
#define MSGTR_DVDsubtitleLanguage "字幕号：%d 语言：%s\n"
#define MSGTR_DVDnumSubtitles "光盘上的字幕数：%d\n"

// stream_bluray.c
#define MSGTR_BlurayNoDevice "未指定蓝光 DVD 的设备/位置...\n"
#define MSGTR_BlurayNoTitles "无法在此找到与蓝光 DVD 相兼容的标题内容。\n"

// stream_radio.c
#define MSGTR_RADIO_ChannelNamesDetected "[radio] 已检测到无线电频道名。\n"
#define MSGTR_RADIO_WrongFreqForChannel "[radio] 频道 %s 的频率错误\n"
#define MSGTR_RADIO_WrongChannelNumberFloat "[radio] 错误的通道号: %.2f\n"
#define MSGTR_RADIO_WrongChannelNumberInt "[radio] 频道号错误：%d\n"
#define MSGTR_RADIO_WrongChannelName "[radio] 频道名错误：%s\n"
#define MSGTR_RADIO_FreqParameterDetected "[radio] 已检测到无线电频率参数。\n"
#define MSGTR_RADIO_GetTunerFailed "[radio] 警告：ioctl 获取无线电调谐器失败：%s。设置 frac 为 %d。\n"
#define MSGTR_RADIO_NotRadioDevice "[radio] %s 非无线电设备！\n"
#define MSGTR_RADIO_SetFreqFailed "[radio] ioctl 设置频率 0x%x (%.2f) 失败：%s\n"
#define MSGTR_RADIO_GetFreqFailed "[radio] ioctl 获取频率失败：%s\n"
#define MSGTR_RADIO_SetMuteFailed "[radio] ioctl 设置静音失败：%s\n"
#define MSGTR_RADIO_QueryControlFailed "[radio] ioctl 查询控制状态失败：%s\n"
#define MSGTR_RADIO_GetVolumeFailed "[radio] ioctl 获取音量失败：%s\n"
#define MSGTR_RADIO_SetVolumeFailed "[radio] ioctl 设置音量失败：%s\n"
#define MSGTR_RADIO_AllocateBufferFailed "[radio] 无法分配音频缓冲 (block=%d,buf=%d)：%s\n"
#define MSGTR_RADIO_CurrentFreq "[radio] 当前频率：%.2f\n"
#define MSGTR_RADIO_SelectedChannel "[radio] 已选择频道：%d - %s（频率：%.2f）\n"
#define MSGTR_RADIO_ChangeChannelNoChannelList "[radio] 无法切换频道：未给出的频道列表。\n"
#define MSGTR_RADIO_UnableOpenDevice "[radio] 无法打开‘%s’：%s\n"
#define MSGTR_RADIO_WrongFreq "[radio] 频率错误：%.2f\n"
#define MSGTR_RADIO_UsingFreq "[radio] 使用频率：%.2f。\n"
#define MSGTR_RADIO_AudioInInitFailed "[radio] audio_in_init 失败。\n"
#define MSGTR_RADIO_AudioInSetupFailed "[radio] audio_in_setup 调用失败：%s\n"
#define MSGTR_RADIO_ClearBufferFailed "[radio] 清空缓冲失败：%s\n"
#define MSGTR_RADIO_StreamEnableCacheFailed "[radio] 调用 stream_enable_cache 失败：%s\n"
#define MSGTR_RADIO_DriverUnknownStr "[radio] 未知驱动名称：%s\n"
#define MSGTR_RADIO_DriverV4L2 "[radio] 使用 V4Lv2 无线电接口。\n"
#define MSGTR_RADIO_DriverV4L "[radio] 使用 V4Lv1 无线电接口。\n"
#define MSGTR_RADIO_DriverBSDBT848 "[radio] 使用 *BSD BT848 无线电接口。\n"

//tv.c
#define MSGTR_TV_BogusNormParameter "tv.c：norm_from_string(%s)：规格化参数、设置非法 %s。\n"
#define MSGTR_TV_NoVideoInputPresent "错误：视频输入不存在！\n"
#define MSGTR_TV_UnknownImageFormat ""\
"==================================================================\n"\
" 警告：请求输出的图像格式未经测试或未知（0x%x）\n"\
" 这可能导致播放故障或程序崩溃！缺陷报告将被忽略！你应该再次尝试使用\n"\
" YV12（这是默认的色彩空间）并阅读文档！\n"\
"==================================================================\n"

#define MSGTR_TV_CannotSetNorm "错误：无法设置规格化参数！\n"
#define MSGTR_TV_MJP_WidthHeight "  MJP：宽 %d 高 %d\n"
#define MSGTR_TV_UnableToSetWidth "无法设置为所请求的宽度：%d\n"
#define MSGTR_TV_UnableToSetHeight "无法设置为所请求的高度：%d\n"
#define MSGTR_TV_NoTuner "所选输入没有频道调谐器！\n"
#define MSGTR_TV_UnableFindChanlist "无法找到所选频道列表！（%s）\n"
#define MSGTR_TV_ChannelFreqParamConflict "无法同时设置频率和频道！\n"
#define MSGTR_TV_ChannelNamesDetected "已检测到电视频道名称。\n"
#define MSGTR_TV_NoFreqForChannel "无法找到 %s（%s）频道的频率\n"
#define MSGTR_TV_SelectedChannel3 "已选择频道：%s - %s（频率：%.3f）\n"
#define MSGTR_TV_SelectedChannel2 "已选择频道：%s（频率：%.3f）\n"
#define MSGTR_TV_UnsupportedAudioType "音频类型‘%s（%x）’不支持！\n"
#define MSGTR_TV_AvailableDrivers "可用驱动：\n"
#define MSGTR_TV_DriverInfo "已选择驱动：%s\n 名称：%s\n 作者：%s\n 注释：%s\n"
#define MSGTR_TV_NoSuchDriver "没有这种驱动：%s\n"
#define MSGTR_TV_DriverAutoDetectionFailed "自动检测电视驱动失败。\n"
#define MSGTR_TV_UnknownColorOption "使用了未知色彩选项（%d）！\n"
#define MSGTR_TV_NoTeletext "无图文电视功能"
#define MSGTR_TV_Bt848IoctlFailed "tvi_bsdbt848：调用 %s ioctl 失败。错误信息：%s\n"
#define MSGTR_TV_Bt848InvalidAudioRate "tvi_bsdbt848：无效的音频码率值。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningBktrDev "tvi_bsdbt848：无法打开 bktr 设备。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningTunerDev "tvi_bsdbt848：无法打开调谐设备。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorOpeningDspDev "tvi_bsdbt848：无法打开 dsp 设备。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorConfiguringDsp "tvi_bsdbt848：配置 dsp 失败。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorReadingAudio "tvi_bsdbt848：读取音频数据出错。错误信息：%s\n"
#define MSGTR_TV_Bt848MmapFailed "tvi_bsdbt848：调用 mmap 失败。错误信息：%s\n"
#define MSGTR_TV_Bt848FrameBufAllocFailed "tvi_bsdbt848：分配帧缓冲失败。错误信息：%s\n"
#define MSGTR_TV_Bt848ErrorSettingWidth "tvi_bsdbt848：设置画面宽度出错。错误信息：%s\n"
#define MSGTR_TV_Bt848UnableToStopCapture "tvi_bsdbt848：无法停止捕捉视频。错误信息：%s\n"
#define MSGTR_TV_TTSupportedLanguages "支持的图文电视语言：\n"
#define MSGTR_TV_TTSelectedLanguage "已选择默认图文电视语言：%s\n"
#define MSGTR_TV_ScannerNotAvailableWithoutTuner "没有频道调谐器无法使用频道搜索器\n"

//tvi_dshow.c
#define MSGTR_TVI_DS_UnableConnectInputVideoDecoder  "无法将所给输入连接至视频解码器。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableConnectInputAudioDecoder  "无法将所给输入连接至音频解码器。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableSelectVideoFormat "tvi_dshow：无法选用视频格式。错误代码：Error:0x%x\n"
#define MSGTR_TVI_DS_UnableSelectAudioFormat "tvi_dshow：无法选用音频格式。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableGetMediaControlInterface "tvi_dshow：无法获取 IMediaControl 接口。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableStartGraph "tvi_dshow：无法启动流程图！错误代码：0x%x\n"
#define MSGTR_TVI_DS_DeviceNotFound "tvi_dshow：未找到设备 #%d\n"
#define MSGTR_TVI_DS_UnableGetDeviceName "tvi_dshow：无法获取设备 #%d 的名称\n"
#define MSGTR_TVI_DS_UsingDevice "tvi_dshow：使用设备 #%d：%s\n"
#define MSGTR_TVI_DS_DirectGetFreqFailed "tvi_dshow：无法直接获取频率值。将使用操作系统内置的频道表。\n"
#define MSGTR_TVI_DS_UnableExtractFreqTable "tvi_dshow：无法从 kstvtune.ax 加载频率对照表\n"
#define MSGTR_TVI_DS_WrongDeviceParam "tvi_dshow：设备参数错误：%s\n"
#define MSGTR_TVI_DS_WrongDeviceIndex "tvi_dshow：设备索引错误：%d\n"
#define MSGTR_TVI_DS_WrongADeviceParam "tvi_dshow：音频设备参数错误：%s\n"
#define MSGTR_TVI_DS_WrongADeviceIndex "tvi_dshow：音频设备索引错误：%d\n"

#define MSGTR_TVI_DS_SamplerateNotsupported "tvi_dshow：设备不支持 %d 采样率。退回使用第一个可用的值。\n"
#define MSGTR_TVI_DS_VideoAdjustigNotSupported "tvi_dshow：设备不支持调整亮度/色度/饱和度/对比度\n"

#define MSGTR_TVI_DS_ChangingWidthHeightNotSupported "tvi_dshow：设备不支持改变视频的宽度/高度。\n"
#define MSGTR_TVI_DS_SelectingInputNotSupported  "tvi_dshow：设备不支持选择视频捕捉的来源\n"
#define MSGTR_TVI_DS_ErrorParsingAudioFormatStruct "tvi_dshow：无法解析音频格式的结构。\n"
#define MSGTR_TVI_DS_ErrorParsingVideoFormatStruct "tvi_dshow：无法解析视频格式的结构。\n"
#define MSGTR_TVI_DS_UnableSetAudioMode "tvi_dshow：无法设置音频模式 %d。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnsupportedMediaType "tvi_dshow：传递至 %s 的媒体格式不支持\n"
#define MSGTR_TVI_DS_UnableFindNearestChannel "tvi_dshow：无法在系统频率对照表中找到最接近的频道\n"
#define MSGTR_TVI_DS_UnableToSetChannel "tvi_dshow：无法切换至系统频率对照表所标出的最接近的频道。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableTerminateVPPin "tvi_dshow：无法使用流程图中的任何过滤器终止 VideoPort 端。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVideoSubGraph "tvi_dshow：无法建立捕捉流程图中的视频处理链。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildAudioSubGraph "tvi_dshow：无法建立捕捉流程图中的音频处理链。错误代码：0x%x\n"
#define MSGTR_TVI_DS_UnableBuildVBISubGraph "tvi_dshow：无法建立捕捉流程图中的 VBI 链。错误代码：0x%x\n"
#define MSGTR_TVI_DS_GraphInitFailure "tvi_dshow：Directshow 流程图初始化失败。\n"
#define MSGTR_TVI_DS_NoVideoCaptureDevice "tvi_dshow：无法找到视频捕捉设备\n"
#define MSGTR_TVI_DS_NoAudioCaptureDevice "tvi_dshow：无法找到音频捕捉设备\n"
#define MSGTR_TVI_DS_GetActualMediatypeFailed "tvi_dshow：无法识别媒体的实际类型（错误代码：0x%x）。使用与所请求相同的类型。\n"

// url.c
#define MSGTR_MPDEMUX_URL_StringAlreadyEscaped "字符串似乎已经经过 url escape %c%c%c 编码了。\n"

// subtitles
#define MSGTR_SUBTITLES_SubRip_UnknownFontColor "SubRip：字幕含有未知字体颜色：%s\n"

