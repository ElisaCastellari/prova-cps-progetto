#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "pitches.h" // for the notes
#include "constants.h"

// LA STRUTTURA DELLA NOTA
typedef struct {
    uint16_t frequency; // La nota da suonare
    uint16_t waitTime;  // Quanti millisecondi aspettare PRIMA di accendere il LED
} GameNote_t;

//struttura canzone
typedef struct {
    const GameNote_t* notes;
    uint16_t length;
    const char* name;
} Song_t;


#endif /* BUZZER_H */
