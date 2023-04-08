# Présentation

Le jeu Puissance 4 se joue à l’aide d’une grille verticale de **sept colonnes** sur **six lignes**.

Chaque joueur dispose de **vingt et un jetons** d’une couleur (le plus souvent, rouge et jaune traduit dans le code par les caractères «O» et«X») et place ceux-ci au sein de la grille à tour de rôle.

Pour gagner le jeu, un joueur doit **aligner quatre jetons** verticalement, horizontalement ou en oblique. Il s’agit donc du même principe que le Morpion à une différence prêt: la grille est
verticale ce qui signifie que les jetons tombent au fond de la colonne choisie par le joueur.

## Comment utiliser le programme

Pour faire un choix, il suffit de lire les consignes sur l’écran lorsque vous lancez le programme.

Le jeu comprend deux Modes :

- Joueur contre Joueur
- Joueur contre Bot

Dans les deux cas vous pouvez sauvegarder le jeu si vous décideriez d’arrêter au milieu du jeu, le programme généra alors un fichier YAML qui contient le sauvegarde de la partie.

Le chemin du stockage du fichier est spécifié dans les macro-constantes `PVP_SAVE_PATH` et `PVP_SAVE_PATH` .

Vous pouvez changer aussi les tokens des deux joueurs ainsi que le nom du joueur IA `PLAYER1_TOKEN`, `PLAYER2_TOKEN`, `COMPUTER_NAME` .

## L'algorithme du bot

L'algorithme est assez simple et marche de la façon suivante :

Pour chaque emplacement possible, nous allons calculer combien de pièces composeraient la ligne si nous jouions à cet endroit sans tenir compte de notre couleur (autrement dit, le jeton que nous jouons est considéré comme étant des deux couleurs). 

- Si une valeur est plus élevée que les autres, l’emplacement correspondant sera choisi. 
- Si en revanche il y a plusieurs nombres égaux, une des cases sera choisie «au hasard».

Cet algorithme connait toutefois une exception: s’il s’avère lors de l’analyse qu’un coup permet à l’ordinateur de gagner, alors le traitement s’arrêtera là et le mouvement est immédiatement joué.
