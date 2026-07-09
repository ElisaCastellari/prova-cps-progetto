#include "buzzer.h"

// 2. CREIAMO LA CANZONE (Array di struct!)
const GameNote_t innoAllaGioia[] = { //inno alla gioia
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

const GameNote_t superMario[] = { //super mario
    {NOTE_E5, 150}, // Mi
    {NOTE_E5, 300}, // Mi
    {NOTE_E5, 300}, // Mi
    {NOTE_C5, 150}, // Do
    {NOTE_E5, 300}, // Mi
    {NOTE_G5, 600}, // Sol (alto)
    {NOTE_G4, 600}, // Sol (basso)
    {NOTE_C5, 450}, // Do
    {NOTE_G4, 300}, // Sol
    {NOTE_E4, 450}, // Mi
    {NOTE_A4, 300}, // La
    {NOTE_B4, 300}, // Si
    {NOTE_AS4, 150},// La#
    {NOTE_A4, 300}, // La
    {NOTE_G4, 200}  // Sol
};

const GameNote_t jingleBells[] = { //jingle bells
    {NOTE_E4, 250},  // Jin-
    {NOTE_E4, 250},  // gle
    {NOTE_E4, 500},  // bells
    {NOTE_E4, 250},  // Jin-
    {NOTE_E4, 250},  // gle
    {NOTE_E4, 500},  // bells
    {NOTE_E4, 250},  // Jin-
    {NOTE_G4, 250},  // gle
    {NOTE_C4, 375},  // all
    {NOTE_D4, 125},  // the
    {NOTE_E4, 1000}, // way
    {NOTE_F4, 250},  // Oh
    {NOTE_F4, 250},  // what
    {NOTE_F4, 375},  // fun
    {NOTE_F4, 125}   // it...
};

const GameNote_t happyBirthday[] = { //happy birthday
    {NOTE_G4, 250},  // Tan-
    {NOTE_G4, 250},  // ti
    {NOTE_A4, 500},  // au-
    {NOTE_G4, 500},  // gu-
    {NOTE_C5, 500},  // ri a
    {NOTE_B4, 1000}, // te!
    {NOTE_G4, 250},  // Tan-
    {NOTE_G4, 250},  // ti
    {NOTE_A4, 500},  // au-
    {NOTE_G4, 500},  // gu-
    {NOTE_D5, 500},  // ri a
    {NOTE_C5, 1000}, // te!
    {NOTE_G4, 250},  // Tan-
    {NOTE_G4, 250},  // ti
    {NOTE_G5, 500}   // au...
};

const GameNote_t starWars[] = {// star wars
    {NOTE_D4, 200}, // Ta
    {NOTE_D4, 200}, // Ta
    {NOTE_D4, 200}, // Ta
    {NOTE_G4, 600}, // Taaaa
    {NOTE_D5, 600}, // Taaaa
    {NOTE_C5, 200}, // ta
    {NOTE_B4, 200}, // ta
    {NOTE_A4, 200}, // ta
    {NOTE_G5, 600}, // Taaaa
    {NOTE_D5, 600}, // Taaaa
    {NOTE_C5, 200}, // ta
    {NOTE_B4, 200}, // ta
    {NOTE_A4, 200}, // ta
    {NOTE_G5, 600}, // Taaaa
    {NOTE_D5, 600}  // Taaaa
};

const GameNote_t superMarioExtended[] = {
    // Intro
    {NOTE_E5, 150}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_C5, 150}, {NOTE_E5, 300},
    {NOTE_G5, 600}, {NOTE_G4, 600},

    // Parte 1 della melodia principale
    {NOTE_C5, 450}, {NOTE_G4, 300}, {NOTE_E4, 450}, {NOTE_A4, 300}, {NOTE_B4, 300},
    {NOTE_AS4, 150}, {NOTE_A4, 300}, {NOTE_G4, 200}, {NOTE_E5, 200}, {NOTE_G5, 200},
    {NOTE_A5, 300}, {NOTE_F5, 150}, {NOTE_G5, 300}, {NOTE_E5, 300}, {NOTE_C5, 150},
    {NOTE_D5, 150}, {NOTE_B4, 450},

    // Ripetizione
    {NOTE_C5, 450}, {NOTE_G4, 300}, {NOTE_E4, 450}, {NOTE_A4, 300}, {NOTE_B4, 300},
    {NOTE_AS4, 150}, {NOTE_A4, 300}, {NOTE_G4, 200}, {NOTE_E5, 200}, {NOTE_G5, 200},
    {NOTE_A5, 300}, {NOTE_F5, 150}, {NOTE_G5, 300}, {NOTE_E5, 300}, {NOTE_C5, 150},
    {NOTE_D5, 150}, {NOTE_B4, 450}
};

const Song_t starWars_song = {
		starWars,
		sizeof(starWars) / sizeof(starWars[0]), //per scoprire quanto è lungo
		"Star Wars"
};

const Song_t innoAllaGioia_song = {
		innoAllaGioia,
		sizeof(innoAllaGioia) / sizeof(innoAllaGioia[0]),
		"Inno alla Gioia"
};

const Song_t happyBirthday_song = {
		happyBirthday,
		sizeof(happyBirthday) / sizeof(happyBirthday[0]),
		"Tanti Auguri"
};

const Song_t jingleBells_song = {
		jingleBells,
		sizeof(jingleBells) / sizeof(jingleBells[0]),
		"Jingle Bells"
};

const Song_t superMario_song = {
		superMario,
		sizeof(superMario) / sizeof(superMario[0]),
		"Super Mario"
};

const Song_t superMarioExtended_song = {
		superMarioExtended,
		sizeof(superMarioExtended)/sizeof(superMarioExtended[0]),
		"superMarioExtended"
};


