#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "pitches.h" // for the notes
#include "constants.h"

// 1. CREIAMO LA STRUTTURA DELLA NOTA
typedef struct {
    uint16_t frequency; // La nota da suonare (es. 330 per il MI)
    uint16_t waitTime;  // Quanti millisecondi aspettare PRIMA di accendere il LED
} GameNote_t;

#endif /* BUZZER_H */
