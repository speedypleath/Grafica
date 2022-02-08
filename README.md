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
     * culorile primitivelor reprezentate!

### [2. Primitive grafice](src/lab2)
  * Adaugati (cel putin) patru varfuri in codul sursa 02_01 si creati o noua primitiva.

![MicrosoftTeams-image (19)](https://user-images.githubusercontent.com/61271015/152969217-9b4dd01c-251f-459a-b0d7-e7c374a48a77.png)
  * Folositi o instructiune "switch" in shader pentru a colora diferit mai multe primitive. Instructiunea va fi adaugata in 02_02_Shader.frag.

![MicrosoftTeams-image (3)](https://user-images.githubusercontent.com/61271015/152969262-7fe003c9-e10b-4ac4-b5f5-3d1ce7899482.png)
 * Transferati unul dintre codurile sursa 01_03_puncte.cpp sau 01_04_poligoane.cpp in versiunea "noua" a OpenGL.
### [3. Fata si spatele poligoanelor](src/lab3)
  * Manevrati codul sursa 03_02_poligoane3d_exemplu2.cpp conform cerintelor.
  * Adaugati la desenul din codul sursa 03_03_poligoane3d_new.cpp un triunghi vazut din fata si unul vazut din spate, desenate in mod diferit.

![MicrosoftTeams-image](https://user-images.githubusercontent.com/61271015/152968564-aac2c331-e470-4581-ad15-556600907f4a.png)

![MicrosoftTeams-image (1)](https://user-images.githubusercontent.com/61271015/152968507-1bc8af7d-b3b1-48fb-84c9-e288fb351e0c.png)
### [4. Transformari](src/lab4)
  * Desenati un dreptunghi D si doua poligoane P1 - convex si P2 - concav situate in dreptunghiul [0,800]x[0,600]
  * Fundalul este un gradient
  * Desenati atat P1 si P2, cat si figura obtinuta aplicand o rotatie cu centrul "intre" P1 si P2
  * Asupra dreptunghiului D aplicati o scalare avand axe oarecare; desenati atat D cat si dreptunghiul rezultat aplicand transformarea
  * Realizati una dintre transformari inmultind matricele in shader, cealalta inmultind matrice in programul principal.

![Screenshot 2022-02-08 at 12 31 27](https://user-images.githubusercontent.com/61271015/152969689-9a3deeae-f699-461f-8c20-e22a9493962c.png)
### [5. Compunerea transformarilor. Utilizarea mouse-ului](src/lab5)
### [6. Indexarea varfurilor, transformari, texturare](src/lab6)
  * Folositi glDrawElements ( ); pentru a desena ![Screenshot 2022-02-08 at 08 45 36](https://user-images.githubusercontent.com/61271015/152932953-39e864af-52f1-4ade-b35a-cf1663be8470.png)

![MicrosoftTeams-image (6)](https://user-images.githubusercontent.com/61271015/152967006-d6cdf320-3793-4218-ba3f-607af65a511c.png)

![MicrosoftTeams-image (7)](https://user-images.githubusercontent.com/61271015/152967131-32f63297-4547-4c1e-97ce-483a2b28065c.png)

  * In codul sursa 06_02 initial (doar cu un patrat) introduceti o scalare cu factori (2.0, 0.5)
si o translatie de vector (30,30). Desenati:
    * cu verde ce se obtine aplicand intai scalarea, apoi translatia 
    * texturat ce se obtine aplicand intai translatia, apoi scalarea


![MicrosoftTeams-image (8)](https://user-images.githubusercontent.com/61271015/152967204-b7a6f4ea-d657-4d2f-80fa-5e24b77efcf2.png)
### [7. Transformari de vizualizare si proiectie](src/lab7)
### [8. Obiecte 3D (partea 1)](src/lab8)
  * Creati un obiect 3D (tetraedru/piramida/con, un alt poliedru), indicand explicit varfurile (utilizati 08_01). 

![MicrosoftTeams-image (9)](https://user-images.githubusercontent.com/61271015/152967414-ff7adbba-9986-47ef-8c65-c12cf1ee6128.png)

![MicrosoftTeams-image (10)](https://user-images.githubusercontent.com/61271015/152967426-51eea492-ab12-4115-be93-bf6ebb97f592.png)

  * Utilizati randarea instantiata pentru a desena mai multe instante ale obiectului nou creat (utilizati 08_03).
 
![MicrosoftTeams-image (11)](https://user-images.githubusercontent.com/61271015/152967464-ccd182c9-e693-4bbe-97ab-7b1591a85416.png)

  * Utilizati randarea instantiata, dispunand instantele pe o curba 3D. Survolati scena (utilizati 08_03 si 08_02).

https://user-images.githubusercontent.com/61271015/152967579-baec9b5e-0f60-4ff5-80a2-3a67f7fdfdea.mp4
### [9. Obiecte 3D (partea 2)](src/lab9)
  * Trasati o curba parametrizata dupa modelul cercului.

![MicrosoftTeams-image (15)](https://user-images.githubusercontent.com/61271015/152967778-bfbea607-9c8c-4a0a-ad35-3b09a4cf7d67.png)

![MicrosoftTeams-image (16)](https://user-images.githubusercontent.com/61271015/152967796-dbdefe1f-3ba6-4a84-a882-5922f770f922.png)
  * Desenati un cilindru sau un con, dupa modelului codului sursa 09_03_sfera.cpp

![MicrosoftTeams-image (17)](https://user-images.githubusercontent.com/61271015/152967925-24953571-f00c-47d1-be8c-182c5413fa43.png)
  * Folositi sfera/cilindrul/conul pentru a schita un obiect 3D (copac, om de zapada, etc.).
![MicrosoftTeams-image (18)](https://user-images.githubusercontent.com/61271015/152967963-8a3751ed-5cb0-4506-bebf-e677710bf8ff.png)
### [10. Iluminare (partea 1)](src/lab10)
  * Explorati folosirea iluminarii aducand doua modificari (valori, mod de calcul) in modelul implementat in codurile 10_02.
  * Aplicati modelul de iluminare in cazul unui cub in care varfurile au culori diferite.
  * Aplicati modelul de iluminare poliedrului construit in tema de la L8.
### [11. Iluminare (partea 2)](src/lab11)
  * Explorati folosirea iluminarii pentru conul / cilindrul realizat la L9.

![MicrosoftTeams-image (12)](https://user-images.githubusercontent.com/61271015/152968054-ffdbc0ff-bf5c-4748-bee5-131a4a327ab0.png)
  * Aplicati umbra unei sfere / obiectului de la (i).

![MicrosoftTeams-image (13)](https://user-images.githubusercontent.com/61271015/152968107-f7ee21a2-dacf-44a0-9c70-9118e4b5c64a.png)
  * Construiti o suprafata de rotatie.

![MicrosoftTeams-image (14)](https://user-images.githubusercontent.com/61271015/152968259-6b6c541c-0f79-4d8a-8d7f-94797c115ac1.png)
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
