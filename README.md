# Image blur

### Echipa:
* [Georgescu Alin-Andrei](@alin.georgescu), 342 C3
* [Iuga Florin-Eugen](@florin_eugen.iuga),&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 342 C5
* [Negru Bogdan-Cristian](@bogdan.negru), 342 C3

### Asistent:
* Florin-Răzvan Mihalache (Joi 8 - 10)

---

### Implementări:
* implementare serială
* *Pthreads*
* *OpenMP*
* *OpenMPI*
* implementare hibridă (*OpenMP* & *OpenMPI*)

### Cum se rulează:
Se intră în folderul aferent variantei dorite și se execută:
```
make run IN=fișier_intrare OUT=fișier_ieșire FILTERS=nr_filtre_de_aplicat [NUM_TASKS=nr_taskuri_mpi] [NUM_THREADS=nr_threaduri]
```
---

### Descriere

Procesarea imaginilor reprezintă un proces destul de intensiv computațional,
deoarece implică operații matematice și înmulțiri de matrice mari. Astfel, am
ales blurarea Gaussiană drept proiect pentru APP 2020.

Utilitatea filtrului vine în editarea fotografiilor sau a videoclipurilor și se
observă, mai ales în perioada pandemiei, când mulți dintre noi utilizăm efectul
de blurare al fundalului în videoconferințe.

În plus, filtrul ales, are o utilitate auxiliară, în detectarea marginilor din
imagini, deoarece reduce zgomotul, înaintea aplicării algoritmului de detecție.

Aplicăm acest filtru pe imagini PNM / PGM, deoarece formatul imaginiilor este
unul simplu, care ne permite să nu irosim timp pe operații neparalelizabile.

Ne propunem să obținem niște implementări cât mai eficiente și să analizăm
performanțele acestora, în diferitele tool-uri de paralelizare.
