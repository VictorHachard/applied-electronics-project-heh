// ===============================================
// File:   bluetooth_proto.h
// Desc:   Header file for bluetooth_proto.h
// ===============================================

#ifndef BLUETOOTH_PROTO_H
#define BLUETOOTH_PROTO_H

#include "../core/types.h"

// Initialise le protocole (reset index)
app_err_t bluetooth_init(void);

// Traite une commande reçue via Bluetooth
// Commandes: 'C'=Config, 'N'=Count, 'R'=Reset, 'A'=Avancer
void bluetooth_handle_rx(char cmd);

#endif // BLUETOOTH_PROTO_H
