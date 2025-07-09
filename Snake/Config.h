#pragma once

class Config
{
	////Utilizziamo constexpr perché vogliamo che questi valori siano costanti a tempo di compilazione 
	//1- Zero overhead a runtime
	//	Il compilatore conosce già il loro valore mentre compila e può “inserirli” direttamente dove servono, senza nemmeno allocare spazio in memoria.

	//2- Possono essere usati in contesti che richiedono costanti compile - time
	//	Ad esempio dimensioni di array statici, parametri template, o per static_assert—cose che con un semplice const non sempre sono garantite.

	//3- Chiarezza d’intento
	//	Scrivere constexpr dice “questa non è una variabile qualunque, è un valore fisso e immutabile fin dall’inizio”.

	//4- Ottimizzazioni migliori
	//	Sapendo che il valore non cambia mai, il compilatore può, se serve, sostituirlo direttamente nelle espressioni o eliminare branch morti.

public:
	// Dimensione della finestra (px)
	constexpr static int WINDOW_SIZE = 1000;

	// 1) Numero di celle per riga/colonna:
	//    Ho scelto GRID_SIZE = 20 perché divide esattamente 600 (600/20 = 30px per cella),
	//    mantenendo una griglia sufficientemente definita senza troppi segmenti.
	//constexpr static int GRID_SIZE = 20;
	constexpr static int GRID_SIZE = 20;

	// 2) Dimensione di ogni cella (px):
	//    CELL_SIZE = WINDOW_SIZE / GRID_SIZE = 30px.
	//    Celle di 30×30 sono abbastanza grandi da vedere bene il serpente,
	//    e allo stesso tempo non appesantiscono il rendering.
	constexpr static int CELL_SIZE = WINDOW_SIZE / GRID_SIZE;
};

