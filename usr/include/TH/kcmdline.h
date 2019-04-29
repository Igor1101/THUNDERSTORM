/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef KCMDLINE_H
#define KCMDLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KCMDLINE_SIZE

#define KCMDLINE_SIZE 200
#endif                          /* KCMDLINE_SIZE */

extern char kcmdline[KCMDLINE_SIZE];

#ifdef __cplusplus
}
#endif

#endif                          /* KCMDLINE_H */
