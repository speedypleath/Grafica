# Grafica
## Structura fisiere
  1. bin - programele compilate pentru fiecare laborator
  2. include - headere pentru fisierele care sunt folosite in mai multe laboratoare
  3. lib - fisierele folosite in mai multe laboratoare (ex pentru includerea shaderelor)
  4. res - resurse aditionale, precum shadere sau imagini
  5. src - laboratoarele, numerotate de la 1 la 11
  6. test - fisier pentru a testa instalarea corecta a OpenGL
## Laboratoare
### [1. Start in lucrul cu OpenGL](src/lab1)
  * Instalati bibliotecile utilizate in lucrul cu OpenGL (freeglut, glew).
  * Creati un proiect folosind Microsoft Visual Studio si integrati codurile sursa din sectiunea Resurse Laborator.
  * Modificati in codul sursa urmatoarele elemente:
     * dimensiunea si pozitionarea ferestrei de vizualizare
     * culorile primitivelor reprezentate
### [2. Primitive grafice](src/lab2)
  * Adaugati (cel putin) patru varfuri in codul sursa 02_01 si creati o noua primitiva.
  * Folositi o instructiune "switch" in shader pentru a colora diferit mai multe primitive. Instructiunea va fi adaugata in 02_02_Shader.frag.
  * Transferati unul dintre codurile sursa 01_03_puncte.cpp sau 01_04_poligoane.cpp in versiunea "noua" a OpenGL.
### [3. Fata si spatele poligoanelor](src/lab3)
  * Manevrati codul sursa 03_02_poligoane3d_exemplu2.cpp conform cerintelor.
  * Adaugati la desenul din codul sursa 03_03_poligoane3d_new.cpp un triunghi vazut din fata si unul vazut din spate, desenate in mod diferit.
### [4. Transformari](src/lab4)
  * Desenati un dreptunghi D si doua poligoane P1 - convex si P2 - concav situate in dreptunghiul [0,800]x[0,600]
  * Fundalul este un gradient
  * Desenati atat P1 si P2, cat si figura obtinuta aplicand o rotatie cu centrul "intre" P1 si P2
  * Asupra dreptunghiului D aplicati o scalare avand axe oarecare; desenati atat D cat si dreptunghiul rezultat aplicand transformarea
  * Realizati una dintre transformari inmultind matricele in shader, cealalta inmultind matrice in programul principal.
### [5. Compunerea transformarilor. Utilizarea mouse-ului](src/lab5)
### [6. Indexarea varfurilor, transformari, texturare](src/lab6)
  * Folositi glDrawElements ( ); pentru a desena ![Screenshot 2022-02-08 at 08 45 36](https://user-images.githubusercontent.com/61271015/152932953-39e864af-52f1-4ade-b35a-cf1663be8470.png)

  * In codul sursa 06_02 initial (doar cu un patrat) introduceti o scalare cu factori (2.0, 0.5)
si o translatie de vector (30,30). Desenati:
    * cu verde ce se obtine aplicand intai scalarea, apoi translatia 
    * texturat ce se obtine aplicand intai translatia, apoi scalarea
### [7. Transformari de vizualizare si proiectie](src/lab7)
### [8. Obiecte 3D (partea 1)](src/lab8)
  * Creati un obiect 3D (tetraedru/piramida/con, un alt poliedru), indicand explicit varfurile (utilizati 08_01). 
  * Utilizati randarea instantiata pentru a desena mai multe instante ale obiectului nou creat (utilizati 08_03).
  * Utilizati randarea instantiata, dispunand instantele pe o curba 3D. Survolati scena (utilizati 08_03 si 08_02).
### [9. Obiecte 3D (partea 2)](src/lab9)
  * Trasati o curba parametrizata dupa modelul cercului.
  * Desenati un cilindru sau un con, dupa modelului codului sursa 09_03_sfera.cpp
  * Folositi sfera/cilindrul/conul pentru a schita un obiect 3D (copac, om de zapada, etc.).
### [10. Iluminare (partea 1)](src/lab10)
  * Explorati folosirea iluminarii aducand doua modificari (valori, mod de calcul) in modelul implementat in codurile 10_02.
  * Aplicati modelul de iluminare in cazul unui cub in care varfurile au culori diferite.
  * Aplicati modelul de iluminare poliedrului construit in tema de la L8.
### [11. Iluminare (partea 2)](src/lab11)
  * Explorati folosirea iluminarii pentru conul / cilindrul realizat la L9.
  * Aplicati umbra unei sfere / obiectului de la (i).
  * Construiti o suprafata de rotatie.
## Proiecte
### [1. Creati o scena 2D in care sa folositi urmatoarele elemente:](https://github.com/speedypleath/alien-invaders)
  * transformari
  * compunerea transformarilor
  * gestionarea matricelor de modelare

### [2. Realizati o scena 3D complexa care include:](https://github.com/speedypleath/Labyrinth)
  * tehnici deja implementate (reprezentare obiecte 3D, iluminare, umbre, amestecare, texturare, etc: 1p/tehnica)
  * elemente mai dificile (2p/element)
  * originalitate+complexitate
  * documentatie+prezentare/discutie
