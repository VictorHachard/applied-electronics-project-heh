// ===============================================
// File:   bluetooth_proto.h
// Desc:   Header file for bluetooth_proto.h
// ===============================================

#ifndef BLUETOOTH_PROTO_H
#define BLUETOOTH_PROTO_H

#include <stdint.h>
#include "../core/types.h"

// Initialisation du module Bluetooth
app_err_t bluetooth_init(void);

// Envoi des données capteurs via Bluetooth
app_err_t bluetooth_send_sensor_data(const sensor_data_t* data);

#endif // BLUETOOTH_PROTO_H
