// ===============================================
// File:   app_main.h
// Desc:   Boucle principale et scheduler de l'application
//         Intégration de tous les modules
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef APP_MAIN_H
#define APP_MAIN_H

// Initialisation complète de l'application
// À appeler au démarrage (depuis main)
void app_main_init(void);

// Boucle principale infinie
// Implémente le scheduler et les tâches périodiques
void app_main_loop(void);

#endif // APP_MAIN_H
