Simulation et étude d'une file d'attente par Lalanne Vincent et Perrier Romain.

--- Infos utiles

Pour compiler le programme, ne pas oublier le -lm à la fin.
Le fichier qui enregistre les données de la simulation d'une journée "InfoClient" sera créé dans le dossier du programme. 
Attention à ne pas avoir un autre fichier du même nom sinon les données seront remplacées.

--- Choix pour l'écriture du programme

La plupart des choix sont écrit en commentaire du code, mais voici les choix principaux utiles à la compréhension du code :
- Nous nous sommes rendu compte que si l'intervalle de temps entre deux créations de nombre pseudo-aléatoires était faible alors les nombres étaient assez proches.
Nous avons donc décidé de créer une fonction (aleatoire()) qui créer un nombre à partir du chiffre des unités de 6 nombre pseudo-aléatoire généré.
Cela permet d'avoir des nombres un peu plus "aléatoires" mais réduit la vitesse d'exécution du programme (peu impactant).

- On suppose qu'un client attend à l'ouverture de l'enseigne, il est donc servi directement.

- Le choix a été fait dans la question 1 pour réduire l'utilisation de la mémoire d'écraser les données du client présent avec celles du suivant à chaque itération.

- Le programme aurait pu être amélioré à la question 2 en pointant avec listclient la queue de la liste chaînée.
Cela aurait permis d'éviter l'utilisation d'une boucle dans la fonction AjouterClientEnFin().
Malheureusement nous n'avons pas réussi à implémenter cette donnée sans créer une erreur de segmentation dans notre programme.


Simulation and study of a queue by Lalanne Vincent and Perrier Romain.

--- Useful information

To compile the program, do not forget the -lm at the end.
The file which records the data of the simulation of a day "InfoClient" will be created in the program folder. 
Be careful not to have another file with the same name, otherwise the data will be overwritten.

--- Choices for writing the program

Most of the choices are written as comments in the code, but here are the main choices useful for understanding the code:
- We realised that if the time interval between two pseudo-random number creations was small then the numbers were quite close.
We therefore decided to create a function (aleatory()) which creates a number from the units digit of 6 generated pseudo-random numbers.
This allows us to have numbers that are a little more "random" but reduces the speed of execution of the program (with little impact).

- It is assumed that a customer is waiting when the shop opens, so he is served directly.

- The choice was made in question 1 to reduce memory usage by overwriting the data of the current customer with that of the next one at each iteration.

- The program could have been improved in question 2 by using listclient to point to the tail of the linked list.
This would have avoided the use of a loop in the AddClientFinish() function.
Unfortunately we were unable to implement this without creating a segmentation error in our program.