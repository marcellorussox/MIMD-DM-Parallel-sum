# PDC1

Algoritmo per il calcolo della somma di N numeri reali, in ambiente di calcolo parallelo su
architettura MIMD a memoria distribuita, che utilizzi la libreria MPI.

L’algoritmo deve
1) Prendere in input il numero N di numeri da sommare,
2) Prendere in input i numeri se N<=20,
3) Generare i numeri random se N>20,
4) Prendere in input il numero di strategia da utilizzare,
5) Implementare la I, la II e la III strategia di comunicazione
• Se l’utente ha chiesto la II o la III strategia MA non sono applicabili, applicare la I strategia
