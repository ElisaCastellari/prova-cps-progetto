#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// --- FREQUENZE DELLE NOTE ---
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define PAUSA    0

// 1. CREIAMO LA STRUTTURA DELLA NOTA
typedef struct {
    uint16_t frequency; // La nota da suonare (es. 330 per il MI)
    uint16_t waitTime;  // Quanti millisecondi aspettare PRIMA di accendere il LED
} GameNote_t;

#define MELODY_LENGTH 15

// 2. CREIAMO LA CANZONE (Array di struct!)
const GameNote_t melody[MELODY_LENGTH] = {
    {NOTE_E4, 1000}, // Prima nota: aspetta 1 secondo, poi fai MI
    {NOTE_E4, 500},  // Seconda nota: aspetta mezzo secondo, poi fai MI
    {NOTE_F4, 500},  // aspetta mezzo secondo, fai FA
    {NOTE_G4, 500},
    {NOTE_G4, 500},
    {NOTE_F4, 500},
    {NOTE_E4, 500},
    {NOTE_D4, 500},
    {NOTE_C4, 500},
    {NOTE_C4, 500},
    {NOTE_D4, 500},
    {NOTE_E4, 500},
    {NOTE_E4, 750},  // Nota un po' più lunga
    {NOTE_D4, 250},  // Nota breve
    {NOTE_D4, 1000}  // Nota finale lunga
};

#endif /* BUZZER_H */
