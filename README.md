# MagikChess

## Présentation :
Ce dépot contient les sources de notre projet réalisé dans le cadre de l'option Makers,
dispensée en 2e année informatique de l'Enseirb-Matmeca.

Le but de ce projet est de faire jouer un bras robotique (le _TinkerKit Braccio_) à
un jeu de plateau avec des pions. Ce jeu est une sorte de morpions sur un plateau en 
4*4.

## Sources :
Le dossier ./code/braccio contient les sources à envoyer sur la carte arduino du bras
robotique.

Le dossier ./Pieces contient un fichier .stl contenant une case et un pion pour la 
réalisation de l'impression 3D, ainsi qu'un fichier .dxf contenant le support pour 
fixer le plateau de jeu par rapport au bras.

## Lancement du projet :
Une fois en possession du _Braccio_ de _TinkerKit_ ainsi que du support et d'un 
plateau de jeu 4*4, et du support de pièce 2*2 imprimés en 3D, le projet peut être 
lancé avec ses différents modes.

2 modes de "jeu" sont possibles en changeant la valeur de _bool manual = [true/false]_: 
- Le mode manuel qui attend en entrée une valeur de case, de la forme "[A-D][1-4]" 
pour y poser une piece.
- Le mode automatique qui fait jouer une IA simple contre un joueur aléatoire. 
Il suffit juste de mettre un pion en alternant la couleur sur la case de stockage.