// ===============================================
// File:   app_menu.h
// Desc:   Header file for app_menu.h
// ===============================================

#ifndef APP_MENU_H
#define APP_MENU_H

// board_init + drivers + modules + menu
void app_init(void);

// scheduler: tasks menu, bt_task, acquisitions périodiques
// utilisé les fonctions de configuration dans datalogger.h
void app_loop(void);

#endif // APP_MENU_H
