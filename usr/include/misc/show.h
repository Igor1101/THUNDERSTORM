/*
 * Copyright (C) 2018-2019  Igor Muravyov <igor.muravyov.2015@gmail.com>
 */
#ifndef SHOW_H
#define SHOW_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * show progress functions
 */
void show_progress(void);
void init_progress(void);
void stop_progress(void);

#ifdef __cplusplus
}
#endif

#endif                          /* SHOW_H */
