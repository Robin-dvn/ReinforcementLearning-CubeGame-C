
# RL pour un jeu simple 

Dans le but de reproduire un jour ce qui est fait dans cette [vidéo](https://youtu.be/v3UBlEJDXR0?si=ilZV-3HeMnOmdmZ-) ytb ,
j'ai essayer de dévellopper un jeu simple pour appliquer un algorithme de reinforcement learning. Le but étant d'appliquer la théorie. J'ai décidé de tout coder à partir de zéro afin de bien comprendre et de bien appliquer la théorie. J'ai choisi de faire ça en C, langage bas niveau que je maitrise le mieux.




## Demo

Démo du jeu en mode manuel:

![Gif du  jeu mode manuel](src/images/gif/mode_manu.gif)

Démo du jeu en mode apprentissage:

![Gif du  jeu mode apprentissage](src/images/gif/mode_apprentissage.gif)



## References
Pour apprendre la théorie du RL je me suis basé sur le cours de David Silver proposé par google deep mind [deep mind](https://youtube.com/playlist?list=PLqYmG7hTraZDM-OYHWgPebj2MfCFzFObQ&si=nTdWduHzlCDsnJc5) sur ytb ainsi que de nombreux post du site [medium](https://medium.com/)


## La structure
Afin de pouvoir implémenter les algorithmes de reinforcement learning en C, j'ai du codé:
- Tous les outils mathématiques d'algèbre linéaire. (Matrices, vecteurs, opérations matricielles...)
- un système de couches (layers) de neuronnes afin de pouvoir les assembler pour créer un réseau.
- Tous les outils liés aux réseaux de neuronnes. (AdamOptimizer, BackPropagation, ForwardPropagation...)
- Un système environnement-agent pour pouvoir faire du RL
- Un système de sauvegarde des weights pour pouvoir entrainer le modèle avec beaucoup d'itérations
- Le jeu graphique avec la lib SDL2






## Agent

J'ai choisi de représenter le modèle par le réseau de neuronnes suivant:
- Input (ensemble des "cases" qui l'entourent, leur nature (sol,air,pique) et le temps de jeu). taille: 53
- Layer 1; taille : 64; activation : tanh.
- Layer 2; taille : 64; activation : tanh.
- Output Layer; taille : 6; activation : softmax. (6 actions possibles ex:droite, gauche,sauter...)

J'ai chosi ce modèle en prennant exemple des nombreux exemples que j'ai vu sur les réseaux de neuronnes. Ce choix est à discuter et j'ai du mal à le justifier. J'ai effectué ce projet en été 2023, aujourdh'ui, j'aurai choisi un modèle différent. Les résultats étant déja largement satisfaisant (pour moi), je n'ai pas essayé d'améliorer le modèle.



## Entrainement

Pour l'entrainement, j'ai choisi d'utiliser l'ADAMOptimizer pour la descente de gradient. Il a été difficile à coder mais il a porté ces fruits.

En mode apprentissage, 20 cube jouent de manière automatique. Je prend ensuite le cube avec la meilleure récompense et je calcul le gradient basé sur son "parcours". 
Les agents sont récompensés par rapport à la distance parcourue selon l'axe x. Il sont pénalisés si ils touchent un pique. Le système de récompense est dur à appréhender. Le choix de la récompense à été dur à faire et il mérite une amélioration.

J'ai codé le jeu pour qu'il fasse 10 essais. Le modèle est mis à jour après chaque essaie. Une fois les 10 essais finis il faut relancer 10 essais pour continuer l'apprentissage.


## Résultats

Au bout d'environ 250 essaiss on arrive au résultat suivant:

![GIF Résultat](src/images/gif/resultat.gif)

Au bout de :
- 110 essais il a passé le premier pique
- 140 essais il a passé les deux suivants
- 200 essais il a passé le troisième

Ces résultats sont preuve de réussite pour moi. Ayant tout dévelloppé en C, en partant de zéro, je suis satisfait de ce que j'ai réussi à produire !




## Utilisation

L'utilisation du jeu n'est disponible que sur windows.

Procédure :
- Clone le répertoire
- lancer jeu.exe qui se trouve dans src/build

Paramètres par défault:

Seul la map 2 est en accès, les autres maps ne sont pas optimisées. J'ai donc bloqué la fonctionnalité de changement de map. Ensuite, par défault il y a le mode apprentissage avec les weights de l'entrainement qui à conduit au gif résultat. 

Commandes:
- Mode manuel : M
- Mode apprentissage : R
- Reset les weight : 0 (attention,les weights de l'apprentissage seront perdu)
- Commancer: ESPACE

En mode Manuel:
- directions: flèches droite et gauche
- saut : ESPACE


NB 1: J'ai enlevé l'affichege du temps (qu'on peut voir sur les gifs), à cause de certains bugs qui font crasher le jeu. Le temps total de l'essai est de 7 secondes.

NB 2: En mode apprentissage, le jeu est codé pour faire 10 essais, il revient ensuite sur le menu principal. Pour continuer l'entrainement il suffit de réappuyer sur ESPACE. 

NB 3: Il est possible que le jeu crash tout de même, si c'est le cas, les weights ne sont pas perdu et il sufit de relancer le jeu.exe et de continuer l'apprentissage.

## Améliorations et critiques

Voici les points qui sont à améliorer:
- meilleur structure de code
- meilleur modèle pour l'agent
- un système de reward plus intéligent
- meilleur visualisation des résultats (metrics)

Mon manque d'expérience et de théorie fait qu'il ya beaucoup de point à revoir. Surtout sur l'analyse du réseau de neuronnes et la théorie mathématique derrière l'apprentissage agent-environnement.

