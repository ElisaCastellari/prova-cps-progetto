#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
#include "pitches.h" // for the notes

// 1. CREIAMO LA STRUTTURA DELLA NOTA
typedef struct {
    uint16_t frequency; // La nota da suonare (es. 330 per il MI)
    uint16_t waitTime;  // Quanti millisecondi aspettare PRIMA di accendere il LED
} GameNote_t;

#define MELODY_LENGTH 15

#endif /* BUZZER_H */
