/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/// \defgroup ConfigParsers Config parsers
///
/// The \ref ConfigParsers make use of the \ref Config to setup the config variables,
/// the command line parsers also build the playlist.
///@{

/// \file

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifdef MP_DEBUG
#include <assert.h>
#endif

#include "parser-cfg.h"
#include "mp_msg.h"
#include "m_option.h"
#include "m_config.h"

/// Maximal include depth.
#define MAX_RECURSION_DEPTH	8

/// Current include depth.
static int recursion_depth = 0;

/// Setup the \ref Config from a config file.
/** \param config The config object.
 *  \param conffile Path to the config file.
 *  \param silent print message when failing to open file only at verbose level
 *  \return 1 on sucess, -1 on error.
 */
int m_config_parse_config_file(m_config_t* config, const char *conffile, int silent)
{
#define PRINT_LINENUM	mp_msg(MSGT_CFGPARSER,MSGL_V,"%s(%d): ", conffile, line_num)
#define MAX_LINE_LEN	10000
#define MAX_OPT_LEN	1000
#define MAX_PARAM_LEN	1500
	FILE *fp = NULL;
	unsigned char *line = NULL;
	unsigned char opt[MAX_OPT_LEN + 1];
	unsigned char param[MAX_PARAM_LEN + 1];
	unsigned char c;		/* for the "" and '' check */
	int tmp;
	int line_num = 0;
	int line_pos;	/* line pos */
	int opt_pos;	/* opt pos */
	int param_pos;	/* param pos */
	int ret = 1;
	int errors = 0;
	int prev_mode = config->mode;
	m_profile_t* profile = NULL;

#ifdef MP_DEBUG
	assert(config != NULL);
	//	assert(conf_list != NULL);
#endif
	mp_msg(MSGT_CFGPARSER,MSGL_V,"Reading config file %s", conffile);

	if (++recursion_depth > MAX_RECURSION_DEPTH) {
		mp_msg(MSGT_CFGPARSER,MSGL_ERR,": too deep 'include'. check your configfiles\n");
		ret = -1;
		goto out;
	} else

	config->mode = M_CONFIG_FILE;

	if ((line = malloc(MAX_LINE_LEN + 1)) == NULL) {
		mp_msg(MSGT_CFGPARSER,MSGL_FATAL,"\ncan't get memory for 'line': %s", strerror(errno));
		ret = -1;
		goto out;
	} else

	mp_msg(MSGT_CFGPARSER,MSGL_V,"\n");

	if ((fp = fopen(conffile, "r")) == NULL) {
	  mp_msg(MSGT_CFGPARSER,silent?MSGL_V:MSGL_ERR,"Failed to read %s: %s\n", conffile, strerror(errno));
		ret = silent ? 0 : -1;
		goto out;
	}

	while (fgets(line, MAX_LINE_LEN, fp)) {
		if (errors >= 16) {
			mp_msg(MSGT_CFGPARSER,MSGL_FATAL,"too many errors\n");
			goto out;
		}

		line_num++;
		line_pos = 0;

		/* skip whitespaces */
		while (isspace(line[line_pos]))
			++line_pos;

		/* EOL / comment */
		if (line[line_pos] == '\0' || line[line_pos] == '#')
			continue;

		/* read option. */
		for (opt_pos = 0; isprint(line[line_pos]) &&
				line[line_pos] != ' ' &&
				line[line_pos] != '#' &&
				line[line_pos] != '='; /* NOTHING */) {
			opt[opt_pos++] = line[line_pos++];
			if (opt_pos >= MAX_OPT_LEN) {
				PRINT_LINENUM;
				mp_msg(MSGT_CFGPARSER,MSGL_ERR,"too long option at line %d\n",line_num);
				errors++;
				ret = -1;
				goto nextline;
			}
		}
		if (opt_pos == 0) {
			PRINT_LINENUM;
			mp_msg(MSGT_CFGPARSER,MSGL_ERR,"parse error at line %d\n",line_num);
			ret = -1;
			errors++;
			continue;
		}
		opt[opt_pos] = '\0';

		/* Profile declaration */
		if(opt_pos > 2 && opt[0] == '[' && opt[opt_pos-1] == ']') {
			opt[opt_pos-1] = '\0';
			if(strcmp(opt+1,"default"))
				profile = m_config_add_profile(config,opt+1);
			else
				profile = NULL;
			continue;
		}

#ifdef MP_DEBUG
		PRINT_LINENUM;
		mp_msg(MSGT_CFGPARSER,MSGL_V,"option: %s\n", opt);
#endif

		/* skip whitespaces */
		while (isspace(line[line_pos]))
			++line_pos;

		/* check '=' */
		if (line[line_pos++] != '=') {
			PRINT_LINENUM;
			mp_msg(MSGT_CFGPARSER,MSGL_ERR,"Option %s needs a parameter at line %d\n",opt,line_num);
			ret = -1;
			errors++;
			continue;
		}

		/* whitespaces... */
		while (isspace(line[line_pos]))
			++line_pos;

		/* read the parameter */
		if (line[line_pos] == '"' || line[line_pos] == '\'') {
			c = line[line_pos];
			++line_pos;
			for (param_pos = 0; line[line_pos] != c; /* NOTHING */) {
				param[param_pos++] = line[line_pos++];
				if (param_pos >= MAX_PARAM_LEN) {
					PRINT_LINENUM;
					mp_msg(MSGT_CFGPARSER,MSGL_ERR,"Option %s has a too long parameter at line %d\n",opt,line_num);
					ret = -1;
					errors++;
					goto nextline;
				}
			}
			line_pos++;	/* skip the closing " or ' */
		} else {
			for (param_pos = 0; isprint(line[line_pos]) && !isspace(line[line_pos])
					&& line[line_pos] != '#'; /* NOTHING */) {
				param[param_pos++] = line[line_pos++];
				if (param_pos >= MAX_PARAM_LEN) {
					PRINT_LINENUM;
					mp_msg(MSGT_CFGPARSER,MSGL_ERR,"too long parameter\n");
					ret = -1;
					errors++;
					goto nextline;
				}
			}
		}
		param[param_pos] = '\0';

		/* did we read a parameter? */
		if (param_pos == 0) {
			PRINT_LINENUM;
			mp_msg(MSGT_CFGPARSER,MSGL_ERR,"Option %s needs a parameter at line %d\n",opt,line_num);
			ret = -1;
			errors++;
			continue;
		}

#ifdef MP_DEBUG
		PRINT_LINENUM;
		mp_msg(MSGT_CFGPARSER,MSGL_V,"parameter: %s\n", param);
#endif

		/* now, check if we have some more chars on the line */
		/* whitespace... */
		while (isspace(line[line_pos]))
			++line_pos;

		/* EOL / comment */
		if (line[line_pos] != '\0' && line[line_pos] != '#') {
			PRINT_LINENUM;
			mp_msg(MSGT_CFGPARSER,MSGL_WARN,"extra characters on line %d: %s\n",line_num, line+line_pos);
			ret = -1;
		}

		if(profile) {
			if(!strcmp(opt,"profile-desc"))
				m_profile_set_desc(profile,param), tmp = 1;
			else
				tmp = m_config_set_profile_option(config,profile,
								  opt,param);
		} else
			tmp = m_config_set_option(config, opt, param);
		if (tmp < 0) {
			PRINT_LINENUM;
			if(tmp == M_OPT_UNKNOWN) {
				mp_msg(MSGT_CFGPARSER,MSGL_WARN,"Warning unknown option %s at line %d\n", opt,line_num);
				continue;
			}
			mp_msg(MSGT_CFGPARSER,MSGL_ERR,"Error parsing option %s=%s at line %d\n",opt,param,line_num);
			ret = -1;
			errors++;
			continue;
			/* break */
		}
nextline:
		;
	}

out:
	free(line);
	if (fp) fclose(fp);
	config->mode = prev_mode;
	--recursion_depth;
	return ret;
}

/// Parse the command line option that must be handled at startup.
int m_config_preparse_command_line(m_config_t *config, int argc, char **argv)
{
	int msg_lvl, i, r, ret = 0;
	char* arg;

	// Hack to shutup the parser error messages.
	msg_lvl = mp_msg_levels[MSGT_CFGPARSER];
	mp_msg_levels[MSGT_CFGPARSER] = -11;

	config->mode = M_COMMAND_LINE_PRE_PARSE;

	for(i = 1 ; i < argc ; i++) {
		const m_option_t* opt;
		arg = argv[i];
		// Ignore non option
		if(arg[0] != '-' || arg[1] == 0) continue;
		arg++;
		// No more options after --
		if(arg[0] == '-' && arg[1] == 0) break;

		opt = m_config_get_option(config,arg);
		// Ignore invalid option
		if(!opt) continue;
		// Set, non-pre-parse options will be ignored
		r = m_config_set_option(config,arg,
					i+1 < argc ? argv[i+1] : NULL);
		if(r < 0) ret = r;
		else i += r;
	}

	mp_msg_levels[MSGT_CFGPARSER] = msg_lvl;

	return ret;
}

///@}
